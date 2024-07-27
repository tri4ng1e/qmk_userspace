#pragma once
#include "tria/tap_dance.h"

/*
░░      ░░░  ░░░░  ░░       ░░░░      ░░░        ░░       ░░
▒  ▒▒▒▒  ▒▒  ▒▒▒▒  ▒▒  ▒▒▒▒  ▒▒  ▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒▒▒  ▒▒▒▒  ▒
▓  ▓▓▓▓▓▓▓▓  ▓▓▓▓  ▓▓       ▓▓▓▓      ▓▓▓      ▓▓▓▓  ▓▓▓▓  ▓
█  ████  ██  ████  ██  ███  █████████  ██  ████████  ████  █
██      ████      ███  ████  ███      ███        ██       ██

░░░░░░  ░░░░  ░░░      ░░░░      ░░░        ░░░      ░░░░░░░
▒▒▒▒▒▒   ▒▒   ▒▒  ▒▒▒▒  ▒▒  ▒▒▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒
▓▓▓▓▓▓        ▓▓  ▓▓▓▓  ▓▓  ▓▓▓   ▓▓▓▓▓  ▓▓▓▓▓  ▓▓▓▓▓▓▓▓▓▓▓▓
██████  █  █  ██        ██  ████  █████  █████  ████  ██████
██████  ████  ██  ████  ███      ███        ███      ███████
*/

#ifdef TAP_DANCES

// we are cursed enough already, take those helpers with you

// those are just to indicate the parameter pack
#define _1T
#define _1H
#define _2T
#define _2H
#define _3T
#define _3H

// and those to exchange long symbols for brackets
#define TD_PRESS(act)           act, TD_NO_ACTION
#define TD_RELEASE(act)         TD_NO_ACTION, act
#define TD_CODE(code)           register_code16(code), unregister_code16(code)
#define TD_FULL(press, release) press, release
#define TD_TWOFOLD(c1, c2)      TD_CODE(c1), TD_CODE(c1), TD_CODE(c2), TD_CODE(c2)
#define TD_IGNORE               TD_NO_ACTION,          TD_NO_ACTION

// Let the Cursed Magic begin!

// Expansion Magic : for those who like to use macros inside macros
#define __TD_EXPAND__(args) args
#define TD_TAPHOLD(...) __TD_EXPAND__(TD_TAPHOLD_E(__VA_ARGS__))
#define TD_DOUBLE(...)  __TD_EXPAND__(TD_DOUBLE_E (__VA_ARGS__))
#define TD_TRIPLE(...)  __TD_EXPAND__(TD_TRIPLE_E (__VA_ARGS__))

// Transformation Magic : for those who wants a different things from a single object

// First Transformation : functions and objects

#define TD_TAPHOLD_E(...) \
    TRIA_TD_CREATE_TAP_HOLD(__VA_ARGS__)
#define TD_DOUBLE_E(...) \
    TRIA_TD_CREATE_DOUBLE(__VA_ARGS__)
#define TD_TRIPLE_E(...) \
    TRIA_TD_CREATE(__VA_ARGS__)
// -- -- -- -- -- --
TAP_DANCES
// -- -- -- -- -- --
#undef TD_TAPHOLD_E
#undef TD_DOUBLE_E
#undef TD_TRIPLE_E


// Second Transformation : tap-dance array

#define TD_TAPHOLD_E(name, ...) TRIA_TD_ENTRY(name)
#define TD_DOUBLE_E(name,  ...) TRIA_TD_ENTRY(name)
#define TD_TRIPLE_E(name,  ...) TRIA_TD_ENTRY(name)
// -- -- -- -- -- --
tap_dance_action_t tap_dance_actions[] = {
    TAP_DANCES
};
// -- -- -- -- -- --
#undef TD_TAPHOLD_E
#undef TD_DOUBLE_E
#undef TD_TRIPLE_E

#else // no TAP_DANCES found, make some placeholders for QMK
tap_dance_action_t tap_dance_actions[] = {};
#endif
