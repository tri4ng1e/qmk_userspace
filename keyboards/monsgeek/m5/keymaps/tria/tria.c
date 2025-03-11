#include "tria.h"

#include "tria_config.inc.h"

extern uint16_t tria_turbo_key;

__attribute__ ((weak))
bool tria_process_custom_keycode_pressed(uint16_t keycode) { return true; }
__attribute__ ((weak))
bool tria_process_custom_keycode_released(uint16_t keycode) { return true; }

bool is_keycode_sentence_case_indicator(uint16_t keycode) {
    return keycode == SNCASET || keycode == KC_SPC;
}

bool is_keycode_mouse_layer_indicator(uint16_t keycode) {
    return (keycode >= KC_NUM && keycode <= KC_KP_DOT) ||
           (keycode >= KC_MS_U && keycode <= KC_MS_BTN2) ||
            keycode == KC_MS_WH_UP ||
            keycode == KC_MS_WH_DOWN ||
           (keycode >= KC_MS_ACCEL0 && keycode <= KC_MS_ACCEL2)
    ;
}

bool is_turbo_key_indicator(uint16_t keycode) {
    return keycode == tria_turbo_key;
}

static bool is_alpha_key(uint16_t keycode) {
    return (keycode >= KC_A && keycode <= KC_0) ||
            keycode == KC_MINS ||
            keycode == KC_EQL  ||
            keycode == KC_LBRC ||
            keycode == KC_RBRC ||
            keycode == KC_SCLN ||
            keycode == KC_QUOT ||
            keycode == KC_COMM ||
            keycode == KC_DOT  ||
            keycode == KC_SLSH;
}
static bool is_numpad_key(uint16_t keycode) {
    return (keycode >= KC_NUM_LOCK && keycode <= KC_KP_DOT);
}

// Add static variables for animation tracking
static bool prev_caps_lock = false;
static int16_t caps_hue_shift = 0;
#define CAPS_HUE_SHIFT_TARGET 100
#define CAPS_HUE_SHIFT_STEP   2

static bool prev_num_lock = false;
static int16_t num_hue_shift = 0;
#define NUM_HUE_SHIFT_TARGET  175
#define NUM_HUE_SHIFT_STEP    2

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t current_layer = get_highest_layer(layer_state);

    // caps animation
    bool current_caps_lock = host_keyboard_led_state().caps_lock;
    if (current_caps_lock != prev_caps_lock) { prev_caps_lock = current_caps_lock; }
    // Animate Caps Lock shift
    if (current_caps_lock) {
        if (caps_hue_shift < CAPS_HUE_SHIFT_TARGET) {
            caps_hue_shift += CAPS_HUE_SHIFT_STEP;
            if (caps_hue_shift > CAPS_HUE_SHIFT_TARGET) caps_hue_shift = CAPS_HUE_SHIFT_TARGET;
        }
    } else {
        if (caps_hue_shift > 0) {
            caps_hue_shift -= CAPS_HUE_SHIFT_STEP;
            if (caps_hue_shift < 0) caps_hue_shift = 0;
        }
    }
    if (caps_hue_shift != 0) {
        rgb_matrix_hsvshift_by_keycode_fn(led_min, led_max, current_layer, is_alpha_key, caps_hue_shift);
    }

    // num animation
    bool current_num_lock = !host_keyboard_led_state().num_lock;
    if (current_num_lock != prev_num_lock) {
        prev_num_lock = current_num_lock;
    }

    // Animate Num Lock shift
    if (current_num_lock) {
        if (num_hue_shift < NUM_HUE_SHIFT_TARGET) {
            num_hue_shift += NUM_HUE_SHIFT_STEP;
            if (num_hue_shift > NUM_HUE_SHIFT_TARGET) num_hue_shift = NUM_HUE_SHIFT_TARGET;
        }
    } else {
        if (num_hue_shift > 0) {
            num_hue_shift -= NUM_HUE_SHIFT_STEP;
            if (num_hue_shift < 0) num_hue_shift = 0;
        }
    }

    // indicators

    if (IS_LAYER_ON(L_LANG)) {
        rgb_matrix_hsvshift_by_keycode(led_min, led_max, current_layer, KC_LGUI, 100);

        switch (tria_lang_get()) {
            case TRIA_EN:
                break;
            case TRIA_RU:
                rgb_matrix_hsvshift_by_keycode(led_min, led_max, current_layer, KC_LALT, 100);
        }
    }

    if (IS_LAYER_ON(L_MOUS)) {
        rgb_matrix_hsvshift_by_keycode_fn(led_min, led_max, L_MOUS, is_keycode_mouse_layer_indicator, 100);
    } else {
        if (num_hue_shift != 0) {
            rgb_matrix_hsvshift_by_keycode_fn(led_min, led_max, current_layer, is_numpad_key, num_hue_shift);
        }
    }

    if (is_sentence_case_on()) {
        rgb_matrix_hsvshift_by_keycode_fn(led_min, led_max, current_layer, is_keycode_sentence_case_indicator, 100);
    }

    // turbo key indicator
    rgb_matrix_hsvshift_by_keycode(led_min, led_max, current_layer, tria_turbo_key, -150);

    // gray out inactive keys
    rgb_matrix_set_color_by_keycode_fn(led_min, led_max, current_layer, tria_is_keycode_norgb, 0, 0, 0);

    return false;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // tria
    tria_reset_idle_timer();
    tria_key_tracker_process_event(keycode, record);
    if (!process_turbo_click_select(keycode, record, TRBSEL)) { return false; }
    if (!process_turbo_click(keycode, record, TURBO))         { return false; }
    lang_word_process(keycode);

    // getreuer
    if (!process_select_word(keycode, record, SELWORD)) { return false; }
    if (!process_sentence_case(keycode, record))        { return false; }

    // custom keycodes
    if (record->event.pressed) {
        tria_lang_check(keycode);
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
    tria_key_tracker_init();
    tria_reset_idle_timer();
}


void housekeeping_task_user(void) {
    // tria tasks
    tria_key_tracker_process_tick();
    tria_process_idle_timer();

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