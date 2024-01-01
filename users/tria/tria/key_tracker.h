#pragma once

#include "quantum.h"

#define TRIA_KEY_TRACKER_MAX_ENTRIES 30

#define TRIA_KEY_TRACKER_REMOVE_STALE_RECORDS true
#define TRIA_KEY_TRACKER_MAX_TICK 5000

#define TRIA_KEY_TRACKER_REUSE_ENTRIES false

typedef struct PACKED {
    uint8_t  count;
    uint8_t  x[TRIA_KEY_TRACKER_MAX_ENTRIES];
    uint8_t  y[TRIA_KEY_TRACKER_MAX_ENTRIES];
    uint16_t tick[TRIA_KEY_TRACKER_MAX_ENTRIES];
    uint8_t  index[TRIA_KEY_TRACKER_MAX_ENTRIES];
} tria_tracker_t;

void process_tria_key_tracker(uint16_t keycode, keyrecord_t *record);
void process_tria_key_tracker_tick(void);