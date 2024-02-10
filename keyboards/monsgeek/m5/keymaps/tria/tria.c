#include "tria.h"

#include "tria_config.inc.h"

__attribute__ ((weak))
bool tria_process_custom_keycode_pressed(uint16_t keycode) { return true; }
__attribute__ ((weak))
bool tria_process_custom_keycode_released(uint16_t keycode) { return true; }

bool is_keycode_sentence_case_indicator(uint16_t keycode) {
    return keycode == SNCASET || keycode == KC_SPC;
}

bool is_keycode_lang_layer_indicator(uint16_t keycode) {
    return keycode == KC_LGUI;
}

bool is_keycode_lang_indicator(uint16_t keycode) {
    return keycode == KC_LALT;
}

bool is_keycode_mouse_layer_indicator(uint16_t keycode) {
    return (keycode >= KC_NUM && keycode <= KC_KP_DOT) ||
           (keycode >= KC_MS_U && keycode <= KC_MS_BTN2) ||
            keycode == KC_MS_WH_UP ||
            keycode == KC_MS_WH_DOWN ||
           (keycode >= KC_MS_ACCEL0 && keycode <= KC_MS_ACCEL2)
    ;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t current_layer = get_highest_layer(layer_state);
    // bool    num_lock_on   = host_keyboard_led_state().num_lock;
    bool    caps_lock_on  = host_keyboard_led_state().caps_lock;
    
    // gray out inactive keys
    rgb_matrix_set_color_by_keycode_fn(led_min, led_max, current_layer, tria_is_keycode_norgb, 0, 0, 0);

    if (caps_lock_on) {
        rgb_matrix_hsvshift_by_keycode_fn(led_min, led_max, current_layer, tria_is_keycode_caps_indicator, 100);
    }

    if (is_sentence_case_on()) {
        rgb_matrix_hsvshift_by_keycode_fn(led_min, led_max, current_layer, is_keycode_sentence_case_indicator, 100);
    }

    if (IS_LAYER_ON(L_LANG)) {
        rgb_matrix_hsvshift_by_keycode_fn(led_min, led_max, current_layer, is_keycode_lang_layer_indicator, 100);

        switch (tria_lang_get()) {
            case TRIA_EN:
                break;
            case TRIA_RU:
                rgb_matrix_hsvshift_by_keycode_fn(led_min, led_max, current_layer, is_keycode_lang_indicator, 100);
        }
    }

    if (IS_LAYER_ON(L_MOUS)) {
        rgb_matrix_hsvshift_by_keycode_fn(led_min, led_max, L_MOUS, is_keycode_mouse_layer_indicator, 100);
    }

    return false;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // process tria-based stuff
    reset_tria_idle_timer();
    process_tria_key_tracker(keycode, record);
    tria_lang_check(keycode, record);

    // getreuer
    if (!process_select_word(keycode, record, SELWORD)) { return false; }
    if (!process_sentence_case(keycode, record)) { return false; }

    // custom keycodes
    if (record->event.pressed) {
        return tria_process_custom_keycode_pressed(keycode);
    } else {
        return tria_process_custom_keycode_released(keycode);
    }


    return true;
}


void keyboard_post_init_user(void) {
    sentence_case_off();
    rgb_matrix_mode(RGB_MATRIX_CUSTOM_TRIA_GRADIENT_UP_DOWN);
    rgb_matrix_config.hsv   = tria_rgb_default_color;
    rgb_matrix_config.speed = tria_rgb_default_speed;

    layer_on(L_LANG);

    init_tria_rgb_utils();
    tria_lang_init();
    reset_tria_idle_timer();
}


void housekeeping_task_user(void) {
    // tria tasks
    process_tria_key_tracker_tick();
    process_tria_idle_timer();

    // getreuer tasks
    select_word_task();
    sentence_case_task();
}


// keyboard implementation bug workaround
bool led_update_user(led_t led_state) {
    writePin(A15, led_state.num_lock);
    writePin(C10, led_state.caps_lock);
    return true;
}