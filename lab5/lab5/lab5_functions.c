#include <xinu.h>

void p1() {
	kprintf("\n\nBeginning part 1:\n\n");
	// Add step 4 implementation here
	// Use kprintf("min address: 0x%x, max address: 0x%x, num bytes: %<format specifier here>\n", your vars here); for step 4
	struct procent *pt;
	int i = 0;
	int minSort[prcount];
	int maxSort[prcount];
	int arrcount = 0;

	for (i = 0; i < NPROC; i++) {

		pt = &proctab[i];
		if (pt->prstate != 0) {

			minSort[arrcount] = pt->prstkbase - pt->prstklen - STACKMAGIC;
			maxSort[arrcount] = pt->prstkbase;
			arrcount++;
			kprintf("min address: 0x%x, max address: 0x%x, num bytes: %d, num KB: %d\n", pt->prstkbase - pt->prstklen - STACKMAGIC, pt->prstkbase, pt->prstklen, (uint32) pt->prstklen / (uint32) 1024);
		}
	}
	// Add step 5 implementation here
	int j = 0;
	int temp = 0;
	for (i = 0; i < prcount; i++) {
		for (j = i + 1; j < prcount; j++) {
			if (maxSort[i] < maxSort[j]) {
				temp = maxSort[i];
				maxSort[i] = maxSort[j];
				maxSort[j] = temp;
			}
			if (minSort[i] < minSort[j]) {
				temp = minSort[i];
				minSort[i] = minSort[j];
				minSort[j] = temp;
			}
		}
	}


	kprintf("Sorted min addresses:\n");
	// Print sorted min addresses using kprintf("address: 0x%x\n", vars here);
	for (i = 0; i < prcount; i++) {
		
		kprintf("address: 0x%x\n", minSort[i]);
	}
			
	kprintf("\nSorted max addresses:\n");
	// Print sorted max addresses using kprintf("address: 0x%x\n", vars here);
	for (i = 0; i < prcount; i++) {

		kprintf("address: 0x%x\n", maxSort[i]);
	}

	kprintf("\n");
	return;
}

void p2() {
	kprintf("\n\nBeginning part 2:\n\n");

	// Add step 6 implementation here
	// Use kprintf("min address: 0x%x, max address: 0x%x, num bytes: %<format specifier here>\n", your vars here); for step 6
  // Use kprintf("calculated total: %<format specifier here>, memlist total: %<format specifier here>\n", your vars here); when printing total and memlist
	struct memblk *curr;
	curr = memlist.mnext;
	int total = 0;
	while (curr != NULL) {
		kprintf("min address: 0x%x, max address: 0x%x, num bytes: %d\n", curr, curr + curr->mlength, curr->mlength);
		total = total + curr->mlength;
		curr = curr->mnext;
	}

	kprintf("calculated total: %d, memlist total: %d\n", total, memlist.mlength); 
	kprintf("\n");
	//part 7
	char *address1;
	char *address2;
	char *address3;
	address1 = getmem(2000);
	address2 = getmem(1000);
	address3 = getmem(2000);
	freemem(address2, 1000);

	kprintf("7.1 address: 0x%x\n", (struct memblk *) address1);
	kprintf("7.2 address: 0x%x\n", (struct memblk *) address2);
	kprintf("7.3 address: 0x%x\n", (struct memblk *) address3);
	kprintf("\n");

	// Add step 8 implementation here
	// Use kprintf("block address: 0x%x, block size: %<format specifier here>\n", your vars here); for step 8
	
	//first, lets print the free list
	curr = memlist.mnext;
	kprintf("current free list after 7.2 is freed: \n");
	
	while (curr != NULL) {
		kprintf("block address: 0x%x block size: %d\n", curr, curr->mlength);
		//kprintf("address? 0x%x\n", curr);	
		curr = curr->mnext;
	}
	kprintf("\n");
	kprintf("free list after 7.1 is freed: \n");
	curr = memlist.mnext;
	kprintf("block address: 0x%x, block size: %d\n", address1, 3000);
	curr = curr->mnext; 
	while (curr != NULL) {
		kprintf("block address: 0x%x, block size: %d\n", curr, curr->mlength);
		curr = curr->mnext;

	}
	kprintf("\n");
	kprintf("free list after 7.3 is freed: \n");
	curr = memlist.mnext;
	//kprintf("block address: 0x%x, block size: %d\n", address1, 5000);
	curr = curr->mnext; 
	int random = 0;
	while (curr != NULL) {
		if (random == 0) {
			kprintf("block address: 0x%x, block size: %d\n", address1, curr->mlength + 5000);
		} 
		else {
			kprintf("block address: 0x%x, block size: %d\n", curr, curr->mlength);

		}
		random++;
		curr = curr->mnext;

	}


	

	return;
}
