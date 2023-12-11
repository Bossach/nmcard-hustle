

ARM_TOOL_PREFIX ?= arm-nm6408-eabihf-

BOARD ?= NM_CARD
BOARD_PATH ?= /usr/local/rc_module/board-nm_card/

ADD_CFLAGS	?= 

export LC_ALL=C

ARM_CFLAGS :=-mcpu=cortex-a5 -mfpu=vfpv4-d16 -mfloat-abi=hard -marm
ARM_CFLAGS +=-std=gnu99 -O0 -nostartfiles -Wl,--wrap=exit -Wl,--wrap=main
ARM_CFLAGS +=-static -Wl,--omagic -Wl,--gc-sections # -Wl,-Map,$(NAME_PTR)_arm_part.map

ARM_CFLAGS +=$(ADD_CFLAGS)

ARM_GCC		:=$(ARM_TOOL_PREFIX)gcc
ARM_DISASM	:=$(ARM_TOOL_PREFIX)objdump -D

ARM_LIB_PATH ?= $(BOARD_PATH)/lib
ARM_INC_PATH ?= $(BOARD_PATH)/include

HUSTLE_CARM ?= hustle_carm

ifeq ($(CARM_IS_DUMMY), TRUE)
	ARM_CFLAGS += -DDUMMY
endif

.PHONY: all
all: $(HUSTLE_CARM)

$(HUSTLE_CARM).elf: carm_link.lds $(HUSTLE_CARM).c get-temp.o
	$(ARM_GCC) $(ARM_CFLAGS) -I $(ARM_INC_PATH) -T$(<) get-temp.o $(HUSTLE_CARM).c -o $(@) \
	-L $(ARM_LIB_PATH) -Wl,--whole-archive -lnm6408load_arm -larm_io -Wl,--no-whole-archive


get-temp.o: get-temp.c
	$(ARM_GCC) $(ARM_CFLAGS) -c $(<) -o $(@)

disasm: $(HUSTLE_CARM).elf
	$(ARM_DISASM) $(<) > $(HUSTLE_CARM).dmp

.PHONY: clean
clean: 
	rm -f *.elf *.map *.dmp *.o

