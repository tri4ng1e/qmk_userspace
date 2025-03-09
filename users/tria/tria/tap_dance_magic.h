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

  This file provides helper macros for defining tap dance functions in a centralized way.
  It leverages the TRIA_TD_* macros defined in tria/tap_dance.h.
*/

#ifdef TAP_DANCES

// We are cursed enough already, take those helpers with you.

// Those are just to indicate the parameter pack order.
#define _1T // Single tap
#define _1H // Single hold
#define _2T // Double tap
#define _2H // Double hold
#define _3T // Triple tap
#define _3H // Triple hold

// And those to exchange long symbols for brackets.
// TD_PRESS   : Execute an action on press.
// TD_RELEASE : Execute an action on release.
// TD_CODE    : Send a key code and then unregister it.
// TD_IGNORE  : Ignore press and release action
// TD_TWOFOLD : Use same codes for pressing and holding for single and double taps.
#define TD_PRESS(act)           act, TD_NO_ACTION
#define TD_RELEASE(act)         TD_NO_ACTION, act
#define TD_CODE(code)           register_code16(code), unregister_code16(code)
#define TD_IGNORE               TD_NO_ACTION,          TD_NO_ACTION
#define TD_TWOFOLD(c1, c2)      TD_CODE(c1), TD_CODE(c1), TD_CODE(c2), TD_CODE(c2)


// Let the Cursed Magic begin!

// Expansion Magic : for those who like to use macros inside macros
#define __TD_EXPAND__(args) args
#define TD_TAPHOLD(...) __TD_EXPAND__(TD_TAPHOLD_E(__VA_ARGS__))
#define TD_DOUBLE(...)  __TD_EXPAND__(TD_DOUBLE_E (__VA_ARGS__))
#define TD_TRIPLE(...)  __TD_EXPAND__(TD_TRIPLE_E (__VA_ARGS__))

// Transformation Magic : for those who wants a different things from a single object

// First Transformation : functions and objects

#define TD_TAPHOLD_E(...) TRIA_TD_CREATE_TAP_HOLD(__VA_ARGS__)
#define TD_DOUBLE_E(...)  TRIA_TD_CREATE_DOUBLE(__VA_ARGS__)
#define TD_TRIPLE_E(...)  TRIA_TD_CREATE(__VA_ARGS__)
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
