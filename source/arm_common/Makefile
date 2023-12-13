

ARM_TOOL_PREFIX ?= arm-nm6408-eabihf-

BOARD ?= NM_CARD
BOARD_PATH ?= /usr/local/rc_module/board-nm_card/

ADD_CFLAGS	?=

export LC_ALL=C

ARM_CFLAGS :=-mcpu=cortex-a5 -mfpu=vfpv4-d16 -mfloat-abi=hard -marm
ARM_CFLAGS +=-std=gnu99 -O2 -Wall -nostartfiles -Wl,--wrap=exit -Wl,--wrap=main
ARM_CFLAGS +=-static -Wl,--omagic -Wl,--gc-sections # -Wl,-Map,$(NAME_PTR)_arm_part.map

ARM_CFLAGS +=$(ADD_CFLAGS)

ARM_GCC		:=$(ARM_TOOL_PREFIX)gcc
ARM_DISASM	:=$(ARM_TOOL_PREFIX)objdump -Ds

ARM_LIB_PATH ?= $(BOARD_PATH)/lib
ARM_INC_PATH ?= $(BOARD_PATH)/include

.PHONY: all
all: arm_dma.o

arm_dma.o: arm_dma.c
	$(ARM_GCC) $(ARM_CFLAGS) -c $(<) -o $(@)

.PHONY: clean
clean: 
	rm -f *.elf *.map *.dmp *.o

