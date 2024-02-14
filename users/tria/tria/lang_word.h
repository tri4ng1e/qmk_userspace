#pragma once

#include "quantum.h"

void lang_word_activate(uint8_t index);
void lang_word_deactivate(void);
bool lang_word_is_active(void);

// weakly linked function that defines the end of lang word span
bool lang_word_check_user(uint16_t keycode);

// this function should be called inside `process_record_user`
bool lang_word_process(uint16_t keycode);