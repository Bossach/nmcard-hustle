


# MAIN MAKEFILE

export HOST_TOOL_PREFIX ?= 

export ARM_TOOL_PREFIX ?= arm-nm6408-eabihf-

export NMC_TOOL_PREFIX ?= nmc-
export NMC_USE_LLVM ?= FALSE
export NMC_LLVM_PREFIX ?= llvm-nmc/bin/

export BOARD ?= NM_CARD
export BOARD_PATH ?= /usr/local/rc_module/board-nm_card/

SRCDIR ?= ./source/
BILDIR ?= ./build/


########

VPATH := $(shell find $(SRCDIR) -type d | tr '\n' ':')

export NMC_IS_DUMMY ?= FALSE
export ARM_IS_DUMMY ?= FALSE
export CARM_IS_DUMMY ?= FALSE


export LAUNCHER ?= nmcard-hustle
export HUSTLE_NMC ?= hustle_nmc
export HUSTLE_ARM ?= hustle_arm
export HUSTLE_CARM ?= hustle_carm


export LNCH_BIN := launcher/$(LAUNCHER)
export NMC_BIN := nmc/$(HUSTLE_NMC).abs
export ARM_BIN := cluster/$(HUSTLE_ARM).elf
export CARM_BIN := central/$(HUSTLE_CARM).elf



.PHONY: all #$(BILDIR)/$(LAUNCHER) $(BILDIR)/$(ARM_BIN) $(BILDIR)/$(CARM_BIN) $(BILDIR)/$(NMC_BIN)
all: $(BILDIR)/$(LAUNCHER) $(BILDIR)/$(ARM_BIN) $(BILDIR)/$(CARM_BIN) $(BILDIR)/$(NMC_BIN)
	@echo [==== Building complete ====]


LAUNCHER_DEPS := $(LAUNCHER).c 

$(BILDIR)/$(LAUNCHER): $(LAUNCHER).c
	@echo [==== Building host part ====]
	$(MAKE) -C $(SRCDIR)/launcher/ $(@F)
	mkdir -p $(BILDIR)
	cp $(SRCDIR)/$(LNCH_BIN) $(@)
	@echo [==== Host part done ====]


NMC_DEPS := $(HUSTLE_NMC).c nmc_link.lds nmc4_cp.h nmc4_cp.c nmc_load.asm nmc_load.h

$(BILDIR)/$(NMC_BIN): $(NMC_DEPS)
	@echo [==== Building NMC part ====]
	$(MAKE) -C $(SRCDIR)/nmc/ $(@F)
	mkdir -p $(BILDIR)/nmc/
	cp $(SRCDIR)/$(NMC_BIN) $(@)
	@echo [==== NMC part done ====]

$(BILDIR)/$(ARM_BIN): $(HUSTLE_ARM).c arm_link.lds
	@echo [===== Building cluster part ====]
	$(MAKE) -C $(SRCDIR)/cluster/ $(@F)
	mkdir -p $(BILDIR)/cluster/
	cp $(SRCDIR)/$(ARM_BIN) $(@)
	@echo [==== Cluster part done ====]

$(BILDIR)/$(CARM_BIN): $(HUSTLE_CARM).c carm_link.lds get-temp.c
	@echo [==== Building central part ====]
	$(MAKE) -C $(SRCDIR)/central/ $(@F)
	mkdir -p $(BILDIR)/central/
	cp $(SRCDIR)/$(CARM_BIN) $(@)
	@echo [==== Central part done ====]


.PHONY: clean
clean:
	@echo [==== Cleaning ====]
	$(MAKE) -C $(SRCDIR)launcher/ clean
	$(MAKE) -C $(SRCDIR)nmc/ clean
	$(MAKE) -C $(SRCDIR)cluster/ clean
	$(MAKE) -C $(SRCDIR)central/ clean
	@echo [==== Cleaning done ====]

.PHONY: clear-builddir
clear-builddir:
	rm -f $(BILDIR)$(LAUNCHER)
	rm -f $(BILDIR)$(NMC_BIN)
	rm -f $(BILDIR)$(ARM_BIN)
	rm -f $(BILDIR)$(CARM_BIN)


