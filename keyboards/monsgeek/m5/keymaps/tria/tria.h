#pragma once

#include QMK_KEYBOARD_H

#include "tria/key_tracker.h"
#include "tria/idle_timer.h"
#include "tria/rgb_utils.h"
#include "tria/multi_lang.h"
#include "tria/multi_lang_enru.h"

#include "getreuer/select_word.h"
#include "getreuer/sentence_case.h"

enum layers {
    L_MAIN,
    L_LANG,
    L_FN,
};

enum custom_keycodes {
  SELWORD = SAFE_RANGE,
  SNCASET,
};

#define SNDDEV LCTL(LGUI(KC_V))

enum combos {
  CMB_PREV,
  CMB_NEXT,
  CMB_PAUSE,
  CMB_RUS
};