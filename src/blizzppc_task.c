/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype. 
 */

#include "blizzppc_device.h"
#include "blizzppc.device_rev.h"

#include <exec/ports.h>
#include <exec/tasks.h>
#include <exec/io.h>
#include <devices/scsidisk.h>
#include <devices/newstyle.h>
#include <devices/trackdisk.h>

static int  BlizzPPC_GrabResources(BlizzPPC_Device *);
static void BlizzPPC_FreeResources(BlizzPPC_Device *);
static int  BlizzPPC_ProcessCommand(BlizzPPC_Device * bppc_Device UNUSED, struct IOStdReq * ioRequest);

/*********************************************************************************************************************/

void BlizzPPC_TaskMain(BlizzPPC_Device * bppc_Device) {
	uint32 expectSignals, receivedSignals;

	IOReqMessage command;

	/* Define the signals we will wait on */
	expectSignals =
		(1UL << bppc_Device->bpd_CommandPort->mp_SigBit) |
		(1UL << bppc_Device->bpd_TimerPort->mp_SigBit) |
		(1UL << bppc_Device->bpd_IntSignalBit)
	;

	pprintf(DBG_NOTICE, "Here\n");

	do {
		int intCommandCount = 0;
		pprintf(DBG_INFO, "Waiting for signals %08X\n", (unsigned)expectSignals);
		receivedSignals = IExec->Wait(expectSignals);
		pprintf(DBG_INFO, "Received signals %08X\n", (unsigned)receivedSignals);

		/* For now, just reply to everything with failure */
		while ( (command.message = IExec->GetMsg(bppc_Device->bpd_CommandPort)) ) {
			BlizzPPC_ProcessCommand(bppc_Device, command.ioRequest);
			command.ioRequest->io_Error = IOERR_OPENFAIL;
			IExec->ReplyMsg(command.message);
			++intCommandCount;
		}
		pprintf(DBG_INFO, "Ignored %d messages received by the CommandPort", intCommandCount);

	} while (1);
}

/*********************************************************************************************************************/

static int BlizzPPC_ProcessCommand(BlizzPPC_Device * bppc_Device UNUSED, struct IOStdReq * ioRequest) {
	uint32 command = ioRequest->io_Command;
	switch (command) {
		case CMD_READ:
		case CMD_WRITE:
		case CMD_UPDATE:

		/* devices/scsidisk.h */
		case HD_SCSICMD:

		/* devices/newstyle.h */
		case NSCMD_TD_READ64:
		case NSCMD_TD_WRITE64:
		case NSCMD_TD_FORMAT64:

		/* devices/trackdisk.h */
		case TD_FORMAT:
		case TD_GETGEOMETRY:
		case TD_PROTSTATUS:
		case TD_ADDCHANGEINT:
		case TD_REMCHANGEINT:
			break;
		default:
			break;
	}
	return 0;
}

/*********************************************************************************************************************/

static int BlizzPPC_GrabResources(BlizzPPC_Device * bppc_Device) {
	/* The things we do to avoid goto */
	do {
		/* Create the command port */
		if (!(bppc_Device->bpd_CommandPort = (struct MsgPort *) IExec->AllocSysObjectTags(
			ASOT_PORT,
			ASO_NoTrack,    TRUE,
			ASOPORT_Action, PA_IGNORE,
			TAG_DONE
		))) {
			pprintf(DBG_ERROR, "[" BPPC_DEVICE_NAME "] Failed to create Command MsgPort\n");
			break;
		}

		pprintf(
			DBG_NOTICE,
			"[" BPPC_DEVICE_NAME "] Allocated Command MsgPort [%p]\n"
			"\tmp_Flags           0x%02X\n"
			"\tmp_SigBit          %d\n"
			"\tmp_SigTask         [%p]\n"
			"\tmp_MsgList.lh_Head [%p]\n",
			bppc_Device->bpd_CommandPort,
			(unsigned) bppc_Device->bpd_CommandPort->mp_Flags,
			(int)      bppc_Device->bpd_CommandPort->mp_SigBit,
			(void *)   bppc_Device->bpd_CommandPort->mp_SigTask,
			(void *)   bppc_Device->bpd_CommandPort->mp_MsgList.lh_Head
		);

		/* Create the timer port */
		if (!(bppc_Device->bpd_TimerPort = (struct MsgPort *) IExec->AllocSysObjectTags(
			ASOT_PORT,
			ASO_NoTrack,    TRUE,
			ASOPORT_Action, PA_IGNORE,
			TAG_DONE
		))) {
			pprintf(DBG_ERROR, "[" BPPC_DEVICE_NAME "] Failed to create Timer MsgPort\n");
			break;
		}

		pprintf(
			DBG_NOTICE,
			"[" BPPC_DEVICE_NAME "] Allocated Timer MsgPort [%p]\n"
			"\tmp_Flags           0x%02X\n"
			"\tmp_SigBit          %d\n"
			"\tmp_SigTask         [%p]\n"
			"\tmp_MsgList.lh_Head [%p]\n",
			bppc_Device->bpd_TimerPort,
			(unsigned) bppc_Device->bpd_TimerPort->mp_Flags,
			(int)      bppc_Device->bpd_TimerPort->mp_SigBit,
			(void *)   bppc_Device->bpd_TimerPort->mp_SigTask,
			(void *)   bppc_Device->bpd_TimerPort->mp_MsgList.lh_Head
		);

		/* Create the time request */
		if (!(bppc_Device->bpd_TimeRequest = (struct TimeRequest*) IExec->AllocSysObjectTags(
			ASOT_IOREQUEST,
			ASO_NoTrack,      TRUE,
			ASOIOR_Size,      sizeof(struct TimeRequest),
			ASOIOR_ReplyPort, (uint32) bppc_Device->bpd_TimerPort,
			TAG_DONE
		))) {
			pprintf(DBG_ERROR, "[" BPPC_DEVICE_NAME "] Failed to create TimeRequest\n");
			break;
		}

		pprintf(
			DBG_NOTICE,
			"[" BPPC_DEVICE_NAME "] Allocated TimeRequest [%p]\n",
			bppc_Device->bpd_TimeRequest
		);

		/* Allocate interrupt signal bit */
		if ((bppc_Device->bpd_IntSignalBit = IExec->AllocSignal(-1))<0) {
			pprintf(DBG_ERROR, "[" BPPC_DEVICE_NAME "] Failed to allocate Signal for Interrupt\n");
			break;
		}

		pprintf(
			DBG_NOTICE,
			"[" BPPC_DEVICE_NAME "] Allocated Signal 0x%08X for Interrupt\n",
			(unsigned) (1UL << bppc_Device->bpd_IntSignalBit)
		);

		return 1;
	} while (0);

	/* Getting here means something went awry */
	BlizzPPC_FreeResources(bppc_Device);
	return 0;
}

/*********************************************************************************************************************/

static void BlizzPPC_FreeResources(BlizzPPC_Device * bppc_Device) {

	if (bppc_Device->bpd_IntSignalBit >= 0) {
		IExec->FreeSignal(bppc_Device->bpd_IntSignalBit);
		bppc_Device->bpd_IntSignalBit = -1;
		pprintf(DBG_NOTICE, "[" BPPC_DEVICE_NAME "] Freed Interrupt Signal\n");
	}

	if (bppc_Device->bpd_TimeRequest) {
		IExec->FreeSysObject(ASOT_IOREQUEST, bppc_Device->bpd_TimeRequest);
		bppc_Device->bpd_TimeRequest = NULL;
		pprintf(DBG_NOTICE, "[" BPPC_DEVICE_NAME "] Freed TimeRequest\n");
	}

	if (bppc_Device->bpd_TimerPort) {
		IExec->FreeSysObject(ASOT_PORT, bppc_Device->bpd_TimerPort);
		bppc_Device->bpd_TimerPort = NULL;
		pprintf(DBG_NOTICE, "[" BPPC_DEVICE_NAME "] Freed Timer MsgPort\n");
	}

	if (bppc_Device->bpd_CommandPort) {
		IExec->FreeSysObject(ASOT_PORT, bppc_Device->bpd_CommandPort);
		bppc_Device->bpd_CommandPort = NULL;
		pprintf(DBG_NOTICE, "[" BPPC_DEVICE_NAME "] Freed Command MsgPort\n");
	}

}

/*********************************************************************************************************************/

void BlizzPPC_TaskEntry(BlizzPPC_Device * bppc_Device) {

	struct Task * thisTask = IExec->FindTask(NULL);
	pprintf(DBG_NOTICE, "[" BPPC_DEVICE_NAME "] Worker Task up and running [%p]\n", thisTask);
	if (BlizzPPC_GrabResources(bppc_Device)) {
		pprintf(DBG_NOTICE, "[" BPPC_DEVICE_NAME "] Resources aquired\n");
		BlizzPPC_TaskMain(bppc_Device);
		BlizzPPC_FreeResources(bppc_Device);
	}

	pprintf(DBG_NOTICE, "[" BPPC_DEVICE_NAME "] Exit\n");

}

/*********************************************************************************************************************/

