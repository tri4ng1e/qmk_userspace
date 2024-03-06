#define TAP_DANCES                                                  \
TD_DOUBLE(TD_CLF13,                                                 \
	_1T	TD_PRESS(tria_lang_switch(TRIA_EN)),                        \
	_1H	TD_CODE (KC_CAPS),                                          \
	_2T	TD_PRESS(lang_word_toggle()),                               \
	_2H	TD_IGNORE                                                   \
)
