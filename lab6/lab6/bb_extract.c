/* bb_insert.c - bb_insert() */

#include <xinu.h>
#include <bb.h>
#include <bb_prototypes.h>

sid32 sem;

int32 bb_extract() {
	/* Your code here */
	//extracts one byte of data from bb once it had been started
	//returns ok or syserr
	int32 mask;
	sem = semcreate(1);	
	mask = disable();
	//kprintf("in bb_extract\n");
	
	if (bb.state != RUNNING && bb.state != OUTPUT_ONLY) {
		//kprintf("issue with bb_extract where the state is not RUNNING or OUTPUT_ONLY\n");
		restore(mask);
		return SYSERR;
	}
	
	if (bb.current_size == 0 && bb.state == OUTPUT_ONLY) {
		//kprintf("bb_extract where the buffer is empty and endinsert were called\n");
		restore(mask);
		return EOF;
	}
	/*
	if (bb.current_size != 0 && bb.state == RUNNING) {
		//kprintf("wait in bb_extract with new wait\n");
		//wait(bb.psem);
		//signal(bb.psem);
		//wait(bb.csem);
	}*/
	
	if (bb.current_size != 0 && bb.state == OUTPUT_ONLY) {
		wait(sem);
		int32 result = bb.bb_buff[bb.head];
		if (bb.head == BB_SIZE - 1) {
			bb.head = 0;
		}
		else {
			bb.head++;
		}
	
		bb.current_size--;
		//int32 result = bb.bb_buff[bb.head];
		kprintf("bb_extract worked with result %d\n", result);
		signal(sem);
		restore(mask);
		return result;
		
	}
	
	//kprintf("in bb_extract\n");
	//kprintf("current size is %d\n", bb.current_size);
	//wait(bb.psem);
	//kprintf("bb_extract worked with %d head at %d new current size is %d\n",bb.bb_buff[bb.head], bb.head, bb.current_size - 1);
	wait(bb.csem);
	wait(sem);
	//kprintf("size is %d\n", bb.current_size);
	int32 result = bb.bb_buff[bb.head];
	if (bb.head == BB_SIZE - 1) {
		bb.head = 0;
	}
	else {
		bb.head++;
	}
	
	bb.current_size--;
	//int32 result = bb.bb_buff[bb.head];
	kprintf("bb_extract worked with result %d\n", result);
	//kprintf("");
	signal(sem);
	signal(bb.psem);
	restore(mask);
	return result;
}
