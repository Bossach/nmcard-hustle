

BOARD ?= NM_CARD
BOARD_PATH ?= /usr/local/rc_module/board-nm_card/


NMC_TOOL_PREFIX ?= nmc-
NMC_ADD_CFLAGS	?=


NMC_USE_LLVM ?= FALSE
NMC_LLVM_PREFIX ?= llvm-nmc/bin/

# ENTRY_POINT	?=--entry=start

override NMC_LLVM_FLAGS += -target neuromatrix -mnmc4-float
override NMC_GCC_FLAGS += -Xassembler -nmc4

ifeq ($(NMC_USE_LLVM),TRUE)
NMC_CC :=$(NMC_LLVM_PREFIX)clang
override NMC_CFLAGS += $(NMC_LLVM_FLAGS)
else
NMC_CC :=$(NMC_TOOL_PREFIX)gcc
override NMC_CFLAGS += $(NMC_GCC_FLAGS)
endif

NMC_ASM := $(NMC_TOOL_PREFIX)asm


# NMC_LLVM	:=/home/user/nmStuff/llvm-nmc-install/bin/clang

override NMC_CFLAGS +=-mgas -O2 -std=gnu99 -static

override NMC_LD_FLAGS += -nostartfiles -Wl,--wrap=_exit -Wl,--wrap=__main -Wl,--omagic -Wl,--gc-sections #-Wl,-Map,$(NAME_PTR)_part.map

override NMC_CFLAGS +=$(NMC_ADD_CFLAGS)

NMC_DISASM	:=$(NMC_TOOL_PREFIX)objdump -D


NMC_LIB_PATH = $(BOARD_PATH)/lib
NMC_INC_PATH = $(BOARD_PATH)/include


# CPLIB = ../nmc4_cp/libnmc4_cp.a


HUSTLE_NMC ?= hustle_nmc

VECLOAD ?= ../vec_load/nmc_load.o

N_CP ?= nmc4_cp/nmc4_cp
N_CP_ASM ?= nmc4_cp/nmc4_cp-asm


N_LOAD ?= nmc_load/nmc_load


.PHONY: all
all: $(HUSTLE_NMC).abs


$(HUSTLE_NMC).abs: nmc_link.lds $(HUSTLE_NMC).c $(N_CP).o $(N_CP_ASM).o $(N_LOAD).o
	$(NMC_CC) $(NMC_CFLAGS) $(NMC_LD_FLAGS) -T$(<) $(HUSTLE_NMC).c $(N_CP).o $(N_CP_ASM).o $(N_LOAD).o -o $(@) \
	-L $(NMC_LIB_PATH) -Wl,--whole-archive -lnm6408load_nmc -lnmc_io -Wl,--no-whole-archive


$(N_CP).o: $(N_CP).c $(N_CP).h
	$(NMC_CC) $(NMC_CFLAGS) -c $(<) -o $(@)

$(N_CP_ASM).o: $(N_CP).s
	$(NMC_ASM) -nmc4 -nm2ms -split_sir $(<) -o $(@)

$(N_LOAD).o: $(N_LOAD).asm $(N_LOAD).h
	$(NMC_ASM) -nmc4 -nm2ms -split_sir $(<) -o $(@)


.PHONY: clean
clean: 
	rm -f *.abs *.map *.dmp *.o *.ao
	rm -f nmc_load/*.o
	rm -f nmc4_cp/*.o

