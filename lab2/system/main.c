/*  main.c  - main */

#include <xinu.h>
#include <stdbool.h>
#include <stdio.h>

bool doit = false;

void pr(char x) {
	
		
	while(1) {
		if (doit) {
			kprintf("%c", x);
					}
		int i = 0;
		for (i = 0; i < 100000000; i++);
	}

}


void prsusp(char x) {
	//kprintf("prsusp\n");
	while (1) {
		kprintf("%c", x);
		int i = 0;
		for (i = 0; i < 100000000; i++);
		
		
	}	
	
	
}


process	main(void)
{
	
	pid32	shpid;		/* Shell process ID */

	printf("\n\n");

	/* Create a local file system on the RAM disk */

	lfscreate(RAM0, 40, 20480);


	
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
	}
	*/
		
	resume(create(pr, 8192, 10, "myproc1", 1, 'A'));
	resume(create(pr, 8192, 10, "myproc2", 1, 'B'));
	//kprintf("Finished processes\n");
	
	while (1) {
		char buffer[128];
		//kprintf("in while loop\n");	
		fgets(buffer, sizeof(buffer), CONSOLE);
		//kprintf("%s\n", buffer);
		//fgets or read
		int i = 0;
		for (i = 0; i < 128; i++) {
			if (buffer[i] == '\n') {
				buffer[i] = '\0';
				break;
			}
		} 
			

		if (strcmp(buffer, "print") == 0) {
			doit = true;
		} 
		else {
			doit = false;
		}

		if (strcmp(buffer, "quit") == 0) {
			//kprintf("loop is done\n");
			break;
		}
		//resume(create(pr, 8192, 10, "myproc3", 1, 'C'));	
		
	}


	pid32 p1;
	pid32 p2;
	resume(p1 = create(prsusp, 8192, 9, "myproc3", 1, 'C'));	
	resume(p2 = create(prsusp, 8192, 9, "myproc4", 1, 'D'));
	char buffer[128];
	while (1) {
		//char buffer[128];
		fgets(buffer, sizeof(buffer), CONSOLE);
		int i = 0;
		for(i = 0; i < 128; i++) {
			if (buffer[i] == '\n') {
				buffer[i] = '\0';
				break;
			}
		}

		if (strcmp(buffer, "print") == 0) {
			resume(p1);
			resume(p2);
			
		}
		else {
			suspend(p1);
			suspend(p2);
		}
		if (strcmp(buffer, "quit") == 0) {
			exit();
		}
		
	}



	sleep(4);

	return OK;
}
