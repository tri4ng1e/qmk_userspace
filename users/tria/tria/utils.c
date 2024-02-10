#include "tria/utils.h"

int8_t get_previous_layer(int8_t current) {
    if (current <= 0) current = get_highest_layer(layer_state);

    // shift the mask to remove checked layers.
    uint32_t shifted = (layer_state | default_layer_state) & ((1U << current) - 1);
    if (shifted == 0) return -1; // no more set bits.

    // calculate the position of the highest set bit using count leading zeros.
    int layer = 31 - __builtin_clz(shifted);
    return layer;
}