#pragma once

#include "quantum.h"

void reset_tria_idle_timer(void);
uint32_t tria_seconds_idle(void);
void process_tria_idle_timer(void);