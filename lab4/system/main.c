/*  main.c  - main */

#include <xinu.h>
#include <stdlib.h>
extern void dx(void);
extern void dx2(int sleep1, int sleep2, int maxmsg);
extern void msgprint(void);
extern void sendlm(pid32 pid, umsg32 msg);

process	main(void)
{
	//pid32	shpid;		/* Shell process ID */

	printf("\n\n");

	/* Create a local file system on the RAM disk */

	lfscreate(RAM0, 40, 20480);

	//kprintf("Quick check\n");
	
	//i = rand() % (50 + 1 - 10) + 10;
	//kprintf("%d\n", i);
	pid32 proc1;
	pid32 proc2;
	pid32 proc3;
	pid32 proc4;
	
	//recvclr();

	resume(proc1 = create(dx, 8192, 10, "myproc1", 1));
	resume(proc2 = create(dx, 8192, 10, "myproc2", 1));
	resume(proc3 = create(dx, 8192, 10, "myproc3", 1));
	resume(proc4 = create(dx, 8192, 10, "myproc4", 1));
	/*
	kprintf("%d\n", proc1);
	kprintf("%d\n", proc2);
	kprintf("%d\n", proc3);
	kprintf("%d\n", proc4);
	*/

	//umsg32 msg1;
	//umsg32 msg2;
	//umsg32 msg3;
	//umsg32 msg4;
	//msg = receive();
	//kprintf("%d\n", msg);
	int count = 0;
	while(TRUE) {
		recvclr();
		receive();
		//kprintf("%d\n", msg1);
		count++;
		//kprintf("%d\n", count);
		/*
		msg2 = receive();
		//kprintf("%d\n", msg2);
		count++;
		msg3 = receive();
		//kprintf("%d\n", msg3);
		count++;
		msg4 = receive();
		//kprintf("%d\n", msg4);
		count++;
		*/
		if (count == 4) {
			kprintf("all four messages received\n");
			break;
		}
		
	}
	
	//kprintf("part 1 finished\n");
	sleep(4);

	pid32 proc5;
	pid32 proc6;
	pid32 proc7;
	pid32 proc8;

	resume(proc5 = create(dx2, 8192, 10, "myproc5", 1, 20, 10, 14));
	resume(proc6 = create(dx2, 8192, 10, "myproc6", 1, 20, 10, 14));
	resume(proc7 = create(dx2, 8192, 10, "myproc7", 1, 20, 10, 14));
	resume(proc8 = create(dx2, 8192, 10, "myproc8", 1, 20, 10, 14));

	pid32 mypid = getpid();
	send(proc5, mypid);
	send(proc6, mypid);
	send(proc7, mypid);
	send(proc8, mypid);

	umsg32 msg5;
	//umsg32 msg6;
	//umsg32 msg7;
	//umsg32 msg8;

	count = 0;
	int proc_count = 0;
	while(TRUE) {
		recvclr();
		msg5 = receive();
		//kprintf("%d\n", msg5);
		
		/*
		msg6 = receive();
		kprintf("%d\n", msg2);

		msg7 = receive();
		kprintf("%d\n", msg3);

		msg8 = receive();
		kprintf("%d\n", msg4);
		*/
		if (msg5 == proc5) {
			//sleepms(200);
			//kprintf("Proc5 received\n");
			proc_count++;
			//kprintf("%d\n", count);
			

		}
		else if (msg5 == proc6) {
			//sleepms(200);
			//kprintf("Proc6 received\n");
			proc_count++;
			//kprintf("%d\n", count);


		}
		else if (msg5 == proc7) {
			//sleepms(200);
			//kprintf("Proc7 received\n");
			proc_count++;
			//kprintf("%d\n", count);


		}
		else if (msg5 == proc8) {
			//sleepms(200);
			//kprintf("Proc8 received\n");
			proc_count++;
			//kprintf("%d\n", count);


		}
		else {
			count++;
		}
		
		
		//kprintf("%d\n", count);
		//kprintf("%d\n", proc_count);
		if (proc_count == 4) {
			kprintf("received all four PIDS   total messages received is %d\n", count + proc_count);
			break;
		}
		
	}
	//kprintf("part 2 finished\n");
	sleep(4);

	pid32 proc9;
	pid32 proc10;

	resume(proc9 = create(msgprint, 8192, 12, "myproc9", 1));
	resume(proc10 = create(msgprint, 8192, 12, "myproc10", 1));
	//kprintf("%d\n", proc9);
	//kprintf("%d\n", proc10);
	//send 0
	int mnum = 0;
	int i = 0;
	for (i = 0; i < 5; i++) {
		//kprintf("%d\n", mnum);
		sendlm(proc9, mnum);
		mnum++;
		//kprintf("%d\n", mnum);
		sendlm(proc10, mnum);
		mnum++;
		
		
	}

	kprintf("main process exits\n");
	sleep(4);



	/* Run the Xinu shell */
	/*
	recvclr();
	resume(shpid = create(shell, 8192, 50, "shell", 1, CONSOLE));
	*/
	/* Wait for shell to exit and recreate it */
	/*
	while (TRUE) {
	    if (receive() == shpid) {
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(shpid = create(shell, 4096, 20, "shell", 1, CONSOLE));
	    }
	}*/
	return OK;
}
