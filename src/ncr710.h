/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype. 
 */

#ifndef _OS4_BLIZZPPC_NCR710_H_
#define _OS4_BLIZZPPC_NCR710_H_

#include <proto/exec.h>
#include <devices/scsidisk.h>
#include <sys/types.h>

#include "debug.h"

/**
 * NCR710 Register Map
 */
typedef struct {
	volatile uint8 NCR710_SCNTL0; /* 0x00 */
	volatile uint8 NCR710_SCNTL1;
	volatile uint8 NCR710_SDID;
	volatile uint8 NCR710_SIEN;

    volatile uint8 NCR710_SCID;   /* 0x04 */
    volatile uint8 NCR710_SXFER;
	volatile uint8 NCR710_SODL;
	volatile uint8 NCR710_SOCL;
	
	volatile uint8 NCR710_SBFR;   /* 0x08 */
	volatile uint8 NCR710_SIDL;
	volatile uint8 NCR710_SBDL;
	volatile uint8 NCR710_SBCL;

	volatile uint8 NCR710_DSTAT;  /* 0x0C */
	volatile uint8 NCR710_SSTAT0;
	volatile uint8 NCR710_SSTAT1;
	volatile uint8 NCR710_SSTAT2;

	volatile uint32 NCR710_DSA;   /* 0x10 */

	volatile uint8 NCR710_CTEST0; /* 0x14 */
	volatile uint8 NCR710_CTEST1;
	volatile uint8 NCR710_CTEST2;
	volatile uint8 NCR710_CTEST3;

	volatile uint8 NCR710_CTEST4; /* 0x18 */
	volatile uint8 NCR710_CTEST5;
	volatile uint8 NCR710_CTEST6;
	volatile uint8 NCR710_CTEST7;

	volatile uint8 NCR710_TEMP;   /* 0x1C */

	volatile uint8 NCR710_DFIFO;  /* 0x20 */
	volatile uint8 NCR710_ISTAT;
	volatile uint8 NCR710_CTEST8;
	volatile uint8 NCR710_LCRC;

	/* Tricky - DBC is a 24 bit register */
	volatile uint8 NCR710_DBC0;  /* 0x24 */
	volatile uint8 NCR710_DBC1;
	volatile uint8 NCR710_DBC2;
	volatile uint8 NCR710_DCMD;

	volatile uint8 NCR710_DNAD;  /* 0x28 */

	volatile uint8 NCR710_DSP;   /* 0x2C */

	volatile uint8 NCR710_DSPS;  /* 0x30 */

	volatile uint8 NCR710_SCRATCH; /* 0x34 */

	volatile uint8 NCR710_DMODE; /* 0x38 */
	volatile uint8 NCR710_DIEN;
	volatile uint8 NCR710_DWT;
	volatile uint8 NCR710_DCNTL;

	volatile uint8 NCR710_ADDER; /* 0x34 */
} NCR710_RegisterMap;

#endif
