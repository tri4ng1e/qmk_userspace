#pragma once

#include "quantum.h"

void tria_reset_idle_timer(void);
uint32_t tria_seconds_idle(void);
void tria_process_idle_timer(void);