/* bb_endinsert.c - bb_endinsert() */

#include <xinu.h>
#include <bb_prototypes.h>
#include <bb.h>

int32 bb_endinsert() {
	/* Your code here */
	//marks the end of input to the bounded buffer
	//returns ok or syserr
	int32 mask;
	mask = disable();
	//sid32 sem = semcreate(1);

	if (bb.state != RUNNING) {
		//kprintf("issue with bb_endinsert where state is not RUNNING\n");
		restore(mask);
		return SYSERR;
	}
	/*
	wait(sem);
	bb.state = OUTPUT_ONLY;
	kprintf("%d processes at endinsert\n", bb.current_size);
	signaln(bb.csem, bb.current_size);
	*/
	//semreset(bb.csem, bb.current_size);
	//semreset(bb.csem, BB_SIZE);
	bb.state = OUTPUT_ONLY;

	//semreset(bb.csem, 0);
	semreset(bb.psem, 0);
	//kprintf("bb_endinsert worked with state %d\n", bb.state);
	//signal(sem);
	restore(mask);
	return OK;
}
