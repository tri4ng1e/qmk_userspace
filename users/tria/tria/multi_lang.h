#pragma once

#include "quantum.h"

#ifndef TRIA_MULTI_LANG_COUNT
#define TRIA_MULTI_LANG_COUNT 2
#endif

enum getreuer_sentence_case_custom {
    SCASE_IGNORED = '\0',
    SCASE_LETTER  = 'a',
    SCASE_PUNCT   = '.',
    SCASE_SYMBOL  = '#',
    SCASE_QUOT    = '\'',
};

enum tria_lang_cycles {
    TRIA_LANG_CYCLE = 0,
    TRIA_LANG_RCYCLE,
    TRIA_LANG_IGNORE,
};

extern uint8_t tria_lang_current;

void tria_lang_init(void);
void tria_lang_set_keycode(uint8_t lang, uint16_t keycode);

void tria_lang_check(uint16_t keycode);
uint8_t tria_lang_get(void);

bool tria_lang_switch(uint8_t);