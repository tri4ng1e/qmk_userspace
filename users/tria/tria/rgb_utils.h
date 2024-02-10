#pragma once

#include "quantum.h"


void init_tria_rgb_utils(void);

uint16_t rgb_led_to_keycode(int8_t layer, uint16_t led, bool fallthrough);

RGB rgb_matrix_get_color(int index);
HSV rgb_matrix_get_color_hsv(int index);
void rgb_matrix_set_color_hsv(int index, HSV hsv);

void rgb_matrix_set_color_by_keycode_fn(uint8_t led_min, uint8_t led_max, uint8_t layer, bool (*is_keycode)(uint16_t), uint8_t red, uint8_t green, uint8_t blue);
void rgb_matrix_hsvshift_by_keycode_fn(uint8_t led_min, uint8_t led_max, uint8_t layer, bool (*is_keycode)(uint16_t), int16_t shift);

void rgb_matrix_set_color_by_keycode(uint8_t led_min, uint8_t led_max, uint8_t layer, uint16_t keycode, uint8_t red, uint8_t green, uint8_t blue);
void rgb_matrix_hsvshift_by_keycode(uint8_t led_min, uint8_t led_max, uint8_t layer, uint16_t keycode, int16_t shift);

bool tria_is_keycode_norgb(uint16_t);
bool tria_is_keycode_caps_indicator(uint16_t);
bool tria_is_keycode_num_indicator(uint16_t);

HSV rgb_to_hsv(RGB rgb);