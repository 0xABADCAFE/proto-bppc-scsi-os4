/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype.
 */

#ifndef _OS4_BLIZZPPC_NCR710_H_
#define _OS4_BLIZZPPC_NCR710_H_

#include <sys/types.h>

#define _BYTE_RW volatile uint8
#define _LONG_RW volatile uint32
#define _BYTE_RO const volatile uint8
#define _LONG_RO const volatile uint32

/**
 * NCR710 Register Map
 *
 * Registers are generally either 32 or 8 bit. This structure to be mapped onto the
 * hardware address of the SCSI controller.
 */
typedef struct {

	/* Offset : 0x00 */
	_BYTE_RW NCR710_SIEN;   /* 0x03 : SCSI Interrupt Enable R/W */
	_BYTE_RW NCR710_SDID;   /* 0x02 : SCSI Destination ID R/W */
	_BYTE_RW NCR710_SCNTL1; /* 0x01 : SCSI Control One R/W */
	_BYTE_RW NCR710_SCNTL0; /* 0x00 : SCSI Control Zero R/W */

	/* Offset : 0x04 */
	_BYTE_RW NCR710_SOCL;   /* 0x07 : SCSI Output Control Latch R/W */
	_BYTE_RW NCR710_SODL;   /* 0x06 : SCSI Output Data Latch R/W */
	_BYTE_RW NCR710_SXFER;  /* 0x05 : SCSI Transfer R/W */
	_BYTE_RW NCR710_SCID;   /* 0x04 : SCSI Chip ID R/W */

	/* Offset : 0x08*/
	_BYTE_RW NCR710_SBCL;   /* 0x0B : SCSI Bus Control Lines R/W */
	_BYTE_RO NCR710_SBDL;   /* 0x0A : SCSI Bus Data Lines RO */
	_BYTE_RO NCR710_SIDL;   /* 0x09 : SCSI Input Data Latch RO */
	_BYTE_RW NCR710_SBFR;   /* 0x08 : SCSI First Byte Received R/W */

	/* Offset : 0x0C */
	_BYTE_RO NCR710_SSTAT2; /* 0x0F : SCSI Status Two RO */
	_BYTE_RO NCR710_SSTAT1; /* 0x0E : SCSI Status One RO */
	_BYTE_RO NCR710_SSTAT0; /* 0x0D : SCSI Status Zero RO */
	_BYTE_RO NCR710_DSTAT;  /* 0x0C : DMA Status RO */

	/* Offset : 0x10 */
	_LONG_RW NCR710_DSA;    /* 0x10 : Data Structure Address R/W */

	/* Offset : 0x14 */
	_BYTE_RO NCR710_CTEST3; /* 0x17 : Chip Test Three RO */
	_BYTE_RO NCR710_CTEST2; /* 0x16 : Chip Test Two RO */
	_BYTE_RO NCR710_CTEST1; /* 0x15 : Chip Test One RO */
	_BYTE_RW NCR710_CTEST0; /* 0x14 : Chip Test Zero R/W */

	/* Offset : 0x18 */
	_BYTE_RW NCR710_CTEST7; /* 0x1B : Chip Test Seven R/W */
	_BYTE_RW NCR710_CTEST6; /* 0x1A : Chip Test Six R/W */
	_BYTE_RW NCR710_CTEST5; /* 0x19 : Chip Test Five R/W */
	_BYTE_RW NCR710_CTEST4; /* 0x18 : Chip Test Four R/W */

	/* Offset : 0x1C */
	_LONG_RW NCR710_TEMP;   /* 0x1C : Temporary Stack R/W */

	/* Offset : 0x20 */
	_BYTE_RW NCR710_LCRC;   /* 0x23 : Longitudinal Parity R/W */
	_BYTE_RW NCR710_CTEST8; /* 0x22 : Chip Test Eight R/W */
	_BYTE_RW NCR710_ISTAT;  /* 0x21 : Interrupt Status R/W */
	_BYTE_RW NCR710_DFIFO;  /* 0x20 : DMA FIFO R/W */


	/* Offset : 0x24 */
	/* Tricky - DBC is a 24 bit register */
	_BYTE_RW NCR710_DCMD;  /* 0x27 : DMA Command R/W */
	_BYTE_RW NCR710_DBC2;  /* 0x26 : DMA Byte Counter (part 2) R/W */
	_BYTE_RW NCR710_DBC1;  /* 0x25 : DMA Byte Counter (part 1) R/W */
	_BYTE_RW NCR710_DBC0;  /* 0x24 : DMA Byte Counter (part 0) R/W */

	/* Offset : 0x28 */
	_LONG_RW NCR710_DNAD;  /* 0x28 : DMA Next Address R/W */

	/* Offset : 0x2C */
	_LONG_RW NCR710_DSP;   /* 0x2C : DMA Scripts Pointer R/W */

	/* Offset : 0x30 */
	_LONG_RW NCR710_DSPS;  /* 0x30 : DMA Scripts Pointer Save R/W */

	/* Offset : 0x34 */
	_LONG_RW NCR710_SCRATCH; /* 0x34 : Scratch R/W */

	/* Offset : 0x38 */
	_BYTE_RW NCR710_DCNTL; /* 0x3B : DMA Control R/W */
	_BYTE_RW NCR710_DWT;   /* 0x3A : DMA Watchdog Timer R/W */
	_BYTE_RW NCR710_DIEN;  /* 0x39 : DMA Interrupt Enable R/W */
	_BYTE_RW NCR710_DMODE; /* 0x38 : DMA Mode R/W */

	/* Offset : 0x3C */
	_LONG_RO NCR710_ADDER; /* 0x3C : Adder Sum Output R/W */
} NCR710_RegisterMap;

#undef _BYTE_RW
#undef _BYTE_RO
#undef _LONG_RW
#undef _LONG_RO

/**
 * Reguster field definitions.
 */

enum {
	/**
	 * NCR710_SCNTL0 Register Bits
	 */
	NCR710_SCNTL0_TRG   = 1<<0, /* Target Mode */
	NCR710_SCNTL0_AAP   = 1<<1, /* Assert ATN/ on Parity Error */
	NCR710_SCNTL0_EPG   = 1<<2, /* Enable Parity Generation Through */
	NCR710_SCNTL0_EPC   = 1<<3, /* Enable Parity Checking */
	NCR710_SCNTL0_WATN  = 1<<4, /* Select with ATN/ on a Start Sequence */
	NCR710_SCNTL0_START = 1<<5, /* Start Sequence */
	NCR710_SCNTL0_ARB0  = 1<<6, /* Arbitration Mode 0 */
	NCR710_SCNTL0_ARB1  = 1<<7, /* Arbitration Mode 1 */

	/**
	 * NCR710_SCNTL1 Register Bits
	 */
	NCR710_SCNTL1_RCV   = 1<<0, /* Start SCSI Receive */
	NCR710_SCNTL1_SND   = 1<<1, /* Start SCSI Send */
	NCR710_SCNTL1_AESP  = 1<<2, /* Assert Even SCSI Parity (force bad) */
	NCR710_SCNTL1_RST   = 1<<3, /* Assert SCSI RST/ Signal */
	NCR710_SCNTL1_CON   = 1<<4, /* Connected */
	NCR710_SCNTL1_ESR   = 1<<5, /* Enable Selection and Reselection */
	NCR710_SCNTL0_ADB   = 1<<6, /* Asser SCSI Data Bus */
	NCR710_SCNTL0_EXC   = 1<<7, /* Extra Clock Cycle of Data Setup */

	/**
	 * NCR710_SIEN Register Bits
	 */
	NCR710_SIEN_PAR     = 1<<0, /* Parity Error */
	NCR710_SIEN_RST     = 1<<1, /* SCSI RST/ Received */
	NCR710_SIEN_UDC     = 1<<2, /* Unexpected Disconnect */
	NCR710_SIEN_SGE     = 1<<3, /* SCSI Gross Error */
	NCR710_SIEN_SEL     = 1<<4, /* Selected or Reselected */
	NCR710_SIEN_STO     = 1<<5, /* SCSI Bus Timeout */
	NCR710_SIEN_FCMP    = 1<<6, /* Function Complete */
	NCR710_SIEN_MA      = 1<<7, /* Initiator: Phase Mismatch or Target: ATN/ Active */

	/**
	 * NCR710_SXFER Register Bits
	 */
	NCR710_SXFER_MO0    = 1<<0, /* Max SCSI Synchronous Offset 0 */
	NCR710_SXFER_MO1    = 1<<1, /* Max SCSI Synchronous Offset 1 */
	NCR710_SXFER_MO2    = 1<<2, /* Max SCSI Synchronous Offset 2 */
	NCR710_SXFER_MO3    = 1<<3, /* Max SCSI Synchronous Offset 3 */
	NCR710_SXFER_TP0    = 1<<4, /* SCSI Synchronous Transfer Period 0 */
	NCR710_SXFER_TP1    = 1<<5, /* SCSI Synchronous Transfer Period 1 */
	NCR710_SXFER_TP2    = 1<<6, /* SCSI Synchronous Transfer Period 2 */
	NCR710_SXFER_DHP    = 1<<7, /* Disable Halt on Parity Error or ATN/ */

	/**
	 * NCR710_SOCL Register Bits
	 */
	NCR710_SOCL_IO      = 1<<0, /* Assert SCSI I/O Signal */
	NCR710_SOCL_CD      = 1<<1, /* Assert SCSI C/D Signal */
	NCR710_SOCL_MSG     = 1<<2, /* Assert SCSI MSG/ Signal */
	NCR710_SOCL_ATN     = 1<<3, /* Assert SCSI ATN/ Signal */
	NCR710_SOCL_SEL     = 1<<4, /* Assert SCSI SEL/ Signal */
	NCR710_SOCL_BSY     = 1<<5, /* Assert SCSI BSY/ Signal */
	NCR710_SOCL_ACK     = 1<<6, /* Assert SCSI ACK/ Signal */
	NCR710_SOCL_REQ     = 1<<7, /* Assert SCSI REQ/ Signal */

	/**
	 * NCR710_SBCL Register Bits
	 */
	NCR710_SBCL_IO      = 1<<0, /* I/O Status */
	NCR710_SBCL_CD      = 1<<1, /* C/D Status */
	NCR710_SBCL_MSG     = 1<<2, /* MSG/ Status */
	NCR710_SBCL_ATN     = 1<<3, /* ATN/ Status */
	NCR710_SBCL_SEL     = 1<<4, /* SEL/ Status */
	NCR710_SBCL_BSY     = 1<<5, /* BSY/ Status */
	NCR710_SBCL_ACK     = 1<<6, /* ACK/ Status */
	NCR710_SBCL_REQ     = 1<<7, /* REQ/ Status */

	/**
	 * NCR710_DSTAT Register Bits
	 */
	NCR710_DSTAT_IID    = 1<<0, /* Illegal Instruction Detected */
	NCR710_DSTAT_WTD    = 1<<1, /* Watchdog Timeout Detected */
	NCR710_DSTAT_SIR    = 1<<2, /* SCRIPTS Interrupt Instruction Received */
	NCR710_DSTAT_SSI    = 1<<3, /* SCRIPTS Step Interrupt */
	NCR710_DSTAT_ABRT   = 1<<4, /* Aborted */
	NCR710_DSTAT_BF     = 1<<5, /* Bus Fault */
	NCR710_DSTAT_R      = 1<<6, /* RESERVED */
	NCR710_DSTAT_DFE    = 1<<7, /* DMA FIFO Empty */

	/**
	 * NCR710_SSTAT0 Register Bits
	 */
	NCR710_SSTAT0_PAR    = 1<<0, /* Parity Error */
	NCR710_SSTAT0_RST    = 1<<1, /* SCSI RST/ Received */
	NCR710_SSTAT0_UDC    = 1<<2, /* Unexpected Disconnect */
	NCR710_SSTAT0_SGE    = 1<<3, /* SCSI Gross Error */
	NCR710_SSTAT0_SEL    = 1<<4, /* Selected or Reselected */
	NCR710_SSTAT0_STO    = 1<<5, /* SCSI Bus Timeout */
	NCR710_SSTAT0_FCMP   = 1<<6, /* Function Complete */
	NCR710_SSTAT0_MA     = 1<<7, /* Initiator: Phase Mismatch or Target: ATN/ Active */

	/**
	 * NCR710_SSTAT1 Register Bits
	 */
	NCR710_SSTAT1_SDP    = 1<<0, /* SCSI SDP Parity Signal */
	NCR710_SSTAT1_RST    = 1<<1, /* SCSI RST/ Signal */
	NCR710_SSTAT1_WOA    = 1<<2, /* Won Arbitration */
	NCR710_SSTAT1_LOA    = 1<<3, /* Lost Arbitration */
	NCR710_SSTAT1_AIP    = 1<<4, /* Arbitration in Progress */
	NCR710_SSTAT1_OLF    = 1<<5, /* SODL Register Full */
	NCR710_SSTAT1_ORF    = 1<<6, /* SODR Register Full */
	NCR710_SSTAT1_ILF    = 1<<7, /* SIDL Register Full */

	/**
	 * NCR710_SSTAT2 Register Bits
	 */
	NCR710_SSTAT2_IO     = 1<<0, /* SCSI I/O Signal */
	NCR710_SSTAT2_CD     = 1<<1, /* SCSI C/D Signal */
	NCR710_SSTAT2_MSG    = 1<<2, /* SCSI MSG/ Signal */
	NCR710_SSTAT2_SDP    = 1<<3, /* Latched SCSI Parity */
	NCR710_SSTAT2_FF0    = 1<<4, /* FIFO Flags 0 */
	NCR710_SSTAT2_FF1    = 1<<5, /* FIFO Flags 1 */
	NCR710_SSTAT2_FF2    = 1<<6, /* FIFO Flags 2 */
	NCR710_SSTAT2_FF3    = 1<<7, /* FIFO Flags 3 */

	/**
	 * NCR710_CTEST0 Register Bits
	 */

	/**
	 * NCR710_CTEST1 Register Bits
	 */

	/**
	 * NCR710_CTEST2 Register Bits
	 */

	/**
	 * NCR710_CTEST4 Register Bits
	 */

	/**
	 * NCR710_CTEST5 Register Bits
	 */

	/**
	 * NCR710_CTEST7 Register Bits
	 */

	/**
	 * NCR710_DFIFO Register Bits
	 */

	/**
	 * NCR710_ISTAT Register Bits
	 */

	/**
	 * NCR710_CTEST8 Register Bits
	 */

	/**
	 * NCR710_DMODE Register Bits
	 */

	/**
	 * NCR710_DIEN Register Bits
	 */

	/**
	 * NCR710_DCNTL Register Bits
	 */
} NCR710_RegisterBits;

/**
 * NCR710_SCNTL1 Register Bits
 */
typedef enum {


} NCR710_SCNTL0_BITS;

#endif
