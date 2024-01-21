#include "tria.h"
#include "tria_config.h"
#include "tria/key_tracker.h"
#include "tria/idle_timer.h"
#include "tria/rgb_utils.h"


bool is_keycode_f_row(uint16_t keycode) {
    return keycode >= KC_F1 && keycode <= KC_F12;
}


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t current_layer = get_highest_layer(layer_state);
    bool    caps_lock_on  = host_keyboard_led_state().caps_lock;
    
    switch (current_layer) {
        case LAYER_MAIN:
        case LAYER_FAIN:
            if (caps_lock_on) {
                rgb_matrix_hsvshift_by_keycode(led_min, led_max, LAYER_MAIN, tria_is_keycode_caps_indicator, 100);
            }
            break;

        case LAYER_FN:
            rgb_matrix_set_color_by_keycode(led_min, led_max, current_layer, tria_is_keycode_norgb, 0, 0, 0);
            break;
    }

    if (IS_LAYER_ON(LAYER_FAIN)) {
        rgb_matrix_hsvshift_by_keycode(led_min, led_max, LAYER_FAIN, is_keycode_f_row, 100);
    }

    return false;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    reset_tria_idle_timer();
    process_tria_key_tracker(keycode, record);

    return true;
}


void keyboard_post_init_user(void) {
    rgb_matrix_mode(RGB_MATRIX_CUSTOM_TRIA_GRADIENT_UP_DOWN);
    rgb_matrix_config.hsv   = tria_rgb_default_color;
    rgb_matrix_config.speed = tria_rgb_default_speed;

    init_tria_rgb_utils();
    reset_tria_idle_timer();
}


void housekeeping_task_user(void) {
    process_tria_key_tracker_tick();
    process_tria_idle_timer();
}