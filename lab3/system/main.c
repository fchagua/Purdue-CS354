/*  main.c  - main */

#include <xinu.h>

int32	nprocesses;

int32	total = 0;

int32	t1, t2, t3;

int32 semid;

#ifndef NITER
#define	NITER	120000
#endif

void	incr1(void) {

	int32	i;
	wait(semid);
	for(i=0; i<NITER;i++) {
		t1 = ++total;
	}
	nprocesses--;
	signal(semid);
}

void	incr2(void) {

	int32	i;
	wait(semid);
	for(i=0; i<NITER;i++) {
		t2 = ++total;
	}
	nprocesses--;
	signal(semid);

}

void	incr3(void) {

	int32	i;
	wait(semid);
	for(i=0; i<NITER;i++) {
		t3 = ++total;
	}
	nprocesses--;
	signal(semid);
}


process timer(void) {

	int32	t;
	for(t=0;t<12;t++) {
		sleepms(1);
	}
	return(OK);

}


process	main(void)

{
	printf("\n\n");

	kill(2);

	resume(create(timer, 8192, 40, "timer", 0));

	kprintf("Expect %d, %d, %d, and total of %d\n\n", NITER, 2*NITER, 3*NITER, 3*NITER);
	
	semid = semcreate(1);

	resume(create(incr1, 8192, 10, "myproc1", 0));
	resume(create(incr2, 8192, 10, "myproc2", 0));
	resume(create(incr3, 8192, 10, "myproc3", 0));

	nprocesses = 3;

	while(nprocesses > 0) {
		sleepms(1);
	}

	kprintf("Got    %d, %d, %d, and total of %d\n\n",t1,t2,t3, total);

	kprintf("Main process exiting\n\n");

	return OK;
}
