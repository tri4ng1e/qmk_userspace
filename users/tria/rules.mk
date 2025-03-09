DEFERRED_EXEC_ENABLE = yes

SRC += tria/rgb_utils.c
SRC += tria/utils.c

ifeq ($(strip $(TAP_DANCE_ENABLE)), yes)
    SRC += tria/tap_dance.c
endif

ifeq ($(strip $(TRIA_IDLE_TIMER_ENABLE)), yes)
    OPT_DEFS += -DTRIA_IDLE_TIMER_ENABLED=1
    SRC += tria/idle_timer.c
endif

ifeq ($(strip $(TRIA_LANG_WORD_ENABLE)), yes)
    OPT_DEFS += -DTRIA_LANG_WORD_ENABLED=1
    SRC += tria/lang_word.c
endif

ifeq ($(strip $(TRIA_KEY_TRACKER_ENABLE)), yes)
    OPT_DEFS += -DTRIA_KEY_TRACKER_ENABLED=1
    SRC += cimple_ring_buffer/cimple_ring_buffer.c
    SRC += tria/key_tracker.c
    ifeq ($(strip $(TRIA_KEY_TRACKER_REUSE_ENTRIES)), yes)
        OPT_DEFS += -DTRIA_KEY_TRACKER_REUSE_ENTRIES=true
    else
        OPT_DEFS += -DTRIA_KEY_TRACKER_REUSE_ENTRIES=false
    endif
endif

ifeq ($(strip $(TRIA_TURBO_CLICK_ENABLE)), yes)
    OPT_DEFS += -DTRIA_TURBO_CLICK_ENABLED=1
    SRC += tria/turbo_click.c
endif

ifeq ($(strip $(TRIA_LANG_ENABLE)), yes)
    OPT_DEFS += -DTRIA_LANG_ENABLED=1
    SRC += tria/multi_lang.c

    ifeq ($(strip $(TRIA_LANG_USE_ENRU)), yes)
        SRC += tria/multi_lang_enru.c
    endif
endif

SRC += getreuer/select_word.c
SRC += getreuer/sentence_case.c