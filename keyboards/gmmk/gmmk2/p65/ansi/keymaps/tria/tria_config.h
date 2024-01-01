#pragma once

#include "quantum.h"

// RGB_MATRIX idle settings
bool     tria_idle_enabled    = true;
uint32_t tria_idle_timeout    = 10;    // s
uint8_t  tria_idle_brightness = 50; 
uint16_t tria_idle_off_time   = 1000;  // ms
uint16_t tria_idle_on_time    = 100;   // ms

HSV     tria_rgb_default_color = {110,255,200};
uint8_t tria_rgb_default_speed = 120;