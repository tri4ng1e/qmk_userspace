#include "idle_timer.h"

uint16_t tria_idle_timer = 0;
uint16_t tria_idle_seconds = 0;

void reset_tria_idle_timer(void) {
    tria_idle_timer = timer_read();
    tria_idle_seconds = 0;
}

uint32_t tria_seconds_idle(void) {
    return tria_idle_seconds;
}

void process_tria_idle_timer(void) {
    uint16_t time_delta = timer_elapsed(tria_idle_timer);
    if (time_delta > 1000) {
        tria_idle_timer = timer_read();
        tria_idle_seconds += time_delta / 1000;
    }
}