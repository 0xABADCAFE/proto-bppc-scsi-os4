/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype. 
 */

#include "blizzppc_device.h"
#include "blizzppc.device_rev.h"

#include <exec/ports.h>
#include <exec/tasks.h>

extern struct ExecIFace * IExec;

void BlizzPPC_TaskMain(BlizzPPC_Device * bppc_Device) {
	pprintf(DBG_NOTICE, "Here\n");
}
