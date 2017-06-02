/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype. 
 */

#ifndef _OS4_BLIZZPPC_DEVICE_H_
#define _OS4_BLIZZPPC_DEVICE_H_

#include <proto/exec.h>
#include <devices/scsidisk.h>
#include <devices/timer.h>
#include <sys/types.h>
#include <exec/interrupts.h>
#include <hardware/intbits.h>

#include "debug.h"

/**
 * Main device structure.
 */
typedef struct {
	struct Library       bpd_Library;    
	struct Task *        bpd_Task;          /* Worker task for the device */
	struct MsgPort *     bpd_CommandPort;   /* Message port to which commands are sent */
	struct MsgPort *     bpd_TimerPort;     /* Message port for use by internal timer */
	struct TimeRequest * bpd_TimeRequest;   /* TimeRequest for use by internal */
	struct Interrupt *   bpd_IntHandler;    /* Interrupt handler */
	int32                bpd_IntSignalBit;  /* Signal bit uset by interrupt handler */
} BlizzPPC_Device;


/**
 * Handy constants.
 */
enum {
	/* Worker task properties */
	BPPC_DEVICE_TASKPRI = 15,
	BPPC_DEVICE_STACK   = 65536,

	/* Harware properties */
	BPPC_NCR710_ADDR    = 0x00F40000,
	BPPC_NCR710_CLOCK   = 50
};

/**
 * To minimise ugly casting.
 */
typedef union {
	struct Message  * message;
	struct IOStdReq * ioRequest;
} IOReqMessage;

/**
 * Useful macros.
 */
#define BPPC_DEVICE_NAME "blizzppc.device"
#define NOP asm("eieio; isync")

/**
 * Shared globals.
 */
extern struct ExecIFace * IExec;

#endif
