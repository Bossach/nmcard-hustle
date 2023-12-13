
#include <stdlib.h>
#include <string.h>

#include "nmc4_cp.h"

const ComunPort CP_TR[4] = {
    // CP0
    { .CNT = (volatile unsigned *)(CP0_BASE + TR_CNT),
      .ADDR = (volatile void **)(CP0_BASE + TR_ADDR),
      .CTL = (volatile unsigned *)(CP0_BASE + TR_CTL),
      .ST = (volatile unsigned *)(CP0_BASE + TR_ST),
      .INT_VEC = CP0_TR_INT_VEC,
      .INT_BIT = CP0_TR_INT_BIT },
    // CP1
    { .CNT = (volatile unsigned *)(CP1_BASE + TR_CNT),
      .ADDR = (volatile void **)(CP1_BASE + TR_ADDR),
      .CTL = (volatile unsigned *)(CP1_BASE + TR_CTL),
      .ST = (volatile unsigned *)(CP1_BASE + TR_ST),
      .INT_VEC = CP1_TR_INT_VEC,
      .INT_BIT = CP1_TR_INT_BIT },
    // CP2
    { .CNT = (volatile unsigned *)(CP2_BASE + TR_CNT),
      .ADDR = (volatile void **)(CP2_BASE + TR_ADDR),
      .CTL = (volatile unsigned *)(CP2_BASE + TR_CTL),
      .ST = (volatile unsigned *)(CP2_BASE + TR_ST),
      .INT_VEC = CP2_TR_INT_VEC,
      .INT_BIT = CP2_TR_INT_BIT },
    //CP3
    { .CNT = (volatile unsigned *)(CP3_BASE + TR_CNT),
      .ADDR = (volatile void **)(CP3_BASE + TR_ADDR),
      .CTL = (volatile unsigned *)(CP3_BASE + TR_CTL),
      .ST = (volatile unsigned *)(CP3_BASE + TR_ST),
      .INT_VEC = CP3_TR_INT_VEC,
      .INT_BIT = CP3_TR_INT_BIT }
};

const ComunPort CP_RC[4] = {
    // CP0
    { .CNT = (volatile unsigned *)(CP0_BASE + RC_CNT),
      .ADDR = (volatile void **)(CP0_BASE + RC_ADDR),
      .CTL = (volatile unsigned *)(CP0_BASE + RC_CTL),
      .ST = (volatile unsigned *)(CP0_BASE + RC_ST),
      .INT_VEC = CP0_RC_INT_VEC,
      .INT_BIT = CP0_RC_INT_BIT },
    // CP1
    { .CNT = (volatile unsigned *)(CP1_BASE + RC_CNT),
      .ADDR = (volatile void **)(CP1_BASE + RC_ADDR),
      .CTL = (volatile unsigned *)(CP1_BASE + RC_CTL),
      .ST = (volatile unsigned *)(CP1_BASE + RC_ST),
      .INT_VEC = CP1_RC_INT_VEC,
      .INT_BIT = CP1_RC_INT_BIT },
    // CP2
    { .CNT = (volatile unsigned *)(CP2_BASE + RC_CNT),
      .ADDR = (volatile void **)(CP2_BASE + RC_ADDR),
      .CTL = (volatile unsigned *)(CP2_BASE + RC_CTL),
      .ST = (volatile unsigned *)(CP2_BASE + RC_ST),
      .INT_VEC = CP2_RC_INT_VEC,
      .INT_BIT = CP2_RC_INT_BIT },
    //CP3
    { .CNT = (volatile unsigned *)(CP3_BASE + RC_CNT),
      .ADDR = (volatile void **)(CP3_BASE + RC_ADDR),
      .CTL = (volatile unsigned *)(CP3_BASE + RC_CTL),
      .ST = (volatile unsigned *)(CP3_BASE + RC_ST),
      .INT_VEC = CP3_RC_INT_VEC,
      .INT_BIT = CP3_RC_INT_BIT }
};

static int __rc_times[4] = { 1 };
static int __tr_times[4] = { 1 };
static int __rc_do_inf[4] = { 0 };
static int __tr_do_inf[4] = { 0 };
static unsigned __rc_size[4];
static unsigned __tr_size[4];
static void *__rc_dst[4];
static void *__tr_src[4];

int launch_cp_transfering(int port, void *src, unsigned size, int times)
{
    if (port < 0 || port > 3) {
        return ERR_TR_WRONG_PORT;
    }

    if (times == 0) {
        return 0;
    }

    if (times > 0) {
        __tr_do_inf[port] = 0;
        __tr_times[port] = times - 1; // sub first launch by this function
    } else /*if negative*/ {
        // run infinite
        __tr_do_inf[port] = 1;
        __tr_times[port] = -1; // make possibility to stop via __tr_do_inf[p] = 0 by hand
    }

    __tr_size[port] = size;
    __tr_src[port] = src;

    // interruption handler for re-launch on repeat
    if (__set_transfer_ihandler(port) != 0) {
        return ERR_TR_IHANDLER_SET_FAIL;
    }
    // launch transmit
    *CP_TR[port].CNT = __tr_size[port];
    *CP_TR[port].ADDR = __tr_src[port];
    *CP_TR[port].CTL = 1;

    return 0;
}

int launch_cp_recieving(int port, void *dst, unsigned size, int times)
{
    if (port < 0 || port > 3) {
        return ERR_RC_WRONG_PORT;
    }

    if (times == 0) {
        return 0;
    }

    if (times > 0) {
        __rc_do_inf[port] = 0;
        __rc_times[port] = times - 1; // sub first launch by this function
    } else /*if negative*/ {
        /* run infinite */
        __rc_do_inf[port] = 1;
        __tr_times[port] = -1; // make possibility to stop via __rc_do_inf[p] = 0 by hand
    }

    __rc_size[port] = size;
    __rc_dst[port] = dst;

    // interruption handler for re-launch on repeat
    if (__set_recieve_ihandler(port) != 0) {
        return ERR_TR_IHANDLER_SET_FAIL;
    }
    // set recieve
    *CP_RC[port].CNT = __rc_size[port];
    *CP_RC[port].ADDR = __rc_dst[port];
    *CP_RC[port].CTL = 1;

    return 0;
}

void __cp_tr_ihandler(int port)
{
    if (__tr_times[port] > 0 || __tr_do_inf[port]) {
        __tr_times[port]--;

        // restart transfering
        *CP_TR[port].CNT = __tr_size[port];
        *CP_TR[port].ADDR = __tr_src[port];
        *CP_TR[port].CTL = 1;
    }
}

void __cp_rc_ihandler(int port)
{
    // check restart condition
    if (__rc_times[port] > 0 || __rc_do_inf[port]) {
        __rc_times[port]--;

        // restart recieving
        *CP_RC[port].CNT = __rc_size[port];
        *CP_RC[port].ADDR = __rc_dst[port];
        *CP_RC[port].CTL = 1;
    }
}

int __set_recieve_ihandler(int port)
{
    // write interrupt vector
    switch (port) {
    case 0:
        memcpy((void *)CP_RC[port].INT_VEC, (void *)__cp0_prim_rc_ihandler, 8);
        break;
    case 1:
        memcpy((void *)CP_RC[port].INT_VEC, (void *)__cp1_prim_rc_ihandler, 8);
        break;
    case 2:
        memcpy((void *)CP_RC[port].INT_VEC, (void *)__cp2_prim_rc_ihandler, 8);
        break;
    case 3:
        memcpy((void *)CP_RC[port].INT_VEC, (void *)__cp3_prim_rc_ihandler, 8);
        break;
    }

    // allow correcponding interruption
    *INT_INCOME_CLR_REG = CP_RC[port].INT_BIT; // drop existing
    *INT_MASK_SET_REG = CP_RC[port].INT_BIT; // allow

    return 0;
}

int __set_transfer_ihandler(int port)
{
    // write interrupt vector
    switch (port) {
    case 0:
        memcpy((void *)CP_TR[port].INT_VEC, (void *)__cp0_prim_tr_ihandler, 8);
        break;
    case 1:
        memcpy((void *)CP_TR[port].INT_VEC, (void *)__cp1_prim_tr_ihandler, 8);
        break;
    case 2:
        memcpy((void *)CP_TR[port].INT_VEC, (void *)__cp2_prim_tr_ihandler, 8);
        break;
    case 3:
        memcpy((void *)CP_TR[port].INT_VEC, (void *)__cp3_prim_tr_ihandler, 8);
        break;
    }

    // allow correcponding interruption
    *INT_INCOME_CLR_REG = CP_TR[port].INT_BIT; // drop existing
    *INT_MASK_SET_REG = CP_TR[port].INT_BIT; // allow

    return 0;
}
