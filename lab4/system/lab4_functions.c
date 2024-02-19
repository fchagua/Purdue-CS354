#include <xinu.h>
#include <stdlib.h>
#include <stdio.h>
//extern void dx (void);

void dx () {
	int i = rand() % (50 + 1 - 10) + 10;
	sleepms(i);
	return;
}

void dx2 (int sleep1, int sleep2, int maxmsg) {
	int p;
	p = receive();
	
	//srand(count1000);
	int first = rand() % (sleep1 + 1);
	
	//kprintf("delay 1: %d\n", first);
	sleepms(first);

	int random_m = rand() % (maxmsg + 1);
	send(p, random_m);
	
	int second = rand() % (sleep2 + 1);
	//kprintf("delay 2: %d\n", second);
	sleepms(second);

	return;
	
}

void msgprint () {
	int p = getpid();
	int m;
	while (TRUE) {
		//recvclr();	
		m = receive();
		kprintf("%d got %d\n", p, m);

		if (m > 4) {
			kprintf("%d exits\n", p);
			break;
		}
		//recvclr();
		
	}
	return;
	
}

