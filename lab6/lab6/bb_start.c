/* bb_start.c - bb_start() */

#include <xinu.h>
#include <bb_prototypes.h>
#include <bb.h>


//bool8 bb_buff_start = FALSE;

int32 bb_start() {
	/* Your code here */
	if (bb.state != INITIALIZED || bb.start != FALSE) {
		//kprintf("issue with bb_start\n");
		return SYSERR;

	}

	bb.state = RUNNING;
	bb.start = TRUE;
	//semreset(bb.psem, 0);
	semreset(bb.csem, 0);
	//kprintf("bb_start worked\n");
	return OK;
	
}
