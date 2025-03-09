#include "tria/tap_dance.h"

/**
 * @brief Determine the current tap dance state based on the tap count and key status.
 *
 * This function distinguishes between tap and hold behaviors for single, double,
 * and triple tap actions. In case of an unsupported tap count, it returns TD_UNKNOWN.
 *
 * @param state Pointer to the current tap dance state.
 * @return td_state_t The resolved tap dance state.
 */
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        // For a single tap: if the key was interrupted or released, register a tap.
        // Otherwise, if still held, register a hold.
        if (state->interrupted || !state->pressed) {
            return TD_SINGLE_TAP;
        } else {
            return TD_SINGLE_HOLD;
        }
    } else if (state->count == 2) {
        // For double taps: use TD_DOUBLE_SINGLE_TAP if interrupted (two separate taps),
        // a hold if still pressed, otherwise a regular double tap.
        if (state->interrupted) {
            return TD_DOUBLE_SINGLE_TAP;
        } else if (state->pressed) {
            return TD_DOUBLE_HOLD;
        } else {
            return TD_DOUBLE_TAP;
        }
    } else if (state->count == 3) {
        // For triple taps: distinguish between tap and hold.
        if (state->interrupted || !state->pressed) {
            return TD_TRIPLE_TAP;
        } else {
            return TD_TRIPLE_HOLD;
        }
    } else {
        // Fallback: unexpected tap count.
        // Consider adding logging here if your environment supports it.
        return TD_UNKNOWN;
    }
}
