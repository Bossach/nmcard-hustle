
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


#if defined(MC12705)
#include "mc12705load.h"
#elif defined(NM_CARD)
#include "nm_card_load.h"
// #elif defined(NM_MEZZO)
// #include "nm_mezzo_load.h"
// #elif defined(NM_QUAD)
// #include "nm_quad_load.h"
// #elif defined(NEURO_MEZZANINE)
// #include "neuro_mezzanine_load.h"
#else
#error use -DMC12705 or -DNM_CARD
#endif

#include "io_host.h"




#ifndef NMC_BIN
#define NMC_BIN "nmc/hustle_nmc.abs"
#endif
#ifndef ARM_BIN
#define ARM_BIN "cluster/hustle_arm.elf"
#endif
#ifndef CARM_BIN
#define CARM_BIN "central/hustle_carm.elf"
#endif



#define RET_SUCCESS 0
#define RET_ERROR 1
#define RET_WRONG_ARG 2
#define RET_NO_BOARDS 3
#define RET_BOARD_FAIL 4
#define RET_ACCESS_FAIL 5
#define RET_LOAD_PROGRAM_FAIL 6
#define RET_PROGRAM_START_FAIL 7
#define RET_TIME_OUT 8
#define RET_IO_SERVICE_FAIL 9
#define RET_SIGNAL_EXIT 10


#define STATUS_NOT_USED 0
#define STATUS_USED 1
#define STATUS_DONE -1
#define STATUS_ERROR -2
#define STATUS_CLOSED -3



#define DBG if(debug)printf


typedef struct {
	int index;
	const char * shrt;
	const char * lng;
	int req_val;
	const char * help_str;
} arg;

static const int args_total = 8;
static arg arglist[] = {
	{0, "-h", "--help", 0, "Show help message\n"},
	{1, "-A", "--all-boards", 0, "Use all boards (disabled by default) (equivalent '-b All')\n"},
	{2, "-b", "--board", 1, "Specify board to use (0 by default), integer index or 'All' (equivalent '-A')\n"},
	{6, "-r", "--reset", 0, "Only reset board(s)\n"},
	{10, "-dw", "--dont-wait", 0, "Launch board part and leave it running infinitely\n\t\t\t\tuntil someone resets board or something else\n"},
	{12, "-t", "--timeout", 1, "Time in seconds that program will running (300 by default)\n\t\t\t\tHas no effect if '-dw' option is present\n\t\t\t\tIf zero runs infinitely\n\t\t\t\t\n"},
	{14, "-d", "--debug", 0, "Produce some debug output\n"},
	{16, "-noio", "--dont-use-io", 0, "Do not pass output from device\n"}
};


static int all_boards_in_use = 0;
static int board_index = 0;

static int wait_result = 1;
static int debug = 0;
static int use_io = 1;
static int only_reset = 0;

static int timeout_seconds = 300;

static volatile sig_atomic_t signal_is_catched_flag = 0;


int help_message(int retValue);
int ll_version(void);

int parse_int(char * str, int * result);
int find_arg(char * arg_str);
int parse_args(int argc, char * argv[]);

void signal_handler(int sig);
void close_everything_and_exit(int ret);


typedef struct{
	PL_Board *board;
	struct ProcessorUnit{
		PL_Access * access;
		PL_CoreNo core;
		IO_Service *io;
	} nm[16];
	struct ProcessorUnit cluster[4];
	struct ProcessorUnit central;
	int status;
} board_s;

static board_s * boards;
static int boardCount;




int main(int argc, char * argv[]){


	//parse args
	parse_args(argc, argv);

	DBG("DBG: In main\n");



	struct sigaction act;

	act.sa_handler = signal_handler;

	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);

	//board count


	if (PL_GetBoardCount(&boardCount) != PL_OK) {
		fprintf(stderr, "ERROR: Failed get count of boards!\n");
		close_everything_and_exit(RET_NO_BOARDS);
	}

	if ( (boardCount < 1) || (board_index >= boardCount) ) {
		fprintf(stderr, "ERROR: Failed find board!\n");
		close_everything_and_exit(RET_NO_BOARDS);
	}

	DBG("DBG:%d boards found\n",boardCount);


	//----boards
	boards = calloc(boardCount, sizeof(board_s));

	// // glob_... needed for cleaning on exit
	// glob_boardCount = boardCount;
	// glob_boards = boards;

	//cycle init board(s)
	for(int b = 0; b < boardCount; b++){
		if(all_boards_in_use || (b == board_index) ){
			boards[b].status = STATUS_USED;
			DBG("DBG:Board %d used\n", b);

				if (PL_GetBoardDesc(b, &boards[b].board) != PL_OK) {
					fprintf(stderr, "ERROR: Failed open board %d!\n", b);
					close_everything_and_exit(RET_BOARD_FAIL);
					// goto clean;
				}

				if (PL_ResetBoard(boards[b].board) != PL_OK) {
					fprintf(stderr, "ERROR: Failed reset board %d!\n", b);
					close_everything_and_exit(RET_BOARD_FAIL);
					// goto clean;
				} else {
					DBG("DBG:Board %d reseted\n", b);

				}

				if (PL_LoadInitCode(boards[b].board) != PL_OK) {
					fprintf(stderr, "ERROR: Failed load init code board %d!\n", b);
					close_everything_and_exit(RET_BOARD_FAIL);
					// goto clean;
				} else {
					DBG("DBG:Board %d inited\n", b);
				}



		} else {
			DBG("DBG:Board %d not used\n", b);
			boards[b].status = STATUS_NOT_USED;
		}
	}

	DBG("DBG: ------ All boards inited\n");


	if(only_reset){
		DBG("DBG: Exit due to 'only reset/init' option\n");
		close_everything_and_exit(RET_SUCCESS);
	}

	if(signal_is_catched_flag)
		close_everything_and_exit(RET_SIGNAL_EXIT);

	printf("[Launching hustle]...\n");

	//cycle access & load programs
	for(int b = 0; b < boardCount; b++){
		if(boards[b].status == STATUS_NOT_USED)
			continue;

		DBG("DBG:Board %d:\n", b);

		//cycle nms
		for(int i = 0; i < 16; i++){

			if(signal_is_catched_flag)
				close_everything_and_exit(RET_SIGNAL_EXIT);


			boards[b].nm[i].core.cluster_id = i / 4;
			// nm_core[b][i].cluster_id = i / 4;
			boards[b].nm[i].core.nm_id = i % 4;
			// nm_core[b][i].nm_id = i % 4;

			if (PL_GetAccess(boards[b].board, &boards[b].nm[i].core, &boards[b].nm[i].access) != PL_OK) {
				fprintf(stderr, "ERROR: Board %d [%d.%d]: Failed get access!\n", b, boards[b].nm[i].core.cluster_id, boards[b].nm[i].core.nm_id);
				close_everything_and_exit(RET_ACCESS_FAIL);
				// goto clean;
			}


			if (PL_LoadProgramFile(boards[b].nm[i].access, NMC_BIN) != PL_OK) {
				fprintf(stderr, "ERROR board %d [%d.%d]: Failed load program '%s'\n", b, boards[b].nm[i].core.cluster_id, boards[b].nm[i].core.nm_id, NMC_BIN);
				close_everything_and_exit(RET_LOAD_PROGRAM_FAIL);
				// goto clean;
			} else {
				// clock_gettime(CLOCK_REALTIME, &nm_start_time[b][i]);
				DBG("DBG:\t[%d.%d]: Program loaded\n", boards[b].nm[i].core.cluster_id, boards[b].nm[i].core.nm_id);
			}

		}



		//cycle arms
		for(int i = 0; i < 4; i++){
			
			if(signal_is_catched_flag)
				close_everything_and_exit(RET_SIGNAL_EXIT);

			//cluster get access & load programs
			boards[b].cluster[i].core.cluster_id = i;
			// cluster_core[b][i].cluster_id = i;
			boards[b].cluster[i].core.nm_id = -1;
			// cluster_core[b][i].nm_id = -1;

			if (PL_GetAccess(boards[b].board, &boards[b].cluster[i].core, &boards[b].cluster[i].access) != PL_OK) {
				fprintf(stderr, "ERROR: Board %d [%d.%d]: Failed get access!\n", b, boards[b].cluster[i].core.cluster_id, boards[b].cluster[i].core.nm_id);
				close_everything_and_exit(RET_ACCESS_FAIL);
				// goto clean;
			}


			if (PL_LoadProgramFile(boards[b].cluster[i].access, ARM_BIN) != PL_OK) {
				fprintf(stderr, "ERROR board %d [%d.%d]: Failed load program '%s'\n", b, boards[b].cluster[i].core.cluster_id, boards[b].cluster[i].core.nm_id, ARM_BIN);
				close_everything_and_exit(RET_LOAD_PROGRAM_FAIL);
				// goto clean;
			} else {
				// clock_gettime(CLOCK_REALTIME, &cluster_start_time[b][i]);
				DBG("DBG:\t[%d.%d]: Program loaded\n", boards[b].cluster[i].core.cluster_id, boards[b].cluster[i].core.nm_id);
			}


		}


		//central arm get access & load program
		boards[b].central.core.cluster_id = -1;
		boards[b].central.core.nm_id = -1;
		boards[b].central.io = NULL;

		if (PL_GetAccess(boards[b].board, &boards[b].central.core, &boards[b].central.access) != PL_OK) {
			fprintf(stderr, "ERROR: Board %d [%d.%d]: Failed get access!\n", b, boards[b].central.core.cluster_id, boards[b].central.core.nm_id);
			close_everything_and_exit(RET_ACCESS_FAIL);
			// goto clean;
		}


		if (PL_LoadProgramFile(boards[b].central.access, CARM_BIN) != PL_OK) {
			fprintf(stderr, "ERROR board %d [%d.%d]: Failed load program '%s'\n", b, boards[b].central.core.cluster_id, boards[b].central.core.nm_id, CARM_BIN);
			close_everything_and_exit(RET_LOAD_PROGRAM_FAIL);
			// goto clean;
		} else {
			// clock_gettime(CLOCK_REALTIME, &central_start_time[b]);
			DBG("DBG:\t[%d.%d]: Program loaded\n", boards[b].central.core.cluster_id, boards[b].central.core.nm_id);
		}

		if(use_io){
			int io_err = 0;
			// central_io[b] = NULL;
			boards[b].central.io = IO_ServiceStart(boards[b].central.access, NULL, NULL, &io_err);

			if (boards[b].central.io == NULL) {
				fprintf(stderr, "ERROR board %d [%d.%d]: Failed create IO_Service: IO_err = %d\n", b, boards[b].central.core.cluster_id, boards[b].central.core.nm_id, io_err);
				close_everything_and_exit(RET_IO_SERVICE_FAIL);
				// goto clean;
			} else {
				DBG("DBG:\t[%d.%d]: IO service started\n", boards[b].central.core.cluster_id, boards[b].central.core.nm_id);
			}
		}


		if(signal_is_catched_flag)
			close_everything_and_exit(RET_SIGNAL_EXIT);

	}

	DBG("DBG: ------ All cores loaded\n");

	printf("[Launced]\n");



	// DWORD len = 1024 * 1024 * 4; // 16 MBytes
	// PL_Word * host_buffer = (PL_Word *)calloc(sizeof(PL_Word),len);

	// if(host_buffer == NULL){
	// 	fprintf(stderr, "ALERT: HOST_BUFFER_IS NULL!!!!\n");
	// 	close_everything_and_exit(RET_ERROR);
	// }

	// int mem_ret;
	
	if(wait_result){
		if (timeout_seconds > 0){
			sleep(timeout_seconds);
			if(signal_is_catched_flag)
				close_everything_and_exit(RET_SIGNAL_EXIT);
		} else {
			for(;;){
				// mem_ret = PL_ReadMemBlock(boards[0].central.access, host_buffer, 0x380000, len);
				// printf("PL_WriteMemBlock returned %d\n", mem_ret);
				if(signal_is_catched_flag) {
					// free(host_buffer);
					close_everything_and_exit(RET_SIGNAL_EXIT);
				}
			}
		}
	}

	//----------

	DBG("DBG:Exit from main\n");

	close_everything_and_exit(RET_SUCCESS);
}


int help_message(int retValue){

	putchar('\n');
	for (int i = 0; i < args_total; i++)
	{
		printf("\t%s/%s %s\t: %s\n", arglist[i].shrt, arglist[i].lng, arglist[i].req_val?"[value]":"", arglist[i].help_str);
	}

	exit(retValue);
}

int find_arg(char * arg_str){
	for (int i = 0; i < args_total; i++)
	{
		if ( (strcmp(arg_str, arglist[i].shrt) == 0) || (strcmp(arg_str, arglist[i].lng) == 0) ){
			return arglist[i].index;
		}
	}
	return -1;
}

int parse_int(char * str, int * result){
	int len = strlen(str);
	if (len == 0) return -2;

	for (int i = 0; i < len; ++i)
	{
		if( (str[i] != '0') && 
			(str[i] != '1') && (str[i] != '2') && (str[i] != '3') && 
			(str[i] != '4') && (str[i] != '5') && (str[i] != '6') && 
			(str[i] != '7') && (str[i] != '8') && (str[i] != '9') 
			){
			return -1;
		}
	}
	*result = atoi(str);
	return 0;
}

int parse_args(int argc, char * argv[]){

	int arg_idx;
	int int_value;


	for (int arg_pos = 1; arg_pos < argc; arg_pos++){

		arg_idx = find_arg(argv[arg_pos]);
		// char * arg_value;

		switch(arg_idx){
		//--help
		case 0:
			help_message(0);
			break;
		//--all-boards
		case 1:
			all_boards_in_use = 1;
			break;
		//--board
		case 2:
			if(++arg_pos >= argc){
				fprintf(stderr, "Invalid option value: %s\nMust be positive integer or 'All'.\n\n", argv[arg_pos - 1]);
				help_message(RET_WRONG_ARG);
			}
			if( (parse_int(argv[arg_pos], &int_value) != 0) &&
			( strcmp(argv[arg_pos], "all") && strcmp(argv[arg_pos], "All") && strcmp(argv[arg_pos], "ALL") ) ){
				fprintf(stderr, "Invalid optiov value: %s %s\nMust be positive integer or 'All'.\n\n", argv[arg_pos - 1], argv[arg_pos]);
				help_message(RET_WRONG_ARG);
			}
			if( !strcmp(argv[arg_pos], "all") || !strcmp(argv[arg_pos], "All") || !strcmp(argv[arg_pos], "ALL") ){
				all_boards_in_use = 1;
			} else {
				board_index = int_value;
			}
			break;
		//--reset
		case 6:
			only_reset = 1;
			break;
		//--dont-wait
		case 10:
			wait_result = 0;
			break;
		//--timeout
		case 12:
			if(++arg_pos >= argc){
				fprintf(stderr, "Invalid option value: %s\nMust be positive integer.\n\n", argv[arg_pos - 1]);
				help_message(RET_WRONG_ARG);
			}
			if( parse_int(argv[arg_pos], &int_value) != 0 ){
				fprintf(stderr, "Invalid optiov value: %s %s\nMust be positive integer.\n\n", argv[arg_pos - 1], argv[arg_pos]);
				help_message(RET_WRONG_ARG);
			}
			timeout_seconds = int_value;
			break;
		case 14:
			debug = 1;
			break;
		//--dont-use-io
		case 16:
			use_io = 0;
			break;
		default:
			fprintf(stderr,"Unresolved option `%s`\n\n",argv[arg_pos]);
			help_message(RET_WRONG_ARG);

		}

		
	}

}


void signal_handler(int sig){

	struct sigaction ign;

	ign.sa_handler = SIG_IGN;

	sigaction(SIGINT, &ign, NULL);
	sigaction(SIGTERM, &ign, NULL);
	sigaction(SIGQUIT, &ign, NULL);

	signal_is_catched_flag = 1;

}


void close_everything_and_exit(int ret){

	DBG("DBG: Enter cleaning\n");

	if ( boards == NULL ){

		DBG("DBG: 'boards' is NULL\n");

	} else {


		for (int b = 0; b < boardCount; b++){

			// skip unused boards
			if ( (boards[b].status == STATUS_NOT_USED) || (boards[b].status == STATUS_CLOSED) )
				continue;

			// nm accesses
			for (int i = 0; i < 16; ++i){
				if (boards[b].nm[i].access)
					PL_CloseAccess(boards[b].nm[i].access);
			}

			// cluster arm accessess
			for (int i = 0; i < 4; ++i){
				if (boards[b].cluster[i].access)
					PL_CloseAccess(boards[b].cluster[i].access);
			}

			// central io
			if(use_io && (boards[b].central.io != NULL) )
				IO_ServiceStop(&boards[b].central.io, NULL);

			// central access
			if (boards[b].central.access)
				PL_CloseAccess(boards[b].central.access);
			
			// reset board
			if(wait_result){
				DBG("DBG: Cleaning reseting board\n");
				PL_ResetBoard(boards[b].board);
			} else {
				DBG("DBG: Cleaning leaving board working\n");
			}

			// close board
			PL_CloseBoardDesc(boards[b].board);
			boards[b].status = STATUS_CLOSED;
		}

		free(boards);
		boards = NULL;
	}

	DBG("DBG: Cleaning done\n");

	exit(ret);

}
