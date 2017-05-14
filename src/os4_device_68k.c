/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype.
 * 
 * 68K stubs
 */

#include "blizzppc_device.h"
#include <exec/interfaces.h>
#include <exec/libraries.h>
#include <exec/devices.h>
#include <exec/emulation.h>
#include <interfaces/exec.h>

/*********************************************************************************************************************/

/* Required device stub functions */

static struct Library * stub_OpenPPC(uint32 *regarray) {
	struct Library *                 Base   = (struct Library *) regarray[REG68K_A6/4];
	struct ExtendedLibrary *         ExtLib = (struct ExtendedLibrary *) ((uint32)Base + Base->lib_PosSize);
	struct LibraryManagerInterface * Self   = (struct LibraryManagerInterface *) ExtLib->ILibrary;

	pprintf(DBG_NOTICE, BPPC_DEVICE_NAME ": Here\n");

	return Self->Open(0);
}

/*********************************************************************************************************************/

static void * stub_ClosePPC(uint32 *regarray) {
	struct Library *                 Base   = (struct Library *) regarray[REG68K_A6/4];
	struct ExtendedLibrary *         ExtLib = (struct ExtendedLibrary *) ((uint32)Base + Base->lib_PosSize);
	struct LibraryManagerInterface * Self   = (struct LibraryManagerInterface *) ExtLib->ILibrary;

	pprintf(DBG_NOTICE, BPPC_DEVICE_NAME ": Here\n");

	return Self->Close();
}

/*********************************************************************************************************************/

static void * stub_ExpungePPC(uint32 * regarray UNUSED) {
	pprintf(DBG_NOTICE, BPPC_DEVICE_NAME ": Here\n");

	return NULL;
}

/*********************************************************************************************************************/

static uint32 stub_ReservedPPC(uint32 * regarray UNUSED) {
	pprintf(DBG_NOTICE, BPPC_DEVICE_NAME ": Here\n");

	return 0UL;
}

/*********************************************************************************************************************/

static uint32 stub_BeginIOPPC(uint32 * regarray UNUSED) {
	struct Library *                Base   = (struct Library *) regarray[REG68K_A6/4];
	struct ExtendedLibrary *        ExtLib = (struct ExtendedLibrary *) ((uint32)Base + Base->lib_PosSize);
	struct DeviceManagerInterface * Self   = (struct DeviceManagerInterface *) ExtLib->ILibrary;
	pprintf(DBG_NOTICE, BPPC_DEVICE_NAME ": Here\n");

	Self->BeginIO((struct IORequest *)regarray[REG68K_A1/4]);
	return 0UL;
}

/*********************************************************************************************************************/

static uint32 stub_AbortIOPPC(uint32 * regarray UNUSED) {
	struct Library *                Base   = (struct Library *) regarray[REG68K_A6/4];
	struct ExtendedLibrary *        ExtLib = (struct ExtendedLibrary *) ((uint32)Base + Base->lib_PosSize);
	struct DeviceManagerInterface * Self   = (struct DeviceManagerInterface *) ExtLib->ILibrary;
	pprintf(DBG_NOTICE, BPPC_DEVICE_NAME ": Here\n");

	Self->AbortIO((struct IORequest *)regarray[REG68K_A1/4]);
	return 0UL;
}

/*********************************************************************************************************************/

/* vector declarations */
struct EmuTrap stub_Open = {
	TRAPINST,
	TRAPTYPE,
	(uint32 (*)(uint32 *))stub_OpenPPC
};

struct EmuTrap stub_Close = {
	TRAPINST,
	TRAPTYPE,
	(uint32 (*)(uint32 *))stub_ClosePPC
};

struct EmuTrap stub_Expunge = {
	TRAPINST,
	TRAPTYPE,
	(uint32 (*)(uint32 *))stub_ExpungePPC
};

struct EmuTrap stub_Reserved = {
	TRAPINST,
	TRAPTYPE,
	stub_ReservedPPC
};

struct EmuTrap stub_BeginIO = {
	TRAPINST,
	TRAPTYPE,
	stub_BeginIOPPC
};

struct EmuTrap stub_AbortIO = {
	TRAPINST,
	TRAPTYPE,
	stub_AbortIOPPC
};

const void * const vectorTable68K[] = {
	&stub_Open,
	&stub_Close,
	&stub_Expunge,
	&stub_Reserved,
	&stub_BeginIO,
	&stub_AbortIO
};
