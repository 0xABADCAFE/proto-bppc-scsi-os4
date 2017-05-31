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

extern void BlizzPPC_TaskMain(BlizzPPC_Device *);
extern void BlizzPPC_TaskEntry(BlizzPPC_Device *);


/*********************************************************************************************************************/

BlizzPPC_Device * BlizzPPC_Init(BlizzPPC_Device * bppc_Device, void * seglist UNUSED, struct ExecIFace * iExec) {
	struct ExecBase * SysBase          = NULL;

	/* set up the global exec interface - TODO should this just be an injected dependency in the Device structure? */
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

	/* The things we do to avoid goto */
	do {
	
		if (SysBase->LibNode.lib_Version < 52) {
			pprintf(DBG_ERROR, "Kernel V52 required\n");
			break;
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
			break;
		}

		pprintf(DBG_NOTICE, "Initialisation complete\n");

		return bppc_Device;

	} while (0);

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

