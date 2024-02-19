
//#include <xinu.h>
#include <pipe.h>
//#include <prototypes.h>

devcall pipe_open(struct dentry * devptr, char *first, char *second) {
//search pipetab array to find unused entry and mark it allocated and return device descriptor
	//search pipetab array
	//struct pipecblk *piptr;
	//piptr = &pipetab[devptr->dvminor];
	if (devptr->dvnum != PIPE) {
		kprintf("not master pipe\n");
		return SYSERR;
	}
	

	struct pipecblk *pipeptr;
	//pipeptr = &pipetab[devptr->dvminor];
	int32 i = 0;
	for (i = 0; i < Npip; i++) {
		pipeptr = &pipetab[i];
		if (pipeptr->pstate == PIPE_FREE) {
			break;		
		}
		
	}

	if (i >= Npip) {
		kprintf("SYSERR was returned\n");
		return SYSERR;
	}

	pipeptr->pstate = PIPE_OPEN;
	pipeptr->ppsem = semcreate(Npip);
	pipeptr->pcsem = semcreate(0);
	pipeptr->phead = 0;
	pipeptr->ptail = 0;
	//kprintf("result is %d\n and pipe is %d", pipeptr->pdevid, pipeptr);
	return pipeptr->pdevid;	
	
}
