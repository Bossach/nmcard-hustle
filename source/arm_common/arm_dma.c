#include "arm_dma.h"

static const struct dma_regs dma_r = {
    .state = (volatile unsigned *)DMA_STATE,
    .src_addr = (void **)DMA_SRC_ADDRESS,
    .dst_addr = (void **)DMA_DST_ADDRESS,
    .counter = (volatile unsigned *)DMA_MAIN_COUNTER,
    .control = (volatile unsigned *)DMA_CONTROL,
};

unsigned dma_get_state(void)
{
    return *dma_r.state;
}

void launch_dma(void *from, void *to, unsigned how_many)
{
    *dma_r.src_addr = from;
    *dma_r.dst_addr = to;
    *dma_r.counter = how_many;
    *dma_r.control = 1;
}