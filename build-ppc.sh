#!/bin/sh


export BOARD=NM_CARD
export BOARD_PATH=/home/user/powerpcle-buildroot-linux-uclibc_sdk-buildroot/powerpcle-buildroot-linux-uclibc/sysroot/usr/local/rc_module/board-nm_card

export HOST_TOOL_PREFIX=/home/user/powerpcle-buildroot-linux-uclibc_sdk-buildroot/bin/powerpcle-linux-
export ARM_TOOL_PREFIX=/home/user/nmStuff/arm-nm6408-eabihf/bin/arm-nm6408-eabihf-
export NMC_TOOL_PREFIX=nmc-

# export NMC_USE_LLVM=TRUE
# export NMC_LLVM_PREFIX=/home/user/nmStuff/llvm-nmc-install/bin/

export BILDIR=ppc-build/



# export CARM_IS_DUMMY=TRUE




make clear-builddir
make clean
make all