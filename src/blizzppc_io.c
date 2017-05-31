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

/*********************************************************************************************************************/

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

int32  BlizzPPC_AbortIO(struct DeviceManagerInterface * Self, struct IORequest * ioRequest) {
	pprintf(DBG_NOTICE, "Here\n");
	return 0;
}

/*********************************************************************************************************************/

