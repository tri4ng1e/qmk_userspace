#pragma once

#include QMK_KEYBOARD_H

#include "tria/key_tracker.h"
#include "tria/idle_timer.h"
#include "tria/rgb_utils.h"
#include "tria/tap_dance.h"
#include "tria/lang_word.h"
#include "tria/multi_lang.h"
#include "tria/multi_lang_enru.h"
#include "tria/turbo_click.h"

#include "getreuer/select_word.h"
#include "getreuer/sentence_case.h"

#define ___0___ KC_NO
#define COMMA ,

enum layers {
    L_MAIN,
    L_LANG,
    L_MOUS,
    L_UNC,
    L_FN,
};

enum custom_keycodes {
    SELWORD = SAFE_RANGE,
    SNCASET, // sentence case toggle
    TRBSEL,  // turbo click select
    TURBO,   // turbo click
};

enum combos {
    CMB_PREV,
    CMB_NEXT,
    CMB_PAUSE,
};

enum unicode_symbols {
    UC_ACNT = UC(0x0301), // ́
    UC_EMD  = UC(0x2014), // —
    UC_CHCK = UC(0x2713), // ✓
    UC_XMRK = UC(0x2717), // ✗
    UC_DEG  = UC(0x00b0), // °
    UC_ARRL = UC(0x2190), // ←
    UC_ARRU = UC(0x2191), // ↑
    UC_ARRR = UC(0x2192), // →
    UC_ARRD = UC(0x2193), // ↓
    UC_PLMN = UC(0x00b1), // ±
    UC_LEEQ = UC(0x2264), // ≤
    UC_GREQ = UC(0x2265), // ≥
};

enum tap_dances {
    TD_CLF13,
};

#define SNDDEV LCTL(LGUI(KC_V)) // open sound devices panel in Win11
#define T_CLF13 TD(TD_CLF13)    // tap-danced capslock / lang switch
#define TGMOUSE TG(L_MOUS)      // mouse layer toggle