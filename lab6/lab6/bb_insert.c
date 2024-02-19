/* bb_insert.c - bb_insert() */

#include <xinu.h>
#include <bb_prototypes.h>
#include <bb.h>

sid32 sem;

int32 bb_insert(byte b) {
	/* Your code here */
	//kprintf("inside bb_insert\n");
	intmask mask;
	sem = semcreate(1);
	mask = disable();
	if (bb.state != RUNNING) {
		//kprintf("issue with bb_insert where state is not RUNNING bc state is %d\n", bb.state);
		restore(mask);
		return SYSERR;
	}
	/*
	if (bb.current_size  >= BB_SIZE) {
		kprintf("issue with bb being full with new wait\n");
		//return SYSERR;
		//wait(bb.csem);
		//signal(bb.csem);
	} 
	*/	
	//kprintf("in bb_insert\n");
	//wait(bb.csem);
	wait(bb.psem);
	wait(sem);
	bb.bb_buff[bb.tail] = b;
	//signal(bb.csem);
	if (bb.tail == BB_SIZE - 1) {
		bb.tail = 0;
	} 
	else {
		bb.tail++;
	}

	bb.current_size++;
	//signal(bb.psem);
	//kprintf("bb_insert worked with %c with current size at %d\n", b, bb.current_size);
	signal(sem);
	signal(bb.csem);
	
	restore(mask);
	return OK;
}
