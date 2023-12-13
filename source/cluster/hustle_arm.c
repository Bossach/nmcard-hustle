#include <stdio.h>
#include "arm_dma.h"

int get_temp(void);

int main(int argc, char *argv[])
{
    const unsigned src = 0x0000;
    const unsigned dst = 0x380000;
    const unsigned size = 0x8000;

#ifndef DUMMY

    while (1) {
        if ((dma_get_state() == 0x2000000) || (dma_get_state() == 0x0)) {
            launch_dma((void *)src, (void *)dst, size);
        }
    }

#endif
}