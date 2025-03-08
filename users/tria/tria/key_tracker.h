#pragma once

#include "quantum.h"
#include "cimple_ring_buffer/cimple_ring_buffer.h"

#define TRIA_KEY_TRACKER_MAX_ENTRIES 32

#ifndef TRIA_KEY_TRACKER_REUSE_ENTRIES
#define TRIA_KEY_TRACKER_REUSE_ENTRIES false
#endif

#define TRIA_KEY_TRACKER_REMOVE_STALE_RECORDS true
#define TRIA_KEY_TRACKER_MAX_TICK 5000


typedef struct PACKED {
    uint8_t  x;
    uint8_t  y;
    uint8_t  index;
    uint16_t tick;
} tria_tracker_item_t;

typedef struct {
    uint8_t rb_index;
    uint8_t led_index;
} tria_tracker_index_pair_t;

extern ring_buffer_t tria_tracker_rb;
extern uint16_t tria_keys_held;

void tria_key_tracker_init(void);

void process_tria_key_tracker(uint16_t keycode, keyrecord_t *record);
void process_tria_key_tracker_tick(void);