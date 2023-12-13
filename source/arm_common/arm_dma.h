
#ifndef __ARM_DMA_H
#define __ARM_DMA_H

#define DMA_SRC_ADDRESS 0x0BC004
#define DMA_DST_ADDRESS 0x0BC024
#define DMA_SRC_ADDR_MODE 0x0BC010
#define DMA_DST_ADDR_MODE 0x0BC030
#define DMA_MAIN_COUNTER 0x0BC000
#define DMA_CONTROL 0x0BC014
#define DMA_STATE 0x0BC03C

struct dma_regs {
    volatile unsigned *state;
    void **src_addr;
    void **dst_addr;
    volatile unsigned *counter;
    volatile unsigned *control;
};

unsigned dma_get_state(void);

void launch_dma(void *from, void *to, unsigned how_many);

#endif // __ARM_DMA_H
