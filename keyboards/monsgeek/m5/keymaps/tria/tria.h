#pragma once

#include QMK_KEYBOARD_H

#define SNDDEV LCTL(LGUI(KC_V))

enum layers {
    L_MAIN,
    L_LANG,
    L_FN,
};

enum custom_keycodes {
  SELWORD = SAFE_RANGE,
  SNCASET,
};

enum combos {
  CMB_PREV,
  CMB_NEXT,
  CMB_PAUSE,
  CMB_RUS
};

bool tria_lang_russian;