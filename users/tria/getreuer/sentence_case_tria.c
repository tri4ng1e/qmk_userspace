#include "getreuer/sentence_case.h"

extern bool tria_lang_russian;

char sentence_case_press_user_en(uint16_t keycode,
                              keyrecord_t* record,
                              uint8_t mods) {
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

char sentence_case_press_user_ru(uint16_t keycode,
                              keyrecord_t* record,
                              uint8_t mods) {
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

char sentence_case_press_user(uint16_t keycode,
                              keyrecord_t* record,
                              uint8_t mods) {
    if (tria_lang_russian) {
        return sentence_case_press_user_ru(keycode, record, mods);
    } else {
        return sentence_case_press_user_en(keycode, record, mods);
    }
}