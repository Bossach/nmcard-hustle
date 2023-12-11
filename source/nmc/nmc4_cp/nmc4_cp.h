

#ifndef __NM6408_CP_H_
#define __NM6408_CP_H_

// Launchs transferring using 'port' read from 'src' address 'size' 64-bit words and repeat whole 'times' times
// If 'times' < 0 do infinite times
// Returns 0 on success and non-0 on error
int launch_cp_transfering(int port, void * src, unsigned size, int times);


// Launchs recieving using 'port' write to 'dst' address 'size' 64-bit words and repeat whole 'times' times
// If 'times' < 0 do infinite times
// Returns 0 on success and non-0 on error
int launch_cp_recieving(int port, void * dst, unsigned size, int times);


typedef int nm_id;





#define ERR_RC_WRONG_PORT -6099
#define ERR_TR_WRONG_PORT -6098
#define ERR_RC_IHANDLER_SET_FAIL -5099
#define ERR_TR_IHANDLER_SET_FAIL -5098




#ifndef __NMC4_IMB
#define __NMC4_IMB

#define IMB0 (void *)0x00000
#define IMB1 (void *)0x04000
#define IMB2 (void *)0x08000
#define IMB3 (void *)0x0C000
#define IMB4 (void *)0x10000
#define IMB5 (void *)0x14000
#define IMB6 (void *)0x18000
#define IMB7 (void *)0x1C000

#endif

#define IMB_SIZE 0x4000


#define SEND_SIZE (IMB_SIZE/2 - 1)	// in 64-words




#define INT_INCOME_REG ((volatile unsigned*)0x20400)
#define INT_MASK_REG ((volatile unsigned*)0x20408)
#define INT_SOFT_REG ((volatile unsigned*)0x20418)

#define INT_INCOME_CLR_REG ((volatile unsigned *)0x00020404)
#define INT_MASK_SET_REG ((volatile unsigned *)0x0002040A)
#define INT_MASK_CLR_REG ((volatile unsigned *)0x0002040C)


#define CP0_TR_INT_VEC ((volatile void *)0xA0)
#define CP0_RC_INT_VEC ((volatile void *)0xA8)
#define CP1_TR_INT_VEC ((volatile void *)0xB0)
#define CP1_RC_INT_VEC ((volatile void *)0xB8)
#define CP2_TR_INT_VEC ((volatile void *)0xC0)
#define CP2_RC_INT_VEC ((volatile void *)0xC8)
#define CP3_TR_INT_VEC ((volatile void *)0xD0)
#define CP3_RC_INT_VEC ((volatile void *)0xD8)


#define CP0_TR_INT_BIT (1<<16)
#define CP0_RC_INT_BIT (1<<17)
#define CP1_TR_INT_BIT (1<<18)
#define CP1_RC_INT_BIT (1<<19)
#define CP2_TR_INT_BIT (1<<20)
#define CP2_RC_INT_BIT (1<<21)
#define CP3_TR_INT_BIT (1<<22)
#define CP3_RC_INT_BIT (1<<23)


#define CP0_BASE 0x00021800
#define CP1_BASE 0x00021C00
#define CP2_BASE 0x00022000
#define CP3_BASE 0x00022400

#define TR_CNT 0x0
#define TR_ADDR 0x2
#define TR_CTL 0xA
#define TR_ST 0xE

#define RC_CNT 0x10
#define RC_ADDR 0x12
#define RC_CTL 0x1A
#define RC_ST 0x1E



typedef struct{
	volatile unsigned * CNT;
	volatile void ** ADDR;
	volatile unsigned * CTL;
	volatile unsigned * ST;
	volatile void * INT_VEC;
	unsigned INT_BIT;
} ComunPort;



int __set_recieve_ihandler(int port);
int __set_transfer_ihandler(int port);

void __cp_tr_ihandler(int port);
void __cp_rc_ihandler(int port);

void __cp0_prim_rc_ihandler(void);
void __cp1_prim_rc_ihandler(void);
void __cp2_prim_rc_ihandler(void);
void __cp3_prim_rc_ihandler(void);


void __cp0_prim_tr_ihandler(void);
void __cp1_prim_tr_ihandler(void);
void __cp2_prim_tr_ihandler(void);
void __cp3_prim_tr_ihandler(void);




#endif