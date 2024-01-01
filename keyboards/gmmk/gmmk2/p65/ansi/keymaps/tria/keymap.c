/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

#include "tria_config.h"
#include "tria/key_tracker.h"
#include "tria/idle_timer.h"
#include "tria/rgb_utils.h"


// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
enum layers {
    LAYER_MAIN,
    LAYER_FAIN,
    LAYER_FN,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[LAYER_MAIN] = LAYOUT_65_ansi_blocker(
    QK_GESC,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,         KC_MINS, KC_EQL,   KC_BSPC,  KC_DEL,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,         KC_LBRC, KC_RBRC,  KC_BSLS,  KC_PGUP,
    KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,      KC_QUOT, KC_ENT,             KC_PGDN,
    KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,      KC_RSFT,           KC_UP,    KC_END,
    KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 MO(LAYER_FN), KC_RCTL, KC_LEFT,  KC_DOWN,  KC_RGHT
),

[LAYER_FAIN] = LAYOUT_65_ansi_blocker(
    _______,  KC_F1,    KC_F2,          KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  _______,
    _______,  _______,  _______,        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
    _______,  _______,  _______,        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
    _______,  _______,  _______,        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
    _______,  _______,  _______,                                _______,                            _______,  _______,  _______,  _______,  _______
),

[LAYER_FN] = LAYOUT_65_ansi_blocker(
    KC_GRV,   KC_F1,    KC_F2,          KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  KC_INS,
    _______,  _______,  _______,        _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_PSCR,  KC_SCRL,  KC_PAUS,  _______,  _______,
    KC_CAPS,  _______,  _______,        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
    _______,  _______,  _______,        _______,  _______,  _______,  _______,  KC_MUTE,  KC_MPRV,  KC_MNXT,  KC_MPLY,  _______,            _______,  KC_HOME,
    _______,  _______,  TG(LAYER_FAIN),                                QK_BOOT,                                _______, _______,  RGB_RMOD, _______,  RGB_MOD
)};

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