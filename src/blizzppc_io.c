/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype. 
 */

#include "blizzppc_device.h"
#include "blizzppc.device_rev.h"

#include <exec/ports.h>
#include <exec/tasks.h>

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

