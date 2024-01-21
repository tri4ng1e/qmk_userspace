#include "key_tracker.h"
#include "tria/rgb_utils.h"

tria_tracker_t g_tria_key_tracker;

uint16_t tria_key_tracker_timer = 0;

void process_tria_key_tracker(uint16_t keycode, keyrecord_t *record) {
    // clean up stale records
#if TRIA_KEY_TRACKER_REMOVE_STALE_RECORDS == true
    uint8_t pos;
    do {
        pos = UINT8_MAX;
        for (uint8_t i = 0; i < g_tria_key_tracker.count; i++) {
            if (g_tria_key_tracker.tick[i] >= TRIA_KEY_TRACKER_MAX_TICK) {
                pos = i;
                break;
            }
        }
        if (pos != UINT8_MAX) {
            uint8_t count = g_tria_key_tracker.count - pos + 1;
            memmove(&g_tria_key_tracker.x[pos], &g_tria_key_tracker.x[pos+1],     count * sizeof(g_tria_key_tracker.x[0]));
            memmove(&g_tria_key_tracker.y[pos], &g_tria_key_tracker.y[pos+1],     count * sizeof(g_tria_key_tracker.y[0]));
            memmove(&g_tria_key_tracker.tick[pos], &g_tria_key_tracker.tick[pos+1],   count * sizeof(g_tria_key_tracker.tick[0]));
            memmove(&g_tria_key_tracker.index[pos], &g_tria_key_tracker.index[pos+1], count * sizeof(g_tria_key_tracker.index[0]));
            g_tria_key_tracker.count--;
        }
    } while (pos != UINT8_MAX);
#endif

    uint8_t leds[TRIA_KEY_TRACKER_MAX_ENTRIES];
    uint8_t leds_count = rgb_matrix_map_row_column_to_led(record->event.key.row, record->event.key.col, leds);

    // skipping the track if the key does not have an LED
    if (leds_count == 0) {
      return;
    }

    // skipping the track if the keycode is actually a MO hotkey
    if (keycode >= QK_MOMENTARY && keycode <= QK_MOMENTARY_MAX) {
        return;
    }

    // is key already tracked?
    uint8_t leds_pos[TRIA_KEY_TRACKER_MAX_ENTRIES];
    uint8_t leds_found = 0;
    for (uint8_t i = 0; i < g_tria_key_tracker.count; i++) {
        for (uint8_t j = 0; j < leds_count; j++) {
            if (
                g_tria_key_tracker.index[i] == leds[j]
#if TRIA_KEY_TRACKER_REUSE_ENTRIES == false
                && g_tria_key_tracker.tick[i] == 0
#endif
            ) {
                leds_pos[leds_found++] = i;
                break;
            }
        }
    }

    uint8_t current_layer = get_highest_layer(layer_state);

    if (record->event.pressed) {
        if (leds_found == 0) {
            // overflow protection
            if (g_tria_key_tracker.count + leds_count > TRIA_KEY_TRACKER_MAX_ENTRIES) {
                memcpy(&g_tria_key_tracker.x[0],   &g_tria_key_tracker.x[leds_count],       TRIA_KEY_TRACKER_MAX_ENTRIES - leds_count);
                memcpy(&g_tria_key_tracker.y[0],   &g_tria_key_tracker.y[leds_count],       TRIA_KEY_TRACKER_MAX_ENTRIES - leds_count);
                memcpy(&g_tria_key_tracker.tick[0],  &g_tria_key_tracker.tick[leds_count], (TRIA_KEY_TRACKER_MAX_ENTRIES - leds_count) * 2); // 16 bit
                memcpy(&g_tria_key_tracker.index[0], &g_tria_key_tracker.index[leds_count], TRIA_KEY_TRACKER_MAX_ENTRIES - leds_count);
                g_tria_key_tracker.count = TRIA_KEY_TRACKER_MAX_ENTRIES - leds_count;
            }

            // add new LEDs
            uint8_t index = g_tria_key_tracker.count;
            for (uint8_t i = 0; i < leds_count; i++) {
                // do not add leds that are not the same keycode (i.e. pressed programatically and not present)
                if (rgb_led_to_keycode(current_layer, leds[i]) != keycode) {
                    continue;
                }
                g_tria_key_tracker.x[index]     = g_led_config.point[leds[i]].x;
                g_tria_key_tracker.y[index]     = g_led_config.point[leds[i]].y;
                g_tria_key_tracker.tick[index]  = 0;
                g_tria_key_tracker.index[index++] = leds[i];
                g_tria_key_tracker.count++;
            }
        } else {
            for (uint8_t i = 0; i < leds_found; i++) {
                g_tria_key_tracker.tick[leds_pos[i]] = 0;
            }
        }
    } else {
        // when key is released, set the tick to non-zero. This starts the handling
        for (uint8_t i = 0; i < leds_found; i++) {
            g_tria_key_tracker.tick[leds_pos[i]] = 1;
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

    for (uint8_t i = 0; i < g_tria_key_tracker.count; i++) {
        // do not handle if key is held
        if (g_tria_key_tracker.tick[i] == 0) {
            continue;
        }
        if (UINT16_MAX - time_elapsed < g_tria_key_tracker.tick[i]) {
            g_tria_key_tracker.count--;
            continue;
        }
        if (g_tria_key_tracker.tick[i] > TRIA_KEY_TRACKER_MAX_TICK) {
            continue;
        }
        g_tria_key_tracker.tick[i] += time_elapsed;
    }
}