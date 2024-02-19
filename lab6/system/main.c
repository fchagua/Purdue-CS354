/*  main.c  - main */

#include <xinu.h>
#include <bb_prototypes.h>
#include <bb.h>


void producer1(void);
void consumer1(void);

void producer2(void);
void consumer2(void);

void producer3(void);
void consumer3(void);

void producer4(void);
void consumer4(void);

void producer5(void);
void producer6(void);
void consumer5(void);

void producer7(void);
void consumer6(void);
void consumer7(void);

void producer8(void);
void producer9(void);
void consumer8(void);
void consumer9(void);

sid32 main_sem;
pid32 proc1;
pid32 proc2;
pid32 proc3;
pid32 proc4;

process	main(void)
{
	
	
	//kprintf("start of program with sem_main as %d\n", sem_main);
	main_sem = semcreate(1);

	bb_init();
	bb_start();

	/*
	resume(create((void *)producer4, INITSTK, 10, "Producer", 0, NULL));
	resume(create((void *)consumer4, INITSTK, 10, "Consumer", 0, NULL));
	*/
	/*
	resume(proc1 = create((void *)producer5, INITSTK, 10, "Producer", 0, NULL));
	resume(proc2 = create((void *)producer6, INITSTK, 10, "Producer2", 0, NULL));
	resume(create((void *)consumer5, INITSTK, 10, "Consumer", 0, NULL));
	*/
	//kprintf("proc1 is %d\n", proc1);
	//kprintf("proc2 is %d\n", proc2);
	
	//kprintf("After processes with state as %d\n", bb.state);
	/*
	resume(create((void *)producer7, INITSTK, 10, "Producer", 0, NULL));
	resume(proc1 = create((void *)consumer6, INITSTK, 10, "Consumer", 0, NULL));
	resume(proc2 = create((void *)consumer7, INITSTK, 10, "Consumer2", 0, NULL));
	*/
	
	resume(proc1 = create((void *)producer8, INITSTK, 10, "Producer", 0, NULL));
	resume(proc2 = create((void *)producer9, INITSTK, 10, "Producer2", 0, NULL));
	resume(proc3 = create((void *)consumer8, INITSTK, 10, "Consumer", 0, NULL));
	resume(proc4 = create((void *)consumer9, INITSTK, 10, "Consumer2", 0, NULL));
	//kprintf("proc1 is %d\n", proc1);
	//kprintf("proc2 is %d\n", proc2);
	//kprintf("proc3 is %d\n", proc3);
	//kprintf("proc4 is %d\n", proc4);

	return OK;
}

void producer1(void) {
	bb_endinsert();
}

void producer2(void) {
	bb_insert('C');
	bb_insert('S');
	bb_insert(3);
	bb_insert(5);
	bb_insert(4);
	bb_endinsert();

}

void producer3(void) {
	//bb_insert(BB_SIZE);
	wait(main_sem);
	kprintf("sending %d bytes\n", BB_SIZE);
	int i = 0;
	for (i = 0; i < BB_SIZE; i++) {
		bb_insert(i);
	}
	bb_endinsert();
	signal(main_sem);
}

void producer4(void) {
	//sleepms(10);
	//sid32 main_sem = semcreate(1);
	//kprintf("sem in pro is %d\n", main_sem);
	//wait(bb.psem);
	//wait(main_sem);
	int sent = 3 * BB_SIZE + 25;
	kprintf("sending %d bytes\n", sent);
	int i = 0;
	//wait(main_sem);
	for (i = 0; i < sent; i++) {
		wait(bb.psem);
		//kprintf("inside producer4 loop\n");
		wait(main_sem);
		//kprintf("inside producer4 loop number %d\n", i);
		//kprintf("");
		//wait(bb.psem);
		bb_insert(i % 256);
		signal(main_sem);
		signal(bb.csem);
		//signal(main_sem);
	}
	//kprintf("finished prod4\n");
	//wait(main_sem);	
	bb_endinsert();
	//signal(main_sem);
	//kprintf("state after prod4 is %d\n", bb.state);
	/*
	struct sentry *semptr;
	struct sentry *semptr2;
	semptr = &semtab[bb.csem];
	semptr2 = &semtab[bb.psem];
	kprintf("consum count %d and produc count is %d\n", semptr->scount, semptr2->scount);
	*/
	//signal(bb.csem);
	//signal(main_sem);
}

void producer5(void) {
	//sleepms(10);
	sid32 sem5 = semcreate(1);
	
	int32 sent = 2 * BB_SIZE;
	kprintf("sending %d As\n", sent);
	int32 i = 0;
	for (i = 0; i < sent; i++) {
		wait(bb.psem);
		wait(sem5);
		//kprintf("sent %d As\n", i);
		bb_insert('A');
		//kprintf("A at %d\n", i);
		signal(sem5);
		signal(bb.csem);
	}
	//kprintf("\n\nproducer5 is done\n\n");
	//wait(main_sem);
	send(proc2, sent);
	//signal(main_sem);
	
	//bb_endinsert();
	
}

void producer6(void) {
	//sleepms(10);
	sid32 sem6 = semcreate(1);
	int32 sent = BB_SIZE;
	/*
	umsg32 notice;	
	while(TRUE) {
		//recvclr();
		notice = receive();
		kprintf("\n\nmessage is %d\n\n", notice);
		if (notice == (2 * BB_SIZE)) {
			break;
		}
	}
	*/
	kprintf("sending %d Bs\n", sent);
	int32 i = 0;
	for (i = 0; i < sent; i++) {
		wait(bb.psem);
		wait(sem6);
		//kprintf("sent %d Bs\n", i);
		bb_insert('B');
		signal(sem6);
		signal(bb.csem);
	}
	//kprintf("\n\nproducer6 is done\n\n");
	umsg32 notice;	
	while(TRUE) {
		//recvclr();
		notice = receive();
		//kprintf("\n\nmessage is %d\n\n", notice);
		if (notice == (2 * BB_SIZE)) {
			bb_endinsert();
		}
	}
	

	//bb_endinsert();
	
}

void producer7(void) {
	int32 sent = 100 * BB_SIZE;
	int32 i = 0;
	for (i = 0; i < sent; i++) {
		wait(bb.psem);
		wait(main_sem);
		bb_insert(i % 256);
		signal(main_sem);
		signal(bb.csem);
	}
	//kprintf("producer7 done\n");
	bb_endinsert();
	
}

void producer8(void) {
	int32 sent = 100 * BB_SIZE;
	int32 i = 0;
	sid32 sem8 = semcreate(1);
	wait(main_sem);
	kprintf("Sending %d As\n", sent);
	signal(main_sem);

	for (i = 0; i < sent; i++) {
		wait(bb.psem);
		wait(sem8);
		bb_insert('A');
		signal(sem8);
		signal(bb.csem);
	}
	/*
	wait(sem8);
	kprintf("producer8 done\n");
	signal(sem8);
	*/
	//bb_endinsert();
	send(proc2, proc1);
	
}


void producer9(void) {
	int32 sent = 2 * BB_SIZE;
	int32 i = 0;
	sid32 sem9 = semcreate(1);
	wait(main_sem);
	kprintf("Sending %d Bs\n", sent);
	signal(main_sem);

	for (i = 0; i < sent; i++) {
		wait(bb.psem);
		wait(sem9);
		bb_insert('B');
		signal(sem9);
		signal(bb.csem);
	}
	
	/*
	wait(main_sem);
	kprintf("producer9 done\n");
	signal(main_sem);
	*/

	umsg32 message;
	while(TRUE) {
		message = receive();
		//kprintf("\n\nmessage is %d\n\n", message);
		if (message == proc1) {
			bb_endinsert();
			break;
		}

	}
	
		
}





void consumer1(void) {
	int32 b = bb_extract();
	if (b == EOF) {
		kprintf("Test 1 Sucessful\n");
	} else {
		kprintf("Test 1 Failed\n expected: EOF actual: %c\n", b);
	}
}

void consumer2(void) {
	bb_extract();
	bb_extract();
	bb_extract();
	bb_extract();
	bb_extract();
	int32 b = bb_extract();	

	if (b == EOF) {
		kprintf("Test 2 Sucessful\n");
	} else {
		kprintf("Test 2 Failed\n expected: EOF actual: %c\n", b);
	}
}

void consumer3(void) {
	wait(main_sem);
	int i = 0;
	for (i = 0; i < BB_SIZE; i++) {
		bb_extract();
	}

	int32 b = bb_extract();
	if (b == EOF) {
		kprintf("Test 3 Sucessful\n");
	} else {
		kprintf("Test 3 Failed\n expected: EOF actual: %c\n", b);
	}
	signal(main_sem);

}

void consumer4(void) {
	//sleepms(40);
	//sid32 main_sem = semcreate(1);
	//kprintf("sem in con is %d\n", main_sem);
	//wait(main_sem);
	//wait(bb.csem);
	int32 sent = 3 * BB_SIZE + 25;
	int32 i = 0;
	//wait(main_sem);
	for (i = 0; i < sent; i++) {
		//kprintf("inside consumer4 loop\n");
		wait(bb.csem);
		wait(main_sem);
		//kprintf("inside consumer4 loop number %d and current size is %d\n", i, bb.current_size);
		bb_extract();
		//kprintf("state of bb is %d at num %d\n", bb.state, i);
		signal(main_sem);
		signal(bb.psem);
	}
	//kprintf("finished cons4\n");
	//bb_extract();
	int32 b = bb_extract();
	//kprintf("final extract was %d\n", b);
	//signal(bb.psem);
	//signal(main_sem);
	if (b == EOF) {
		kprintf("Test 4 Sucessful\n");
	} else {
		kprintf("Test 4 Failed\n expected: EOF actual: %d\n", b);
	}
	//signal(bb.psem);
	//signal(main_sem);

}

void consumer5(void) {
	
	int32 sent = 3 * BB_SIZE;
	kprintf("%d number of messages\n", sent);
	int32 acount = 0;
	int32 bcount = 0;
	int32 i = 0;
	int32 temp;
	for (i = 0; i < sent; i++) {
		wait(bb.csem);
		wait(main_sem);
		temp = bb_extract();
		//kprintf("%c at %d\n", temp, i);
		if (temp == 'A') {
			acount++;
		} 
		if (temp == 'B') {
			bcount++;
		}
	
		signal(main_sem);
		signal(bb.psem);

		//bb_extract();
	}

	int32 b = bb_extract();
	//kprintf("bb attr: state %d, start %d\n", bb.state, bb.start);
	
	if (b == EOF) {
		kprintf("Test 5 Sucessful\n");
		kprintf("acount is %d and bcount is %d\n", acount, bcount);
		bb_endextract();
		//kprintf("message after endextract\n");
		//kprintf("%d\n", bb.state); 
	} else {
		kprintf("Test 5 Failed\n expected: EOF actual: %d\n", b);
	}

}

void consumer6(void) {
	int32 con6 = 0;
	sid32 sem6 = semcreate(1);
	int32 temp = bb_extract();
	char result[256] = {0};
	result[temp]++;

	while (temp != EOF) {
		wait(bb.csem);
		wait(sem6);

		temp = bb_extract();
		result[temp]++;
		con6++;

		signal(sem6);
		signal(bb.psem);
	}
	wait(main_sem);
	kprintf("consumer6 done with count6:%d\n", con6);
	int i = 0;
	for (i = 0; i < 256; i++) {
		kprintf("Char value %d has count %d\n", i, result[i]);
	} 
	
	signal(main_sem);
	send(proc2, proc1);
}

void consumer7(void) {
	int32 con7 = 0;
	sid32 sem7 = semcreate(1);
	int32 temp = bb_extract();
	char result[256] = {0};
	result[temp]++;

	while (temp != EOF) {
		wait(bb.csem);
		wait(sem7);

		temp = bb_extract();
		result[temp]++;
		con7++;

		signal(sem7);
		signal(bb.psem);
	}

	wait(main_sem);
	kprintf("consumer7 done with count7:%d\n", con7);
	int i = 0;
	for (i = 0; i < 256; i++) {
		kprintf("Char value %d has count %d\n", i, result[i]);
	} 
	
	signal(main_sem);

	umsg32 message = receive();
	if (message == proc1) {
		bb_endextract();
	}

}

void consumer8(void) {
	int32 con8 = -1;
	sid32 sem8 = semcreate(1);
	int32 temp;
	
	int32 acount = 0;
	int32 bcount = 0;

	while (temp != EOF) {
		wait(bb.csem);
		wait(sem8);

		temp = bb_extract();
		if (temp == 'A') {
			acount++;
		}
		if (temp == 'B') {
			bcount++;
		}
		con8++;

		signal(sem8);
		signal(bb.psem);
	}

	
	wait(main_sem);
	kprintf("consumer8 done with count8:%d, acount:%d, bcount:%d\n", con8, acount, bcount);
	//kprintf("\n\nmessage sent\n\n");
	signal(main_sem);

	send(proc4, proc3);
}


void consumer9(void) {
	int32 con9 = -1;
	sid32 sem9 = semcreate(1);
	int32 temp;
	
	int32 acount = 0;
	int32 bcount = 0;
	

	while (temp != EOF) {
		wait(bb.csem);
		wait(sem9);

		temp = bb_extract();
		if (temp == 'A') {
			acount++;
		}
		if (temp == 'B') {
			bcount++;
		}
		con9++;

		signal(sem9);
		signal(bb.psem);
	}

	wait(main_sem);
	kprintf("consumer9 done with count9:%d, acount:%d, bcount:%d\n", con9, acount, bcount);
	//kprintf("looking to receive");
	signal(main_sem);
	
	umsg32 message;

	while(TRUE) {
		message = receive();
		//kprintf("\n\nmessage received for con %d\n\n", message);
		if (message == proc3) {
			bb_endextract();
			break;
		}
	}

}



