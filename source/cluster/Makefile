

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

HUSTLE_ARM ?= hustle_arm
DMA ?= ../arm_common/arm_dma

ifeq ($(ARM_IS_DUMMY), TRUE)
	ARM_CFLAGS += -DDUMMY
endif

.PHONY: all
all: $(HUSTLE_ARM)

$(HUSTLE_ARM).elf: arm_link.lds $(HUSTLE_ARM).c $(DMA).o
	$(ARM_GCC) $(ARM_CFLAGS) -I $(ARM_COMMON_DIR) -T$(<) $(DMA).o $(HUSTLE_ARM).c -o $(@) \
	-L $(ARM_LIB_PATH) -Wl,--whole-archive -lnm6408load_arm -Wl,--no-whole-archive

$(DMA).o: $(DMA).c $(DMA).h
	$(MAKE) -C $(@D) $(@F)

disasm: $(HUSTLE_ARM).elf
	$(ARM_DISASM) $(<) > $(HUSTLE_ARM).dmp

.PHONY: clean
clean: 
	rm -f *.elf *.map *.dmp *.o

