
#include "nmc_load/nmc_load.h"
#include "nmc4_cp/nmc4_cp.h"

int main(int argc, char * argv[]){

	// fill banks
	for (int i = 0; i < 192; ++i)
	{

		#define anum -99.3399
		#define bnum 100

		*((float*)(IMB2 + i)) = anum + bnum * (i % 3);
		*((float*)(IMB3 + i)) = anum + bnum * (i % 3);
		*((float*)(IMB4 + i)) = anum + bnum * (i % 3);

		*((float*)(IMB5 + i)) = anum + bnum * (i % 3);
		*((float*)(IMB6 + i)) = anum + bnum * (i % 3);
		*((float*)(IMB7 + i)) = anum + bnum * (i % 3);
		// 1.37

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


