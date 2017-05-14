/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype. 
 */

#include <proto/exec.h>
#include "debug.h"

#ifdef DEBUG

/* Error level strings for debugging */
const char * BPPC_DebugLevel[DBG_MISC+1] = {
	"Critical",
	"Error",
	"Warning",
	"Notice",
	"Debug",
	"Misc"
};

#endif
