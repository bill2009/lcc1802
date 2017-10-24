unsigned int millis=0; unsigned char fractmillis=0;

void LDC(unsigned char c){
	asm(" glo r12 ; pick up the value\n"
		" LDC ;		set the timer\n");
}
void tone(int freq, int dur){ //tone at a particular frequency for a period
	unsigned char t;
	if (0!=freq){
		asm(" STPC ; stop the timer\n");
		if (freq>7800) t=1;
		else if (freq<30) t=255;
		else t=7800/freq;
		printf("f=%d,t=%d\n",freq,t);
		LDC(t);
		asm(" ETQ;  enable the Q toggle\n");
		asm(" STM; start the timer\n");
		delay(dur);
		asm(" STPC\n");
	}else{
		delay(dur);
	}
}
unsigned char GEC(){
	asm(" GEC ;		get the value\n"
		" plo r15\n ldi 0\n phi r15 \n"
		" cretn ;	this is the actual return\n");
	return 42;//just to keep the compiler happy
}
void initmillis(){
	asm(" CID; disable timer interrupts\n");
	LDC(16);//load the timer
	asm(" ldaD R1,.handler\n");
	asm(" STM; start the timer\n");
	asm(" CIE; enable timer interrupts\n");
	return;
	asm(".done: ;millis interrupt cleanup\n"
		" INC 2	  ; X=2!\n"
		" RLXA r15\n"
		" RLXA r14\n"
		" LDA 2	  ; RESTORE DF\n"
		" SHR\n"
		" LDA 2	  ; NOW D\n"
		" RET	  ; now X&P\n");

	asm(".handler: ;actual interrupt handler prolog\n"
		" DEC 2	  ; prepare stack to\n"
		" SAV	  ; SAVE X AND P (from T)\n"
		" BCI .go ; clear timer int\n"
		".go: \n"
		" DEC 2\n"
		" STXD	  ; SAVE D\n"
		" SHLC	  \n"
		" STXD	  ; SAVE DF\n"
		" RSXD r14  ;save memaddr helper reg\n"
		" RSXD r15  ;save work reg\n");

	asm(" ld2 r15,'D',(_millis),0 ;load current millis value\n"
		" inc r15	;increase millis\n"
		" inc r14	;point to fractional part of millis\n"
		" ldn r14	;pick up fractional value immediately following\n"
		" adi 3\n str r14		;add 3 to the fractional part and put it back\n"
		" smi 125	;test for extra count\n"
		" lbnf 		.noxtra ;no borrow, no extra counts\n"
		" str r14	;store the fraction\n"
		" inc r15	;add extra count to millis\n"
		".noxtra: 	;bypass extra count\n"
		" dec r14\n glo r15\n str r14\n"
		" dec r14\n ghi r15\n str r14\n"
		" lbr .done\n");
}




