;Standalone reg save included at front of lcc1802init
;16-02-05 changed bnz to lbnz for page error
;16-02-06 changed another bnz to lbnz
	seq
	ldad r15,0x7f00+127	;begin register save
	sex r15			;set up for reg saves
	pushm r14
	pushm r13
	pushm r12
	pushm r11
	pushm r10
	pushm r9
	pushm r8
	pushm r7
	pushm r6
	pushm r5
	pushm r4
	pushm r3
	pushm r2
	pushm r1
;now copy 64 bytes of  main memory from 2900
	ldad r15,0x7f00
	ldad r14,0x2900
	ldad r13,64
$$stm:
	lda r14
	str r15
	inc r15
	dec r13
	glo r13
	lbnz $$stm
;now copy top 128+16 bytes of  stack area
	ldad r15,0x7f70
	ldad r14,0x7e70
	ldad r13,128+16
	
$$stk:
	lda r14
	str r15
	inc r15
	dec r13
	glo r13
	lbnz $$stk
;end of standalone save
	req
	
