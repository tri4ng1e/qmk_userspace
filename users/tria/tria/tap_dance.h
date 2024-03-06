#pragma once

#include "quantum.h"

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

#define TD_NO_ACTION

td_state_t cur_dance(tap_dance_state_t *state);

#define TRIA_TD_CREATE(name, _1_on, _1_off, _1h_on, _1h_off, _2_on, _2_off, _2h_on, _2h_off, _3_on, _3_off, _3h_on, _3h_off) \
    td_tap_t name##_state ={.is_press_action = true, .state = TD_NONE}; \
    void name##_finished(tap_dance_state_t *state, void *user_data) { \
        name##_state.state = cur_dance(state); \
        switch (name##_state.state) { \
            case TD_SINGLE_TAP:        _1_on;  break; \
            case TD_SINGLE_HOLD:       _1h_on; break; \
            case TD_DOUBLE_TAP:        _2_on;  break; \
            case TD_DOUBLE_HOLD:       _2h_on; break; \
            case TD_DOUBLE_SINGLE_TAP: _1_on; _1_off; _1_on; break; \
            case TD_TRIPLE_TAP:        _3_on;  break; \
            case TD_TRIPLE_HOLD:       _3h_on; break; \
            default: break; \
        } \
    } \
    void name##_reset(tap_dance_state_t *state, void *user_data) { \
        switch (name##_state.state) { \
            case TD_SINGLE_TAP: case TD_DOUBLE_SINGLE_TAP: _1_off; break; \
            case TD_SINGLE_HOLD: _1h_off; break; \
            case TD_DOUBLE_TAP:  _2_off;  break; \
            case TD_DOUBLE_HOLD: _2h_off; break; \
            case TD_TRIPLE_TAP:  _3_off;  break; \
            case TD_TRIPLE_HOLD: _3h_off; break; \
            default: break; \
        } \
        name##_state.state = TD_NONE; \
    }

#define TRIA_TD_CREATE_DOUBLE(name, _1_on, _1_off, _1h_on, _1h_off, _2_on, _2_off, _2h_on, _2h_off) \
    td_tap_t name##_state ={.is_press_action = true, .state = TD_NONE}; \
    void name##_finished(tap_dance_state_t *state, void *user_data) { \
        name##_state.state = cur_dance(state); \
        switch (name##_state.state) { \
            case TD_SINGLE_TAP:        _1_on;  break; \
            case TD_SINGLE_HOLD:       _1h_on; break; \
            case TD_DOUBLE_TAP:        _2_on;  break; \
            case TD_DOUBLE_HOLD:       _2h_on; break; \
            case TD_DOUBLE_SINGLE_TAP: _1_on; _1_off; _1_on; break; \
            default: break; \
        } \
    } \
    void name##_reset(tap_dance_state_t *state, void *user_data) { \
        switch (name##_state.state) { \
            case TD_SINGLE_TAP: case TD_DOUBLE_SINGLE_TAP: _1_off; break; \
            case TD_SINGLE_HOLD: _1h_off; break; \
            case TD_DOUBLE_TAP:  _2_off;  break; \
            case TD_DOUBLE_HOLD: _2h_off; break; \
            default: break; \
        } \
        name##_state.state = TD_NONE; \
    }

#define TRIA_TD_CREATE_TAP_HOLD(name, _1_on, _1_off, _1h_on, _1h_off) \
    td_tap_t name##_state ={.is_press_action = true, .state = TD_NONE}; \
    void name##_finished(tap_dance_state_t *state, void *user_data) { \
        name##_state.state = cur_dance(state); \
        switch (name##_state.state) { \
            case TD_SINGLE_TAP:        _1_on;  break; \
            case TD_SINGLE_HOLD:       _1h_on; break; \
            case TD_DOUBLE_SINGLE_TAP: _1_on; _1_off; _1_on; break; \
            default: break; \
        } \
    } \
    void name##_reset(tap_dance_state_t *state, void *user_data) { \
        switch (name##_state.state) { \
            case TD_SINGLE_TAP: case TD_DOUBLE_SINGLE_TAP: _1_off; break; \
            case TD_SINGLE_HOLD: _1h_off; break; \
            default: break; \
        } \
        name##_state.state = TD_NONE; \
    }

#define TRIA_TD_ENTRY(name) [name] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, name##_finished, name##_reset),
