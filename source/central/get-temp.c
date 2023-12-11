
#define TS_PWDN 0x0D1000
#define TS_START 0x0D1004
#define TS_DATA 0x0D1018


int get_temp(void){

	//turn on temp
	*(unsigned*)TS_PWDN = 0;
	*(unsigned*)TS_START = 1;

	unsigned value = *(unsigned*)TS_DATA;
	float result = (-0.000374421 * value + 2.27432) * value - 3277.72;
	return (int)result;

}

