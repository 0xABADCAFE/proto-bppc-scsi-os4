/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype. 
 */

#include "blizzppc_device.h"
#include "blizzppc.device_rev.h"

#include <exec/ports.h>
#include <exec/tasks.h>

void BlizzPPC_TaskMain(BlizzPPC_Device * bppc_Device) {
	uint32 expectSignals, receivedSignals;

	union {
		struct Message  * message;
		struct IOStdReq * ioRequest;
	} command;

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
			command.ioRequest->io_Error = IOERR_OPENFAIL;
			IExec->ReplyMsg(command.message);
			++intCommandCount;
		}
		pprintf(DBG_INFO, "Ignored %d messages received by the CommandPort", intCommandCount);

	} while (1);
}

