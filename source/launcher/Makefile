
# BOARD ?= MC12705
BOARD ?= NM_CARD
BOARD_PATH ?= /usr/local/rc_module/board-nm_card/

HOST_TOOL_PREFIX ?= 
HOST_GCC = $(HOST_TOOL_PREFIX)gcc


HOST_ADD_CFLAGS ?=

override HOST_CFLAGS += $(HOST_ADD_CFLAGS)


ifeq ($(BOARD),MC12705)
	LIBLOAD := mc12705load
else ifeq ($(BOARD),NM_CARD)
	LIBLOAD := nm_card_load
endif

LIB_PATH = $(BOARD_PATH)/lib
INC_PATH = $(BOARD_PATH)/include

LAUNCHER ?= nmcard-hustle

NMC_BIN ?= nmc/$(HUSTLE_NMC).abs
ARM_BIN ?= cluster/$(HUSTLE_ARM).elf
CARM_BIN ?= central/$(HUSTLE_CARM).elf

override DEFINES += -D$(BOARD) -DNMC_BIN=\"$(NMC_BIN)\" -DARM_BIN=\"$(ARM_BIN)\" -DCARM_BIN=\"$(CARM_BIN)\" 


.PHONY: all
all:

$(LAUNCHER): $(LAUNCHER).c
	$(HOST_GCC) $(HOST_CFLAGS) $(<) $(DEFINES) -o $(@) -I$(INC_PATH) \
	-L$(LIB_PATH) -l$(LIBLOAD) -lio_host -lstdc++


.PHONY: clean
clean:
	rm -f $(LAUNCHER)