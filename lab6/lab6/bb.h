/* bb.h -- declare global variables used for the bounded buffer */
//fndef bb_buff_len
//define bb_buff_len	128
#include <bb_prototypes.h>

#define UNINITIALIZED	0
#define INITIALIZED	1
#define RUNNING		2
#define OUTPUT_ONLY	3


struct bb_blk {
	int32 head;
	int32 tail;
	sid32 csem;
	sid32 psem;
	int32 current_size;
	char  bb_buff[BB_SIZE];
	int32 state;
	bool8 start;
	

};
extern struct bb_blk bb;
extern int32 bb_buff_state;
extern bool8 bb_buff_start;
