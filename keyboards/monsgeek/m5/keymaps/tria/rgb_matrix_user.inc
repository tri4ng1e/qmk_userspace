RGB_MATRIX_EFFECT(TRIA_GRADIENT_UP_DOWN)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "tria/key_tracker.h"
#include "tria/idle_timer.h"
#include "cimple_ring_buffer/cimple_ring_buffer.h"
extern ring_buffer_t tria_tracker_rb;

// extern const uint16_t tria_idle_timeout;
// extern uint16_t tria_idle_brightness;
// extern uint8_t tria_brightness;

// extern bool tria_gradient_running;

// idle settings
extern bool     tria_idle_enabled;
extern uint32_t tria_idle_timeout;
extern uint8_t  tria_idle_brightness;
extern uint16_t tria_idle_off_time;
extern uint16_t tria_idle_on_time;

// idle internals
bool     tria_idle_animation = false;
bool     tria_idle = true;
uint16_t tria_anim_timer = 0;

uint8_t tria_gradient_size_factor = 5; // smaller the number, tighter the gradient

uint8_t tria_splash_factor = 3.5;  // bigger the number, smaller the splash
uint8_t tria_splash_effect_amount = 70;
bool    tria_splash_effect_positive = true;

// uint16_t idle_tick = 0;
// bool tria_idle = false;

// uint16_t time = 0;

#define LED_DISTANCE(a_x, a_y, b_x, b_y) sqrt16(((int16_t)(a_x - b_x) * (int16_t)(a_x - b_x)) + ((int16_t)(a_y - b_y) * (int16_t)(a_y - b_y)))

// Math effect functions
// =====================

static HSV TRIA_GRADIENT_UP_DOWN_math(HSV hsv, uint8_t dist, uint16_t tick) {

    uint16_t h_effect = tick / 20 + dist * tria_splash_factor;
    if (h_effect > tria_splash_effect_amount) h_effect = tria_splash_effect_amount;
    hsv.h = tria_splash_effect_positive? hsv.h - tria_splash_effect_amount + h_effect : hsv.h + tria_splash_effect_amount - h_effect;

    return hsv;
}

bool TRIA_GRADIENT_UP_DOWN(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t scale = scale8(64, rgb_matrix_config.speed);

    // idle brightness animation setup
    uint8_t idle_effect = 0;
    if (tria_idle_enabled) {
        uint16_t idle_seconds = tria_seconds_idle();
        bool tria_idle_new = idle_seconds >= tria_idle_timeout;
        if (tria_idle != tria_idle_new) {
            tria_idle = tria_idle_new;
            tria_idle_animation = true;
            tria_anim_timer = timer_read();
        }

        uint8_t idle_diff = rgb_matrix_config.hsv.v - tria_idle_brightness;

        if (tria_idle_animation) {
            uint16_t idle_elapsed = timer_elapsed(tria_anim_timer);
            uint16_t idle_target_time = tria_idle ? tria_idle_off_time : tria_idle_on_time;

            if (idle_elapsed > idle_target_time) {
                tria_idle_animation = false;
            } else {
                idle_effect = (float)idle_diff * ((float)idle_elapsed / (float)idle_target_time);
                if (!tria_idle) {
                    idle_effect = idle_diff - idle_effect;
                }
            }
        }

        if (tria_idle && !tria_idle_animation) {
            idle_effect = idle_diff;
        }
    }
    
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
            uint8_t dist  = LED_DISTANCE(
                g_led_config.point[i].x, g_led_config.point[i].y,
                item->x, item->y
            );
            hsv = TRIA_GRADIENT_UP_DOWN_math(hsv, dist, item->tick);
        }

        // apply idle effect
        hsv.v -= idle_effect;

        // convert to RGB and set the values
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    } // for each led

    return rgb_matrix_check_finished_leds(led_max);
}

#endif