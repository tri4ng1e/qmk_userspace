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

// we are cursed enough, take those helpers with you
// those are just to indicate the parameter pack
#define _1T
#define _1H
#define _2T
#define _2H
// and those to exchange long symbols for brackets
#define TD_PRESS(act) act, TD_NO_ACTION
#define TD_CODE(code) register_code16(code), unregister_code16(code)
#define TD_IGNORE     TD_NO_ACTION,          TD_NO_ACTION

// Let the Cursed Magic begin!

// Expansion Magic : for those who like to use macros inside macros
#define __TD_EXPAND__(args) args
#define TD_DOUBLE(...) __TD_EXPAND__(TD_DOUBLE_E(__VA_ARGS__))

// Transformation Magic : for those who wants a different things from a single object

// First Transformation : functions and objects

#define TD_DOUBLE_E(name, _1_on, _1_off, _1h_on, _1h_off, _2_on, _2_off, _2h_on, _2h_off, ...) \
    TRIA_TD_CREATE_DOUBLE(name, _1_on, _1_off, _1h_on, _1h_off, _2_on, _2_off, _2h_on, _2h_off)
TAP_DANCES
#undef TD_DOUBLE_E


// Second Transformation : tap-dance array

#define TD_DOUBLE_E(name, ...) [name] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, name##_finished, name##_reset),

tap_dance_action_t tap_dance_actions[] = {
    TAP_DANCES
};

#undef TD_DOUBLE_E


#endif