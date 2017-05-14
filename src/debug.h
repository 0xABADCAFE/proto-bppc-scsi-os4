/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype. 
 */

#ifndef _OS4_BLIZZPPC_DEVICE_DEBUG_H_
	#define _OS4_BLIZZPPC_DEVICE_DEBUG_H_

	#define DBG_CRITICAL 0  /* Most severe */
	#define DBG_ERROR    1  /* serious problem */
	#define DBG_WARN     2  /* warning */
	#define DBG_NOTICE   3  /* notification */
	#define DBG_INFO     4  /* basic debug information */
	#define DBG_MISC     5  /* miscellaneous extra information */

	#ifdef DEBUG

		extern const char * BPPC_DebugLevel[DBG_MISC+1];

		/* pprintf() macro - pretty print for debugging */
		#define pprintf(level, format, args...) if ((level)<=DEBUGLEVEL) ((struct ExecIFace *)((*(struct ExecBase **)4)->MainInterface))->DebugPrintF("[%-8s] [%s] " format, BPPC_DebugLevel[(level)], __PRETTY_FUNCTION__ , ## args)

	#else

		#define pprint(level, format, args...)

	#endif

#endif
