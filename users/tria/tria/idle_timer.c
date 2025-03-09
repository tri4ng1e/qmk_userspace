#include "idle_timer.h"

#ifdef TRIA_KEY_TRACKER_ENABLED
#include "tria/key_tracker.h"
#endif

uint16_t tria_idle_timer = 0;
uint16_t tria_idle_seconds = 0;

void reset_tria_idle_timer(void) {
    tria_idle_timer = timer_read();
    tria_idle_seconds = 0;
}

uint32_t tria_seconds_idle(void) {
#ifdef TRIA_KEY_TRACKER_ENABLED
    return tria_keys_held ? 0 : tria_idle_seconds;
#else
    return tria_idle_seconds;
#endif
}

void process_tria_idle_timer(void) {
    uint16_t time_delta = timer_elapsed(tria_idle_timer);
    if (time_delta > 1000) {
        tria_idle_timer = timer_read();
        tria_idle_seconds += time_delta / 1000;
    }
}