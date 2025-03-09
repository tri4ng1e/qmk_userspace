#pragma once

#include "quantum.h"

/**
 * Enumeration for tap dance states.
 */
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Two individual single taps.
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

/**
 * Structure to store tap dance state and press-action flag.
 */
typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

#define TD_NO_ACTION /* Placeholder for no action */

/**
 * Function prototype for resolving tap dance state.
 * Implementation provided in tap_dance.c.
 */
td_state_t cur_dance(tap_dance_state_t *state);

/**
 * Macro to create a tap dance function handling triple tap scenarios.
 *
 * Parameters:
 * - name            : Unique base name for this tap dance.
 * - _1_on, _1_off   : Actions for single tap press and release.
 * - _1h_on, _1h_off : Actions for single hold press and release.
 * - _2_on, _2_off   : Actions for double tap press and release.
 * - _2h_on, _2h_off : Actions for double hold press and release.
 * - _3_on, _3_off   : Actions for triple tap press and release.
 * - _3h_on, _3h_off : Actions for triple hold press and release.
 */
#define TRIA_TD_CREATE(name, _1_on, _1_off, _1h_on, _1h_off,               \
                         _2_on, _2_off, _2h_on, _2h_off,                   \
                         _3_on, _3_off, _3h_on, _3h_off)                   \
    td_tap_t name##_state = { .is_press_action = true, .state = TD_NONE }; \
    void name##_finished(tap_dance_state_t *state, void *user_data) {      \
        name##_state.state = cur_dance(state);                             \
        switch (name##_state.state) {                                      \
            case TD_SINGLE_TAP:        _1_on;  break;                      \
            case TD_SINGLE_HOLD:       _1h_on; break;                      \
            case TD_DOUBLE_TAP:        _2_on;  break;                      \
            case TD_DOUBLE_HOLD:       _2h_on; break;                      \
            case TD_DOUBLE_SINGLE_TAP: _1_on; _1_off; _1_on; break;        \
            case TD_TRIPLE_TAP:        _3_on;  break;                      \
            case TD_TRIPLE_HOLD:       _3h_on; break;                      \
            default: break;                                                \
        }                                                                  \
    }                                                                      \
    void name##_reset(tap_dance_state_t *state, void *user_data) {         \
        switch (name##_state.state) {                                      \
            case TD_SINGLE_TAP:                                            \
            case TD_DOUBLE_SINGLE_TAP:                                     \
                _1_off; break;                                             \
            case TD_SINGLE_HOLD: _1h_off; break;                           \
            case TD_DOUBLE_TAP :  _2_off; break;                           \
            case TD_DOUBLE_HOLD: _2h_off; break;                           \
            case TD_TRIPLE_TAP :  _3_off; break;                           \
            case TD_TRIPLE_HOLD: _3h_off; break;                           \
            default: break;                                                \
        }                                                                  \
        name##_state.state = TD_NONE;                                      \
    }

/**
 * Macro to create a tap dance function for double tap scenarios.
 */
#define TRIA_TD_CREATE_DOUBLE(name, _1_on, _1_off, _1h_on, _1h_off,        \
                              _2_on, _2_off, _2h_on, _2h_off)              \
    td_tap_t name##_state = { .is_press_action = true, .state = TD_NONE }; \
    void name##_finished(tap_dance_state_t *state, void *user_data) {      \
        name##_state.state = cur_dance(state);                             \
        switch (name##_state.state) {                                      \
            case TD_SINGLE_TAP:        _1_on;  break;                      \
            case TD_SINGLE_HOLD:       _1h_on; break;                      \
            case TD_DOUBLE_TAP:        _2_on;  break;                      \
            case TD_DOUBLE_HOLD:       _2h_on; break;                      \
            case TD_DOUBLE_SINGLE_TAP: _1_on; _1_off; _1_on; break;        \
            default: break;                                                \
        }                                                                  \
    }                                                                      \
    void name##_reset(tap_dance_state_t *state, void *user_data) {         \
        switch (name##_state.state) {                                      \
            case TD_SINGLE_TAP:                                            \
            case TD_DOUBLE_SINGLE_TAP:                                     \
                _1_off; break;                                             \
            case TD_SINGLE_HOLD: _1h_off; break;                           \
            case TD_DOUBLE_TAP :  _2_off; break;                           \
            case TD_DOUBLE_HOLD: _2h_off; break;                           \
            default: break;                                                \
        }                                                                  \
        name##_state.state = TD_NONE;                                      \
    }

/**
 * Macro to create a tap dance function for single tap-hold scenarios.
 */
#define TRIA_TD_CREATE_TAP_HOLD(name, _1_on, _1_off, _1h_on, _1h_off)      \
    td_tap_t name##_state = { .is_press_action = true, .state = TD_NONE }; \
    void name##_finished(tap_dance_state_t *state, void *user_data) {      \
        name##_state.state = cur_dance(state);                             \
        switch (name##_state.state) {                                      \
            case TD_SINGLE_TAP:        _1_on;  break;                      \
            case TD_SINGLE_HOLD:       _1h_on; break;                      \
            case TD_DOUBLE_SINGLE_TAP: _1_on; _1_off; _1_on; break;        \
            default: break;                                                \
        }                                                                  \
    }                                                                      \
    void name##_reset(tap_dance_state_t *state, void *user_data) {         \
        switch (name##_state.state) {                                      \
            case TD_SINGLE_TAP:                                            \
            case TD_DOUBLE_SINGLE_TAP:                                     \
                _1_off; break;                                             \
            case TD_SINGLE_HOLD: _1h_off; break;                           \
            default: break;                                                \
        }                                                                  \
        name##_state.state = TD_NONE;                                      \
    }

/**
 * Macro to create an entry for the tap dance actions array.
 */
#define TRIA_TD_ENTRY(name) [name] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, name##_finished, name##_reset),
