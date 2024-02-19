/* bb_endextract.c - bb_endextract() */

#include <xinu.h>
#include <bb_prototypes.h>
#include <bb.h>

int32 bb_endextract() {
    /* Your code here */
	//marks the end of the output from the bb
	//returns the buffer to the initialized-but-not-started state
	//and leaves the data structure ready for bb_start to restart it for another round of use
	//returns ok or syserr
	int32 mask;
	mask = disable();
	if (bb.state != OUTPUT_ONLY) {
		kprintf("issue with bb_endextract where state is not OUTPUT_ONLY\n");
		restore(mask);
		return SYSERR;
	}
	
	bb.state = INITIALIZED;
	bb.start = FALSE;
	//kprintf("bb_endextract worked with state as %d\n", bb.state);
	restore(mask);
	return OK;
}
