//#include <xinu.h>
#include <pipe.h>

devcall pipe_write (struct dentry *devptr, char *message, int32 bytes) {
	if (bytes == 0) {
		return 0;
	}

	

	int32 i = 0;
	int32 result = 0;
	struct pipecblk *piptr;
	piptr = &pipetab[devptr->dvminor];

	if (piptr->pstate != PIPE_OPEN) {
		kprintf("pipe not open for write\n");
		return SYSERR;
	}
	//kprintf("before write loop with piptr at %d and devid as %d and pstate as %d\n", piptr, piptr->pdevid, piptr->pstate);
	
	
	//kprintf("before write loop with message %s and bytes %d\n", message, bytes);
	for (i = 0; i < bytes; i++) {
		
		
		//kprintf("%c with pstate %d\n", message[i], piptr->pstate);
		
		result = pipe_putc(devptr, message[i]);
		//kprintf("%c with pstate %d and result %d\n", message[i], piptr->pstate, result);
		
		if (result == SYSERR) {
			kprintf("SYSERR for putc\n");
			break;	
		}

	}
	
	if (i < bytes && i < 1) {
		kprintf("SYSERR for write\n");
		return SYSERR;
	}
	
	return i;

}
