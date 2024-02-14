#include "tria/lang_word.h"
#include "tria/multi_lang.h"

bool lang_word_state;
uint8_t lang_word_saved_index;

void lang_word_activate(uint8_t index) {
    lang_word_saved_index = tria_lang_get();
    lang_word_state = tria_lang_switch(index);
}
void lang_word_deactivate(void) {
    if (lang_word_state) {
        lang_word_state = false;
        tria_lang_switch(lang_word_saved_index);
    }
}
bool lang_word_is_active(void) {
    return lang_word_state;
}

__attribute__ ((weak))
bool lang_word_check_user(uint16_t keycode) {
    return keycode == KC_SPC;
}

bool lang_word_process(uint16_t keycode) {
    if (lang_word_is_active() && lang_word_check_user(keycode)) {
        lang_word_deactivate();
    }
    return false;
}