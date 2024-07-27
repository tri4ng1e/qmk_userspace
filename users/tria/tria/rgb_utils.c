#include "tria/rgb_utils.h"
#include "tria/utils.h"

#define TRIA_ARRAY_SIZEOF(a) (sizeof(a) / sizeof(a[0]))

/*
    normally, the buffers are hidden from user code, but since we want to get a color from it,
    we need to manually define all types that live inside QMK's .c files related to buffers
    and then extern those buffers, so we can access them
*/
#if defined(AW20216S_DRIVER_COUNT)
    #define TRIA_PWM_REGISTER_COUNT 216
    #define LED_TYPE                aw_led
    #define LEDS_TYPE               g_aw_leds

    typedef struct aw20216s_driver_t {
        uint8_t pwm_buffer[TRIA_PWM_REGISTER_COUNT];
        bool    pwm_buffer_dirty;
    } PACKED aw20216s_driver_t;
    extern aw20216s_driver_t driver_buffers[AW20216S_DRIVER_COUNT];

#elif defined(SNLED27351_I2C_ADDRESS_1)
    #define TRIA_PWM_REGISTER_COUNT     192
    #define TRIA_CONTROL_REGISTER_COUNT 24
    #define LED_TYPE                    ckled2001_led
    #define LEDS_TYPE                   g_ckled2001_leds

    typedef struct snled27351_driver_t {
        uint8_t pwm_buffer[TRIA_PWM_REGISTER_COUNT];
        bool    pwm_buffer_dirty;
        uint8_t led_control_buffer[TRIA_CONTROL_REGISTER_COUNT];
        bool    led_control_buffer_dirty;
    } PACKED snled27351_driver_t;
    extern snled27351_driver_t driver_buffers[SNLED27351_DRIVER_COUNT];

#else
  #error Your LED driver is not supported by tria/rgb_utils.h yet
#endif

#if defined(AW20216S_DRIVER_COUNT) || defined(SNLED27351_I2C_ADDRESS_1)
    RGB tria_rgb_get_color_from_buffer(const LED_TYPE *led) {
        RGB color = {
            .r = driver_buffers[led->driver].pwm_buffer[led->r],
            .g = driver_buffers[led->driver].pwm_buffer[led->g],
            .b = driver_buffers[led->driver].pwm_buffer[led->b]
        };
        return color;
    }
#endif

keypos_t led_key_pos[RGB_MATRIX_LED_COUNT];

void init_tria_rgb_utils(void) {
    // fill up lookup table for corresponding key position for a led
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t led_index = g_led_config.matrix_co[row][col];
            if (led_index != NO_LED) {
                led_key_pos[led_index] = (keypos_t){.row = row, .col = col};
            }
        }
    }
}

////////////////////////////////////////////////////////////

uint16_t rgb_led_to_keycode(int8_t layer, uint16_t led, bool fallthrough) {
    uint16_t ret = keymap_key_to_keycode(layer, led_key_pos[led]);
    while (fallthrough && ret == KC_TRNS && layer >= 0) {
        layer = get_previous_layer(layer);
        ret = keymap_key_to_keycode(layer, led_key_pos[led]);
    }
    return ret;
}

RGB rgb_matrix_get_color(int index) { 
    RGB led_color;
    LED_TYPE led;
    if (index >= 0 && index < RGB_MATRIX_LED_COUNT) {
        memcpy_P(&led, (&LEDS_TYPE[index]), sizeof(led));
        led_color = tria_rgb_get_color_from_buffer(&led);
    }
    return led_color;
}

HSV rgb_matrix_get_color_hsv(int index) {
    RGB c = rgb_matrix_get_color(index);
    return rgb_to_hsv(c);
}

void rgb_matrix_set_color_hsv(int index, HSV hsv) {
    RGB c = hsv_to_rgb(hsv);
    rgb_matrix_set_color(index, c.r, c.g, c.b);
}



void rgb_matrix_set_color_by_keycode_fn(uint8_t led_min, uint8_t led_max, uint8_t layer, bool (*is_keycode)(uint16_t), uint8_t red, uint8_t green, uint8_t blue) {
    for (uint8_t i = led_min; i < led_max; i++) {
        if (i == NO_LED) { continue; }
        uint16_t keycode = rgb_led_to_keycode(layer, i, true);
        if ((*is_keycode)(keycode)) {
            rgb_matrix_set_color(i, red, green, blue);
        }
    }
}

void rgb_matrix_hsvshift_by_keycode_fn(uint8_t led_min, uint8_t led_max, uint8_t layer, bool (*is_keycode)(uint16_t), int16_t shift) {
    for (uint8_t i = led_min; i < led_max; i++) {
        if (i == NO_LED) { continue; }
        uint16_t keycode = rgb_led_to_keycode(layer, i, true);
        if ((*is_keycode)(keycode)) {
            HSV hsv = rgb_matrix_get_color_hsv(i);
            hsv.h += shift;
            rgb_matrix_set_color_hsv(i, hsv);
        }
    }
}

bool (*tria_create_predicate_from_value(uint16_t kc))(uint16_t) {
    bool predicate(uint16_t x) { return x == kc; }
    return &predicate;
}

void rgb_matrix_set_color_by_keycode(uint8_t led_min, uint8_t led_max, uint8_t layer, uint16_t keycode, uint8_t red, uint8_t green, uint8_t blue) {
    bool (*pred)(uint16_t) = tria_create_predicate_from_value(keycode);
    rgb_matrix_set_color_by_keycode_fn(led_min, led_max, layer, pred, red, green, blue);
}

void rgb_matrix_hsvshift_by_keycode(uint8_t led_min, uint8_t led_max, uint8_t layer, uint16_t keycode, int16_t shift) {
    bool (*pred)(uint16_t) = tria_create_predicate_from_value(keycode);
    rgb_matrix_hsvshift_by_keycode_fn(led_min, led_max, layer, pred, shift);
}

////////////////////////////////////////////////////////////

__attribute__ ((weak))
bool tria_is_keycode_norgb_user(uint16_t keycode) {
    return keycode == KC_NO;
}

__attribute__ ((weak))
bool tria_is_keycode_caps_indicator_user(uint16_t keycode) {
    return keycode == KC_CAPS;
}

__attribute__ ((weak))
bool tria_is_keycode_num_indicator_user(uint16_t keycode) {
    return keycode == KC_NUM;
}

bool tria_is_keycode_norgb(uint16_t keycode) {
    return tria_is_keycode_norgb_user(keycode);
}

bool tria_is_keycode_caps_indicator(uint16_t keycode) {
    return tria_is_keycode_caps_indicator_user(keycode);
}

bool tria_is_keycode_num_indicator(uint16_t keycode) {
    return tria_is_keycode_num_indicator_user(keycode);
}

////////////////////////////////////////////////////////////

HSV rgb_to_hsv(RGB rgb) {
    HSV hsv;
    uint8_t rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);
    
    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * (long)(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}