/* Copyright (C) 2022 jonylee@hfd
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

#include "tria.h"

#include "tap_dances.h"
#include "tria/tap_dance_magic.h"

// clang-format off

		// here, some quick templates for ya

		// _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
		// _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		// _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		// _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,                            _______, _______, _______,
		// _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,          _______,          _______, _______, _______, _______,
		// _______, _______, _______,                   _______,                            _______, _______, _______,          _______, _______, _______, _______,          _______, _______),

		// ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,          ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,
		// ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,
		// ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,
		// ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,          ___0___,                            ___0___, ___0___, ___0___,
		// ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,                   ___0___,          ___0___,          ___0___, ___0___, ___0___, ___0___,
		// ___0___, ___0___, ___0___,                   ___0___,                            ___0___, ___0___, ___0___,          ___0___, ___0___, ___0___, ___0___,          ___0___, ___0___),

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [L_MAIN] = LAYOUT( /* Base */
		KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           KC_PSCR, KC_SCRL, KC_PAUS, SNDDEV, KC_MUTE, KC_VOLD, KC_VOLU,
		KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_INS,  KC_HOME, KC_PGUP, KC_NUM, KC_PSLS, KC_PAST, KC_PMNS,
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,  KC_END,  KC_PGDN, KC_P7,  KC_P8,   KC_P9,   KC_PPLS,
		SELWORD, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,                             KC_P4,  KC_P5,   KC_P6,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,                   KC_RSFT,          KC_UP,            KC_P1,  KC_P2,   KC_P3,   KC_PENT,
		KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RALT, KC_RGUI, MO(L_FN),         KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT,         KC_P0,   KC_PDOT),

    [L_LANG] = LAYOUT( /* LANG */
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
		T_CLF13, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,                            _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,          _______,          _______, _______, _______, _______,
		_______, _______, _______,                   _______,                            _______, _______, _______,          _______, _______, _______, _______,          _______, _______),

	[L_MOUS] = LAYOUT(
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_WH_U,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BTN1, KC_MS_U, KC_BTN2, KC_WH_D,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,                            KC_MS_L, KC_MS_D, KC_MS_R,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,          _______,          KC_ACL0, KC_ACL1, KC_ACL2, _______,
		_______, _______, _______,                   _______,                            _______, _______, _______,          _______, _______, _______, _______,          _______, _______),

	[L_UNC] = LAYOUT(
		___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,          ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,
		___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, UC_DEG,  UC_EMD,  UC_PLMN, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,
		___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,
		___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, UC_ACNT,          ___0___,                            ___0___, ___0___, ___0___,
		___0___, ___0___, UC_XMRK, UC_CHCK, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,                   ___0___,          UC_ARRU,          ___0___, ___0___, ___0___, ___0___,
		___0___, ___0___, ___0___,                   ___0___,                            ___0___, ___0___, ___0___,          LLOCK,   UC_ARRL, UC_ARRD, UC_ARRR,          ___0___, ___0___),


    [L_FN] = LAYOUT( /* FN */
	    QK_BOOT,    KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,              ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,
		___0___,    ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,    ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,
		___0___,    ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, RGB_MOD,    ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, ___0___,
		KC_CAPS,    ___0___, ___0___, ___0___, ___0___, ___0___, QK_LOCK, ___0___, ___0___, RGB_TOG, ___0___, ___0___,          RGB_HUI,                                        ___0___, ___0___, ___0___,
		___0___,    ___0___, ___0___, ___0___, ___0___, ___0___, ___0___, TGMOUSE, ___0___, ___0___, ___0___,                   OSL(L_UNC),          RGB_VAI,          ___0___, ___0___, ___0___, ___0___,
		TG(L_LANG), GUI_TOG, ___0___,                   SNCASET,                            ___0___, ___0___, ___0___,          LLOCK,      RGB_SAD, RGB_VAD, RGB_SAI,          TURBO,   TRBSEL),

};
// clang-format on

const uint16_t PROGMEM cmb_prev[]   = {SNDDEV,  KC_VOLD, COMBO_END};
const uint16_t PROGMEM cmb_next[]   = {SNDDEV,  KC_VOLU, COMBO_END};
const uint16_t PROGMEM cmb_pause[]  = {SNDDEV,  KC_MUTE, COMBO_END};

combo_t key_combos[] = {
    [CMB_PREV]   = COMBO(cmb_prev,   KC_MPRV),
    [CMB_NEXT]   = COMBO(cmb_next,   KC_MNXT),
    [CMB_PAUSE]  = COMBO(cmb_pause,  KC_MPLY),
};

void tria_lang_init_user(void) {
	tria_lang_set_keycode(TRIA_EN, KC_F13);
	tria_lang_set_keycode(TRIA_RU, KC_F14);
}

bool tria_process_custom_keycode_pressed(uint16_t keycode) {
	switch (keycode) {
		case SNCASET:
			sentence_case_toggle();
			return false;
		case T_CLF13: // combo-like behavior for Shift+F13, combo messes up the shift mod state
			if ((get_mods() | get_oneshot_mods() | get_weak_mods()) == MOD_BIT(KC_LSFT)) {
				tria_lang_switch(TRIA_RU);
				return false;
			} else {
				return true;
			}
	}
	return true;
}