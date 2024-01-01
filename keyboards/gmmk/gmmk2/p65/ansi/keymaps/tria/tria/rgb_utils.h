#pragma once

#include "quantum.h"

#ifndef TRIA_TRANSPARENT_KEYCODES
#define TRIA_TRANSPARENT_KEYCODES KC_TRNS
#endif

#ifndef TRIA_CAPS_LOCK_INDICATORS
#define TRIA_CAPS_LOCK_INDICATORS KC_CAPS
#endif

#ifndef TRIA_NUM_LOCK_INDICATORS
#define TRIA_NUM_LOCK_INDICATORS KC_NUM
#endif



void init_tria_rgb_utils(void);

RGB rgb_matrix_get_color(int index);
HSV rgb_matrix_get_color_hsv(int index);
void rgb_matrix_set_color_hsv(int index, HSV hsv);

void rgb_matrix_set_color_by_keycode(uint8_t led_min, uint8_t led_max, uint8_t layer, bool (*is_keycode)(uint16_t), uint8_t red, uint8_t green, uint8_t blue);
void rgb_matrix_hsvshift_by_keycode(uint8_t led_min, uint8_t led_max, uint8_t layer, bool (*is_keycode)(uint16_t), int16_t shift);

bool is_keycode_transparent(uint16_t);
bool is_keycode_caps_lock_indicator(uint16_t);
bool is_keycode_num_lock_indicator(uint16_t);

HSV rgb_to_hsv(RGB rgb);