ifeq ($(strip $(TRIA_KEY_TRACKER_REUSE_ENTRIES)), yes)
    OPT_DEFS += -DTRIA_KEY_TRACKER_REUSE_ENTRIES=true
else
    OPT_DEFS += -DTRIA_KEY_TRACKER_REUSE_ENTRIES=false
endif

SRC += cimple_ring_buffer/cimple_ring_buffer.c

SRC += tria/key_tracker.c
SRC += tria/idle_timer.c
SRC += tria/rgb_utils.c
SRC += tria/tap_dance.c
SRC += tria/utils.c
SRC += tria/multi_lang.c

ifeq ($(strip $(TRIA_LANG_USE_ENRU)), yes)
    SRC += tria/multi_lang_enru.c
endif

SRC += getreuer/select_word.c
SRC += getreuer/sentence_case.c