/* bb_init.c - bb_init() */

#include <xinu.h>
#include <bb.h>
#include <bb_prototypes.h>

//struct bb_buff_blk b_buffb[bb_buff_size];
//int32 bb_buff_size;
//int32 bb_buff_state = BB_UNINITIALIZED;
//struct bb_blk bb = (struct bb_blk *) malloc(sizeof(struct bb_blk));
//bb->head = 0;
//bb->tail = 0;
//bb->psem = semcreate();
//bb->csem = semcreate();
struct bb_blk bb;
int32 bb_buff_state = UNINITIALIZED;
bool8 bb_buff_start = FALSE;
//bb.state = UNINITIALIZED;

int32 bb_init() {
	/* Your code here */
	bb.head = 0;
	bb.tail = 0;
	bb.current_size = 0;
	bb.start = FALSE;
	bb.state = bb_buff_state;
	bb.csem = semcreate(0);
	bb.psem = semcreate(BB_SIZE - bb.current_size);
	
	if (bb_buff_state != UNINITIALIZED) {
		//bb_buff_size = 0;
		//bb_buff_state = INITIALIZED;
		//kprintf("issue with bb_init\n");
		return SYSERR;
	} 
	bb.state = INITIALIZED;
	bb_buff_state = INITIALIZED;
	//kprintf("bb_init worked\n");
	//kprintf("values of bb are that head is %d, tail is %d, current size is %d, state is %d, start is %d, psem is %d, csem is %d, and length of buffer is %d\n", bb.head, bb.tail, bb.current_size, bb.state, bb.start, bb.psem, bb.csem, sizeof(bb.bb_buff));
	return OK;
}
