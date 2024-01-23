#include "tria/rgb_utils.h"

#define TRIA_ARRAY_SIZEOF(a) (sizeof(a) / sizeof(a[0]))

#if defined(AW20216S_DRIVER_COUNT)
  #define TRIA_DRIVER_COUNT AW20216S_DRIVER_COUNT
  #define TRIA_PWM_REGISTER_COUNT 216
  #define LED_TYPE       aw_led
  #define LEDS_TYPE      g_aw_leds
#elif defined(SNLED27351_I2C_ADDRESS_1)
  #define TRIA_DRIVER_COUNT       SNLED27351_DRIVER_COUNT
  #define TRIA_PWM_REGISTER_COUNT 192
  #define LED_TYPE                ckled2001_led
  #define LEDS_TYPE               g_ckled2001_leds
#else
  #error Your LED driver is not supported by tria/rgb_utils.h yet
#endif

extern uint8_t g_pwm_buffer[TRIA_DRIVER_COUNT][TRIA_PWM_REGISTER_COUNT];

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

uint16_t rgb_led_to_keycode(uint8_t layer, uint16_t led) {
    return keymap_key_to_keycode(layer, led_key_pos[led]);
}

RGB rgb_matrix_get_color(int index) { 
    RGB led_color;
    LED_TYPE led;
    if (index >= 0 && index < RGB_MATRIX_LED_COUNT) {
        memcpy_P(&led, (&LEDS_TYPE[index]), sizeof(led));
        led_color.r = g_pwm_buffer[led.driver][led.r];
        led_color.g = g_pwm_buffer[led.driver][led.g];
        led_color.b = g_pwm_buffer[led.driver][led.b];
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
        uint16_t keycode = keymap_key_to_keycode(layer, led_key_pos[i]);
        if ((*is_keycode)(keycode)) {
            rgb_matrix_set_color(i, red, green, blue);
        }
    }
}

void rgb_matrix_hsvshift_by_keycode_fn(uint8_t led_min, uint8_t led_max, uint8_t layer, bool (*is_keycode)(uint16_t), int16_t shift) {
    for (uint8_t i = led_min; i < led_max; i++) {
        if (i == NO_LED) { continue; }
        uint16_t keycode = keymap_key_to_keycode(layer, led_key_pos[i]);
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
    return keycode == KC_TRNS || keycode == KC_NO;
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