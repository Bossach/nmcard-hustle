

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
ARM_DISASM	:=$(ARM_TOOL_PREFIX)objdump -D

ARM_LIB_PATH ?= $(BOARD_PATH)/lib
ARM_INC_PATH ?= $(BOARD_PATH)/include

ARM_COMMON_DIR ?= ../arm_common

HUSTLE_CARM ?= hustle_carm
DMA ?= ../arm_common/arm_dma

ifeq ($(CARM_IS_DUMMY), TRUE)
	ARM_CFLAGS += -DDUMMY
endif

.PHONY: all
all: $(HUSTLE_CARM)

$(HUSTLE_CARM).elf: carm_link.lds $(HUSTLE_CARM).c get-temp.o $(DMA).o
	$(ARM_GCC) $(ARM_CFLAGS) -I $(ARM_INC_PATH) -I $(ARM_COMMON_DIR) -T$(<) \
	get-temp.o $(DMA).o $(HUSTLE_CARM).c -o $(@) \
	-L $(ARM_LIB_PATH) -Wl,--whole-archive -lnm6408load_arm -larm_io -Wl,--no-whole-archive

$(DMA).o: $(DMA).c $(DMA).h
	$(MAKE) -C $(@D) $(@F)

get-temp.o: get-temp.c
	$(ARM_GCC) $(ARM_CFLAGS) -c $(<) -o $(@)

disasm: $(HUSTLE_CARM).elf
	$(ARM_DISASM) $(<) > $(HUSTLE_CARM).dmp

.PHONY: clean
clean: 
	rm -f *.elf *.map *.dmp *.o

