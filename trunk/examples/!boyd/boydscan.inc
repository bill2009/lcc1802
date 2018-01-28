_boydscan:			;SCAN THE KEYBOARD
		sex	r14 	;set up "don't care" X register
		rldi	r15,0	; r15 is return value
.scan:		OUT	1                     ;109: 61 	
		B1	.KEY_12                     ;10A: 34 50 	
		B2	.KEY_8                      ;10C: 35 60 	
		B3	.KEY_4                      ;10E: 36 70 	
		B4	.KEY_0                      ;110: 37 80 	
		OUT	2                     ;112: 62 	
		B1	.KEY_13                     ;113: 34 54 	
		B2	.KEY_9                      ;115: 35 64 	
		B3	.KEY_5                      ;117: 36 74 	
		B4	.KEY_1                      ;119: 37 84 	
		OUT	3                     ;11B: 63 	
		B1	.KEY_14                     ;11C: 34 58 	
		B2	.KEY_10                     ;11E: 35 68 	
		B3	.KEY_6                      ;120: 36 78 	
		B4	.KEY_2                      ;122: 37 88 	
		OUT	4                     ;124: 64 	
		B1	.KEY_15                     ;125: 34 5C 	
		B2	.KEY_11                     ;127: 35 6C 	
		B3	.KEY_7                      ;129: 36 7C 	
		B4	.KEY_3                      ;12B: 37 8C 	
		OUT	5                     ;12D: 65 	
		B1	.KEY_DIV_WHOLE              ;12E: 34 99 	
		B2	.KEY_MUL                    ;130: 35 96 	
		B3	.KEY_SUB                    ;132: 36 93 	
		B4	.KEY_ADD                    ;134: 37 90 	
		OUT	6                     ;136: 66 	
		B1	.KEY_REM                    ;137: 34 A5 	
		B2	.KEY_MEM_STORE              ;139: 35 A2 	
		B3	.KEY_MEM_RECALL             ;13B: 36 9F 	
		B4	.KEY_EQU                    ;13D: 37 9C 	
		SEQ                                ;13F: 7B 	
		B1	.KEY_DIV_FIS                ;140: 34 B1 	
		B2	.KEY_CLEAR                  ;142: 35 AE 	
		B3	.KEY_CLR_ENTRY              ;144: 36 AB 	
		B4	.KEY_INV_SIGN               ;146: 37 A8 	
		REQ                                ;148: 7A 	
;here we have no keys pressed, if r15.0 has a value, return it -1
		glo	r15
		bz	.scan
		dec	r15
		sex	r2	;restore the X register before returning
		cretn

.KEY_12		LDI	13                         ;150: F8  C 	
		BR	.KEY_SAVE                   ;152: 30 B4 	
.KEY_13		LDI	14                         ;154: F8  D 	
		BR	.KEY_SAVE                   ;156: 30 B4 	
.KEY_14		LDI	15                         ;158: F8  E 	
		BR	.KEY_SAVE                   ;15A: 30 B4 	
.KEY_15		LDI	16                         ;15C: F8  F 	
		BR	.KEY_SAVE                   ;15E: 30 B4 	
.KEY_8		LDI	 9                         ;160: F8  8 	
		BR	.KEY_SAVE                   ;162: 30 B4 	
.KEY_9		LDI	 10                         ;164: F8  9 	
		BR	.KEY_SAVE                   ;166: 30 B4 	
.KEY_10		LDI	11                         ;168: F8  A 	
		BR	.KEY_SAVE                   ;16A: 30 B4 	
.KEY_11		LDI	12                         ;16C: F8  B 	
		BR	.KEY_SAVE                   ;16E: 30 B4 	
.KEY_4		LDI	 5                         ;170: F8  4 	
		BR	.KEY_SAVE                   ;172: 30 B4 	
.KEY_5		LDI	 6                         ;174: F8  5 	
		BR	.KEY_SAVE                   ;176: 30 B4 	
.KEY_6		LDI	 7                         ;178: F8  6 	
		BR	.KEY_SAVE                   ;17A: 30 B4 	
.KEY_7		LDI	 8                         ;17C: F8  7 	
		BR	.KEY_SAVE                   ;17E: 30 B4 	
.KEY_0		LDI	 1                         ;180: F8  0 	
		BR	.KEY_SAVE                   ;182: 30 B4 	
.KEY_1		LDI	 2                         ;184: F8  1 	
		BR	.KEY_SAVE                   ;186: 30 B4 	
.KEY_2		LDI	 3                         ;188: F8  2 	
		BR	.KEY_SAVE                   ;18A: 30 B4 	
.KEY_3		LDI	 4                         ;18C: F8  3 	
		BR	.KEY_SAVE                   ;18E: 30 B4 	
		
.KEY_ADD	ldi	16+1
		br	.key_save
.KEY_SUB:	ldi	17+1
		br	.key_save
.KEY_MUL:	ldi	20+1
		br	.key_save
.KEY_MEM_STORE:	ldi	19+1
		br	.key_save

.KEY_REM:	ldi	18+1
		br	.key_save
.KEY_CLEAR:	req
		ldi	20+1
		br	.key_save
.KEY_INV_SIGN:	req
.KEY_CLR_ENTRY:	req
.KEY_DIV_FIS:	req
.KEY_EQU:
.KEY_DIV_WHOLE:
.KEY_MEM_RECALL:
.KEY_SAVE:	plo 15
		br .scan