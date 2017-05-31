/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype.
 * 
 * Main OS4 device API implementation
 */

#include "blizzppc_device.h"
#include "blizzppc.device_rev.h"
#include "debug.h"
#include <dos/startup.h>

/*********************************************************************************************************************/

/* Forward references */
static uint32 genericObtain(struct Interface *);
static uint32 genericRelease(struct Interface *);
extern int32  BlizzPPC_Open(struct DeviceManagerInterface *, struct IORequest *, uint32 , uint32);
extern void * BlizzPPC_Close(struct DeviceManagerInterface *, struct IORequest *);
extern void * BlizzPPC_Expunge(struct DeviceManagerInterface *Self);
extern void * BlizzPPC_BeginIO(struct DeviceManagerInterface *, struct IORequest *);
extern int32  BlizzPPC_AbortIO(struct DeviceManagerInterface *, struct IORequest *);
extern BlizzPPC_Device* BlizzPPC_Init(BlizzPPC_Device *, void *, struct ExecIFace *);

/*********************************************************************************************************************/

/* Library|Device structures required */
static void *deviceVectors[] = {
	(void *) genericObtain,
	(void *) genericRelease,
	(void *) NULL,
	(void *) NULL,
	(void *) BlizzPPC_Open,
	(void *) BlizzPPC_Close,
	(void *) BlizzPPC_Expunge,
	(void *) NULL,
	(void *) BlizzPPC_BeginIO,
	(void *) BlizzPPC_AbortIO,
	(void *) -1,
};

static struct TagItem managerTags[] = {
	{MIT_Name,        (uint32) "__device"},
	{MIT_VectorTable, (uint32) deviceVectors},
	{MIT_Version,     1},
	{TAG_DONE,        0}
};

static void *mainVectors[] = {
	(void *) genericObtain,
	(void *) genericRelease,
	(void *) NULL,
	(void *) NULL,
	(void *) -1
};

static struct TagItem mainTags[] = {
	{MIT_Name,        (uint32) "main"},
	{MIT_VectorTable, (uint32) mainVectors},
	{MIT_Version,     1},
	{TAG_DONE,        0}
};

/* MLT_INTERFACES array */
static ULONG libraryInterfaces[] = {
	(uint32) managerTags,
	(uint32) mainTags,
	0
};

extern uint32 vectorTable68K;

/* CreateLibrary tag list */
static struct TagItem libraryCreateTags[] = {
	{CLT_DataSize,   (uint32) (sizeof(BlizzPPC_Device))},
	{CLT_Interfaces, (uint32) libraryInterfaces},
	{CLT_Vector68K,  (uint32) &vectorTable68K},
	{CLT_InitFunc,   (uint32) BlizzPPC_Init},
	{TAG_DONE,       0}
};

static const struct Resident restag USED = {
	RTC_MATCHWORD,
	(struct Resident *) &restag,
	(void *) (&restag+1),
	RTF_AUTOINIT|RTF_NATIVE|RTF_COLDSTART,
	VERSION,
	NT_DEVICE,
	48, // after timer.device
	(char *) BPPC_DEVICE_NAME,
	(char *) &"$VER: "VSTRING[6],
	(void *) libraryCreateTags
};

/*********************************************************************************************************************/

/* required */
uint32 genericObtain(struct Interface *Self) {
	pprintf(DBG_NOTICE, BPPC_DEVICE_NAME ": Here\n");
	return Self->Data.RefCount++;
}

/*********************************************************************************************************************/

uint32 genericRelease(struct Interface *Self) {
	pprintf(DBG_NOTICE, BPPC_DEVICE_NAME ": Here\n");
	return Self->Data.RefCount--;
}

/*********************************************************************************************************************/

int32 _start(char* argstring UNUSED, int32 arglen UNUSED, struct ExecBase *sysbase UNUSED) {
	pprintf(DBG_NOTICE, BPPC_DEVICE_NAME ": Here\n");
	return RETURN_FAIL;
}

/*********************************************************************************************************************/

