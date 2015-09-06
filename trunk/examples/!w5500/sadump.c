//sadump print the contents of memory block 7f00-ff
//where init has saved registers and diagnostics
	unsigned int * sadptr=(unsigned int *)0x7f00;//pointer to sadump block
	unsigned int saloop;
	printf("\nmemory from 2900\n");
	for (saloop=0; saloop<32;saloop+=8){
		printf("%x: %x %x %x %x %x %x %x %x\n",
				saloop*2, sadptr[saloop],sadptr[saloop+1],sadptr[saloop+2],sadptr[saloop+3],sadptr[saloop+4],sadptr[saloop+5],sadptr[saloop+6],sadptr[saloop+7]);
	}
	printf("\nregisters 1-14\n");
	for (saloop=1; saloop<15;saloop+=1){
		printf("%x: %x\n",
				saloop, sadptr[saloop+49]);
	}
	printf("\nstack area\n");
	for (saloop=48; saloop<128;saloop+=8){
		printf("%x: %x %x %x %x %x %x %x %x\n",
				saloop*2+0x7e00, sadptr[saloop],sadptr[saloop+1],sadptr[saloop+2],sadptr[saloop+3],sadptr[saloop+4],sadptr[saloop+5],sadptr[saloop+6],sadptr[saloop+7]);
	}
