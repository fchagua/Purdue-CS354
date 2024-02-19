//#include <xinu.h>
#include <pipe.h>

//#include <prototypes.h>

devcall pipe_read(struct dentry * devptr, char *buffer, int32 bytes) {
	if (bytes == 0) {
		return 0;
	}
	
	struct pipecblk *piptr;
	piptr = &pipetab[devptr->dvminor];
	int32 i = 0;
	int32 result = 0;

	
	//kprintf("before read loop\n");
	for (i = 0; i < bytes; i++) {
		
		result = pipe_getc(devptr);
		if (result == SYSERR || result == EOF) {
			kprintf("SYSERR in getc\n");
			break;
		}
		//kprintf("%c", result);
		buffer[i] = result;
		
					
	}

	if (i < bytes && i < 1) {
		kprintf("SYSERR in read\n");
		return SYSERR;
	}

	return i;
}
