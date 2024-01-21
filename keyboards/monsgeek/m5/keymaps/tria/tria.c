#include "tria.h"
#include "tria_config.h"
#include "tria/key_tracker.h"
#include "tria/idle_timer.h"
#include "tria/rgb_utils.h"

#include "getreuer/select_word.h"
#include "getreuer/sentence_case.h"

bool is_keycode_sentence_case_indicator(uint16_t keycode) {
    return keycode == SNCASET || keycode == KC_SPC;
}

bool is_keycode_lang_layer_indicator(uint16_t keycode) {
    return keycode == KC_LGUI;
}

bool is_keycode_lang_indicator(uint16_t keycode) {
    return keycode == KC_LCTL;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t current_layer = get_highest_layer(layer_state);
    bool    caps_lock_on  = host_keyboard_led_state().caps_lock;
    
    switch (current_layer) {
        case L_MAIN:
            if (caps_lock_on) {
                rgb_matrix_hsvshift_by_keycode(led_min, led_max, L_MAIN, tria_is_keycode_caps_indicator, 100);
            }
            break;

        case L_FN:
            rgb_matrix_set_color_by_keycode(led_min, led_max, current_layer, tria_is_keycode_norgb, 0, 0, 0);
            break;
    }

    if (is_sentence_case_on()) {
        rgb_matrix_hsvshift_by_keycode(led_min, led_max, current_layer, is_keycode_sentence_case_indicator, 100);
    }

    if (IS_LAYER_ON(L_LANG)) {
        rgb_matrix_hsvshift_by_keycode(led_min, led_max, current_layer, is_keycode_lang_layer_indicator, 50);
        if (tria_lang_russian) {
            rgb_matrix_hsvshift_by_keycode(led_min, led_max, current_layer, is_keycode_lang_indicator, 50);
        } else {
            // rgb_matrix_set_color_by_keycode(led_min, led_max, current_layer, is_keycode_lang_indicator, 186, 31, 31);
        }
    }

    return false;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // process tria-based stuff
    reset_tria_idle_timer();
    process_tria_key_tracker(keycode, record);

    // getreuer
    if (!process_select_word(keycode, record, SELWORD)) { return false; }
    if (!process_sentence_case(keycode, record)) { return false; }

    // custom keycodes
    if (record->event.pressed) {
        switch (keycode) {
            case SNCASET:
                sentence_case_toggle();
                return false;
            case KC_F13: // EN select
                tria_lang_russian = false;
                return true;
            case KC_F14: // RU select
                tria_lang_russian = true;
                return true;
        }
    }


    return true;
}


void keyboard_post_init_user(void) {
    sentence_case_off();
    rgb_matrix_mode(RGB_MATRIX_CUSTOM_TRIA_GRADIENT_UP_DOWN);
    rgb_matrix_config.hsv   = tria_rgb_default_color;
    rgb_matrix_config.speed = tria_rgb_default_speed;

    tria_lang_russian = false;

    init_tria_rgb_utils();
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