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

#define _BYTE_RW volatile uint8
#define _LONG_RW volatile uint32
#define _BYTE_RO const volatile uint8
#defome _LONG_RO const volatile uint32

typedef struct {
	_BYTE_RW NCR710_SCNTL0; /* 0x00 : SCSI Control Zero R/W */
	_BYTE_RW NCR710_SCNTL1; /* 0x01 : SCSI Control One R/W */
	_BYTE_RW NCR710_SDID;   /* 0x02 : SCSI Destination ID R/W */
	_BYTE_RW NCR710_SIEN;   /* 0x03 : SCSI Interrupt Enable R/W */

    _BYTE_RW NCR710_SCID;   /* 0x04 : SCSI Chip ID R/W */
    _BYTE_RW NCR710_SXFER;  /* 0x05 : SCSI Transfer R/W */
	_BYTE_RW NCR710_SODL;   /* 0x06 : SCSI Output Data Latch R/W */
	_BYTE_RW NCR710_SOCL;   /* 0x07 : SCSI Output Control Latch R/W */
	
	_BYTE_RW NCR710_SBFR;   /* 0x08 : SCSI First Byte Received R/W */
	_BYTE_RO NCR710_SIDL;   /* 0x09 : SCSI Input Data Latch RO */
	_BYTE_RO NCR710_SBDL;   /* 0x0A : SCSI Bus Data Lines RO */
	_BYTE_RW NCR710_SBCL;   /* 0x0B : SCSI Bus Control Lines R/W */

	_BYTE_RO NCR710_DSTAT;  /* 0x0C : DMA Status RO */
	_BYTE_RO NCR710_SSTAT0; /* 0x0D : SCSI Status Zero RO */
	_BYTE_RO NCR710_SSTAT1; /* 0x0E : SCSI Status One RO */
	_BYTE_RO NCR710_SSTAT2; /* 0x0F : SCSI Status Two RO */

	_LONG_RW NCR710_DSA;    /* 0x10 : Data Structure Address R/W */

	_BYTE_RW NCR710_CTEST0; /* 0x14 : Chip Test Zero R/W */
	_BYTE_RO NCR710_CTEST1; /* 0x15 : Chip Test One RO */
	_BYTE_RO NCR710_CTEST2; /* 0x16 : Chip Test Two RO */
	_BYTE_RO NCR710_CTEST3; /* 0x17 : Chip Test Three RO */

	_BYTE_RW NCR710_CTEST4; /* 0x18 : Chip Test Four R/W */
	_BYTE_RW NCR710_CTEST5; /* 0x19 : Chip Test Five R/W */
	_BYTE_RW NCR710_CTEST6; /* 0x1A : Chip Test Six R/W */
	_BYTE_RW NCR710_CTEST7; /* 0x1B : Chip Test Seven R/W */

	_LONG_RW NCR710_TEMP;   /* 0x1C : Temporary Stack R/W */

	_BYTE_RW NCR710_DFIFO;  /* 0x20 : DMA FIFO R/W */
	_BYTE_RW NCR710_ISTAT;  /* 0x21 : Interrupt Status R/W */
	_BYTE_RW NCR710_CTEST8; /* 0x22 : Chip Test Eight R/W */
	_BYTE_RW NCR710_LCRC;   /* 0x23 : Longitudinal Parity R/W */

	/* Tricky - DBC is a 24 bit register */
	_BYTE_RW NCR710_DBC0;  /* 0x24 : DMA Byte Counter (part 0) R/W */
	_BYTE_RW NCR710_DBC1;  /* 0x25 : DMA Byte Counter (part 1) R/W */
	_BYTE_RW NCR710_DBC2;  /* 0x26 : DMA Byte Counter (part 2) R/W */
	_BYTE_RW NCR710_DCMD;  /* 0x27 : DMA Command R/W */

	_LONG_RW NCR710_DNAD;  /* 0x28 : DMA Next Address R/W */

	_LONG_RW NCR710_DSP;   /* 0x2C : DMA Scripts Pointer R/W */

	_LONG_RW NCR710_DSPS;  /* 0x30 : DMA Scripts Pointer Save R/W */

	_LONG_RW NCR710_SCRATCH; /* 0x34 : Scratch R/W */

	_BYTE_RW NCR710_DMODE; /* 0x38 : DMA Mode R/W */
	_BYTE_RW NCR710_DIEN;  /* 0x39 : DMA Interrupt Enable R/W */
	_BYTE_RW NCR710_DWT;   /* 0x3A : DMA Watchdog Timer R/W */
	_BYTE_RW NCR710_DCNTL; /* 0x3B : DMA Control R/W */

	_LONG_RO NCR710_ADDER; /* 0x3C : Adder Sum Output R/W */
} NCR710_RegisterMap;

#undef _BYTE_RW
#undef _BYTE_RO
#iundef _LONG_RW
#undef _LONG_RO

#endif
