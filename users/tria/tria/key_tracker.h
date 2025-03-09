#pragma once

#include "quantum.h"
#include "cimple_ring_buffer/cimple_ring_buffer.h"

#define TRIA_KEY_TRACKER_MAX_ENTRIES 32

#ifndef TRIA_KEY_TRACKER_REMOVE_STALE_RECORDS
#define TRIA_KEY_TRACKER_REMOVE_STALE_RECORDS true
#endif

#ifndef TRIA_KEY_TRACKER_MAX_TICK
#define TRIA_KEY_TRACKER_MAX_TICK 5000
#endif

#ifndef TRIA_KEY_TRACKER_HELD_MS
#define TRIA_KEY_TRACKER_HELD_MS 500
#endif

typedef enum {
    TRIA_KEY_STATE_PRESSED,
    TRIA_KEY_STATE_HELD,
    TRIA_KEY_STATE_RELEASED,
    TRIA_KEY_STATE_FADING
} tria_key_state_t;

typedef struct PACKED {
    uint8_t  x;      // LED x position
    uint8_t  y;      // LED y position
    uint8_t  index;  // LED index
    uint16_t tick;
    uint8_t  state;  // tria_key_state_t
} tria_tracker_item_t;

typedef enum {
    TRIA_KEY_EVENT_PRESSED,
    TRIA_KEY_EVENT_RELEASED,
    TRIA_KEY_EVENT_TICK
} tria_key_event_t;

extern ring_buffer_t tria_tracker_rb;
extern uint16_t tria_keys_held;

typedef void (*tria_key_event_callback_t)(tria_tracker_item_t *item, tria_key_event_t event, void *context);

bool tria_key_tracker_init(void);
void tria_key_tracker_process_event(uint16_t keycode, keyrecord_t *record);
void tria_key_tracker_process_tick(void);
bool tria_register_key_event_callback(tria_key_event_callback_t callback, void *context);
bool tria_unregister_key_event_callback(tria_key_event_callback_t callback);
