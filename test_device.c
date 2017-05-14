/**
 * blizzppc.device
 *
 * Experimental OS4 SCSI driver, prototype. 
 * 
 * Simple program that atempts to open the device. And close it again.
 */

#include <stdio.h>
#include <proto/exec.h>
#include <exec/devices.h>
#include <string.h>

extern struct ExecIFace * IExec;

int main(void) {

	struct IOStdReq ioRequest;
	uint32 resultCode = 0;
	do {
/*
		if (!(ioRequest = IExec->AllocSysObjectTags(
			ASOT_IOREQUEST,
			ASO_NoTrack, TRUE,
			TAG_DONE
		))) {
			puts("Couldn't create IORequest");
			break;
		}
*/
		memset(&ioRequest, 0, sizeof(struct IOStdReq));

		ioRequest.io_Message.mn_Length = sizeof(struct IOStdReq);

		if ((resultCode = IExec->OpenDevice("PROGDIR:build/blizzppc.device", 0, (struct IORequest *) &ioRequest, 0))!=0) {
			printf("Failed to open device, result was : %lu\n", resultCode);
			break;
		}

		puts("Pretty successful so far!");

	} while (0);


	/* Clean up */
	/*if (ioRequest) {*/
		if (resultCode==0) {
			puts("Closing device");
			IExec->CloseDevice((struct IORequest *) &ioRequest);
		}
		/*IExec->FreeSysObject(ASOT_IOREQUEST, ioRequest);*/
	/*}*/
	return 0;
}
