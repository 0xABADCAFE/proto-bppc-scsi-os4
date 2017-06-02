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

void BlizzPPC_PinIORequest(struct IOStdReq * ioRequest);
void BlizzPPC_UnpinIORequest(struct IOStdReq * ioRequest);

/*********************************************************************************************************************/

/**
 * BlizzPPC_BeginIO()
 *
 * Library vector.
 */
void BlizzPPC_BeginIO(struct DeviceManagerInterface * Self, struct IORequest * ioRequest) {
	pprintf(DBG_NOTICE, "Here\n");

	BlizzPPC_Device* bppc_Device = (BlizzPPC_Device*)Self->Data.LibBase;
	uint32 command = ioRequest->io_Command;

	switch (command) {

		/* Commands that can be processed here and now */

		/* exec/io.h */
		case CMD_CLEAR:
		
		/* devices/newstyle.h */
		case NSCMD_TD_SEEK64:
		case NSCMD_DEVICEQUERY:

		/* devices/trackdisk.h */
		case TD_CHANGENUM:
		case TD_CHANGESTATE:
		case TD_MOTOR:
		case TD_SEEK:
			/* Assume success for now */
			ioRequest->io_Error = 0;
			break;

		/* Commands that have to be processed using IPC via the command port */
		/* exec/io.h */
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
			ioRequest->io_Message.mn_Node.ln_Type = NT_MESSAGE;
			ioRequest->io_Flags                  &= ~IOF_QUICK;
			IExec->PutMsg(bppc_Device->bpd_CommandPort, &ioRequest->io_Message);
			break;

		default:
			/* Something unexpected here */
			ioRequest->io_Error = IOERR_NOCMD;
			break;
	}

	/* If the IORequest is not quick, be sure to reply to it */
	if (!(ioRequest->io_Flags & IOF_QUICK)) {
		IExec->ReplyMsg(&ioRequest->io_Message);
	}
}

/*********************************************************************************************************************/

/**
 * BlizzPPC_AbortIO()
 *
 * Library vector.
 */
int32  BlizzPPC_AbortIO(struct DeviceManagerInterface * Self, struct IORequest * ioRequest) {
	pprintf(DBG_NOTICE, "Here\n");
	return 0;
}

/*********************************************************************************************************************/

/**
 * BlizzPPC_PinIORequest()
 *
 * Prevent the IORequest and associated data from being swapped out of physical memory.
 */
void BlizzPPC_PinIORequest(struct IOStdReq * ioRequest) {
	pprintf(DBG_NOTICE, "Here\n");

	/* Get the size of the message. It should at least be the size of the IOStdRequest */
	size_t requestSize = ioRequest->io_Message.mn_Length ?
		ioRequest->io_Message.mn_Length :
		sizeof(struct IOStdReq);

	/* Fistly lock the IOStdRequest area */
	IExec->LockMem(ioRequest, requestSize);

	pprintf(DBG_MISC, "Pinned IOStdRequest %u [%p]\n", requestSize, ioRequest);

	/* If there is an io_Data entry in the request, we should lock that */
	if (
		ioRequest->io_Data && 
		ioRequest->io_Length
	) {
		IExec->LockMem(ioRequest->io_Data, ioRequest->io_Length);
		pprintf(DBG_MISC, "Pinned IOStdRequest->io_Data %u [%p]\n", ioRequest->io_Length, ioRequest->io_Data);
	}

	/* For a SCSI command, lock any associated command data and sense areas */
	if (ioRequest->io_Command == HD_SCSICMD) {
		struct SCSICmd * scsiCommand = (struct SCSICmd *)ioRequest->io_Data;
		if (
			scsiCommand->scsi_Command &&
			scsiCommand->scsi_CmdLength
		) {
			IExec->LockMem(scsiCommand->scsi_Command, scsiCommand->scsi_CmdLength);
			pprintf(
				DBG_MISC, "Pinned IOStdRequest->io_Data->scsi_Command %u [%p]\n",
				scsiCommand->scsi_CmdLength, scsiCommand->scsi_Command
			);
		}
		if (
			scsiCommand->scsi_Data &&
			scsiCommand->scsi_Length
		) {
			IExec->LockMem(scsiCommand->scsi_Data, scsiCommand->scsi_Length);
			pprintf(
				DBG_MISC, "Pinned IOStdRequest->io_Data->scsi_Data %u [%p]\n",
				scsiCommand->scsi_Length, scsiCommand->scsi_Data
			);
		}
		if (
			scsiCommand->scsi_Flags & SCSIF_AUTOSENSE &&
			scsiCommand->scsi_SenseData &&
			scsiCommand->scsi_SenseLength
		) {
			IExec->LockMem(scsiCommand->scsi_SenseData, scsiCommand->scsi_SenseLength);
			pprintf(
				DBG_MISC, "Pinned IOStdRequest->io_Data->scsi_SenseData %u [%p]\n",
				scsiCommand->scsi_SenseLength, scsiCommand->scsi_SenseData
			);
		}
	}
}

/*********************************************************************************************************************/

/**
 * BlizzPPC_UnpinIORequest()
 *
 * Removes the restriction that the IORequest and associated data must not be swapped out of physical memory.
 */
void BlizzPPC_UnpinIORequest(struct IOStdReq * ioRequest) {
	pprintf(DBG_NOTICE, "Here\n");
	/* Get the size of the message. It should at least be the size of the IOStdRequest */
	size_t requestSize = ioRequest->io_Message.mn_Length ?
		ioRequest->io_Message.mn_Length :
		sizeof(struct IOStdReq);

	/* For a SCSI command, unlock any associated command data and sense areas */
	if (ioRequest->io_Command == HD_SCSICMD) {
		struct SCSICmd * scsiCommand = (struct SCSICmd *)ioRequest->io_Data;
		if (
			scsiCommand->scsi_Flags & SCSIF_AUTOSENSE &&
			scsiCommand->scsi_SenseData &&
			scsiCommand->scsi_SenseLength
		) {
			IExec->UnlockMem(scsiCommand->scsi_SenseData, scsiCommand->scsi_SenseLength);
			pprintf(
				DBG_MISC, "Unpinned IOStdRequest->io_Data->scsi_SenseData %u [%p]\n",
				scsiCommand->scsi_SenseLength, scsiCommand->scsi_SenseData
			);
		}
		if (
			scsiCommand->scsi_Data &&
			scsiCommand->scsi_Length
		) {
			IExec->UnlockMem(scsiCommand->scsi_Data, scsiCommand->scsi_Length);
			pprintf(
				DBG_MISC, "Unpinned IOStdRequest->io_Data->scsi_Data %u [%p]\n",
				scsiCommand->scsi_Length, scsiCommand->scsi_Data
			);
		}
		if (
			scsiCommand->scsi_Command &&
			scsiCommand->scsi_CmdLength
		) {
			IExec->LockMem(scsiCommand->scsi_Command, scsiCommand->scsi_CmdLength);
			pprintf(
				DBG_MISC, "Unpinned IOStdRequest->io_Data->scsi_Command %u [%p]\n",
				scsiCommand->scsi_CmdLength, scsiCommand->scsi_Command
			);
		}
	}

	/* If there is an io_Data entry in the request, we should unlock that */
	if (
		ioRequest->io_Data && 
		ioRequest->io_Length
	) {
		IExec->UnlockMem(ioRequest->io_Data, ioRequest->io_Length);
		pprintf(DBG_MISC, "Unpinned IOStdRequest->io_Data %u [%p]\n", ioRequest->io_Length, ioRequest->io_Data);
	}

	/* Finally unlock the IOStdRequest area */
	IExec->LockMem(ioRequest, requestSize);
	pprintf(DBG_MISC, "Unpinned IOStdRequest %u [%p]\n", requestSize, ioRequest);
}

