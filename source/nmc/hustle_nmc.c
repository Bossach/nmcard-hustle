
#include "nmc_load/nmc_load.h"
#include "nmc4_cp/nmc4_cp.h"

int main(int argc, char * argv[]){

	float vals[3] = {-128.1, 0.9998, 100.9};

	// fill banks
	for (int i = 0; i < 192; ++i)
	{

		*((float*)(IMB2 + i)) = vals[i % 3];
		*((float*)(IMB3 + i)) = vals[i % 3];
		*((float*)(IMB4 + i)) = vals[i % 3];

		*((float*)(IMB5 + i)) = vals[i % 3];
		*((float*)(IMB6 + i)) = vals[i % 3];
		*((float*)(IMB7 + i)) = vals[i % 3];
		// 1.31

	}

	// communication ports
	launch_cp_transfering(1, IMB2 + 0x80 , SEND_SIZE - 0x40, -1);
	launch_cp_recieving(1, IMB5 + 0x80 , SEND_SIZE - 0x40, -1);

	launch_cp_transfering(2, IMB3 + 0x80 , SEND_SIZE - 0x40, -1);
	launch_cp_recieving(2, IMB6 + 0x80 , SEND_SIZE - 0x40, -1);

	launch_cp_transfering(3, IMB4 + 0x80 , SEND_SIZE - 0x40, -1);
	launch_cp_recieving(3, IMB7 + 0x80 , SEND_SIZE - 0x40, -1);

	// FPU
	while(1){
		mul_vecs();
	}

}


