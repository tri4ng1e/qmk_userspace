RGB_MATRIX_EFFECT(TRIA_GRADIENT_UP_DOWN)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "tria/key_tracker.h"
#include "tria/idle_timer.h"
#include "cimple_ring_buffer/cimple_ring_buffer.h"
extern ring_buffer_t tria_tracker_rb;

// idle settings from tria_config.inc.h
extern bool     tria_idle_enabled;
extern uint32_t tria_idle_timeout;
extern uint8_t  tria_idle_brightness;
extern uint16_t tria_idle_off_time;
extern uint16_t tria_idle_on_time;

// idle internals
typedef enum {
    TRIA_IDLE_NORMAL, // normal
    TRIA_IDLE_FADING, // starts to fade
    TRIA_IDLE_FADED,  // fully faded
    TRIA_IDLE_RAISING // raising back from faded to normal
} tria_idle_anim_state_t;

tria_idle_anim_state_t tria_idle_state = TRIA_IDLE_NORMAL;
uint16_t tria_idle_anim_timer = 0;
uint8_t  tria_idle_power_offset = 0;

// gradient settings
uint8_t tria_gradient_size_factor = 5; // smaller the number, tighter the gradient
// extern bool tria_gradient_running;

// reactive effect settings
uint8_t tria_splash_factor = 4;  // bigger the number, smaller the splash
uint8_t tria_splash_effect_amount = 70;
bool    tria_splash_effect_positive = true;


uint8_t led_distance(uint8_t a_x, uint8_t a_y, uint8_t b_x, uint8_t b_y) {
    return sqrt16(
        ((int16_t)(a_x - b_x) * (int16_t)(a_x - b_x)) +
        ((int16_t)(a_y - b_y) * (int16_t)(a_y - b_y))
    );
}

static HSV tria_reactive_animation_hsv(HSV hsv, uint8_t dist, uint16_t tick) {
    uint16_t h_effect = tick / 20 + dist * tria_splash_factor;
    if (h_effect > tria_splash_effect_amount) { h_effect = tria_splash_effect_amount; }
    hsv.h = tria_splash_effect_positive ?
        hsv.h - tria_splash_effect_amount + h_effect :
        hsv.h + tria_splash_effect_amount - h_effect
    ;
    return hsv;
}

void idle_init(uint8_t offset) { tria_idle_power_offset = offset; tria_idle_anim_timer = timer_read(); }
void idle_clear(void)          { tria_idle_power_offset = 0;      tria_idle_anim_timer = 0;            }

void idle_start_fading (uint8_t offset) { tria_idle_state = TRIA_IDLE_FADING ; idle_init(offset); }
void idle_start_raising(uint8_t offset) { tria_idle_state = TRIA_IDLE_RAISING; idle_init(offset); }
void idle_start_normal (void)           { tria_idle_state = TRIA_IDLE_NORMAL ; idle_clear();      }
void idle_start_faded  (void)           { tria_idle_state = TRIA_IDLE_FADED  ; idle_clear();      }

uint8_t idle_get_power(uint8_t idle_diff, uint16_t target_time) {
    if (tria_idle_anim_timer == 0) { return 0; }
    int time = timer_elapsed(tria_idle_anim_timer);
    int16_t power = (float)idle_diff * ((float)time / (float)target_time);
    if (power < 0) { return 0; }
    return power > idle_diff ? idle_diff : power;
}


bool TRIA_GRADIENT_UP_DOWN(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // idle brightness animation
    uint8_t idle_power = 0; // decrease final brightness by this value
    if (tria_idle_enabled) {
        uint8_t idle_diff = rgb_matrix_config.hsv.v - tria_idle_brightness;
        bool is_idle = tria_seconds_idle() >= tria_idle_timeout;
        switch (tria_idle_state) {
            case TRIA_IDLE_NORMAL: // to: fading
                idle_power = 0;
                if (is_idle) { idle_start_fading(0); }
                break;
            case TRIA_IDLE_FADING: { // to: faded, raising
                uint8_t new_fade = tria_idle_power_offset + idle_get_power(idle_diff, tria_idle_off_time);;
                if (new_fade > idle_diff) { new_fade = idle_diff; }
                idle_power = new_fade;
                if (!is_idle) {
                    idle_start_raising(idle_power);
                } else if (idle_power >= idle_diff) {
                    idle_start_faded();
                }
                break;
            }
            case TRIA_IDLE_FADED: // to: raising
                idle_power = idle_diff;
                if (!is_idle) { idle_start_raising(idle_diff); }
                break;
            case TRIA_IDLE_RAISING: { // to: normal, fading
                uint8_t progress = idle_get_power(idle_diff, tria_idle_on_time);
                uint8_t new_fade = (progress >= tria_idle_power_offset) ? 0 : (tria_idle_power_offset - progress);
                idle_power = new_fade;
                if (is_idle) {
                    idle_start_fading(idle_power);
                } else if (idle_power == 0) {
                    idle_start_normal();
                }
                break;
            }
        }
    }

    uint8_t scale = scale8(64, rgb_matrix_config.speed);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        HSV hsv = rgb_matrix_config.hsv;

        // static effect: top-down gradient
        hsv.h += scale * (g_led_config.point[i].y >> tria_gradient_size_factor);

        // apply color shift if gradient is set as running
        // if (tria_gradient_running) hsv.h += scale16by8(g_rgb_timer, rgb_matrix_config.speed / 40);

        // reactive effect: multi-splash based on custom key tracker
        for (uint8_t j = 0; j < tria_tracker_rb.count; j++) {
            tria_tracker_item_t * item = (tria_tracker_item_t*)ring_buffer_get(&tria_tracker_rb, j);
            uint8_t dist  = led_distance(
                g_led_config.point[i].x, g_led_config.point[i].y,
                item->x, item->y
            );
            hsv = tria_reactive_animation_hsv(hsv, dist, item->tick);
        }

        // apply idle effect
        hsv.v -= idle_power;

        // convert to RGB and set the values
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    } // for each led

    return rgb_matrix_check_finished_leds(led_max);
}

#endif
