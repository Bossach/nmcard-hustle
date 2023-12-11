#!/bin/sh

export BOARD=MC12705
# export BOARD=NM_CARD
export BOARD_PATH=/usr/local/rc_module/board-mc12705/

export HOST_TOOL_PREFIX=
export ARM_TOOL_PREFIX=/home/user/nmStuff/arm-nm6408-eabihf/bin/arm-nm6408-eabihf-
export NMC_TOOL_PREFIX=nmc-

# export NMC_USE_LLVM=TRUE
# export NMC_LLVM_PREFIX=/home/user/nmStuff/llvm-nmc-install/bin/

export BILDIR=build/



# export CARM_IS_DUMMY=TRUE




make clear-builddir
make clean
make all