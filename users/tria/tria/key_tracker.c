#include "key_tracker.h"
#include "tria/rgb_utils.h"

ring_buffer_t tria_tracker_rb;

uint16_t tria_key_tracker_timer = 0;

void tria_key_tracker_init() {
    ring_buffer_init(&tria_tracker_rb, TRIA_KEY_TRACKER_MAX_ENTRIES, sizeof(tria_tracker_item_t));
}

bool tria_key_tracker_stale_predicate(const void *item, void *context) {
    return ((const tria_tracker_item_t*)item)->tick >= TRIA_KEY_TRACKER_MAX_TICK;
}

void process_tria_key_tracker(uint16_t keycode, keyrecord_t *record) {
    // clean up stale records
#if TRIA_KEY_TRACKER_REMOVE_STALE_RECORDS == true
    ring_buffer_remove_all(&tria_tracker_rb, tria_key_tracker_stale_predicate, NULL);
#endif

    uint8_t leds[TRIA_KEY_TRACKER_MAX_ENTRIES];
    uint8_t leds_count = rgb_matrix_map_row_column_to_led(record->event.key.row, record->event.key.col, leds);

    // don't track if the key does not have an LED
    if (leds_count == 0) {
      return;
    }

    // don't track if the keycode is a MO hotkey
    if (keycode >= QK_MOMENTARY && keycode <= QK_MOMENTARY_MAX) {
        return;
    }

    // is key already tracked?
    for (uint8_t i = 0; i < tria_tracker_rb.count; i++) {
        for (uint8_t j = 0; j < leds_count; j++) {
            if (leds[j] == NO_LED) {
                continue;
            }
            tria_tracker_item_t *item = (tria_tracker_item_t*)ring_buffer_get(&tria_tracker_rb, i);
            if (
                item->index == leds[j]
#if TRIA_KEY_TRACKER_REUSE_ENTRIES == false
                && (item->tick == 0 && !record->event.pressed) 
#endif
            ) {
                item->tick = record->event.pressed? 0 : 1;
                leds[j] = NO_LED;
                break;
            }
        }
    }

    uint8_t current_layer = get_highest_layer(layer_state);

    if (record->event.pressed) {
        // add new LEDs
        for (uint8_t i = 0; i < leds_count; i++) {
            if (leds[i] == NO_LED) {
                continue;
            }
            // do not add leds that are not the same keycode (i.e. pressed programatically and not present)
            if (rgb_led_to_keycode(current_layer, leds[i], true) != keycode) {
                continue;
            }
            tria_tracker_item_t *item = (tria_tracker_item_t*)ring_buffer_next_slot(&tria_tracker_rb);
            item->x = g_led_config.point[leds[i]].x;
            item->y = g_led_config.point[leds[i]].y;
            item->tick = 0;
            item->index = leds[i];
        }
    }
}

void process_tria_key_tracker_tick() {
    // init timer
    if (tria_key_tracker_timer == 0) {
        tria_key_tracker_timer = timer_read();
    }

    uint16_t time_elapsed = timer_elapsed(tria_key_tracker_timer);
    tria_key_tracker_timer = timer_read();

    for (uint8_t i = 0; i < tria_tracker_rb.count; i++) {
        tria_tracker_item_t *item = (tria_tracker_item_t*)ring_buffer_get(&tria_tracker_rb, i);
        if (item->tick == 0 || item->tick > TRIA_KEY_TRACKER_MAX_TICK) {
            continue;
        }
        item->tick += time_elapsed;
    }
}