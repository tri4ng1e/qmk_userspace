#include "tria/multi_lang.h"

uint8_t tria_lang_current = 0;
uint16_t tria_lang_keycodes_select[TRIA_MULTI_LANG_COUNT];

void tria_lang_set_keycode(uint8_t lang, uint16_t keycode) {
    tria_lang_keycodes_select[lang] = keycode;
}

__attribute__ ((weak))
void tria_lang_init_user(void) {}

// by default, react to Alt+Shift
__attribute__ ((weak))
uint8_t tria_lang_check_cycle_user(uint16_t keycode) {
    uint8_t mods = get_mods();
    if (
        (mods == MOD_BIT(KC_LALT) && keycode == KC_LSFT) ||
        (mods == MOD_BIT(KC_LSFT) && keycode == KC_LALT)
    ) {
        return TRIA_LANG_CYCLE;
    }
    return TRIA_LANG_IGNORE;
}

void tria_lang_init(void) {
    memset(tria_lang_keycodes_select, KC_NO, TRIA_MULTI_LANG_COUNT * sizeof(uint16_t));
    tria_lang_init_user();
}

uint8_t tria_lang_get(void) {
    return tria_lang_current;
}

void tria_lang_check_select(uint16_t keycode) {
    for (uint8_t i = 0; i < TRIA_MULTI_LANG_COUNT; i++) {
        if (tria_lang_keycodes_select[i] == KC_NO || tria_lang_keycodes_select[i] != keycode) {
            continue;
        }
        tria_lang_current = i;
        break;
    }
}

void tria_lang_cycle(void) {
    if (tria_lang_current == TRIA_MULTI_LANG_COUNT) {
        tria_lang_current = 0;
    }
    tria_lang_current++;
}
void tria_lang_rcycle(void) {
    if (tria_lang_current == 0) {
        tria_lang_current = TRIA_MULTI_LANG_COUNT - 1;
    }
    tria_lang_current--;
}

void tria_lang_check(uint16_t keycode, keyrecord_t* record) {
    if (record-> event.pressed) {
        tria_lang_check_select(keycode);
        switch (tria_lang_check_cycle_user(keycode)) {
            case TRIA_LANG_IGNORE:
                break;
            case TRIA_LANG_CYCLE:
                tria_lang_cycle(); break;
            case TRIA_LANG_RCYCLE:
                tria_lang_rcycle(); break;
        }
    }
}
