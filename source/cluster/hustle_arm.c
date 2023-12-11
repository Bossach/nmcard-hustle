

#define DMA_SRC_ADDRESS 0x0BC004
#define DMA_DST_ADDRESS 0x0BC024
#define DMA_SRC_ADDR_MODE 0x0BC010
#define DMA_DST_ADDR_MODE 0x0BC030
#define DMA_MAIN_COUNTER 0x0BC000
#define DMA_CONTROL 0x0BC014
#define DMA_STATE 0x0BC03C

int main(int argc, char * argv[]){


#ifndef DUMMY

	while (1){
		if ( (*(volatile unsigned *)DMA_STATE == 0x2000000) || (*(volatile unsigned *)DMA_STATE == 0x0) ){
			*(volatile unsigned *)DMA_SRC_ADDRESS = 0x0000;
			*(volatile unsigned *)DMA_DST_ADDRESS = 0x380000;
			*(volatile unsigned *)DMA_MAIN_COUNTER = 0x8000;
			*(volatile unsigned *)DMA_CONTROL = 1;
		}
	}

#endif

}