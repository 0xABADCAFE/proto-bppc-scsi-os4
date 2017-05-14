/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype. 
 */

#include "blizzppc_device.h"
#include "blizzppc.device_rev.h"

#include <exec/ports.h>
#include <exec/tasks.h>

struct ExecIFace * IExec = 0;

static void BlizzPPC_TaskEntry(BlizzPPC_Device *);
static int  BlizzPPC_GrabResources(BlizzPPC_Device *);
static void BlizzPPC_FreeResources(BlizzPPC_Device *);

/*********************************************************************************************************************/

BlizzPPC_Device * BlizzPPC_Init(BlizzPPC_Device * bppc_Device, void * seglist UNUSED, struct ExecIFace * iExec) {
	struct ExecBase * SysBase          = NULL;

	/* set up the global exec interface */
	IExec = iExec;
	IExec->Obtain();

	SysBase = (struct ExecBase *) IExec->Data.LibBase;

	pprintf(DBG_NOTICE, "Initialising driver\n");

	/* Paranoia */
	bppc_Device->bpd_Task         = NULL;
	bppc_Device->bpd_CommandPort  = NULL;
	bppc_Device->bpd_TimerPort    = NULL;
	bppc_Device->bpd_TimeRequest  = NULL;
	bppc_Device->bpd_IntHandler   = NULL;
	bppc_Device->bpd_IntSignalBit = -1;

	if (SysBase->LibNode.lib_Version < 52) {
		pprintf(DBG_ERROR, "Kernel V52 required\n");
		goto bail;
	}

	/* Populate any missing fields */
	bppc_Device->bpd_Library.lib_Revision = REVISION;

	/* Create the driver task, which will attempt to do the rest of the initialisation */
	if (!(bppc_Device->bpd_Task = IExec->CreateTaskTags(
		BPPC_DEVICE_NAME,
		BPPC_DEVICE_TASKPRI,
		BlizzPPC_TaskEntry,
		BPPC_DEVICE_STACK,
		AT_Param1, bppc_Device,
		TAG_DONE
	))) {
		pprintf(DBG_ERROR, "Failed to create Task\n");
		goto bail;
	}

	pprintf(DBG_NOTICE, "Initialisation complete\n");

	return bppc_Device;

bail:

	return NULL;
}

/*********************************************************************************************************************/

int32  BlizzPPC_Open(struct DeviceManagerInterface * Self, struct IOStdReq * ioRequest, uint32 unit, uint32 flags UNUSED) {
	pprintf(DBG_NOTICE, BPPC_DEVICE_NAME ": Here\n");
	if (
		ioRequest->io_Message.mn_Length > 0 &&
		ioRequest->io_Message.mn_Length < sizeof(struct IOStdReq)
	) {
		pprintf(
			DBG_ERROR,
			"Illegal request size %u, should be at least %u\n",
			ioRequest->io_Message.mn_Length,
			(uint32) sizeof(struct IOStdReq)
		);
		ioRequest->io_Error = IOERR_OPENFAIL;
	}
	else {
		ioRequest->io_Error = 0;
	}
	return ioRequest->io_Error;
}

/*********************************************************************************************************************/

void * BlizzPPC_Close(struct DeviceManagerInterface * Self, struct IOStdReq * ioRequest ) {
	pprintf(DBG_NOTICE, "Here\n");
	return NULL;
}

/*********************************************************************************************************************/

void * BlizzPPC_Expunge(struct DeviceManagerInterface * Self) {
	pprintf(DBG_NOTICE, "Here\n");
	return NULL;
}

/*********************************************************************************************************************/

void * BlizzPPC_BeginIO(struct DeviceManagerInterface * Self, struct IORequest * ioRequest) {
	pprintf(DBG_NOTICE, "Here\n");
	return NULL;
}

/*********************************************************************************************************************/

int32  BlizzPPC_AbortIO(struct DeviceManagerInterface * Self, struct IORequest * ioRequest) {
	pprintf(DBG_NOTICE, "Here\n");
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
			(unsigned) (1L << bppc_Device->bpd_IntSignalBit)
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

		/* TODO - command processing loop */
		
		BlizzPPC_FreeResources(bppc_Device);
	}

	pprintf(DBG_NOTICE, "[" BPPC_DEVICE_NAME "] Exit\n");

}
