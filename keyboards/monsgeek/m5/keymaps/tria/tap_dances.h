#define TAP_DANCES                                                  \
	TD_DOUBLE(TD_CLF13,                                             \
	_1T	TD_PRESS(tap_code16(KC_F13) COMMA tria_lang_check(KC_F13)), \
	_1H	TD_CODE (KC_CAPS),                                          \
	_2T	TD_PRESS(lang_word_toggle()),                               \
	_2H	TD_IGNORE                                                   \
	)
