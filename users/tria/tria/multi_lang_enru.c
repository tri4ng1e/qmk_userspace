// getreuer overrides for en,ru
#include "tria/multi_lang.h"
#include "tria/multi_lang_enru.h"

#include "getreuer/sentence_case.h"

// custom behavior for getreuer sentence case
char sentence_case_press_user_en(uint16_t keycode, keyrecord_t* record, uint8_t mods) {
    if ((mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
        const bool shifted = mods & MOD_MASK_SHIFT;
        switch (keycode) {
            case KC_LCTL ... KC_RGUI:  // Mod keys.
                return '\0';  // These keys are ignored.

            case KC_A ... KC_Z:
                return 'a';  // Letter key.

            case KC_DOT:  // . is punctuation, Shift . is a symbol (>)
                return !shifted ? '.' : '#';
            case KC_1:
            case KC_SLSH:
                return shifted ? '.' : '#';
            case KC_2 ... KC_0:  // 2 3 4 5 6 7 8 9 0
            case KC_MINS ... KC_SCLN:  // - = [ ] ; backslash
            case KC_GRV:
            case KC_COMM:
                return '#';  // Symbol key.

            case KC_SPC:
                return ' ';  // Space key.

            case KC_QUOT:
                return '\'';  // Quote key.
        }
    }

    // Otherwise clear Sentence Case to initial state.
    sentence_case_clear();
    return '\0';
}

char sentence_case_press_user_ru(uint16_t keycode, keyrecord_t* record, uint8_t mods) {
    if ((mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
        const bool shifted = mods & MOD_MASK_SHIFT;
        switch (keycode) {
            // ignored keys: mods
            case KC_LCTL ... KC_RGUI:  // Mod keys.
                return '\0';  // These keys are ignored.

            // letters
            case KC_LBRC:
            case KC_RBRC:
            case KC_SCLN:
            case KC_QUOT:
            case KC_COMM:
            case KC_DOT:
            case KC_A ... KC_Z:
                return 'a';

            case KC_SLSH: // '.' is sentence end, ',' is symbol
                return shifted ? '#' : '.';

            case KC_1: // '1' is symbol, '!' is sentence end
                return shifted ? '.' : '#';
            
            case KC_2: // '2' is symbol, '"' is quote
                return shifted ? '\'' : '#';

            case KC_3 ... KC_0:  // 2 3 4 5 6 7 8 9 0
                return '#';

            case KC_SPC:
                return ' ';  // Space key.
        }
    }

    // Otherwise clear Sentence Case to initial state.
    sentence_case_clear();
    return '\0';
}

char sentence_case_press_user(uint16_t keycode, keyrecord_t* record, uint8_t mods) {
    if (tria_lang_get() == TRIA_RU) {
        return sentence_case_press_user_ru(keycode, record, mods);
    } else {
        return sentence_case_press_user_en(keycode, record, mods);
    }
}

// fix for WinCompose's troubles with entering Unicode in non-English layout
extern led_t unicode_saved_led_state;
extern uint8_t unicode_saved_mods;
#ifndef UNICODE_KEY_MAC
#    define UNICODE_KEY_MAC KC_LEFT_ALT
#endif
#ifndef UNICODE_KEY_LNX
#    define UNICODE_KEY_LNX LCTL(LSFT(KC_U))
#endif
#ifndef UNICODE_KEY_WINC
#    define UNICODE_KEY_WINC KC_RIGHT_ALT
#endif
#ifndef UNICODE_TYPE_DELAY
#    define UNICODE_TYPE_DELAY 10
#endif
extern uint16_t tria_lang_keycodes_select[TRIA_MULTI_LANG_COUNT];
bool tria_unicode_switched = false;

void unicode_input_start(void) {
    unicode_saved_led_state = host_keyboard_led_state();

    // Note the order matters here!
    // Need to do this before we mess around with the mods, or else
    // UNICODE_KEY_LNX (which is usually Ctrl-Shift-U) might not work
    // correctly in the shifted case.
    if (unicode_config.input_mode == UNICODE_MODE_LINUX && unicode_saved_led_state.caps_lock) {
        tap_code(KC_CAPS_LOCK);
    }

    unicode_saved_mods = get_mods(); // Save current mods
    clear_mods();                    // Unregister mods to start from a clean state
    clear_weak_mods();

    switch (unicode_config.input_mode) {
        case UNICODE_MODE_MACOS:
            register_code(UNICODE_KEY_MAC);
            break;
        case UNICODE_MODE_LINUX:
            tap_code16(UNICODE_KEY_LNX);
            break;
        case UNICODE_MODE_WINDOWS:
            // For increased reliability, use numpad keys for inputting digits
            if (!unicode_saved_led_state.num_lock) {
                tap_code(KC_NUM_LOCK);
            }
            register_code(KC_LEFT_ALT);
            wait_ms(UNICODE_TYPE_DELAY);
            tap_code(KC_KP_PLUS);
            break;
        case UNICODE_MODE_WINCOMPOSE:
            if (tria_lang_current == TRIA_RU) {
                tria_unicode_switched = true;
                tap_code16(tria_lang_keycodes_select[TRIA_EN]);
            }
            tap_code(UNICODE_KEY_WINC);
            tap_code(KC_U);
            break;
        case UNICODE_MODE_EMACS:
            // The usual way to type unicode in emacs is C-x-8 <RET> then the unicode number in hex
            tap_code16(LCTL(KC_X));
            tap_code16(KC_8);
            tap_code16(KC_ENTER);
            break;
    }

    wait_ms(UNICODE_TYPE_DELAY);
}
void unicode_input_finish(void) {
    switch (unicode_config.input_mode) {
        case UNICODE_MODE_MACOS:
            unregister_code(UNICODE_KEY_MAC);
            break;
        case UNICODE_MODE_LINUX:
            tap_code(KC_SPACE);
            if (unicode_saved_led_state.caps_lock) {
                tap_code(KC_CAPS_LOCK);
            }
            break;
        case UNICODE_MODE_WINDOWS:
            unregister_code(KC_LEFT_ALT);
            if (!unicode_saved_led_state.num_lock) {
                tap_code(KC_NUM_LOCK);
            }
            break;
        case UNICODE_MODE_WINCOMPOSE:
            tap_code(KC_ENTER);
            break;
        case UNICODE_MODE_EMACS:
            tap_code16(KC_ENTER);
            break;
    }

    if (tria_unicode_switched) {
        tria_unicode_switched = false;
        tap_code16(tria_lang_keycodes_select[TRIA_RU]);
    }

    set_mods(unicode_saved_mods); // Reregister previously set mods
}