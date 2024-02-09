
ifeq ($(strip $(TRIA_LANG_ENABLE)), yes)
    OPT_DEFS += -DTRIA_LANG_ENABLED
endif

SRC += tria/key_tracker.c
SRC += tria/idle_timer.c
SRC += tria/rgb_utils.c
SRC += tria/tap_dance.c

SRC += tria/multi_lang.c

ifeq ($(strip $(TRIA_LANG_USE_ENRU)), yes)
    SRC += tria/multi_lang_enru.c
endif

SRC += getreuer/select_word.c
SRC += getreuer/sentence_case.c