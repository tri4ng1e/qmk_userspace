#pragma once

#include QMK_KEYBOARD_H

#include "tria/key_tracker.h"
#include "tria/idle_timer.h"
#include "tria/rgb_utils.h"
#include "tria/tap_dance.h"
#include "tria/multi_lang.h"
#include "tria/multi_lang_enru.h"

#include "getreuer/select_word.h"
#include "getreuer/sentence_case.h"

enum layers {
    L_MAIN,
    L_LANG,
    L_FN,
    L_UNC
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
    CMB_RUS,
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
    
};

enum tap_dances {
    TD_CLF13,
};
#define T_CLF13 TD(TD_CLF13)