//#include <xinu.h>
#include <pipe.h>

devcall pipe_close (struct dentry *devptr)
{	
	kprintf("in pipe close\n");
	struct pipecblk *piptr;

	piptr = &pipetab[devptr->dvminor];

	if (piptr->pstate == PIPE_FREE) {
		kprintf("pipe close SYSERR\n");
		return SYSERR;
	}
	
	if (piptr->pstate == PIPE_OPEN) {
		piptr->pstate = PIPE_EOF;
		resched_cntl(DEFER_START);
		struct sentry *semptr1;
		semptr1 = &semtab[piptr->ppsem];
		struct sentry *semptr2;
		semptr2 = &semtab[piptr->pcsem];	
		if (semptr1->scount < 0) {
			semreset(piptr->ppsem, 0);
		}
		if (semptr2->scount < 0) {
			semreset(piptr->pcsem, 0);
		}
			

		resched_cntl(DEFER_STOP);
	}
	
	if (piptr->pstate == PIPE_EOF) {
		piptr->pstate = PIPE_FREE;
		semdelete(piptr->ppsem);
		semdelete(piptr->pcsem);
		//kprintf("still need to work on EOF case\n");
	}
	
		
	return OK;
}
