#include "tria/key_tracker.h"
#include "tria/rgb_utils.h"

ring_buffer_t tria_tracker_rb;
uint16_t tria_keys_held = 0;
uint16_t tria_key_tracker_timer = 0;

#define TRIA_MAX_KEY_EVENT_CALLBACKS 8

typedef struct {
    tria_key_event_callback_t cb;
    void *context;
} tria_key_event_cb_entry_t;

static tria_key_event_cb_entry_t tria_key_event_callbacks[TRIA_MAX_KEY_EVENT_CALLBACKS];
static size_t tria_key_event_cb_count = 0;

bool tria_key_tracker_init(void) {
    return ring_buffer_init(&tria_tracker_rb, TRIA_KEY_TRACKER_MAX_ENTRIES, sizeof(tria_tracker_item_t));
}

// Predicate: remove items if their tick exceeds the max tick.
bool tria_key_tracker_stale_predicate(const void *item, void *context) {
    (void)context;
    return ((const tria_tracker_item_t*)item)->tick >= TRIA_KEY_TRACKER_MAX_TICK;
}

bool tria_register_key_event_callback(tria_key_event_callback_t cb, void *ctx) {
    if (tria_key_event_cb_count >= TRIA_MAX_KEY_EVENT_CALLBACKS) return false;
    tria_key_event_callbacks[tria_key_event_cb_count].cb = cb;
    tria_key_event_callbacks[tria_key_event_cb_count].context = ctx;
    tria_key_event_cb_count++;
    return true;
}

bool tria_unregister_key_event_callback(tria_key_event_callback_t cb) {
    for (size_t i = 0; i < tria_key_event_cb_count; i++) {
        if (tria_key_event_callbacks[i].cb == cb) {
            for (size_t j = i; j < tria_key_event_cb_count - 1; j++) {
                tria_key_event_callbacks[j] = tria_key_event_callbacks[j + 1];
            }
            tria_key_event_cb_count--;
            return true;
        }
    }
    return false;
}

static void tria_invoke_callbacks(tria_tracker_item_t *item, tria_key_event_t event) {
    for (size_t i = 0; i < tria_key_event_cb_count; i++) {
        if (tria_key_event_callbacks[i].cb) {
            tria_key_event_callbacks[i].cb(item, event, tria_key_event_callbacks[i].context);
        }
    }
}

void tria_key_tracker_process_event(uint16_t keycode, keyrecord_t *record) {
#if TRIA_KEY_TRACKER_REMOVE_STALE_RECORDS == true
    ring_buffer_remove_all(&tria_tracker_rb, tria_key_tracker_stale_predicate, NULL);
#endif

    uint8_t leds[TRIA_KEY_TRACKER_MAX_ENTRIES];
    uint8_t led_count = rgb_matrix_map_row_column_to_led(record->event.key.row, record->event.key.col, leds);
    if (led_count == 0) return;
    if (keycode >= QK_MOMENTARY && keycode <= QK_MOMENTARY_MAX) return;

    // Update any already tracked keys.
    for (uint8_t i = 0; i < tria_tracker_rb.count; i++) {
        for (uint8_t j = 0; j < led_count; j++) {
            if (leds[j] == NO_LED) continue;
            tria_tracker_item_t *item = (tria_tracker_item_t*)ring_buffer_get(&tria_tracker_rb, i);
            if (item->index == leds[j]
#if TRIA_KEY_TRACKER_REUSE_ENTRIES == false
                && (item->tick == 0 && !record->event.pressed)
#endif
            ) {
                item->tick = record->event.pressed ? 0 : 1;
                item->state = record->event.pressed ? TRIA_KEY_STATE_PRESSED : TRIA_KEY_STATE_RELEASED;
                leds[j] = NO_LED;
                tria_invoke_callbacks(item, record->event.pressed ? TRIA_KEY_EVENT_PRESSED : TRIA_KEY_EVENT_RELEASED);
                break;
            }
        }
    }

    uint8_t layer = get_highest_layer(layer_state);
    if (record->event.pressed) {
        if (tria_keys_held < (uint16_t)(-1)) tria_keys_held++;
        // Add new key entries.
        for (uint8_t i = 0; i < led_count; i++) {
            if (leds[i] == NO_LED) continue;
            if (rgb_led_to_keycode(layer, leds[i], true) != keycode) continue;
            tria_tracker_item_t *item = (tria_tracker_item_t*)ring_buffer_next_slot(&tria_tracker_rb);
            if (item) {
                item->x = g_led_config.point[leds[i]].x;
                item->y = g_led_config.point[leds[i]].y;
                item->tick = 0;
                item->index = leds[i];
                item->state = TRIA_KEY_STATE_PRESSED;
                tria_invoke_callbacks(item, TRIA_KEY_EVENT_PRESSED);
            }
        }
    } else {
        if (tria_keys_held > 0) tria_keys_held--;
        // On release, mark any just-pressed key as released.
        for (uint8_t i = 0; i < tria_tracker_rb.count; i++) {
            tria_tracker_item_t *item = (tria_tracker_item_t*)ring_buffer_get(&tria_tracker_rb, i);
            if (item->tick == 0) {
                item->tick = 1;
                item->state = TRIA_KEY_STATE_RELEASED;
                tria_invoke_callbacks(item, TRIA_KEY_EVENT_RELEASED);
            }
        }
    }
}

void tria_key_tracker_process_tick(void) {
    if (tria_key_tracker_timer == 0) {
        tria_key_tracker_timer = timer_read();
    }
    uint16_t elapsed = timer_elapsed(tria_key_tracker_timer);
    tria_key_tracker_timer = timer_read();

    size_t count = tria_tracker_rb.count;
    for (size_t i = 0; i < count; i++) {
        tria_tracker_item_t *item = (tria_tracker_item_t*)ring_buffer_get(&tria_tracker_rb, i);
        if (item->tick == 0 || item->tick > TRIA_KEY_TRACKER_MAX_TICK) continue;
        item->tick += elapsed;
        // Transition from PRESSED to HELD
        if (item->state == TRIA_KEY_STATE_PRESSED && item->tick > TRIA_KEY_TRACKER_HELD_MS) {
            item->state = TRIA_KEY_STATE_HELD;
        }
        tria_invoke_callbacks(item, TRIA_KEY_EVENT_TICK);
    }
}
