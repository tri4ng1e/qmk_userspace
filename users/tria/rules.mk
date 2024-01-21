
ifeq ($(strip $(TRIA_LANG_ENABLE)), yes)
    OPT_DEFS += -DTRIA_LANG_ENABLED
endif

SRC += tria/key_tracker.c
SRC += tria/idle_timer.c
SRC += tria/rgb_utils.c

SRC += getreuer/select_word.c
SRC += getreuer/sentence_case.c

ifeq ($(strip $(TRIA_LANG_ENABLE)), yes)
    SRC += getreuer/sentence_case_tria.c
endif