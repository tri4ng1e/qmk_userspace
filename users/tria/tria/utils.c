#include "tria/utils.h"

uint8_t tria_saved_mods;

int8_t get_previous_layer(int8_t current) {
    if (current <= 0) current = get_highest_layer(layer_state);

    // shift the mask to remove checked layers.
    uint32_t shifted = (layer_state | default_layer_state) & ((1U << current) - 1);
    if (shifted == 0) return -1; // no more set bits.

    // calculate the position of the highest set bit using count leading zeros.
    int layer = 31 - __builtin_clz(shifted);
    return layer;
}

void tria_save_mods(void) {
    tria_saved_mods = get_mods(); // Save current mods
    clear_mods();                    // Unregister mods to start from a clean state
    clear_weak_mods();
}

void tria_restore_mods(void) {
    set_mods(tria_saved_mods); // Reregister previously set mods
}