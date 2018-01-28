;BOYD DIMENSION PRINTER.  Eprom Marked Rev 7.
;ACS1805
;
;Original BIN file downloaded March 3, 2017 by Bob Armstrong, Spare Time Gizmos.
;
;Disassembled March 7, 2017.  Josh Bensadon, Toronto, Ont.
;
;First attempt to disassemble.  There is still a lot of unknown code flow due to flags setting/clearing.
;Instead of Subroutine calls, the code uses a number of flags to control return jumps.
;There is definitely a lot of repetious code, unoptimized code and some dead code.
;The code was left intact to produce an identical .bin file to the original file copied from the EPROM.
;
;There are undoubtedly a lot of comment errors as many functions, jumps, etc were simply guessed.
;The exact path of the program flow needs to be further analyzed in detail with great care to the operation of the flags.
;
;
;HARDWARE:
;RCA CDP1805ACE MICROPROCESSOR WITH INTERNAL 64 BYTES RAM AND TIMER
;2532 EPROM, 4K
;
;KEYS PF (PRINT FEED) and NP (NO PRINT) are not under software control.
;NP likely causes an Interrupt to cause the software to stop printer output.
;The Interrupt might also come from the printer on error or paper out?
;And is likely connected to this switch also.
;
;Calculator processes distances as a compound number containing Feet, Inches and Sixteen's of an inch.
;Comments refer to these portions as F, I and S.
;
;In memory, there appears to be 7 of these compound numbers.
;Most use 7 bytes and are a form of non-packed bcd numbers.
;The byte for Sixteenths holds a value from 0 to F.
;The byte for Inches holds a value from 0 to B
;The bytes for Feet Units, Tens and Hundreds are regular BCD
;The next two bytes are flags, either 00 or 01 to indicate Overflow and Sign
;
;Any byte that is set to E0, causes that digit to be either blank on the LED display, or a space printed.
;This allows for leading 0 blanking.
;
;The 7 compound numbers are:
;ACC - Main accumulator for data input and data display
;OPERAND1 - ??? Destination of math functions (does not use code E0 for Leading 0 blanking)
;OPERAND2 - ??? Destination of math functions (does not use code E0 for Leading 0 blanking)
;MEM1 to MEM4 - Memory Storage
;
;??? OPERAND 1 & 2 don't use code E0 for leading 0 blanking, this gets applied when the number is copied to the ACC for display.
;OPERAND 2 doesn't hold OverFlow flag, instead, this is a 6 byte number with the Sign flag as the highest addressed byte
;
;
;Key values entered are stored in the ACC.  The ACC is the variable used to print and display on the LED.
;
;Upon an operation selected (+, -, etc), the value in the ACC moves to the OPERAND and a new value is entered into the ACC as the OPERATOR.
;
;
;Printer output is done by first setting Q=1, then outputting sending ASCII codes via OUT 1, then clearing Q=0
;
;LED Output is done by first reseting the LED Driver chip (Set Q=1, send any value OUT 7, Clear Q=0)
;Then send the values for digits D0 to D7 via OUT 7, chip will advance to next digit automatically.
;First OUT 7 goes to D0, then D1, etc.
; D7 D6 D5 D4 D3 D2 D1 D0
;
;Keyboard is arranged as 28 keys, 7 Rows of 4.
;Columns 1 to 4 are read by EF lines 1-4
;Rows 1 to 6 are activated by sending any value OUT 1 to OUT 6
;Row 7 is activated when Q=1
;
;Col-1	Col-2	Col-3	Col-4
;(12)	(8)	(4)	(0)	Row 1
;(13)	(9)	(5)	(1)	Row 2
;(14)	(10)	(6)	(2)	Row 3
;(15)	(11)	(7)	(3)	Row 4
;(/?)	(X)	(-)	(+)	Row 5
;(REM)	(MS)	(MR)	(=)	Row 6
;(/?)	(C)	(CE)	(+/-)	Row 7
;
;
;
;Memory Usage
;ADDR INIT USAGE
;		1000 to 1006 is the working Accumulator register, used for printing and LED display
;1000 F0   Outputs last digits from ' 0' to '15', (E0= '  ') 	- S (Sixteens)
;1001 E0   Outputs digits from ' 0' to '15', (E0= '  ') 	- I (Inches)
;1002 E0   Outputs digit  from '0' to '9', (E0= ' ')		- F (Feet Units)
;1003 E0   Outputs digit  from '0' to '9', (E0= ' ')		- F (Feet Tens)
;1004 E0   Outputs digit  from '0' to '9', (E0= ' ')		- F (Feet Hundreds)
;1005 E0   Outputs first digit as either '1' or ' '		- Over flow??
;1006 E0   Outputs sign (00=' ', 01='-')			- Sign
ACC	.EQU	06h	;Upper address of compound variable
;1007 00 S	 (OPERAND1)
;1008 00 I	 (OPERAND1)
;1009 00 F	 (OPERAND1)
;100A 00 F	 (OPERAND1)
;100B 00 F	 (OPERAND1)
;100C 00 OV	 (OPERAND1)
;100D 00 SG	 (OPERAND1)
;100E 00 SIXTEENTHS (OPERAND2)
;100F 00 INCHES	    (OPERAND2)
;1010 00 UNITS	    (OPERAND2)
;1011 00 TENS	    (OPERAND2)
;1012 00 HUNDREDS   (OPERAND2)
;1013 00 SIGN       (OPERAND2)
;1014 00 MULTIPLY: Product (high 8 bits)			Multiply Routine Variables
;1015 00 MULTIPLY: Multiplier / Product (low 8 bits)
;1016 00 MULTIPLY:  Multiplicand
;1017 00 ADD: Running Sum (high 8 bits)	;ACC final sum high
;1018 00 ADD: Running Sum (low 8 bits)	;ACC final sum mid
;1019 00				;ACC final sum low
;101A 00 ADD: Running sum of 18 bit number high	;Saved Remainder high
;101B 00 ADD: Running sum of 18 bit number mid	;Saved Remainder mid
;101C 00 ADD: Running sum of 18 bit number low	;Saved Remainder low
;101D ' ' ASCII Char2 to print after output, also TEMP running sum of ACC 18 bit number high
;101E 'C' ASCII Char1 to print after output, also TEMP running sum of ACC 18 bit number mid
;101F -- Temp for storing byte to OUTPUT   , also TEMP running sum of ACC 18 bit number low

;1020 F0 S   MEMORY STORAGE 1
;1021 E0 I
;1022 E0 F
;1023 E0 F
;1024 E0 F
;1025 00 OVR
;1026 00 NEG
MEM1	.EQU	26h

;1027 F0 S   MEMORY STORAGE 2
;1028 E0 I
;1029 E0 F
;102A E0 F
;102B E0 F
;102C 00 OVR
;102D 00 NEG
MEM2	.EQU	2Dh

;102E F0 S   MEMORY STORAGE 3
;102F E0 I
;1030 E0 F
;1031 E0 F
;1032 E0 F
;1033 00 OVR
;1034 00 NEG
MEM3	.EQU	34h

;1035 F0 S   MEMORY STORAGE 4
;1036 E0 I
;1037 E0 F
;1038 E0 F
;1039 E0 F
;103A 00 OVR
;103B 00 NEG
MEM4	.EQU	3Bh

;103C
;103D
;103E Temp for Inches
;103F Temp for Sixteenths

;ACC	EQU	06h	;Upper address of compound variable
;MEM1	EQU	26h	;Upper address of compound variable
;MEM2	EQU	2Dh	;Upper address of compound variable
;MEM3	EQU	34h	;Upper address of compound variable
;MEM4	EQU	3Bh	;Upper address of compound variable

		
		.MSFIRST	;BIG ENDIAN
		
		
		.org	0
		LBR	INIT	;JUMP TO INIT, JUMP BACK TO START0

				;R0=MAIN PC
				;R1=0EFD	ISR_SUB
				;R2=103D	STACK IN RAM FOR ISR
				;R3=0933	SUB PRINT THE ACC
				;R4=0000	BIT FLAGS.  
				;	R4.0.0	Special Addition 0=Normal Addition????
				;	R4.0.6	PRINTER NOT READY (Do not output to Printer)
				;	R4.0.7	Prevents re-entry of ADDITION and SUBTRACTION Routines until a new Number key has been entered
				;
				;	R4.1.0	Reverse order of Difference in Sign between ACC and OPERAND
				;	R4.1.5	Do Subtraction
				;	R4.1.6	Directs program flow 
				;	R4.1.7	Directs program flow to return after addition
				;
				;R5=00??
				;R6=02E1	ADD_MRE_MRD Add Compound Digit at M(RD) to M(RE), Use M(RB) for Size Limit/Carry, M(RC) for Adjust
				;R7=03F9	SUB
				;R8=0647	MULTIPLY Subroutine 8 bit * 8 bit -> 16 bit
				;R9=060C	Divide Subroutine
				;RA.0		Count of keys entered into the ACC buffer, also bit counter for math routines
				;		also holds Whole Number for loop counts
				;RA.1		FLAGS
				;	RA.1.0  Set when ACC is full or cannot accept new key input (can't shift values of 10+ into Feet Units)
				;	RA.1.1	New OPERATOR VALUE, Causes ACC to be cleared upon new data entry of next OPERATOR value
				;	RA.1.2	Directs Key input to enter Whole numbers
				;	RA.1.3	Set when a key Down is processed, prevents re-entry to Key processing Routine
				;		Prevents re-entry to ADDTION routine
				;	RA.1.4	Directs Key input to enter Whole numbers, Divides Feet by Whole Number
				;	RA.1.5	OPERAND Present???
				;	RA.1.6	Resume to L305_RESUME
				;	R4.1.7	Resume to L6E2_RESUME
				
				;RB=00EC	Pointer to Data Limit Size for ADD_MRE_MRD (Compounded Coded Digit)
				;RC=00EE	Pointer to Data Size Adjust for ADD_MRE_MRD (Compounded Coded Digit)
				;RD=101F	POINTER TO ACC BYTES FOR PRINTER OUTPUT & LED DISPLAY OUTPUT
				;RD		POINTER DURING INIT TO CLEAR VARIABLES, POINTER FOR MOVING NUMBERS IN MATH ROUTINES
				;RE=10??	POINTER TO RAM FOR CLEAR, DESTINATION POINTER FOR ACC SHIFTING IN KEYS, POINTER FOR MOVING NUMBERS IN MATH ROUTINES
				;RF=101F	TEMP FOR OUTPUT COMMAND, POINTER FOR 16 BIT DIVISION, Temp for nibble addition

;Define Instruction Substitution
				

	.define PRINT_ACC	sep r3
	.define ADD_MRE_MRD	sep r6
	.define SUB_MRE_MRD	sep r7
	.define MULTIPY		sep r8
	.define DIVIDE		sep r9
	
	
START0		LDI	 7	;Clear RAM
		PLO	RE	;FOR RE=1007 TO 101F
CLR_LP		LDI	 0	;   M(RE)=0
		STR	RE	;NEXT RE
		INC	RE
		GLO	RE
		XRI	20h
		BNZ	CLR_LP
		
				;Clear ACC
RESTART		SEX	RD	;X=D               ;  F: ED 	
		LDI	 6                         ; 10: F8  6 	
		PLO	RD	;RD=1006           ; 12: AD 	
		LDI	 0	;V   M             ; 13: F8  0 	
		STXD		;00  1006  SIGN   (1=NEGATIVE)
		STXD		;00  1005  DIGIT3 (1 OR 0)
		LDI	0E0h	;E0  1004  DIGIT2 (E0=SPACE, ELSE LOW NIBBLE=DIGIT)
		STXD		;E0  1003  DIGIT1
		STXD		;E0  1002  DIGIT0
		STXD		;E0  1001
		STXD		;F0  1000
		LDI	0F0h
		STR	RD	;RD=1000

		LDI	 0      ;RA=0000
		PLO	RA	;Clear Counter
		PHI	RA	;Clear bits RA.1.0 , RA.1.1 , RA.1.2 , RA.1.3 , RA.1.4 , RA.1.5 , RA.1.6 , RA.1.7
			
		PHI	R5	;R5=00??
		LBR	PRINT_C_                     ; 25: C0  8 7B


;------------------------------------		
UPDATE_LED	LDI	10h	                   ; 28: F8 10 	
		PHI	RF                       ; 2A: BF 	
		LDI	1Fh                         ; 2B: F8 1F 	
		PLO	RF	;RF=101F (TEMP FOR OUTPUT)
		SEX	RF	;X=F
		REQ
		
		SEQ		;Q=1	;RESET THE LED SCAN COUNTER???  
		OUT	7	;OUT7 JUNK (LAST CHAR PRINTED) TO RESET COUNTER
		REQ		;Q=0
		DEC	RF                       ; 33: 2F 	

	;
	;OUTPUT ACC TO LEDS
	;FIRST OUT7 GOES TO THE RIGHT MOST LED DIGIT
	;NEXT OUT7 GOES TO THE DIGIT TO THE LEFT OF THE PREVIOUS DIGIT.
	;IE, SCAN IS FROM RIGHT TO LEFT (OPPOSITE THAT OF PRINTER OUTPUT)
	;
	; D7 D6 D5 D4 D3 D2 D1 D0
	;  |  |  |  | M1001 M1000
	;  |  |  |  M1002
	;  |  |  M1003
	;  |  M1004
	;  M1005/M1006, DIGIT WORKS AS BLANK, -, 1, -1
	;
	;
	;DISPLAY FOR M(1000) AND M(1001) TAKE 2 LED DIGITS
	;IF M(RD)=E0 THEN 
	;   OUT7 F, F.  (F IS THE CODE THAT BLANKS THE LED)
	;ELSEIF M(RD)<A THEN
	;   OUT7 M(RD), F   'DISPLAY 0 TO 9 AS 0 TO 9
	;ELSE
	;   OUT7 M(RD)-A, 01  'DISPLAY A TO F AS 10 TO 15
	;ENDIF
	;
	;DISPLAY FOR M(1002) TO M(1004) TAKE 1 LED DIGIT
	;IF M(RD)=E0 THEN 
	;   OUT7 F  (BLANK)
	;ELSE
	;   OUT7 M(RD)   'DISPLAY 0 TO 9 AS 0 TO 9
	;ENDIF
	;
	;DISPLAY FOR M(1005)/M(1006) IS EITHER A: 1, BLANK OR MINUS SIGN ON D7
	;IF M(1005)>0 THEN
	;   OUT7 1
	;ELSEIF M(1006)>0 THEN
	;   OUT7 A  (MINUS SIGN)
	;ELSE
	;   OUT7 F  (BLANK)
	;ENDIF

		LDI	 0                         ; 34: F8  0 	
		PLO	RD	;RD=1000  RIGHT HAND DIGIT
		
				;DISPLAY M(1000) - ON PAIR OF LED DISPLAYS D1&D0
		LDN	RD                       ; 37:  D 	
		XRI	0E0h                         ; 38: FB E0 	
		BZ	UL_D1D0_BLANK                       ; 3A: 32 4E 	
				;	IF M(1000)<>E0 THEN
		LDN	RD                       ; 3C:  D 	
		ANI	0Fh                         ; 3D: FA  F 	
		SMI	10                          ; 3F: FF  A 	
		BDF	UL_D1D0_DDIGIT	;Jump to display Double Digit
				;	***IF M(1000)<A THEN
		LDA	RD                       ; 43: 4D 	
		STR	RF                       ; 44: 5F 	
		OUT	7	;OUT7 LOWNIBBLE(M(1000))
		DEC	RF                       ; 46: 2F 	
		LDI	0Fh                         ; 47: F8  F 	
		STR	RF                       ; 49: 5F 	
		OUT	7	;OUT7 0F
		DEC	RF                       ; 4B: 2F 	
		BR	UL_D1D0_END
				;	ENDIF  (Not in proper order)
UL_D1D0_BLANK	LDI	0Fh                         ; 4E: F8  F 	
		STR	RF                       ; 50: 5F 	
		OUT	7	;OUT7 0F
		DEC	RF                       ; 52: 2F 	
		OUT	7	;OUT7 0F
		DEC	RF                       ; 54: 2F 	
		INC	RD                       ; 55: 1D 	
		BR	UL_D1D0_END
				;	***ELSE
UL_D1D0_DDIGIT	LDA	RD                       ; 58: 4D 	
		ANI	0Fh                         ; 59: FA  F 	
		SMI	10                          ; 5B: FF  A 	
		STR	RF                       ; 5D: 5F 	
		OUT	7	;OUT7 LOWNIBBLE(M(1000)-A)
		DEC	RF                       ; 5F: 2F 	
		LDI	1                         ; 60: F8  1 	
		STR	RF                       ; 62: 5F 	
		OUT	7	;OUT7 01
		DEC	RF                       ; 64: 2F 	
UL_D1D0_END
				;DISPLAY M(1001) - ON PAIR OF LED DISPLAYS D3&D2
		LDN	RD                       ; 65:  D 	
		XRI	0E0h                         ; 66: FB E0 	
		BZ	UL_D3D2_BLANK                         ; 68: 32 7C 	
				;IF M(1001)<>E0 THEN
		LDN	RD                       ; 6A:  D 	
		ANI	0Fh                         ; 6B: FA  F 	
		SMI	10                          ; 6D: FF  A 	
		BDF	UL_D3D2_DDIGIT                         ; 6F: 33 86 	
				;IF M(1001)<A THEN
		LDA	RD                       ; 71: 4D 	
		STR	RF                       ; 72: 5F 	
		OUT	7	;OUT7 SINGLE DIGIT 0-9
		DEC	RF                       ; 74: 2F 	
		LDI	0Fh                         ; 75: F8  F 	
		STR	RF                       ; 77: 5F 	
		OUT	7	;OUT7 BLANK DIGIT
		DEC	RF                       ; 79: 2F 	
		BR	UL_D3D2_END
		
UL_D3D2_BLANK	LDI	0Fh                         ; 7C: F8  F 	
		STR	RF                       ; 7E: 5F 	
		OUT	7	;OUT7 BLANK DIGIT
		DEC	RF                       ; 80: 2F 	
		OUT	7	;OUT7 BLANK DIGIT
		DEC	RF                       ; 82: 2F 	
		INC	RD                       ; 83: 1D 	
		BR	UL_D3D2_END
				
UL_D3D2_DDIGIT	LDA	RD                       ; 86: 4D 	
		ANI	0Fh                         ; 87: FA  F 	
		SMI	10                          ; 89: FF  A 	
		STR	RF                       ; 8B: 5F 	
		OUT	7	;OUT7 0 TO 5 (FROM A-F)
		DEC	RF                       ; 8D: 2F 	
		LDI	1                         ; 8E: F8  1 	
		STR	RF                       ; 90: 5F 	
		OUT	7	;OUT7 1
		DEC	RF                       ; 92: 2F 	
UL_D3D2_END
				;DISPLAY M(1002)  - SINGLE LED DISPLAY ON D4
		LDN	RD                       ; 93:  D 	
		XRI	0E0h                         ; 94: FB E0 	
		BZ	UL_D4_BLANK                   ; 96: 32 9E 	
				;DISPLAY DIGIT
		LDA	RD                       ; 98: 4D 	
		STR	RF                       ; 99: 5F 	
		OUT	7	;OUT7 DIGIT
		DEC	RF                       ; 9B: 2F 	
		BR	UL_D4_END                         ; 9C: 30 A4 	

UL_D4_BLANK	LDI	0Fh                         ; 9E: F8  F 	
		STR	RF                       ; A0: 5F 	
		OUT	7	;OUT7 BLANK LED
		DEC	RF                       ; A2: 2F 	
		INC	RD                       ; A3: 1D
UL_D4_END
				;DISPLAY M(1003)  - SINGLE LED DISPLAY ON D5
		LDN	RD                       ; A4:  D 	
		XRI	0E0h                         ; A5: FB E0 	
		BZ	UL_D5_BLANK                ; A7: 32 AF 	

		LDA	RD                       ; A9: 4D 	
		STR	RF                       ; AA: 5F 	
		OUT	7                     ; AB: 67 	
		DEC	RF                       ; AC: 2F 	
		BR	UL_D5_END                         ; AD: 30 B5 	

UL_D5_BLANK	LDI	0Fh                         ; AF: F8  F 	
		STR	RF                       ; B1: 5F 	
		OUT	7                     ; B2: 67 	
		DEC	RF                       ; B3: 2F 	
		INC	RD                       ; B4: 1D 	
UL_D5_END
				;DISPLAY M(1004)  - SINGLE LED DISPLAY ON D6
		LDN	RD                       ; B5:  D 	
		XRI	0E0h                         ; B6: FB E0 	
		BZ	UL_D6_BLANK                         ; B8: 32 C0 	

		LDA	RD                       ; BA: 4D 	
		STR	RF                       ; BB: 5F 	
		OUT	7                     ; BC: 67 	
		DEC	RF                       ; BD: 2F 	
		BR	UL_D6_END                         ; BE: 30 C6 	

UL_D6_BLANK	LDI	0Fh                         ; C0: F8  F 	
		STR	RF                       ; C2: 5F 	
		OUT	7                     ; C3: 67 	
		DEC	RF                       ; C4: 2F 	
		INC	RD                       ; C5: 1D 	
UL_D6_END
				;DISPLAY M(1005)  - SINGLE LED DISPLAY '1' ON D7 (AND SKIP M(1006)) OR JUMP TO DISPLAY M(1006)
		LDA	RD	;RD increment
		BZ	UL_D7_NOT_OV               ; C7: 32 D0 	
		LDI	1                          ; C9: F8  1 	
		STR	RF                       ; CB: 5F 	
		OUT	7	;OUT7 1
		DEC	RF                       ; CD: 2F 	
		BR	UL_D7_END                  ; CE: 30 E1 	

				;DISPLAY M(1006)  - SINGLE LED DISPLAY '-' OR BLANK ON D7
UL_D7_NOT_OV	LDN	RD                       ; D0:  D 	
		BZ	UL_D7_BLANK                ; D1: 32 DB 	
		LDI	10	;This is the '-' character in the ICM7218B LED Driver font
		STR	RF                       ; D5: 5F 	
		OUT	7                     ; D6: 67 	
		DEC	RF                       ; D7: 2F 	
		BR	UL_D7_END                  ; D8: 30 E1 	
		.DB	0                          ; DA:  0 	

UL_D7_BLANK	LDI	0Fh                        ; DB: F8  F 	
		STR	RF                       ; DD: 5F 	
		OUT	7	;OUT7 F (BLANK LED)
		DEC	RF                       ; DF: 2F 	
		INC	RD                       ; E0: 1D 	
UL_D7_END
	;- - - - - - - LED DISPLAY UPDATE DONE - - - - - - - -


SCAN_KEY_LOOP	LDI	10h                         ; E1: F8 10 	
		PHI	R5                       ; E3: B5 	
		LDI	1Fh                         ; E4: F8 1F 	
		PLO	R5	;R5=101F RAM FOR KEY PRESSED
		REQ                                ; E7: 7A 	
		SEX	RF                       ; E8: EF 	
		LBR	KEY_SCAN_0                  ; E9: C0  0 F0	
		
TBL_LIMIT	.DB	12, 10		;00EC Look up table for ADD_MRE_MRD
TBL_ADJUST	.DB	4, 6		;00EE Ajustment table to add when >= Limit

		
KEY_SCAN_0	GHI	R4                       ; F0: 94 	
		ANI	4	;Test bit R4.1.2
		LBZ	EX_100		;SCAN ALL KEYS if bit R4.1.2 is clear
		LBR	SHORT_SCAN	;SCAN ONLY 2 KEYS
		
		NOP                                ; F9: C4 	Bunch of code NOP'ed?
		NOP                                ; FA: C4 	
		NOP                                ; FB: C4 	
		NOP                                ; FC: C4 	
		NOP                                ; FD: C4 	
		NOP                                ; FE: C4 	
		NOP                                ; FF: C4 	
EX_100		NOP                                ;100: C4 	
		NOP                                ;101: C4 	
		NOP                                ;102: C4 	
		NOP                                ;103: C4 	
		NOP                                ;104: C4 	
		NOP                                ;105: C4 	
		NOP                                ;106: C4 	
		NOP                                ;107: C4 	
		NOP                                ;108: C4 	
		
		;SCAN THE KEYBOARD
		
		OUT	1                     ;109: 61 	
		B1	KEY_12                     ;10A: 34 50 	
		B2	KEY_8                      ;10C: 35 60 	
		B3	KEY_4                      ;10E: 36 70 	
		B4	KEY_0                      ;110: 37 80 	
		OUT	2                     ;112: 62 	
		B1	KEY_13                     ;113: 34 54 	
		B2	KEY_9                      ;115: 35 64 	
		B3	KEY_5                      ;117: 36 74 	
		B4	KEY_1                      ;119: 37 84 	
		OUT	3                     ;11B: 63 	
		B1	KEY_14                     ;11C: 34 58 	
		B2	KEY_10                     ;11E: 35 68 	
		B3	KEY_6                      ;120: 36 78 	
		B4	KEY_2                      ;122: 37 88 	
		OUT	4                     ;124: 64 	
		B1	KEY_15                     ;125: 34 5C 	
		B2	KEY_11                     ;127: 35 6C 	
		B3	KEY_7                      ;129: 36 7C 	
		B4	KEY_3                      ;12B: 37 8C 	
		OUT	5                     ;12D: 65 	
		B1	KEY_DIV_WHOLE              ;12E: 34 99 	
		B2	KEY_MUL                    ;130: 35 96 	
		B3	KEY_SUB                    ;132: 36 93 	
		B4	KEY_ADD                    ;134: 37 90 	
		OUT	6                     ;136: 66 	
		B1	KEY_REM                    ;137: 34 A5 	
		B2	KEY_MEM_STORE              ;139: 35 A2 	
		B3	KEY_MEM_RECALL             ;13B: 36 9F 	
		B4	KEY_EQU                    ;13D: 37 9C 	
		SEQ                                ;13F: 7B 	
		B1	KEY_DIV_FIS                ;140: 34 B1 	
		B2	KEY_CLEAR                  ;142: 35 AE 	
		B3	KEY_CLR_ENTRY              ;144: 36 AB 	
		B4	KEY_INV_SIGN               ;146: 37 A8 	
		REQ                                ;148: 7A 	
		GHI	RA                       ;149: 9A 	
		ANI	0F7h                       ;14A: FA F7 	
		PHI	RA	;Clear bit RA.1.3 All Keys Released
		LBR	 SCAN_KEY_LOOP             ;14D: C0  0 E1	

KEY_12		LDI	12                         ;150: F8  C 	
		BR	KEY_SAVE                   ;152: 30 B4 	
KEY_13		LDI	13                         ;154: F8  D 	
		BR	KEY_SAVE                   ;156: 30 B4 	
KEY_14		LDI	14                         ;158: F8  E 	
		BR	KEY_SAVE                   ;15A: 30 B4 	
KEY_15		LDI	15                         ;15C: F8  F 	
		BR	KEY_SAVE                   ;15E: 30 B4 	
KEY_8		LDI	 8                         ;160: F8  8 	
		BR	KEY_SAVE                   ;162: 30 B4 	
KEY_9		LDI	 9                         ;164: F8  9 	
		BR	KEY_SAVE                   ;166: 30 B4 	
KEY_10		LDI	10                         ;168: F8  A 	
		BR	KEY_SAVE                   ;16A: 30 B4 	
KEY_11		LDI	11                         ;16C: F8  B 	
		BR	KEY_SAVE                   ;16E: 30 B4 	
KEY_4		LDI	 4                         ;170: F8  4 	
		BR	KEY_SAVE                   ;172: 30 B4 	
KEY_5		LDI	 5                         ;174: F8  5 	
		BR	KEY_SAVE                   ;176: 30 B4 	
KEY_6		LDI	 6                         ;178: F8  6 	
		BR	KEY_SAVE                   ;17A: 30 B4 	
KEY_7		LDI	 7                         ;17C: F8  7 	
		BR	KEY_SAVE                   ;17E: 30 B4 	
KEY_0		LDI	 0                         ;180: F8  0 	
		BR	KEY_SAVE                   ;182: 30 B4 	
KEY_1		LDI	 1                         ;184: F8  1 	
		BR	KEY_SAVE                   ;186: 30 B4 	
KEY_2		LDI	 2                         ;188: F8  2 	
		BR	KEY_SAVE                   ;18A: 30 B4 	
KEY_3		LDI	 3                         ;18C: F8  3 	
		BR	KEY_SAVE                   ;18E: 30 B4 	
		
KEY_ADD		LBR	EX_KEY_ADD_PT		;190: C0  F  D  ADD FUNCTION (pretest for overflow)	
KEY_SUB		LBR	EX_KEY_SUB_PT		;193: C0  F 91	SUBTRACT FUNCTION (pretest for overflow)
KEY_MUL		LBR	EX_KEY_MUL_PT		;196: C0  F A0	MULTIPLY FUNCTION (pretest for overflow)
KEY_DIV_WHOLE	LBR	EX_KEY_DIVW_PT		;199: C0  F AF	DIVIDE FUNCTION (pretest for overflow)
KEY_EQU		LBR	EX_KEY_EQU		;19C: C0  8 38	EQUATE FUNCTION (pretest for overflow)
KEY_MEM_RECALL	LBR	MEM_RECALL_GKEY		;19F: C0  7 2E	MEMORY RECALL
KEY_MEM_STORE	LBR	MEM_STORE_PT		;1A2: C0  F BE	MEMORY STORE (pretest for overflow)
KEY_REM		LBR	EX_KEY_F8		;1A5: C0  6 63	REMAINDER KEY????
KEY_INV_SIGN	LBR	EX_KEY_INVSIGN		;1A8: C0  A 65	INVERT SIGN
KEY_CLR_ENTRY	LBR	RESTART			;               CLEAR ENTRY
KEY_CLEAR	LBR	DO_CLEAR		;1AE: C0  B 27	CLEAR (FULL)
KEY_DIV_FIS	LBR	EX_KEY_DIVFIS_P		;1B1: C0  A 79	DIVIDE FIS?


				;K E Y   S A V E 
				;
KEY_SAVE	STR	R5	;Temp store pressed key in memory M(101F)    ;1B4: 55 	
				;Test bit to ensure only 1 Key press gets through at a time, 
				;must release all keys to reset flag
		GHI	RA     
		ANI	8	;Test bit RA.1.3 All keys (previously) released
		LBNZ	SCAN_KEY_LOOP ;Jump bit set  (Loop back indefinitely until all keys are released)


				;If flag has been set, Then accept all keys (including zero)
		GHI	R4                       ;1BB: 94 	
		ANI	20h	;Test bit R4.1.5 Do Subtraction?
		BNZ	KS_10	;Jump if set        ;1BE: 3A C7 	
		
				;If there are already Keys Entered into ACC, Then accept all keys (including zero)
		GLO	RA	;Test RA.0 Key Count > 0 
		BNZ	KS_10	;Jump if > 0        ;1C1: 3A C7 	
		
				;Test if Key = 0.  
				;If we get Key_0 when no other keys entered, RESTART
				;ie: Ignore leading 0's entered.
		LDN	R5	;Test Key          
		LBZ	RESTART	;Jump if Key = 0   
		
KS_10		GHI	R4                        ;1C7: 94 	
		ANI	9Fh                         ;1C8: FA 9F 	
		PHI	R4	;Clear bits R4.1.5 & R4.1.6
		
		GHI	RA                        ;1CB: 9A 	
		ORI	0A8h                        ;1CC: F9 A8 	
		PHI	RA	;Set bits RA.1.3 , RA.1.5 , RA.1.7
				;	RA.1.3	Set when a key Down is processed, prevents re-entry to Key processing Routine
		
				;Test if Keys 10 to 15 are acceptable (ie entering S or I values)
		GHI	RA                       
		ANI	14h	;Test bits RA.1.2 (Directs Key input to enter Whole numbers) , RA.1.4
		BZ	KS_20 	;Jump both bits clear
		
				;Test and Reject Keys 10 to 15
		LDN	R5	;Test Key          ;1D4:  5 	
		SMI	10                          ;1D5: FF  A 	
		LBDF	 SCAN_KEY_LOOP  ;Jump if Key >= 10

KS_20		GHI	RA	;Reject all keys when Data Entry is full or cannot shift
		ANI	 1	;Test bit RA.1.0
		LBNZ	 SCAN_KEY_LOOP ;Jump bit Set        ;1DD: CA  0 E1	
		
		LDN	R5	;Test Key          ;1E0:  5 	
		BNZ	KS_30	;Jump if Key > 0   ;1E1: 3A F4 	
		
				;IF KEY = 0 THEN
				;  IF FLAG(S) THEN
				;    IF COUNTER=0 THEN GOTO SCAN_KEY_LOOP
				;  ELSE
				;    IF COUNTER=3 THEN GOTO SCAN_KEY_LOOP
				;  ENDIF
				;ENDIF
		GHI	RA                       ;1E3: 9A 	
		ANI	14h	;Test bits RA.1.2 (Directs Key input to enter Whole numbers) , RA.1.4
		BNZ	KS_20B                         ;1E6: 3A EE 	
		
				;Prevent 0 Key entry when no other value has been entered yet.
		GLO	RA	;Test RA.0 Key Count = 0 
		LBZ	SCAN_KEY_LOOP
		BR	KS_30
		
				;Prevent 0 Key entry on 3rd digit occasion ???
KS_20B		GLO	RA	;Test RA.0 Key Count = 3
		XRI	3                          ;1EF: FB  3 	
		LBZ	SCAN_KEY_LOOP              ;1F1: C2  0 E1	
;---
;				;Are we now entering the OPERATOR? If so, Clear the ACC of it's value and enter numbers
;
KS_30		GHI	RA                       ;1F4: 9A 	
		ANI	 2	;Test bit RA.1.1
		LBZ	KS_40                       ;1F7: C2  2  E	
				;If bit RA.1.1 then clear ACC before entering key

			;FOR RD=1000 TO 1004
			;  M(RD)=E0 (BLANK)
			;NEXT RD
			;M(1005)=00
			;M(1006)=00
		LDI	 0                         ;1FA: F8  0 	
		PLO	RD	  ;RD=1000
KS_30_LP	LDI	0E0h      ;E0 (BLANK)        ;1FD: F8 E0 	
		STR	RD	
		INC	RD                       ;200: 1D 	
		GLO	RD                       ;201: 8D 	
		XRI	5                          ;202: FB  5 	
		BZ	KS_30_EXIT                 ;204: 32  9 	
		LBR	KS_30_LP                   ;206: C0  1 FD		
KS_30_EXIT	LDI	 0                         ;209: F8  0 	
		STR	RD	;M(1005)=00
		INC	RD                       ;20C: 1D 	
		STR	RD	;M(1006)=00	


		;Can we shift the numbers?
		;IF M(1000) >= 12 THEN No shift  (Inches cannot accept 12 or greater)
		;IF M(1001) >= 10 THEN No shift  (Feet cannot accept 10 or greater)
KS_40		LDI	10h                         ;20E: F8 10 	
		PHI	RF                       ;210: BF 	
		LDI	 0                         ;211: F8  0 	
		PLO	RF	;RF=1000
		LDA	RF	;D=M(1000)
		ANI	0Fh                         ;215: FA  F 	
		SMI	12                          ;217: FF  C 	
		BDF	KS_REJECT_KEY	;Jump if M >= 12 No Shift

		LDN	RF	;D=M(1001)                       ;21B:  F 	
		ANI	0Fh                         ;21C: FA  F 	
		SMI	10                          ;21E: FF  A 	
		BDF	KS_REJECT_KEY	;Jump if M >= 10 No Shift

		;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> (almost) Accept Key into ACC
		INC	RA	;RA.0 Increment Count of Keys entered into ACC

		GHI	RA                       ;223: 9A 	
		ANI	14h	;Test bits RA.1.2 (Directs Key input to enter Whole numbers) , RA.1.4
		BNZ	KS_70                      ;226: 3A 2F 	

		GLO	RA	;Test RA.0 Key Count = 1
		XRI	1                          ;229: FB  1 	
		BZ	KS_SAVE_SIXTH
		BR	KS_80                      ;22D: 30 34 	

KS_70		GLO	RA	;Test RA.0 Key Count = 4  (if it's the first key in for Whole number Entry)
		XRI	4                          ;230: FB  4 	
		BZ	KS_SAVE_FEET               ;232: 32 56 	

				;If this is the 6th Key entered, Reject, ACC is FULL		
KS_80		GLO	RA	;Test RA.0 Key Count = 6
		XRI	6                          ;235: FB  6 	
		BZ	KS_REJECT_KEY              ;237: 32 64 	
		
		;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Now Accept Key into ACC
				;SHIFT ACC LEFT
		LDI	 4      ;RD=1004           ;239: F8  4 	
		PLO	RD	
		LDI	 5      ;RE=1005           ;23C: F8  5 	
		PLO	RE	
KS_SHIFT_LOOP	DEC	RD                       ;23F: 2D 	
		DEC	RE                       ;240: 2E 	
		LDN	RD                       ;241:  D 	
		STR	RE                       ;242: 5E 	
		GLO	RD                       ;243: 8D 	
		BZ	KS_90                      ;244: 32 48 	
		BR	KS_SHIFT_LOOP              ;246: 30 3F 	
		
KS_90		GHI	RA	;Test if we are expecting a whole number (ie, Divide by N)
		ANI	14h	;Test bits RA.1.2 (Directs Key input to enter Whole numbers) , RA.1.4
		BNZ	KS_SAVE_FEET ;Jump to put keys into Feet units
		
				;Get key and put it in M(1000)
KS_SAVE_SIXTH	LDI	 0
		PLO	RD	;RD=1000
		LDN	R5	;Get Key
		ORI	0F0h	;Mask it with F0 to make it a NON-ZERO byte?
		STR	RD	;Save Key to ACC Sixteens position
		BR	KS_SAVE_DONE                     ;254: 30 5D 	

				;Get key and put it in M(1002)
KS_SAVE_FEET	LDI	 2
		PLO	RD	;RD=1002
		LDN	R5	;Get Key
		ORI	0F0h	;Mask it with F0 to make it a NON-ZERO byte?
		STR	RD	;Save Key to ACC Feet Units position
		
KS_SAVE_DONE	GHI	RA                       ;25D: 9A 	
		ANI	0FDh                       ;25E: FA FD 	
		PHI	RA	;Clear bit RA.1.1	Unflag the Clearing of the ACC on key input (which was done when accepting OPERAND)
		
KS_RETURN	LBR	KS_RETURN_PATCH	;Clear bit R4.0.7 then jump to LED_UPDATE

KS_REJECT_KEY	GHI	RA                       ;264: 9A 	
		ORI	 1                         ;265: F9  1 	
		PHI	RA	;Set bit RA.1.0 to Reject all keys when Data Entry is full or cannot shift
		LDI	 0                         ;268: F8  0 	
		PLO	RA	;RA.0 Reset Count of Keys entered into ACC
		BR	KS_RETURN                  ;26B: 30 61 	




EX_SUB		GHI	R4                       ;26D: 94 	
		ORI	20h                        ;26E: F9 20 	
		PHI	R4	;Set bit R4.1.5 Do Subtraction

EX_ADD		GHI	RA			;Do not re-enter Addition if Key still down
		ANI	 8	;Test bit RA.1.3
		BNZ	SCAN_KEY_LOOP_P		;Jump if a key is still down
		
				;Set bits to request new OPERATOR, prevent re-entry, and bit RA.1.7
L276		GHI	RA                       ;276: 9A 	
		ORI	08Ah                         ;277: F9 8A 	
		PHI	RA	;Set bits RA.1.1 , RA.1.3 , RA.1.7

		ANI	20h     ;Test bit RA.1.5   ;27A: FA 20 	
		BZ	SCAN_KEY_LOOP_P	;Jump if clear, Exit if NO OPERAND ???
		
		GHI	RA                       ;27E: 9A 	
		ANI	0DEh                       ;27F: FA DE 	
		PHI	RA	;Clear bits RA.1.0 , RA.1.5
		
		NOP                                ;282: C4 	
		NOP                                ;283: C4 	
		NOP                                ;284: C4 	
		NOP                                ;285: C4 	
		NOP                                ;286: C4 	
		NOP                                ;287: C4 	
		NOP                                ;288: C4
		
L289		GLO	R4                       ;289: 84 	
		ANI	1	;Test bit R4.0.0
		LBNZ	EX_ADD_R1                        ;28C: CA  B 60
		
		GHI	RA                       ;28F: 9A 	
		ANI	 4	;Test bit RA.1.2 (Directs Key input to enter Whole numbers)
		LBNZ	L689                        ;292: CA  6 89
		
		GHI	RA                       ;295: 9A 	
		ANI	10h	;Test bit RA.1.4
		LBNZ	L444                        ;298: CA  4 44
		
L29B		LDI	 0                         ;29B: F8  0 	
		PLO	RA	;RA.0 Reset Count of Keys entered into ACC
		SEX	RE                       ;29E: EE 	
		GHI	R4                       ;29F: 94 	
		ANI	20h	;Test bit R4.1.5 Do Subtraction
		LBNZ	L37F                       ;2A2: CA  3 7F
		LBR	L35A                       ;2A5: C0  3 5A
		
		
;ADD ACC TO OPERAND
ADD_OPR_ACC	LDI	 0	;RD=1000           ;2A8: F8  0 	ADD Sixteenths (nice & easy binary add of 2 nibbles)
		PLO	RD                       ;2AA: AD 	M(1007) = M(1007) + M(1000), DF=Carry
		LDI	 7	;RE=1007           ;2AB: F8  7 	
		PLO	RE                       ;2AD: AE 	
		SEX	RE	;X=E               ;2AE: EE 	
		LDN	RD                       ;2AF:  D 	;Get ACC Sixteeths
		ANI	0Fh                        ;2B0: FA  F 	;Mask Nibble
		ADD                                ;2B2: F4 	;D = D + M(1007)
		PHI	RF                       ;2B3: BF 	;Temp save in RF.1
		SHL                                ;2B4: FE 	;Move carry out bit in D.4 to DF
		SHL                                ;2B5: FE 	
		SHL                                ;2B6: FE 	
		SHL                                ;2B7: FE 	
		GHI	RF                       ;2B8: 9F 	
		ANI	0Fh                        ;2B9: FA  F 	
		STR	RE                       ;2BB: 5E 	;Save Nibble only, DF = Carry out
		
		INC	RD	;RD=1001           ;2BC: 1D 	M(1008) = M(1008) + M(1001), DF=Carry
		INC	RE	;RE=1008           ;2BD: 1E 	
		ADD_MRE_MRD                        ;2BE: D6 	Inches
		INC	RD	;RD=1002           ;2BF: 1D 	
		INC	RE	;RE=1009           ;2C0: 1E 	
		INC	RB	;Limit of 10 for BCD addition
		INC	RC	;Set corresponding Adjustment for limit 10
		ADD_MRE_MRD                       ;2C3: D6 	Feet Units
		INC	RD                       ;2C4: 1D 	
		INC	RE	;RE=100A           ;2C5: 1E 	
		ADD_MRE_MRD                       ;2C6: D6 	Feet Tens
		INC	RD                       ;2C7: 1D 	
		INC	RE	;RE=100B
		ADD_MRE_MRD                       ;2C9: D6 	Feet Hundreds
		INC	RE	;RE=100C
		INC	RD                       ;2CB: 1D 	
		LDN	RD                       ;2CC:  D 	
		ADC                                ;2CD: 74 	
		STR	RE                       ;2CE: 5E 	Over Flow
		
		DEC	RB	;Restore pointers to Limit of 12 for Inches
		DEC	RC	;Restore pointers to Limit of 12 for Inches
		
				;Find which routine to Resume
L2D1		GHI	R4                       ;2D1: 94 	
		ANI	80h	;Test bit R4.1.7 = Resume to L6E2_RESUME
		LBNZ	L6E2_RESUME                ;2D4: CA  6 E2	
		GHI	RA                       ;2D7: 9A 	
		ANI	40h	;Test bit RA.1.6 = Resume to L305_RESUME
		LBNZ	L305_RESUME               ;2DA: CA  3  5	
SCAN_KEY_LOOP_P	LBR	SCAN_KEY_LOOP             ;2DD: C0  0 E1	
		


		
;ADD Inches in M(RD) to M(RE) with DF holding Carry In (from Sixteenths)
;	M(RE) = M(RE) + M(RD) + DF,  DF = Carry Out
;	RF.1 temp register
;TBL_LIMIT	.DB	12, 10		;RB = 00EC Look up table for ADD_MRE_MRD
;TBL_ADJUST	.DB	4, 6		;RC = 00EE Ajustment table to add when >= Limit
;
;ADD_MRE_MRD			;X=E
SEP_R6_RET	SEP	R0                       ;2E0: D0 	
SEP_R6		LDN	RD                       ;2E1:  D 	
		ANI	0Fh                         ;2E2: FA  F 	
		ADC                                ;2E4: 74 	
		PHI	RF                       ;2E5: BF 	
		SEX	RB	;X=B               ;2E6: EB Point X to Limit Look Up Table	
		SM                                 ;2E7: F7 D = D - LIMIT
		BNF	SEP_R6_0 ;Jump if Borrow ie < LIMIT
		GHI	RF                       ;2EA: 9F 	
		SEX	RC	;X=C               ;2EB: EC 	
		ADD                                ;2EC: F4 	
		ANI	0Fh                         ;2ED: FA  F 	
		PHI	RF                       ;2EF: BF 	
		LDI	80h	;DF = 1            ;2F0: F8 80 	
		SHLC                               ;2F2: 7E 	
SEP_R6_0	GHI	RF                       ;2F3: 9F 	
		SEX	RE	;X=E               ;2F4: EE 	
		STR	RE                       ;2F5: 5E 	
		BR	SEP_R6_RET




L2F8		GHI	RA                       ;2F8: 9A 	
		ANI	 8	;Test bits RA.1.3
		LBNZ	 SCAN_KEY_LOOP                       ;2FB: CA  0 E1	
		GHI	RA                       ;2FE: 9A 	
		ORI	06Ah                         ;2FF: F9 6A 	
		PHI	RA	;Set bits RA.1.1 , RA.1.3 , RA.1.5 , RA.1.6
				;RA.1.5 = Do Subtraction?
				;RA.1.6 = Resume to L305_RESUME
		LBR	L289                       ;302: C0  2 89	
		
		
							;Copy OPERAND to ACC (apply Leading 0 blanking)
L305_RESUME	LDI	0Dh	;RE=100D OPERAND SIGN  ;305: F8  D 	
		PLO	RE                       ;307: AE 	
		LDI	 6	;RD=1006 ACC SIGN   ;308: F8  6 	
		PLO	RD                       ;30A: AD 	
		LDN	RE                       ;30B:  E 	
		SEX	RD	;X=D               ;30C: ED 	Copy Sign
		STXD                               ;30D: 73 	M(1006) = M(100D)
		DEC	RE                       ;30E: 2E 	
		LDN	RE                       ;30F:  E 	Copy Overflow flag
		STXD                               ;310: 73 	M(1005) = M(100C)
		BNZ	L321_LOOP ;Jump if overflow  (no leading 0 blanking)
L313_LOOP	DEC	RE                       ;313: 2E 	
		LDN	RE                       ;314:  E 	
		BNZ	L323                       ;315: 3A 23  Test for leading 0's
		LDI	0E0h                       ;317: F8 E0 	Copy blank code for leading 0's
		STXD                               ;319: 73 	
		GLO	RE                       ;31A: 8E 	
		XRI	8     ;Loop until RE=1008  ;31B: FB  8 	Repeat for all digits to Inches
		BZ	L32D                       ;31D: 32 2D 	
		BR	L313_LOOP                  ;31F: 30 13 	
L321_LOOP	DEC	RE                       ;321: 2E 	
		LDN	RE                       ;322:  E 	
L323		ORI	0F0h                       ;323: F9 F0 	Copy non zero numbers in this loop
		STXD                               ;325: 73 	
		GLO	RE                       ;326: 8E 	
		XRI	8                          ;327: FB  8 	
		BZ	L32D                       ;329: 32 2D 	 Repeat for all digits to Inches
		BR	L321_LOOP                  ;32B: 30 21 	
		
L32D		DEC	RE	;RE=1007           ;32D: 2E 	Copy Sixteenths (but never as a blank digit)
		LDN	RE                       ;32E:  E 	
		ORI	0F0h                         ;32F: F9 F0 	
		STR	RD                       ;331: 5D 	

		GHI	RA                       ;332: 9A 	
		ANI	0BFh                         ;333: FA BF 	
		PHI	RA	;Clear bits RA.1.6 (no more Resume to L305_RESUME)
		
		LDI	0Dh	;RD=100D           ;336: F8  D 	CLEAR OPERAND 1
		PLO	RD                       ;338: AD 	
L339_LOOP	LDI	 0                         ;339: F8  0 	M(100D):M(100C):M(100B):M(100A):M(1009):M(1008):M(1007) = 00000000000000
		STXD                               ;33B: 73 	
		GLO	RD                       ;33C: 8D 	
		XRI	6                          ;33D: FB  6 	
		BZ	L343                       ;33F: 32 43 	
		BR	L339_LOOP                  ;341: 30 39 	
		
L343		GHI	R4                       ;343: 94 	
		ANI	0DEh                         ;344: FA DE 	
		PHI	R4	;Clear bits R4.1.0 & R4.1.5
		LBR	PRINT___                       ;347: C0  8 53	



;-------------------------------------------------------------------		
L34A		NOP                                ;34A: C4 	
		NOP                                ;34B: C4 	
		NOP                                ;34C: C4 	
		NOP                                ;34D: C4 	
		NOP                                ;34E: C4 	
		NOP                                ;34F: C4 	
		NOP                                ;350: C4 	
		NOP                                ;351: C4 	
		NOP                                ;352: C4 	
		NOP                                ;353: C4 	
		NOP                                ;354: C4 	
		NOP                                ;355: C4 	
		NOP                                ;356: C4 	
		NOP                                ;357: C4 	
		NOP                                ;358: C4 	
		NOP                                ;359: C4 	
L35A		GHI	R4                       ;35A: 94 	
		ANI	 1	;Test bit R4.1.0
		BNZ	L36D                         ;35D: 3A 6D 	
		
L35F		LDI	6      ;RD=1006 ACC Sign    ;35F: F8  6 	
		PLO	RD                       ;361: AD 	
		LDI	0Dh    ;RE=100D OPERAND Sign  ;362: F8  D 	
		PLO	RE                       ;364: AE 	
		LDN	RD                       ;365:  D 	
		XOR                                ;366: F3 	
		LBNZ	L38D	;Jump if opposite sign   ;367: CA  3 8D	
		LBR	ADD_OPR_ACC                ;36A: C0  2 A8
		
L36D		GHI	R4                       ;36D: 94 	
		ANI	0FEh                         ;36E: FA FE 	
		PHI	R4	;Clear bit R4.1.0
		
L371		LDI	 6      ;RD=1006 ACC Sign  ;371: F8  6 	
		PLO	RD                       ;373: AD 	
		LDI	0Dh    ;RE=100D OPERAND Sign  ;374: F8  D 	
		PLO	RE                       ;376: AE 	
		LDN	RD                       ;377:  D 	
		XOR                                ;378: F3 	
		LBNZ	ADD_OPR_ACC ;Jump if opposite sign  ;379: CA  2 A8	
		LBR	L38D                       ;37C: C0  3 8D

;--------------------------------------------------------------		
L37F		GHI	R4                       ;37F: 94 	
		ANI	 1	;Test bit R4.1.0 Reverse order subtraction
		BZ	L386                         ;382: 32 86 	
		BR	L371                         ;384: 30 71 	
L386		GHI	R4                       ;386: 94 	
		ORI	 1                         ;387: F9  1 	
		PHI	R4	;Set bit R4.1.0
		LBR	L35F                       ;38A: C0  3 5F	


		
;------------------------------------------------------------------	TEST IF ACC < OPERAND1		
L38D		LDI	 6      ;RD=1006 ACC Sign  ;38D: F8  6 	
		PLO	RD                       ;38F: AD 	
		LDI	0Dh	;RE=100D OPERAND1  ;390: F8  D 	
		PLO	RE                       ;392: AE 	
		SEX	RE	;X=E               ;393: EE 	
L394_LOOP	DEC	RE                       ;394: 2E 	
		DEC	RD                       ;395: 2D 	
		GLO	RD                       ;396: 8D 	
		BZ	L39B                       ;397: 32 9B	
		BR	L3A5                       ;399: 30 A5 
			
L39B		LDN	RD                       ;39B:  D 	 For the last digit (Sixteenths digit) subtract test is done here.
		ANI	0Fh                        ;39C: FA  F 	
		SM      	;D=D-M(RX)         ;39E: F7 	
		BZ	L3AD                       ;39F: 32 AD 	Jump if ACC = OPERAND1
		BNF	L3BB                       ;3A1: 3B BB 	Jump if ACC < OPERAND1, (DF=0 this will be tested later)
		BNZ	L3AD	;This is an always branch since BZ @3F9 would happen if Zero  ;3A3: 3A AD 	
		
L3A5		LDN	RD                       ;3A5:  D 	For all other digits, subract test is done here
		ANI	0Fh                        ;3A6: FA  F 	
		SM              ;D=D-M(RX)         ;3A8: F7 	D = ACC - OPERAND1
		BZ	L394_LOOP                  ;3A9: 32 94 	Loopback if same
		BNF	L3BB	;Jump if Borrow    ;3AB: 3B BB 	Jump if ACC < OPERAND1, (DF=0 this will be tested later)
		
		
								;EXECUTE ACC >= OPERAND1 (Invert Sign of Operand1)
L3AD		LDI	0Dh	;RD=100D           ;3AD: F8  D 	
		PLO	RD                       ;3AF: AD 	
		LDN	RD                       ;3B0:  D 	
		BZ	L3B8                       ;3B1: 32 B8 	
			;OPERAND1 is Negative (Make it Positive)
		LDI	 0                         ;3B3: F8  0 	
		STR	RD                       ;3B5: 5D 	
		BR	L3BB                       ;3B6: 30 BB
		
			;OPERAND1 is Positive (Make it Negative)
L3B8		LDI	 1                         ;3B8: F8  1 	
		STR	RD                       ;3BA: 5D 	
			
								;EXECUTE ACC < OPERAND1
L3BB		LDI	 0	;RD=1000 ACC       ;3BB: F8  0 	
		PLO	RD                       ;3BD: AD 	
		LDI	 0                         ;3BE: F8  0 	
		PLO	RD	;RD=1000 STILL ACC ;3C0: AD 	
		LDI	 7                         ;3C1: F8  7 	
		PLO	RE	;RE=1007 OPERAND1  ;3C3: AE 	
		LDN	RD                       ;3C4:  D 	
		ANI	0Fh                        ;3C5: FA  F
		BNF	L3CD   	                   ;3C7: 3B CD 	IF DF=0 THEN ACC < OPERAND1
		
		REQ     ;Q=0 normal order of subtraction ;3C9: 7A  ;When ACC >= OPERAND
		SM      	;D=D-M(RX)         ;3CA: F7 	   ;Execute OPERAND = OPERAND - ACC
		BR	L3CF                       ;3CB: 30 CF 	
		
L3CD		SEQ	;SET Q to reverse order of SUB_MRE_MRD subtraction  ;3CD: 7B 	
		SD              ;D=M(RX)-D         ;3CE: F5 	;When ACC < OPERAND
								;Execute OPERAND = ACC - OPERAND
		
		
L3CF		ANI	0Fh                        ;3CF: FA  F 	;
		STR	RE                       ;3D1: 5E 	;Store Sixteenths difference
		INC	RE                       ;3D2: 1E 	
		INC	RD                       ;3D3: 1D 	
		SUB_MRE_MRD                        ;3D4: D7 	;Subtract & store Inches (with Borrow)
		INC	RE                       ;3D5: 1E 	
		INC	RD                       ;3D6: 1D 	
		INC	RB                       ;3D7: 1B 	;Switch to decimal BCD (0-9)
		SUB_MRE_MRD                       ;3D8: D7 	;Subtract & store Feet Units (with Borrow)
		INC	RD                       ;3D9: 1D 	
		INC	RE                       ;3DA: 1E 	
		SUB_MRE_MRD                       ;3DB: D7 	;Subtract & store Feet Tens (with Borrow)
		INC	RD                       ;3DC: 1D 	
		INC	RE                       ;3DD: 1E 	
		SUB_MRE_MRD                       ;3DE: D7 	;Subtract & store Feet Hundreds (with Borrow)
		DEC	RB                       ;3DF: 2B 	;Switch back to inches coded digit (0-B)

		LDI	0Bh	;RD=100B OPERAND1 HUNDREDS ;3E0: F8 B ;Test Operand1 = 0, Jump NOT Zero
		PLO	RD                       ;3E2: AD 	
L3E3_LOOP	LDN	RD                       ;3E3:  D 	
		LBNZ	L2D1	;Jump if not zero  ;3E4: CA  2 D1  ;JUMP NOT ZERO L2D1
		DEC	RD                       ;3E7: 2D 	
		GLO	RD                       ;3E8: 8D 	
		XRI	6                          ;3E9: FB  6 	
		BZ	L3EF                       ;3EB: 32 EF 	
		BR	L3E3_LOOP                  ;3ED: 30 E3 	
		
L3EF		LDI	0Dh	;RD=100D OPERAND1 Sign ;3EF: F8  D ;Clear Operand1 Sign
		PLO	RD                       ;3F1: AD 	
		LDI	 0                         ;3F2: F8  0 	
		STR	RD                       ;3F4: 5D 	
		LBR	L2D1                       ;3F5: C0  2 D1	

	
;SUBTRACT M(RE) FROM M(RD)
;If Q=1 THEN
;   M(RE) = M(RD) - M(RE) - NOT(DF),  DF = BORROW OUT (0=BORROW)    ;ie OPERAND = ACC - OPERAND
;ELSE
;   M(RE) = M(RE) - M(RD) - NOT(DF),  DF = BORROW OUT (0=BORROW)    ;ie OPERAND = OPERAND - ACC
;
;SUB_MRE_MRD			;X=E
SEP_R7_RET	SEP	R0                       ;3F8: D0 	
SEP_R7		LBNQ	SEP_R7_0                   ;3F9: C9  4  4	
		LDN	RD                       ;3FC:  D 	
		ANI	0Fh                         ;3FD: FA  F 	
		SDB                                ;3FF: 75 	
		BDF	SEP_R7_3 ;Jump no borrow
		BR	SEP_R7_1                   ;402: 30  A 	
SEP_R7_0	LDN	RD                       ;404:  D 	
		ANI	0Fh                        ;405: FA  F 	
		SMB 	;D=D-M(RE)                 ;407: 77 	
		BDF	SEP_R7_3 ;Jump no borrow   ;408: 33 14 	
SEP_R7_1	SEX	RB	;X=B               ;40A: EB 	
		ADD                                ;40B: F4 	
		STR	RE                       ;40C: 5E 	
		LDI	 0                         ;40D: F8  0 	
		SHR 		;Set Borrow        ;40F: F6 	
		SEX	RE	;X=E               ;410: EE 	
SEP_R7_2	LBR	SEP_R7_RET                 ;411: C0  3 F8

SEP_R7_3	STR	RE                       ;414: 5E 	
		BR	SEP_R7_2
	
	
		
EX_MUL		GHI	RA                       ;417: 9A 	
		ORI	 6                         ;418: F9  6 	
		PHI	RA	;Set bits RA.1.1 , RA.1.2 (Directs Key input to enter Whole numbers)
		BR	EX_MULDIV                         ;41B: 30 21 	
		
EX_DIV_WHOLE	GHI	RA                       ;41D: 9A 	
		ORI	012h                       ;41E: F9 12 	
		PHI	RA	;Set bits RA.1.1 , RA.1.4 (Directs Key input to enter Whole numbers)
		
EX_MULDIV	LDI	 3                         ;421: F8  3 	
		PLO	RA	;Set RA.0 Key Count = 3  This is the count to preset whole number entry

L424		GHI	RA                       ;424: 9A 	
		ANI	0FEh                         ;425: FA FE 	
		PHI	RA	;Clear bit RA.1.0
		
							;Copy ACC (less Blank codes) to OPERAND2
							;OPERAND2 = ACC
		LDI	 0	;RD=1000 ACC       ;428: F8  0 	
		PLO	RD                       ;42A: AD 	
		LDI	0Eh	;RE=100E OPERAND2  ;42B: F8  E 	
		PLO	RE                       ;42D: AE 	
L42E_LOOP	LDA	RD                       ;42E: 4D 	;Fetch ACC
		ANI	0Fh                        ;42F: FA  F 	;Eliminate Blank Code
		STR	RE                       ;431: 5E 	;Save OPERAND2
		INC	RE                       ;432: 1E 	
		GLO	RE                       ;433: 8E 	
		XRI	13h                        ;434: FB 13 	
		BZ	L43A                       ;436: 32 3A 	;Repeat for all digits
		BR	L42E_LOOP                  ;438: 30 2E 	
		
L43A		INC	RD                       ;43A: 1D 	;Copy the ACC Sign
		LDN	RD                       ;43B:  D 	
		STR	RE                       ;43C: 5E 	;Overwrite the OPERAND2 Sign
		GHI	R4                       ;43D: 94 	
		ANI	0DFh                         ;43E: FA DF 	
		PHI	R4	;Clear bit R4.1.5
		LBR	SCAN_KEY_LOOP              ;441: C0  0 E1
		
		
							;DIVIDE FEET BY WHOLE NUMBER
							;CONVERT FEET HUNDREDS, TENS AND UNITS TO BINARY NUMBER AT M(1017):M(1018)
L444		LDI	16h	;RE=1016 MULTIPLICAND ;444: F8 16 	
		PLO	RE                       ;446: AE 	
		LDI	12h	;RD=1012 OPERAND2 HUNDREDS ;447: F8 12 	
		PLO	RD                       ;449: AD 	
		SEX	RE	;X=E               ;44A: EE 	
		LDN	RD                       ;44B:  D 	
		STXD                               ;44C: 73 	
		LDI	100     ;MULTIPLIER        ;44D: F8 64 	
		STXD                               ;44F: 73 	
		LDI	 0      ;ZERO PRODUCT      ;450: F8  0 	
		STR	RE                       ;452: 5E 	
		PLO	RF	;RF.0=0            ;453: AF 	
		SHR		;DF=0              ;454: F6 	
		MULTIPY                           ;455: D8 	
		LDI	14h     ;RD=1014 (high product)  ;456: F8 14 	
		PLO	RD                       ;458: AD 	
		LDI	17h     ;RE=1017 (Temp sum) ;459: F8 17 	
		PLO	RE                       ;45B: AE 	
		LDA	RD                       ;45C: 4D 	M(1017):M(1018) = M(1014):M(1015)
		STR	RE                       ;45D: 5E 	
		INC	RE                       ;45E: 1E 	
		LDN	RD                       ;45F:  D 	
		STR	RE                       ;460: 5E 	
		
		LDI	16h     ;RE=1016 MULTIPLICAND  ;461: F8 16 	
		PLO	RE                       ;463: AE 	
		LDI	11h     ;RD=1012 OPERAND2 TENS  ;464: F8 11 	
		PLO	RD                       ;466: AD 	
		LDN	RD                       ;467:  D 	
		STXD                               ;468: 73 	
		LDI	10	;Multiplier        ;469: F8  A 	
		STXD                               ;46B: 73 	
		LDI	 0      ;Zero Product      ;46C: F8  0 	
		STR	RE                       ;46E: 5E 	
		PLO	RF	;RF.0=0            ;46F: AF 	
		SHR		;DF=0              ;470: F6 	
		MULTIPY                        ;471: D8 	
		LDI	15h     ;RD=1015 (low product)     ;472: F8 15 	
		PLO	RD                       ;474: AD 	
		LDI	18h     ;RE=1018 (Temp sum low)  ;475: F8 18 	
		PLO	RE                       ;477: AE 	
		LDN	RD                       ;478:  D 	
		ADD                                ;479: F4 	
		STXD                               ;47A: 73 	
		DEC	RD                       ;47B: 2D 	
		LDN	RD                       ;47C:  D 	
		ADC                                ;47D: 74 	
		STR	RE                       ;47E: 5E 	

		LDI	10h    ;RD=1010 OPERAND2 UNITS , Add into Temp Sum
		PLO	RD                       ;481: AD 	
		LDI	18h                         ;482: F8 18 	
		PLO	RE                       ;484: AE 	
		LDN	RD                       ;485:  D 	
		ADC                                ;486: 74 	
		STXD                               ;487: 73 	
		LDI	 0                         ;488: F8  0 	
		ADC                                ;48A: 74 	
		STR	RE                       ;48B: 5E 	*** Temp Sum = 16 bit OPERAND2 FEET
		
		LDI	16h	;RE=1016           ;48C: F8 16 	
		PLO	RE                       ;48E: AE 	
		LDI	 3      ;RD=1003 ACC FEET TENS  (WHOLE NUMBER?) ;48F: F8  3 	
		PLO	RD                       ;491: AD 	
		LDN	RD                       ;492:  D 	
		ANI	0Fh                         ;493: FA  F 	
		STXD                               ;495: 73 	
		LDI	10	;MULTIPLIER             ;496: F8  A 	
		STXD                               ;498: 73 	
		LDI	 0                         ;499: F8  0 	
		STR	RE    ;Clear product           ;49B: 5E 	
		PLO	RF	;RF.0=0            ;49C: AF 	
		SHR		;DF=0              ;49D: F6 	
		MULTIPY                        ;49E: D8 	
		LDI	 2      ;RD=1002 ACC FEET UNITS  (WHOLE NUMBER?)   ;49F: F8  2 	
		PLO	RD                       ;4A1: AD 	
		LDI	15h     ;RE=1015 product low                    ;4A2: F8 15 	
		PLO	RE                       ;4A4: AE 	
		LDN	RD    ;Fetch Units       ;4A5:  D 	
		ANI	0Fh                        ;4A6: FA  F 	
		ADC             ;Add with Tens     ;4A8: 74 	
		INC	RE                       ;4A9: 1E 	
		STR	RE    ;M(1016)=Count     ;4AA: 5E 	

		PLO	RA	;Set RA.0 Count of Whole numbers to add/subtract?

		LDI	14h	;RD=1014           ;4AC: F8 14 	M(1014):M(1015) = M(1017):M(1018) (FEET in binary)
		PLO	RD                       ;4AE: AD 	
		INC	RE	;RE=1017           ;4AF: 1E 	
		LDA	RE                       ;4B0: 4E 	
		STR	RD                       ;4B1: 5D 	
		INC	RD                       ;4B2: 1D 	
		LDN	RE                       ;4B3:  E 	
		STR	RD                       ;4B4: 5D 	
		
		LDI	16h	;RD=1016           ;4B5: F8 16 	
		PLO	RD                       ;4B7: AD 	
		LDI	19h     ;RE=1019           ;4B8: F8 19 	
		PLO	RE                       ;4BA: AE 	
		LDN	RD                       ;4BB:  D 	M(1019) = M(1016) ;=Whole number
		STXD            ;RE=1018           ;4BC: 73 	
		
		DEC	RE    ;RE=1017           ;4BD: 2E 	M(1016) = M(1017)
		LDA	RE    ;RE=1018           ;4BE: 4E 	
		STR	RD                       ;4BF: 5D 	
		
		INC	RD    ;RD=1017           ;4C0: 1D 	M(1017) = M(1018)
		LDN	RE                       ;4C1:  E 	
		STR	RD                       ;4C2: 5D 	
		
		LDI	 0                         ;4C3: F8  0 	
		STR	RE                       ;4C5: 5E 	M(1018) = 0	
		DEC	RD    ;RD=1016           ;4C6: 2D 	
		DEC	RD    ;RD=1015           ;4C7: 2D 	M(1014):M(1015) = 0000
		STR	RD                       ;4C8: 5D 	
		DEC	RD    ;RD=1014           ;4C9: 2D 	
		STR	RD                       ;4CA: 5D 	
		PLO	RF    ;RF.0=0            ;4CB: AF 	
		DIVIDE                             ;4CC: D9 	;DIVIDE FEET BY WHOLE NUMBER
		
								;Convert Binary back to FIS
		LDI	15h                        ;4CD: F8 15 	
		PLO	RE                       ;4CF: AE 	
		LDI	1Ch                        ;4D0: F8 1C 	
		PLO	RD                       ;4D2: AD 	
		LDN	RE                       ;4D3:  E 	
		STR	RD                       ;4D4: 5D 	
		LDI	19h                        ;4D5: F8 19 	
		PLO	RE                       ;4D7: AE 	
		LDI	100                        ;4D8: F8 64 	DIVIDE BY HUNDRED
		STXD                               ;4DA: 73 	
		LDI	 0                         ;4DB: F8  0 	
		STXD                               ;4DD: 73 	
		DEC	RE                       ;4DE: 2E 	
		DEC	RE                       ;4DF: 2E 	
		STXD                               ;4E0: 73 	
		STR	RE                       ;4E1: 5E 	
		PLO	RF                       ;4E2: AF 	
		SHR                                ;4E3: F6 	
		DIVIDE                             ;4E4: D9 	
		LDI	17h	;RE=1017           ;4E5: F8 17 	
		PLO	RE                       ;4E7: AE 	
		LDI	1Fh     ;RD=101F TEMP      ;4E8: F8 1F 	
		PLO	RD                       ;4EA: AD 	
		LDN	RE                       ;4EB:  E 	
		STR	RD                       ;4EC: 5D 	
		LDI	19h                         ;4ED: F8 19 	
		PLO	RE                       ;4EF: AE 	
		LDI	10                         ;4F0: F8  A 	DIVIDE BY 10
		STXD                               ;4F2: 73 	
		LDI	 0                         ;4F3: F8  0 	
		STXD                               ;4F5: 73 	
		LDI	15h                         ;4F6: F8 15 	
		PLO	RD                       ;4F8: AD 	
		LDN	RD                       ;4F9:  D 	
		STXD                               ;4FA: 73 	
		DEC	RD                       ;4FB: 2D 	
		LDN	RD                       ;4FC:  D 	
		STXD                               ;4FD: 73 	
		LDI	 0                         ;4FE: F8  0 	
		STXD                               ;500: 73 	
		STR	RE                       ;501: 5E 	
		PLO	RF                       ;502: AF 	
		SHR                                ;503: F6 	
		DIVIDE                             ;504: D9 
		
		LDI	17h	;RE=1017           ;505: F8 17 	RUNNING SUM, 24 BIT?
		PLO	RE                       ;507: AE 	
		LDI	1Eh	;RD=101E (TEMP)    ;508: F8 1E 	
		PLO	RD                       ;50A: AD 	
		LDN	RE                       ;50B:  E 	
		STR	RD                       ;50C: 5D 	
		DEC	RD                       ;50D: 2D 	
		DEC	RE                       ;50E: 2E 	
		DEC	RE                       ;50F: 2E 	
		LDN	RE                       ;510:  E 	
		STR	RD                       ;511: 5D 	
		LDI	1Ch                        ;512: F8 1C 	
		PLO	RE                       ;514: AE 	
		LDI	15h                        ;515: F8 15 	
		PLO	RD                       ;517: AD 	
		LDN	RE                       ;518:  E 	
		STR	RD                       ;519: 5D 	
		LDI	15h                        ;51A: F8 15 	
		PLO	RE                       ;51C: AE 	
		LDA	RE                       ;51D: 4E 	
		STXD                               ;51E: 73 	
		LDI	12                         ;51F: F8  C 	MULTIPLY BY 12 (INCHES)
		STXD                               ;521: 73 	
		LDI	 0                         ;522: F8  0 	
		STR	RE                       ;524: 5E 	
		PLO	RF	;RF.0=0            ;525: AF 	
		SHR		;DF=0              ;526: F6 	
		MULTIPY                        ;527: D8 	
		
		LDI	15h                         ;528: F8 15 	
		PLO	RE                       ;52A: AE 	
		LDI	0Fh                         ;52B: F8  F 	
		PLO	RD                       ;52D: AD 	
		LDN	RD                       ;52E:  D 	
		ADD                                ;52F: F4 	
		STXD                               ;530: 73 	
		LDI	 0                         ;531: F8  0 	
		ADC                                ;533: 74 	
		STR	RE                       ;534: 5E 	
		LDI	15h                         ;535: F8 15 	
		PLO	RE                       ;537: AE 	
		LDA	RE                       ;538: 4E 	
		STXD                               ;539: 73 	
		DEC	RE                       ;53A: 2E 	
		LDA	RE                       ;53B: 4E 	
		STR	RE                       ;53C: 5E 	
		LDI	16h                         ;53D: F8 16 	
		PLO	RE                       ;53F: AE 	
		LDA	RE                       ;540: 4E 	
		STXD                               ;541: 73 	
		DEC	RE                       ;542: 2E 	
		LDA	RE                       ;543: 4E 	
		STR	RE                       ;544: 5E 	
		LDI	19h                         ;545: F8 19 	
		PLO	RE                       ;547: AE 	
		GLO	RA	;Get RA.0 Whole Number
		STXD                               ;549: 73 	
		LDI	 0                         ;54A: F8  0 	
		STR	RE                       ;54C: 5E 	
		LDI	15h                         ;54D: F8 15 	
		PLO	RE                       ;54F: AE 	
		LDI	 0                         ;550: F8  0 	
		STXD                               ;552: 73 	
		STR	RE                       ;553: 5E 	
		LDI	 0                         ;554: F8  0 	
		PLO	RF                       ;556: AF 	
		DIVIDE                       ;557: D9 	
		LDI	17h                         ;558: F8 17 	
		PLO	RE                       ;55A: AE 	
		LDI	1Ch                         ;55B: F8 1C 	
		PLO	RD                       ;55D: AD 	
		LDN	RE                       ;55E:  E 	
		STR	RD                       ;55F: 5D 	
		LDI	15h                         ;560: F8 15 	
		PLO	RE                       ;562: AE 	
		LDA	RE                       ;563: 4E 	
		STXD                               ;564: 73 	
		LDI	10h                         ;565: F8 10 	
		STXD                               ;567: 73 	
		LDI	 0                         ;568: F8  0 	
		STR	RE                       ;56A: 5E 	
		PLO	RF	;RF.0=0            ;56B: AF 	
		SHR		;DF=0              ;56C: F6 	
		MULTIPY                        ;56D: D8 	
		LDI	15h                         ;56E: F8 15 	
		PLO	RE                       ;570: AE 	
		LDI	0Eh                         ;571: F8  E 	
		PLO	RD                       ;573: AD 	
		LDN	RD                       ;574:  D 	
		ANI	0Fh                         ;575: FA  F 	
		ADD                                ;577: F4 	
		STXD                               ;578: 73 	
		LDI	 0                         ;579: F8  0 	
		ADC                                ;57B: 74 	
		STR	RE                       ;57C: 5E 	
		LDI	15h                         ;57D: F8 15 	
		PLO	RE                       ;57F: AE 	
		LDA	RE                       ;580: 4E 	
		STXD                               ;581: 73 	
		DEC	RE                       ;582: 2E 	
		LDA	RE                       ;583: 4E 	
		STR	RE                       ;584: 5E 	
		LDI	16h                         ;585: F8 16 	
		PLO	RE                       ;587: AE 	
		LDA	RE                       ;588: 4E 	
		STXD                               ;589: 73 	
		DEC	RE                       ;58A: 2E 	
		LDA	RE                       ;58B: 4E 	
		STR	RE                       ;58C: 5E 	
		LDI	19h                         ;58D: F8 19 	
		PLO	RE                       ;58F: AE 	
		GLO	RA	;Get RA.0 Whole Number
		STXD                               ;591: 73 	
		LDI	 0                         ;592: F8  0 	
		STR	RE                       ;594: 5E 	
		LDI	15h                         ;595: F8 15 	
		PLO	RE                       ;597: AE 	
		LDI	 0                         ;598: F8  0 	
		STXD                               ;59A: 73 	
		STR	RE                       ;59B: 5E 	
		PLO	RF                       ;59C: AF 	
		DIVIDE                             ;59D: D9 	
		LDI	17h                        ;59E: F8 17 	
		PLO	RE                       ;5A0: AE 	
		LDI	1Bh                        ;5A1: F8 1B 	
		PLO	RD                       ;5A3: AD 	
		LDN	RE                       ;5A4:  E 	
		STR	RD                       ;5A5: 5D 	
		LDI	15h                        ;5A6: F8 15 	
		PLO	RE                       ;5A8: AE 	
		LDA	RE                       ;5A9: 4E 	
		STXD                               ;5AA: 73 	
		DEC	RE                       ;5AB: 2E 	
		LDA	RE                       ;5AC: 4E 	
		STR	RE                       ;5AD: 5E 	
		LDI	16h                        ;5AE: F8 16 	
		PLO	RE                       ;5B0: AE 	
		LDA	RE                       ;5B1: 4E 	
		STXD                               ;5B2: 73 	
		DEC	RE                       ;5B3: 2E 	
		LDA	RE                       ;5B4: 4E 	
		STR	RE                       ;5B5: 5E 	
		LDI	19h                         ;5B6: F8 19 	
		PLO	RE                       ;5B8: AE 	
		LDI	10h                         ;5B9: F8 10 	
		STXD                               ;5BB: 73 	
		LDI	 0                         ;5BC: F8  0 	
		STR	RE                       ;5BE: 5E 	
		LDI	15h                         ;5BF: F8 15 	
		PLO	RE                       ;5C1: AE 	
		LDI	 0                         ;5C2: F8  0 	
		STXD                               ;5C4: 73 	
		STR	RE                       ;5C5: 5E 	
		PLO	RF                       ;5C6: AF 	
		DIVIDE                             ;5C7: D9 	

		LDI	12h	;RE=1012 OPERAND2 HUNDREDS  ;5C8: F8 12 	
		PLO	RE                       ;5CA: AE 	
		LDI	0E0h                         ;5CB: F8 E0 	
		STXD                               ;5CD: 73 	
		STXD                               ;5CE: 73 	
		STXD                               ;5CF: 73 	
		LDI	16h	;RD=1016           ;5D0: F8 16 	
		PLO	RD                       ;5D2: AD 	
		LDN	RD                       ;5D3:  D 	
		BZ	L5DA                       ;5D4: 32 DA 	
		ORI	0F0h                       ;5D6: F9 F0 	ACC CODING FOR BLANK/NON BLANK
		BR	L5DC                       ;5D8: 30 DC 	
L5DA		LDI	0E0h                       ;5DA: F8 E0 	E0=BLANK
L5DC		STXD                               ;5DC: 73 	
		DEC	RD                       ;5DD: 2D 	
		DEC	RD                       ;5DE: 2D 	
		LDN	RD                       ;5DF:  D 	
		ORI	0F0h                         ;5E0: F9 F0 	
		STR	RE                       ;5E2: 5E 	
		LDI	20h                         ;5E3: F8 20 	
		PLO	RD                       ;5E5: AD 	
		LDI	 5                         ;5E6: F8  5 	
		PLO	RE                       ;5E8: AE 	
L5E9_LOOP	DEC	RE                       ;5E9: 2E 	COPY OPERAND2 TO ACC
		DEC	RD                       ;5EA: 2D 	
		LDN	RD                       ;5EB:  D 	
		STR	RE                       ;5EC: 5E 	
		GLO	RE                       ;5ED: 8E 	
		BZ	L5F2                       ;5EE: 32 F2 	
		BR	L5E9_LOOP                  ;5F0: 30 E9 	
L5F2		GHI	RA                       ;5F2: 9A 	
		ANI	0EFh                       ;5F3: FA EF 	
		PHI	RA	;Clear bit RA.1.4
		
		LDI	13h	;RD=1013 OPERAND2 SIGN  ;5F6: F8 13 COPY OPERAND2 SIGN TO ACC SIGN
		PLO	RD                       ;5F8: AD 	
		LDI	 6      ;RE=1006 ACC SIGN  ;5F9: F8  6 	
		PLO	RE                       ;5FB: AE 	
		LDN	RD                       ;5FC:  D 	
		STR	RE                       ;5FD: 5E 	
		
L5FE		GHI	R4                       ;5FE: 94 	
		ANI	10h	;Test bit R4.1.4
		LBZ	L29B                       ;601: C2  2 9B	
		GHI	R4                       ;604: 94 	
		ANI	0EFh                         ;605: FA EF 	
		PHI	R4	;Clear bit R4.1.4
		LBR	L276                       ;608: C0  2 76	


;DIVIDE				;X=E
SEP_R9_RET	SEP	R0                       ;60B: D0 	
SEP_R9		LDI	15h	;RD=1015           ;60C: F8 15 	
		PLO	RD                       ;60E: AD 	
		LDI	19h	;RE=1019           ;60F: F8 19 	
		PLO	RE                       ;611: AE 	
		LDN	RD                       ;612:  D 	
		SM                                 ;613: F7 	
		INC	RE                       ;614: 1E 	
		STXD                               ;615: 73 	
		DEC	RE                       ;616: 2E 	
		DEC	RD                       ;617: 2D 	
		LDN	RD                       ;618:  D 	
		SMB                                ;619: 77 	
		LBNF	SEP_R9_0                   ;61A: CB  6 24	
		STR	RD                       ;61D: 5D 	
		INC	RD                       ;61E: 1D 	
		LDI	1Ah	;RE=101A           ;61F: F8 1A 	
		PLO	RE                       ;621: AE 	
		LDN	RE	;M(101A)=M(1019)   ;622:  E 	
		STR	RD                       ;623: 5D 	
		
SEP_R9_0	LDI	17h	;RE=1017           ;624: F8 17 	
		PLO	RE                       ;626: AE 	
		LDN	RE                       ;627:  E 	SHLC M(1014):M(1015):M(1016):M(1017)
		SHLC                               ;628: 7E 	
		STXD                               ;629: 73 	
		LDN	RE                       ;62A:  E 	
		SHLC                               ;62B: 7E 	
		STXD                               ;62C: 73 	
		LDN	RE                       ;62D:  E 	
		SHLC                               ;62E: 7E 	
		STXD                               ;62F: 73 	
		LDN	RE                       ;630:  E 	
		SHLC                               ;631: 7E 	
		STR	RE                       ;632: 5E 	
		INC	RF                       ;633: 1F 	
		GLO	RF                       ;634: 8F 	
		XRI	17                         ;635: FB 11 	
		BZ	SEP_R9_1                   ;637: 32 3C 	
		LBR	SEP_R9                     ;639: C0  6  C
		
SEP_R9_1	LDN	RE                       ;63C:  E 	SHR M(1014):M(1015)
		SHR                                ;63D: F6 	
		STR	RE                       ;63E: 5E 	
		INC	RE                       ;63F: 1E 	
		LDN	RE                       ;640:  E 	
		SHR                                ;641: F6 	
		STR	RE                       ;642: 5E 	
		LBR	SEP_R9_RET                 ;643: C0  6  B	



;Multiply Routine
;Input:	X=E
;	DF = 0 for add in carry
;	RF.0=0 to do 8 bits
;	M1014=0
;	M1015=Multiplier
;	M1016=Multiplicand
;Output: M1014:M1015 = 16 bit Product
;
;MULTIPY
SEP_R8_RET	SEP	R0                       ;646: D0 	
SEP_R8		LDI	14h                        ;647: F8 14 	
		PLO	RE	;RE=1014           ;649: AE 	
		LDN	RE                       ;64A:  E 	
		SHRC                               ;64B: 76	Shift D Right, LSB(D)->DF, DF->MSB(D) 	
		STR	RE                       ;64C: 5E 	
		INC	RE	;RE=1015           ;64D: 1E 	
		LDN	RE                       ;64E:  E 	
		SHRC                               ;64F: 76	Shift D Right, LSB(D)->DF, DF->MSB(D) 	
		STR	RE                       ;650: 5E 	
		BDF	SEP_R8_0                   ;651: 33 55 	
		BR	SEP_R8_1                   ;653: 30 5B 
		
SEP_R8_0	INC	RE	;RE=1016           ;655: 1E 	
		LDN	RE	;D=M(1016)         ;656:  E 	
		DEC	RE                       ;657: 2E 	
		DEC	RE	;RE=1014           ;658: 2E 	
		ADD		;D=D+M(1014)       ;659: F4 	
		STR	RE	;M(1014)=M(1014)+M(1016)  ;65A: 5E 	
		
SEP_R8_1	INC	RF	;Increment Bit Counter ;65B: 1F 	
		GLO	RF                       ;65C: 8F 	
		XRI	9                          ;65D: FB  9 	
		BZ	SEP_R8_RET                 ;65F: 32 46 	
		BR	SEP_R8                     ;661: 30 47 	
		
		
EX_KEY_F8	GHI	R4                       ;663: 94 	
		ANI	 8	;Test bit R4.1.3
		LBNZ	EX_KEY_F8_EAC                       ;666: CA  E AC	
		GHI	R4                       ;669: 94 	
		ANI	 4	;Test bit R4.1.2
		LBNZ	EX_KEY_F8_A92                       ;66C: CA  A 92	
		LBR	UPDATE_LED                       ;66F: C0  0 28	
		
		ORI	0F0h                       ;672: F9 F0 	
		STR	RE                       ;674: 5E 	
		ORI	0E0h                       ;675: F9 E0 	
		STR	RE                       ;677: 5E 	
		INC	RE                       ;678: 1E 	
		LDI	0E0h                       ;679: F8 E0 	
		STR	RE                       ;67B: 5E 	
		INC	RE                       ;67C: 1E 	
		STR	RE                       ;67D: 5E 	
		INC	RE                       ;67E: 1E 	
		STR	RE                       ;67F: 5E 	
		LDI	 0                         ;680: F8  0 	
		INC	RE                       ;682: 1E 	
		STR	RE                       ;683: 5E 	
		INC	RE                       ;684: 1E 	
		STR	RE                       ;685: 5E 	
		LBR	PRINT_RE                   ;686: C0  8 61
		
		
						;Process Whole Number???
L689		LDI	16h	;RE=1016           ;689: F8 16 	
		PLO	RE                       ;68B: AE 	
		LDI	 3	;RD=1003 ACC Feet Tens
		PLO	RD                       ;68E: AD 	
		SEX	RE	;X=E
		LDN	RD                       ;690:  D 	M(1016) = M(1003) AND 0Fh
		ANI	0Fh                        ;691: FA  F 	
		STXD                               ;693: 73 	
		LDI	10                         ;694: F8  A 	M(1015) = 0A  (10 Decimal)
		STXD                               ;696: 73 	
		LDI	 0                         ;697: F8  0 	M(1014) = 00
		STR	RE                       ;699: 5E 	
		PLO	RF	;RF.0=0            ;69A: AF 	
		SHR		;DF=0              ;69B: F6 	
		MULTIPY                        ;69C: D8 	Product in M(1015)
		LDI	 2	;RD=1002 ACC Feet Units		M(1015) = M(1015) + (M(1002) AND 0Fh)
		PLO	RD                       ;69F: AD 	
		LDI	15h	;RE=1015
		PLO	RE                       ;6A2: AE 	
		LDN	RD                       ;6A3:  D 	
		ANI	0Fh                        ;6A4: FA  F 	
		ADC                                ;6A6: 74 	
		STR	RE                       ;6A7: 5E 	
		
		PLO	RA	;Set RA.0 Counter = Whole Number.  It will be the counter of the number of times to ADD.
				;ie OPERAND = ACC X Whole_Number
		
		LDI	 0	;RD=1000           ;6A9: F8  0 	M(1000):M(1001):M(1002):M(1003):M(1004) = M(100E):M(100F):M(1010):M(1011):M(1012)
		PLO	RD                       ;6AB: AD 	
		LDI	0Eh	;RE=100E           ;6AC: F8  E 	
		PLO	RE                       ;6AE: AE 	
L6AF_LOOP	LDA	RE                       ;6AF: 4E 	
		STR	RD                       ;6B0: 5D 	
		INC	RD                       ;6B1: 1D 	
		GLO	RD                       ;6B2: 8D 	
		XRI	5                          ;6B3: FB  5 	
		BZ	L6B9_EXIT                  ;6B5: 32 B9 	
		BR	L6AF_LOOP                  ;6B7: 30 AF 		
L6B9_EXIT
		INC	RD                       ;6B9: 1D 	M(1006) = M(1013) Copy Sign
		LDN	RE                       ;6BA:  E 	
		STR	RD                       ;6BB: 5D 	
		
		LDI	0Eh	;RD=100E           ;6BC: F8  E 	M(100E):M(100F):M(1010):M(1011):M(1012) = M(1007):M(1008):M(1009):M(100A):M(100B)
		PLO	RD                       ;6BE: AD 	
		LDI	 7      ;RE=1007           ;6BF: F8  7 	
		PLO	RE                       ;6C1: AE 	
L6C2_LOOP	LDA	RE                       ;6C2: 4E 	
		STR	RD                       ;6C3: 5D 	
		INC	RD                       ;6C4: 1D 	
		GLO	RD                       ;6C5: 8D 	
		XRI	13h                         ;6C6: FB 13 	
		BZ	L6CC_EXIT                  ;6C8: 32 CC 	
		BR	L6C2_LOOP                  ;6CA: 30 C2 			
L6CC_EXIT
		INC	RE                       ;6CC: 1E 	M(1013) = M(100D) Copy Sign
		LDN	RE                       ;6CD:  E 	
		STR	RD                       ;6CE: 5D 	
		
		LDI	 7	;RD=1007           ;6CF: F8  7 	M(1007):M(1008):M(1009):M(100A):M(100B):M(100B) = 000000000000
		PLO	RD                       ;6D1: AD 	
L6D2_LOOP	LDI	 0                         ;6D2: F8  0 	
		STR	RD                       ;6D4: 5D 	
		INC	RD                       ;6D5: 1D 	
		GLO	RD                       ;6D6: 8D 	
		XRI	0Eh                          ;6D7: FB  E 	
		BNZ	L6D2_LOOP                  ;6D9: 3A D2 	
		
		GHI	R4                       ;6DB: 94 	
		ORI	80h                         ;6DC: F9 80 	
		PHI	R4	;Set bit R4.1.7 So Program will resume here after addition
				;R4.1.7 = Resume to L6E2_RESUME

				;ADD ACC to OPERAND "RA" number of times. (ie OPERAND = ACC X Whole_Number)
L6DF_LOOP	LBR	ADD_OPR_ACC_PT              ;6DF: C0  F 47 ADD ACC to OPERAND, Pre-Test for Overflow		
		
L6E2_RESUME	DEC	RA	;Decrement RA.0 Counter
		GLO	RA	;Get RA.0 Counter ?
		BNZ	L6DF_LOOP                  ;6E4: 3A DF
		
		GHI	R4                       ;6E6: 94 	
		ANI	07Fh                         ;6E7: FA 7F 	
		PHI	R4	;Clear bit R4.1.7
							;COPY OPERAND 1 TO ACC
		LDI	 0	;RD=1000           ;6EA: F8  0 	
		PLO	RD                       ;6EC: AD 	
		LDI	 7      ;RE=1007           ;6ED: F8  7 	
		PLO	RE                       ;6EF: AE 	
L6F0_LOOP	LDA	RE                       ;6F0: 4E 	
		STR	RD                       ;6F1: 5D 	
		INC	RD                       ;6F2: 1D 	
		GLO	RD                       ;6F3: 8D 	
		XRI	6                          ;6F4: FB  6 	
		BNZ	L6F0_LOOP                  ;6F6: 3A F0 	
		
							;COPY OPERAND 2 TO OPERAND 1
		LDI	7	;RD=1007           ;6F8: F8  7 	
		PLO	RD                       ;6FA: AD 	
		LDI	0Eh     ;RE=100E           ;6FB: F8  E 	
		PLO	RE                       ;6FD: AE 	
L6FE_LOOP	LDA	RE                       ;6FE: 4E 	
		STR	RD                       ;6FF: 5D 	
		INC	RD                       ;700: 1D 	
		GLO	RD                       ;701: 8D 	
		XRI	0Ch                      ;702: FB  C 	
		LBNZ	L6FE_LOOP                ;704: CA  6 FE	

		LDI	 0                       ;707: F8  0 	CLEAR OPERAND 1 OVERFLOW
		STR	RD                       ;709: 5D 	
		INC	RD                       ;70A: 1D 	
		LDN	RE                       ;70B:  E 	COPY OPERAND 2 SIGN TO OPERAND 1 SIGN
		STR	RD                       ;70C: 5D
		
		GHI	RA                       ;70D: 9A 	
		ANI	0FBh                       ;70E: FA FB 	
		PHI	RA	;Clear bits RA.1.2 (UnDirects Key input to enter Whole numbers)
		LDI	 5                         ;711: F8  5 	
		PLO	RD                       ;713: AD 	
		LDN	RD                       ;714:  D 	
		XRI	2                          ;715: FB  2 	
		LBZ	L34A                       ;717: C2  3 4A	
		LBR	L5FE                       ;71A: C0  5 FE	
				
				;MEMORY STORE GET KEY LOOP
MEM_STORE_GKEY	OUT	2                     ;71D: 62 	
		B4	MEM1_STORE  ;(KEY_1)       ;71E: 37 50 	
		OUT	3                     ;720: 63 	
		B4	MEM2_STORE  ;(KEY_2)       ;721: 37 55 	
		OUT	4                     ;723: 64 	
		B4	MEM3_STORE  ;(KEY_3)       ;724: 37 5A 	
		OUT	1                     ;726: 61 	
		B3	MEM4_STORE  ;(KEY_4)       ;727: 36 5F 	
		OUT	6                     ;729: 66 	
		NOP                                ;72A: C4 	
		NOP                                ;72B: C4 	
		BR	MEM_STORE_GKEY         ;72C: 30 1D 	
		
				;MEMORY RECALL GET KEY LOOP	
MEM_RECALL_GKEY	OUT	2                     ;72E: 62 	
		B4	MEM1_RECALL ;(KEY_1)       ;72F: 37 64 	
		OUT	3                     ;731: 63 	
		B4	MEM2_RECALL ;(KEY_2)       ;732: 37 69 	
		OUT	4                     ;734: 64 	
		B4	MEM3_RECALL ;(KEY_3)       ;735: 37 6E 	
		OUT	1                     ;737: 61 	
		B3	MEM4_RECALL ;(KEY_4)       ;738: 36 73 	
		OUT	6                     ;73A: 66 	
		NOP                                ;73B: C4 	
		NOP                                ;73C: C4 	
		BR	MEM_RECALL_GKEY            ;73D: 30 2E 	
		
WAIT_KEY_UP	OUT	2                     ;73F: 62 	
		B4	$-1	;Wait for KEY_1 released  ;740: 37 3F 	
		OUT	3                     ;742: 63 	
		B4	$-1	;Wait for KEY_2 released  ;743: 37 42 	
		OUT	4                     ;745: 64 	
		B4	$-1	;Wait for KEY_3 released  ;746: 37 45 	
		OUT	1                     ;748: 61 	
		B3	$-1	;Wait for KEY_4 released  ;749: 36 48 	
		LBR	UPDATE_LED
		
		BR	WAIT_KEY_UP                ;74E: 30 3F   ;DEAD CODE???	
		
MEM1_STORE	LDI	MEM1                       ;750: F8 26 	
		PLO	RD                       ;752: AD 	
		BR	STORE_COPY                 ;753: 30 8C 	
MEM2_STORE	LDI	MEM2                       ;755: F8 2D 	
		PLO	RD                       ;757: AD 	
		BR	STORE_COPY                 ;758: 30 8C 	
MEM3_STORE	LDI	MEM3                       ;75A: F8 34 	
		PLO	RD                       ;75C: AD 	
		BR	STORE_COPY                 ;75D: 30 8C 	
MEM4_STORE	LDI	MEM4                       ;75F: F8 3B 	
		PLO	RD                       ;761: AD 	
		BR	STORE_COPY                 ;762: 30 8C 	

MEM1_RECALL	LDI	MEM1+1                     ;764: F8 27 	
		PLO	RD                       ;766: AD 	
		BR	RECALL_COPY_F              ;767: 30 D1 	
MEM2_RECALL	LDI	MEM2+1                     ;769: F8 2E 	
		PLO	RD                       ;76B: AD 	
		BR	RECALL_COPY_F              ;76C: 30 D1 	
MEM3_RECALL	LDI	MEM3+1                     ;76E: F8 35 	
		PLO	RD                       ;770: AD 	
		BR	RECALL_COPY_F              ;771: 30 D1 	
MEM4_RECALL	LDI	MEM4+1                     ;773: F8 3C 	
		PLO	RD                       ;775: AD 	
		BR	RECALL_COPY_F              ;776: 30 D1 	

		LDI	MEM1                       ;778: F8 26 	DEAD CODE (WRONG CODE)
		PLO	RD                       ;77A: AD 	RECALL_COPY IS EXPECTING MEMX+1 ADDRESS IN RD
		BR	RECALL_COPY_F              ;77B: 30 D1 	
		LDI	MEM2                       ;77D: F8 2D 	
		PLO	RD                       ;77F: AD 	
		BR	RECALL_COPY_F              ;780: 30 D1 	
		LDI	MEM3                       ;782: F8 34 	
		PLO	RD                       ;784: AD 	
		BR	RECALL_COPY_F              ;785: 30 D1 	
		LDI	MEM4                       ;787: F8 3B 	
		PLO	RD                       ;789: AD 	
		BR	RECALL_COPY_F              ;78A: 30 D1 	

				;Copy 6 BYTES FROM RE TO RD
STORE_COPY	SEX	RD	;X=D               ;78C: ED 	
		LDI	ACC+1	                   ;78D: F8  7 	
		PLO	RE	;RE=1007           ;78F: AE 	

STORE_LOOP	DEC	RE                       ;790: 2E 	
		LDN	RE	;Fetch ACC byte    ;791:  E 	
		STXD		;Store in MEMORY   ;792: 73 	
		GLO	RE	;Repeat until 1000 ;793: 8E 	
		BNZ	STORE_LOOP                 ;794: 3A 90
		
				;Test which Memory it was saved to, for the Print Sx message
				;RD will be 1 less than the Sixteens location of that Memory
		GLO	RD                       ;796: 8D 	
		XRI	MEM1-7                     ;797: FB 1F 	
		LBZ	PRINT_S1                   ;799: C2  8 89	
		GLO	RD                       ;79C: 8D 	
		XRI	MEM2-7                     ;79D: FB 26 	
		LBZ	PRINT_S2                   ;79F: C2  8 97	
		GLO	RD                       ;7A2: 8D 	
		XRI	MEM3-7                     ;7A3: FB 2D 	
		LBZ	PRINT_S3                   ;7A5: C2  8 A5	
		GLO	RD                       ;7A8: 8D 	
		XRI	MEM4-7                     ;7A9: FB 34 	
		LBZ	PRINT_S4                   ;7AB: C2  8 B3	
		
RECALL_COPY	LDI	ACC+1                      ;7AE: F8  7 	
		PLO	RE	;RE=1007 ACC+1     ;7B0: AE 	
RECALL_LOOP	DEC	RD                       ;7B1: 2D 	
		DEC	RE                       ;7B2: 2E 	
		LDN	RD    ;Get Memory        ;7B3:  D 	
		STR	RE    ;Put ACC           ;7B4: 5E 	
		GLO	RE                       ;7B5: 8E 	
		BNZ	RECALL_LOOP                ;7B6: 3A B1 	
		NOP                                ;7B8: C4 	
		GLO	RD                       ;7B9: 8D 	
		XRI	MEM1-6                     ;7BA: FB 20 	
		LBZ	PRINT_R1                   ;7BC: C2  8 C1	
		GLO	RD                       ;7BF: 8D 	
		XRI	MEM2-6                     ;7C0: FB 27 	
		LBZ	PRINT_R2                   ;7C2: C2  8 CF	
		GLO	RD                       ;7C5: 8D 	
		XRI	MEM3-6                     ;7C6: FB 2E 	
		LBZ	PRINT_R3                   ;7C8: C2  8 DD	
		GLO	RD                       ;7CB: 8D 	
		XRI	MEM4-6                     ;7CC: FB 35 	
		LBZ	PRINT_R4                   ;7CE: C2  8 EB	
		
RECALL_COPY_F	GHI	RA                       ;7D1: 9A 	
		ORI	20h                        ;7D2: F9 20 	
		PHI	RA	;Set bit RA.1.5 (Memory Recalled???)
		BR	RECALL_COPY                ;7D5: 30 AE 	
		
							
		SEX	R0	;X=0               ;7D7: E0 	;DEAD CODE????
		STXD                               ;7D8: 73 	;DEAD CODE????
		STXD                               ;7D9: 73 	;DEAD CODE????
		STXD                               ;7DA: 73 	;DEAD CODE????
		STXD                               ;7DB: 73 	;DEAD CODE????
		LDI	0F0h                       ;7DC: F8 F0 	;DEAD CODE????
		STR	RD                       ;7DE: 5D 	;DEAD CODE????
		LDI	 0	;RE=1000           ;7DF: F8  0 	;DEAD CODE????
		PLO	RE                       ;7E1: AE 	;DEAD CODE????
		LDI	0E0h                       ;7E2: F8 E0 	;DEAD CODE????
		STR	RE                       ;7E4: 5E 	;DEAD CODE????
		GLO	RD                       ;7E5: 8D 	;DEAD CODE????
		XRI	20h                         ;7E6: FB 20 	
		LBZ	PRINT_C1                   ;7E8: C2  8 F9	
		GLO	RD                       ;7EB: 8D 	;DEAD CODE????
		XRI	27h                         ;7EC: FB 27 	
		LBZ	PRINT_C2                   ;7EE: C2  9  7	
		GLO	RD                       ;7F1: 8D 	;DEAD CODE????
		XRI	2Eh                         ;7F2: FB 2E 	
		LBZ	PRINT_C3                   ;7F4: C2  9 15	
		GLO	RD                       ;7F7: 8D 	
		XRI	35h                         ;7F8: FB 35 	
		LBZ	PRINT_C4                   ;7FA: C2  9 23	
		LBR	LFCA                       ;7FD: C0  F CA	;DEAD CODE????

	;Print Number & prefix with '+ '
PRINT_PLUS	LDI	1Eh	                   ;800: F8 1E
		PLO	RD                       ;802: AD 	
		LDI	'+'                        ;803: F8 2B 	
		STR	RD                       ;805: 5D 	
		DEC	RD                       ;806: 2D 	
		LDI	' '                        ;807: F8 20 	
		STR	RD                       ;809: 5D 	
		PRINT_ACC                          ;80A: D3 			
		LBR	EX_ADD                     ;80B: C0  2 71	
		
PRINT_MINUS	LDI	1Eh                        ;80E: F8 1E 	
		PLO	RD                       ;810: AD 	
		LDI	'-'                        ;811: F8 2D 	
		STR	RD                       ;813: 5D 	
		DEC	RD                       ;814: 2D 	
		LDI	' '                        ;815: F8 20 	
		STR	RD                       ;817: 5D 	
		PRINT_ACC                          ;818: D3 	
		LBR	EX_SUB                     ;819: C0  2 6D	
		
PRINT_TIMES	LDI	1Eh                        ;81C: F8 1E 	
		PLO	RD                       ;81E: AD 	
		LDI	'X'                        ;81F: F8 58 	
		STR	RD                       ;821: 5D 	
		DEC	RD                       ;822: 2D 	
		LDI	' '                        ;823: F8 20 	
		STR	RD                       ;825: 5D 	
		PRINT_ACC                          ;826: D3 	
		LBR	EX_MUL                     ;827: C0  4 17	
		
PRINT_DIVIDE	LDI	1Eh                        ;82A: F8 1E 	
		PLO	RD                       ;82C: AD 	
		LDI	'%'                        ;82D: F8 25 	
		STR	RD                       ;82F: 5D 	
		DEC	RD                       ;830: 2D 	
		LDI	' '                        ;831: F8 20 	
		STR	RD                       ;833: 5D 	
		PRINT_ACC                          ;834: D3 	
		LBR	EX_DIV_WHOLE               ;835: C0  4 1D	

;PRINT_=_
EX_KEY_EQU	LDI	1Eh                        ;838: F8 1E 	
		PLO	RD                       ;83A: AD 	
		LDI	'='                        ;83B: F8 3D 	
		STR	RD                       ;83D: 5D 	
		DEC	RD                       ;83E: 2D 	
		LDI	' '                        ;83F: F8 20 	
		STR	RD                       ;841: 5D 	
		PRINT_ACC                          ;842: D3 	
		
Delay		LDI	0FFh                       ;843: F8 FF 	
		PLO	RF                       ;845: AF 	
		LDI	10h                        ;846: F8 10 	
		PHI	RF                       ;848: BF 	
DELAY_LOOP	DEC	RF                       ;849: 2F 	
		GLO	RF                       ;84A: 8F 	
		BNZ	DELAY_LOOP                 ;84B: 3A 49 	
		GHI	RF                       ;84D: 9F 	
		LBZ	L2F8                       ;84E: C2  2 F8	
		BR	DELAY_LOOP                 ;851: 30 49 	
		
		
		
PRINT___	LDI	1Eh                        ;853: F8 1E 	
		PLO	RD                       ;855: AD 	
		LDI	' '                        ;856: F8 20 	
		STR	RD                       ;858: 5D 	
		DEC	RD                       ;859: 2D 	
		LDI	' '                        ;85A: F8 20 	
		STR	RD                       ;85C: 5D 	
		PRINT_ACC                          ;85D: D3 	
		LBR	UPDATE_LED
		
		
		
PRINT_RE	LDI	1Eh                        ;861: F8 1E 	
		PLO	RD                       ;863: AD 	
		LDI	'R'                        ;864: F8 52 	
		STR	RD                       ;866: 5D 	
		DEC	RD                       ;867: 2D 	
		LDI	'E'                        ;868: F8 45 	
		STR	RD                       ;86A: 5D 	
		PRINT_ACC                          ;86B: D3 	
		LBR	UPDATE_LED
		
PRINT___2	LDI	1Eh                        ;86F: F8 1E 	
		PLO	RD                       ;871: AD 	
		LDI	' '                        ;872: F8 20 	
		STR	RD                       ;874: 5D 	
		DEC	RD                       ;875: 2D 	
		STR	RD                       ;876: 5D 	
		PRINT_ACC                          ;877: D3 	
		LBR	UPDATE_LED
	
	
;------------------------------------------------	
	
PRINT_C_	LDI	1Eh                        ;87B: F8 1E 	
		PLO	RD	;RD=101E
		LDI	'C'                        ;87E: F8 43 	
		STR	RD	;M(101E)=43
		DEC	RD	;RD=101D
		LDI	' '                        ;882: F8 20 	
		STR	RD	;M(101D)=20
		PRINT_ACC	;PRINT WORKING VARIABLE
		LBR	UPDATE_LED
		
PRINT_S1	LDI	1Eh                        ;889: F8 1E 	
		PLO	RD                       ;88B: AD 	
		LDI	'S'                        ;88C: F8 53 	
		STR	RD                       ;88E: 5D 	
		DEC	RD                       ;88F: 2D 	
		LDI	'1'                        ;890: F8 31 	
		STR	RD                       ;892: 5D 	
		PRINT_ACC                          ;893: D3 	
		LBR	WAIT_KEY_UP                ;894: C0  7 3F	


PRINT_S2	LDI	1Eh                        ;897: F8 1E 	
		PLO	RD                       ;899: AD 	
		LDI	'S'                        ;89A: F8 53 	
		STR	RD                       ;89C: 5D 	
		DEC	RD                       ;89D: 2D 	
		LDI	'2'                        ;89E: F8 32 	
		STR	RD                       ;8A0: 5D 	
		PRINT_ACC                          ;8A1: D3 	
		LBR	WAIT_KEY_UP                ;8A2: C0  7 3F	
		
PRINT_S3	LDI	1Eh                        ;8A5: F8 1E 	
		PLO	RD                       ;8A7: AD 	
		LDI	'S'                        ;8A8: F8 53 	
		STR	RD                       ;8AA: 5D 	
		DEC	RD                       ;8AB: 2D 	
		LDI	'3'                        ;8AC: F8 33 	
		STR	RD                       ;8AE: 5D 	
		PRINT_ACC                          ;8AF: D3 	
		LBR	WAIT_KEY_UP                ;8B0: C0  7 3F	
		
PRINT_S4	LDI	1Eh                        ;8B3: F8 1E 	
		PLO	RD                       ;8B5: AD 	
		LDI	'S'                        ;8B6: F8 53 	
		STR	RD                       ;8B8: 5D 	
		DEC	RD                       ;8B9: 2D 	
		LDI	'4'                        ;8BA: F8 34 	
		STR	RD                       ;8BC: 5D 	
		PRINT_ACC                          ;8BD: D3 	
		LBR	WAIT_KEY_UP                ;8BE: C0  7 3F	
		
PRINT_R1	LDI	1Eh                        ;8C1: F8 1E 	
		PLO	RD                       ;8C3: AD 	
		LDI	'R'                        ;8C4: F8 52 	
		STR	RD                       ;8C6: 5D 	
		DEC	RD                       ;8C7: 2D 	
		LDI	'1'                        ;8C8: F8 31 	
		STR	RD                       ;8CA: 5D 	
		PRINT_ACC                          ;8CB: D3 	
		LBR	WAIT_KEY_UP                ;8CC: C0  7 3F	
		
PRINT_R2	LDI	1Eh                        ;8CF: F8 1E 	
		PLO	RD                       ;8D1: AD 	
		LDI	'R'                        ;8D2: F8 52 	
		STR	RD                       ;8D4: 5D 	
		DEC	RD                       ;8D5: 2D 	
		LDI	'2'                        ;8D6: F8 32 	
		STR	RD                       ;8D8: 5D 	
		PRINT_ACC                          ;8D9: D3 	
		LBR	WAIT_KEY_UP                ;8DA: C0  7 3F	
		
PRINT_R3	LDI	1Eh                        ;8DD: F8 1E 	
		PLO	RD                       ;8DF: AD 	
		LDI	'R'                        ;8E0: F8 52 	
		STR	RD                       ;8E2: 5D 	
		DEC	RD                       ;8E3: 2D 	
		LDI	'3'                        ;8E4: F8 33 	
		STR	RD                       ;8E6: 5D 	
		PRINT_ACC                          ;8E7: D3 	
		LBR	WAIT_KEY_UP                ;8E8: C0  7 3F	
		
PRINT_R4	LDI	1Eh                        ;8EB: F8 1E 	
		PLO	RD                       ;8ED: AD 	
		LDI	'R'                        ;8EE: F8 52 	
		STR	RD                       ;8F0: 5D 	
		DEC	RD                       ;8F1: 2D 	
		LDI	'4'                        ;8F2: F8 34 	
		STR	RD                       ;8F4: 5D 	
		PRINT_ACC                          ;8F5: D3 	
		LBR	WAIT_KEY_UP                ;8F6: C0  7 3F	
		
PRINT_C1	LDI	1Eh                        ;8F9: F8 1E  ;DEAD CODE????
		PLO	RD                       ;8FB: AD 	;DEAD CODE????
		LDI	'C'                        ;8FC: F8 43 	;DEAD CODE????
		STR	RD                       ;8FE: 5D 	;DEAD CODE????
		DEC	RD                       ;8FF: 2D 	;DEAD CODE????
		LDI	'1'                        ;900: F8 31 	;DEAD CODE????	
		STR	RD                       ;902: 5D 	;DEAD CODE????
		SEP	R1	;CALL ISR_SUB
		PRINT_ACC                          ;904: D3 	;DEAD CODE????
		BR	PCX_RET                    ;905: 30 2F 	
		
PRINT_C2	LDI	1Eh                        ;907: F8 1E 	;DEAD CODE????
		PLO	RD                       ;909: AD 	
		LDI	'C'                        ;90A: F8 43 	
		STR	RD                       ;90C: 5D 	
		DEC	RD                       ;90D: 2D 	
		LDI	'2'                        ;90E: F8 32 	
		STR	RD                       ;910: 5D 	
		SEP	R1	;CALL ISR_SUB
		PRINT_ACC                          ;912: D3 	
		BR	PCX_RET                    ;913: 30 2F 	
		
PRINT_C3	LDI	1Eh                        ;915: F8 1E 	;DEAD CODE????
		PLO	RD                       ;917: AD 	
		LDI	'C'                        ;918: F8 43 	
		STR	RD                       ;91A: 5D 	
		DEC	RD                       ;91B: 2D 	
		LDI	'3'                        ;91C: F8 33 	
		STR	RD                       ;91E: 5D 	
		SEP	R1	;CALL ISR_SUB
		PRINT_ACC                          ;920: D3 	
		BR	PCX_RET                    ;921: 30 2F 	
		
PRINT_C4	LDI	1Eh                        ;923: F8 1E 	;DEAD CODE????
		PLO	RD                       ;925: AD 	
		LDI	'C'                        ;926: F8 43 	
		STR	RD                       ;928: 5D 	
		DEC	RD                       ;929: 2D 	
		LDI	'4'                        ;92A: F8 34 	
		STR	RD                       ;92C: 5D 	
		SEP	R1	;CALL ISR_SUB
		PRINT_ACC                          ;92E: D3 	
PCX_RET		LBR	UPDATE_LED


;----------------------------------------
;PRINT_ACC	Outputs working variable at M(1006) to M(1000)
;
;		sddd hh hh CC
;
;		s=' ', '1' or '-' depending on M(1006) & M(1005)
;		d=' ' or '0' to '9' for bytes M(1004) to M(1002)
;		hh='  ' or ' 0' to '15' for bytes M(1001) and M(1000)
;		C=ASCII char stored in M(101E) & M(101D)
;
;		IF FLAG=1 THEN RETURN  (No Print flag, paper out error?) 
;		Q=1
;		RF=101F 'Temp for OUT command
;		IF M(1006)<>0 THEN
;		   OUT1 '-' 
;		ELSEIF M(1005)<>0 THEN
;		   OUT1 '1'
;		ELSE
;		   OUT1 ' '
;		ENDIF
;		IF M(1004)<>E0 THEN
;		   OUT1 HEX2ASC(M(1004))   ' AND 0xF OR 0x30 = ASCII DIGIT
;		ELSE
;		   OUT1 20  ' '
;		ENDIF
;		IF M(1003)<>E0 THEN
;		   OUT1 HEX2ASC(M(1003))
;		ELSE
;		   OUT1 20  ' '
;		ENDIF
;		IF M(1002)<>E0 THEN
;		   OUT1 HEX2ASC(M(1002))
;		ELSE
;		   OUT1 20  ' '
;		ENDIF
;		OUT1 20 ' '
;		IF M(1001)<>E0 THEN   
;		   IF M(1001)<0A THEN     'OUTPUT ' 0' to '15'
;  		      OUT1 20 ' '
;		      OUT1 HEX2ASC(M(1001))
;		   ELSE
;  		      OUT1 20 '1'
;		      OUT1 HEX2ASC(M(1001) - 10)
;		   ENDIF
;		ELSE
;		   OUT1 20 '  '
;		ENDIF
;		OUT1 20 ' '
;		IF M(1000)<>E0 THEN   
;		   IF M(1000)<0A THEN     'OUTPUT ' 0' to '15'
;  		      OUT1 20 ' '
;		      OUT1 HEX2ASC(M(1000))
;		   ELSE
;  		      OUT1 20 '1'
;		      OUT1 HEX2ASC(M(1000) - 10)
;		   ENDIF
;		ELSE
;		   OUT1 20 '  '
;		ENDIF
;		OUT1 20 ' '
;		OUT1 M(101E)    'Character
;		OUT1 M(101D)    'Character
;		Q=0

SEP_R3_RET	SEP	R0                       ;932: D0 	
;PRINT_ACC:
SEP_R3		GLO	R4    ;TEST FOR PRINTER ONLINE
		ANI	40h	;Test bit R4.0.6
		BNZ	SEP_R3_RET	;EXIT IF PRINTER ERROR
		NOP			;NOP PLACE HOLDER FOR LONG BRANCH?

		SEX	RF	;X=RF
		SEQ		;Q=1
		LDI	10h                         ;93B: F8 10 	
		PHI	RF                        ;93D: BF 	
		LDI	1Fh                         ;93E: F8 1F 	
		PLO	RF	;RF=101F Temp for OUTPUT
		
		LDI	 6                         ;941: F8  6 	
		PLO	RD                       ;943: AD 	
		LDN	RD	;D=M(1006)  (INIT=00)
		BZ	SUB_R3_0                   ;945: 32 50 	
		
				;		IF M(1006)<>0 THEN
		LDI	'-'                        ;947: F8 2D 	
		STR	RF                       ;949: 5F 	
		OUT	1	;OUT1 = '-'
		DEC	RF	;UNDO THE RF+1 DONE BY THE OUT
		DEC	RD	;RD=1005
		BR	SUB_R3_2                   ;94D: 30 60 	
		NOP                                ;94F: C4 	
				;		ELSEIF M(1005)<>0 THEN		
SUB_R3_0	DEC	RD	;RD=1005
		LDN	RD	;D=M(1005)  (INIT:00)
		BZ	SUB_R3_1
		LDI	31h                         ;954: F8 31 	
		STR	RF                       ;956: 5F 	
		OUT	1                     ;957: 61 	
		DEC	RF
		BR	SUB_R3_2                   ;959: 30 60 	
				;		ELSE
SUB_R3_1	LDI	20h                         ;95B: F8 20 	
		STR	RF                       ;95D: 5F 	
		OUT	1	;OUT1 = 20
		DEC	RF
				;		ENDIF
SUB_R3_2	NOP                                ;960: C4 	
		DEC	RD	;RD=1004                       ;961: 2D 	
		LDN	RD	;D=M(1004)  (INIT:E0)
		XRI	0E0h
		BZ	SUB_R3_3
				;		IF M(1004)<>E0 THEN
		LDN	RD                       ;967:  D 	
		ANI	0Fh                         ;968: FA  F 	
		ORI	30h                         ;96A: F9 30 	
		STR	RF                       ;96C: 5F 	
		OUT	1                     ;96D: 61 	
		DEC	RF
		BR	SUB_R3_4                   ;96F: 30 76 	
				;		ELSE
SUB_R3_3	LDI	20h                         ;971: F8 20 	
		STR	RF                       ;973: 5F 	
		OUT	1                     ;974: 61 	
		DEC	RF
				;		ENDIF
SUB_R3_4	NOP                                ;976: C4 	
		DEC	RD	;RD=1003
		LDN	RD	;D=M(1003)  (INIT:E0)
		XRI	0E0h                         ;979: FB E0 	
		BZ	SUB_R3_5                   ;97B: 32 87 	
				;		IF M(1003)<>E0 THEN
		LDN	RD                       ;97D:  D 	
		ANI	0Fh                         ;97E: FA  F 	
		ORI	30h                         ;980: F9 30 	
		STR	RF                       ;982: 5F 	
		OUT	1                     ;983: 61 	
		DEC	RF
		BR	SUB_R3_6                   ;985: 30 8C 	
				;		ELSE
SUB_R3_5	LDI	20h                         ;987: F8 20 	
		STR	RF                       ;989: 5F 	
		OUT	1                     ;98A: 61 	
		DEC	RF
				;		ENDIF
SUB_R3_6	NOP                                ;98C: C4 	
		DEC	RD	;RD=1002
		LDN	RD	;D=M(1002)  (INIT:E0)
		XRI	0E0h                         ;98F: FB E0 	
		BZ	SUB_R3_7                   ;991: 32 9D 	
				;		IF M(1002)<>E0 THEN
		LDN	RD                       ;993:  D 	
		ANI	0Fh                         ;994: FA  F 	
		ORI	30h                         ;996: F9 30 	
		STR	RF                       ;998: 5F 	
		OUT	1                     ;999: 61 	
		DEC	RF
		BR	SUB_R3_8                   ;99B: 30 A2 	
				;		ELSE
SUB_R3_7	LDI	20h                         ;99D: F8 20 	
		STR	RF                       ;99F: 5F 	
		OUT	1                     ;9A0: 61 	
		DEC	RF
				;		ENDIF
SUB_R3_8	NOP                                ;9A2: C4 	
		LDI	20h                         ;9A3: F8 20 	
		STR	RF                       ;9A5: 5F 	
		OUT	1                     ;9A6: 61 	
		DEC	RF                       ;9A7: 2F 			
		
		NOP                                ;9A8: C4 	
		DEC	RD	;RD=1001
		LDN	RD	;D=M(1001)  (INIT:E0)
		XRI	0E0h                         ;9AB: FB E0 	
		BZ	SUB_R3_10                  ;9AD: 32 DA 	
				;		IF M(1001)<>E0 THEN		
		LDN	RD                       ;9AF:  D 	
		ANI	0Fh                         ;9B0: FA  F 	
		SMI	10                          ;9B2: FF  A 	
		BDF	SUB_R3_9	;Jump if D>=A (DF=1 if no Borrow)
				;		   IF M(1001)<0A THEN
		LDI	20h                         ;9B6: F8 20 	
		STR	RF                       ;9B8: 5F 	
		OUT	1                     ;9B9: 61 	
		DEC	RF                       ;9BA: 2F 	
		NOP                                ;9BB: C4 	
		LDN	RD                       ;9BC:  D 	
		ANI	0Fh                         ;9BD: FA  F 	
		ORI	30h                         ;9BF: F9 30 	
		STR	RF                       ;9C1: 5F 	
		OUT	1                     ;9C2: 61 	
		DEC	RF                       ;9C3: 2F 	
		NOP                                ;9C4: C4 	
		BR	SUB_R3_11                   ;9C5: 30 E6 	
				;		   ELSE
SUB_R3_9	LDI	31h                         ;9C7: F8 31 	
		STR	RF                       ;9C9: 5F 	
		OUT	1                     ;9CA: 61 	
		DEC	RF                       ;9CB: 2F 	
		NOP                                ;9CC: C4 	
		LDN	RD                       ;9CD:  D 	
		ANI	0Fh                         ;9CE: FA  F 	
		SMI	10                          ;9D0: FF  A 	
		ORI	30h                         ;9D2: F9 30 	
		STR	RF                       ;9D4: 5F 	
		OUT	1                     ;9D5: 61 	
		DEC	RF                       ;9D6: 2F 	
		NOP                                ;9D7: C4 	
		BR	SUB_R3_11                   ;9D8: 30 E6
				;		   ENDIF
				;		ELSE
SUB_R3_10	LDI	20h                         ;9DA: F8 20 	
		STR	RF                       ;9DC: 5F 	
		OUT	1                     ;9DD: 61 	
		DEC	RF                       ;9DE: 2F 	
		NOP                                ;9DF: C4 	
		LDI	20h                         ;9E0: F8 20 	
		STR	RF                       ;9E2: 5F 	
		OUT	1                     ;9E3: 61 	
		DEC	RF                       ;9E4: 2F 	
		NOP                                ;9E5: C4 	
				;		ENDIF
SUB_R3_11	LDI	20h                         ;9E6: F8 20 	
		STR	RF                       ;9E8: 5F 	
		OUT	1                     ;9E9: 61 	
		DEC	RF                       ;9EA: 2F 	
		NOP                                ;9EB: C4 	
		DEC	RD	;RD=1000
		LDN	RD	;D=M(1000)  (INIT:F0)
		XRI	0E0h                         ;9EE: FB E0 	
		LBZ	SUB_R3_13                  ;9F0: C2  A 1F
				;		IF M(1000)<>E0 THEN   
		LDN	RD                       ;9F3:  D 	
		ANI	0Fh                         ;9F4: FA  F 	
		SMI	10                          ;9F6: FF  A 	
		LBDF	SUB_R3_12                  ;9F8: C3  A  C
				;		   IF M(1000)<0A THEN     'OUTPUT ' 0' to '15'
		LDI	20h                         ;9FB: F8 20 	
		STR	RF                       ;9FD: 5F 	
		OUT	1                     ;9FE: 61 	
		DEC	RF                       ;9FF: 2F 	
		NOP                                ;A00: C4 	
		LDN	RD                       ;A01:  D 	
		ANI	0Fh                         ;A02: FA  F 	
		ORI	30h                         ;A04: F9 30 	
		STR	RF                       ;A06: 5F 	
		OUT	1                     ;A07: 61 	
		DEC	RF                       ;A08: 2F 	
		NOP                                ;A09: C4 	
		BR	SUB_R3_14                  ;A0A: 30 2B 	
				;		   ELSE
SUB_R3_12	LDI	31h                         ;A0C: F8 31 	
		STR	RF                       ;A0E: 5F 	
		OUT	1                     ;A0F: 61 	
		DEC	RF                       ;A10: 2F 	
		NOP                                ;A11: C4 	
		LDN	RD                       ;A12:  D 	
		ANI	0Fh                         ;A13: FA  F 	
		SMI	10                          ;A15: FF  A 	
		ORI	30h                         ;A17: F9 30 	
		STR	RF                       ;A19: 5F 	
		OUT	1                     ;A1A: 61 	
		DEC	RF                       ;A1B: 2F 	
		NOP                                ;A1C: C4 	
		BR	SUB_R3_14                  ;A1D: 30 2B 	
				;		   ENDIF
				;		ELSE
SUB_R3_13	LDI	20h                         ;A1F: F8 20 	
		STR	RF                       ;A21: 5F 	
		OUT	1                     ;A22: 61 	
		DEC	RF                       ;A23: 2F 	
		NOP                                ;A24: C4 	
		LDI	20h                         ;A25: F8 20 	
		STR	RF                       ;A27: 5F 	
		OUT	1                     ;A28: 61 	
		DEC	RF                       ;A29: 2F 	
		NOP                                ;A2A: C4 	
				;		ENDIF

SUB_R3_14	LDI	20h                         ;A2B: F8 20 	
		STR	RF                       ;A2D: 5F 	
		OUT	1                     ;A2E: 61 	
		DEC	RF                       ;A2F: 2F 	
		NOP                                ;A30: C4 	
		LDI	1Eh                         ;A31: F8 1E 	
		PLO	RD	;RD=101E
		LDN	RD
		STR	RF                       ;A35: 5F 	
		OUT	1                     ;A36: 61 	
		DEC	RF                       ;A37: 2F 	
		NOP                                ;A38: C4 	
		DEC	RD	;RD=101D
		LDN	RD                       ;A3A:  D 	
		STR	RF                       ;A3B: 5F 	
		OUT	1                     ;A3C: 61 	
		DEC	RF                       ;A3D: 2F 	
		NOP                                ;A3E: C4 	
		REQ                                ;A3F: 7A 	
		LBR	SEP_R3_RET                 ;A40: C0  9 32
		
;+++++++++++++++++++++++++++++   SUR_R3 END +++++++++++++++++++++++++++++++++++
		
		
		
		
		
LA43_LOOP	PRINT_ACC                          ;A43: D3 	;DEAD CODE????
		LDI	20h                        ;A44: F8 20 	
		PLO	RE                       ;A46: AE 	
LA47_LOOP	DEC	RE                       ;A47: 2E 	
		GLO	RE                       ;A48: 8E 	
		BZ	LA43_LOOP                  ;A49: 32 43 	
		BR	LA47_LOOP                  ;A4B: 30 47 	
		NOP                                ;A4D: C4 	
								;CLEAR ACC
		SEX	RD                       ;A4E: ED 	;DEAD CODE????
		LDI	 6                         ;A4F: F8  6 	
		PLO	RD                       ;A51: AD 	
		LDI	 0                         ;A52: F8  0 	
		PLO	RA	;Clear Counter
		PHI	RA	;Clear bits RA.1.0 , RA.1.1 , RA.1.2 , RA.1.3 , RA.1.4 , RA.1.5 , RA.1.6 , RA.1.7
		PHI	R5                       ;A56: B5 	
		STXD                               ;A57: 73 	;V   M
		STXD                               ;A58: 73 	;00  1006 
		LDI	0E0h                       ;A59: F8 E0 	;00  1005 
		STXD                               ;A5B: 73 	;E0  1004 
		STXD                               ;A5C: 73 	;E0  1003 
		STXD                               ;A5D: 73 	;E0  1002 
		STXD                               ;A5E: 73 	;E0  1001 
		LDI	0F0h                       ;A5F: F8 F0 	;F0  1000 
		STR	RD                       ;A61: 5D 	
		LBR	UPDATE_LED
		
EX_KEY_INVSIGN	GHI	RA                       ;A65: 9A 	
		ANI	 8	;Test bits RA.1.3 NO re-entry
		LBNZ	 SCAN_KEY_LOOP             ;A68: CA  0 E1
		
		GHI	RA                       ;A6B: 9A 	
		ORI	 8                         ;A6C: F9  8 	
		PHI	RA	;Set bit RA.1.3  Set to prevent re-entry until key is released
		
		LDI	6                          ;A6F: F8  6 	
		PLO	RD	;RD=1006 ACC       ;A71: AD 	
		LDN	RD                       ;A72:  D 	
		XRI	1       ;INVERT SIGN       ;A73: FB  1 	
		STR	RD                       ;A75: 5D 	
		LBR	PRINT___2                  ;A76: C0  8 6F	

;PRINT_%_XXX	
EX_KEY_DIVFIS_P	LDI	1Eh                         ;A79: F8 1E 	
		PLO	RD                       ;A7B: AD 	
		LDI	'%'                         ;A7C: F8 25 	
		STR	RD                       ;A7E: 5D 	
		DEC	RD                       ;A7F: 2D 	
		LDI	' '                         ;A80: F8 20 	
		STR	RD                       ;A82: 5D 	
		PRINT_ACC                          ;A83: D3 	
		
		GLO	R4                       ;A84: 84 	
		ORI	 1                         ;A85: F9  1 	
		PLO	R4	;Set Bit R4.0.0
		
		GHI	RA                       ;A88: 9A 	
		ORI	 2                         ;A89: F9  2 	
		PHI	RA	;Set bit RA.1.1
		
		LDI	 0                         ;A8C: F8  0 	
		PLO	RA	;RA.0 Reset Count of Keys entered into ACC
		LBR	L424                       ;A8F: C0  4 24	
		
EX_KEY_F8_A92	GHI	R4                       ;A92: 94 	
		ANI	 4	;Test bit R4.1.2
		LBZ	UPDATE_LED
		
		LDI	1Ch	;RD=101C           ;A98: F8 1C 	
		PLO	RD                       ;A9A: AD 	
		LDI	 4      ;RE=1004 ACC FEET HUNDREDS ;A9B: F8  4 	
		PLO	RE                       ;A9D: AE 	
		LDN	RD                       ;A9E:  D 	
		BNZ	LAC4    ;Jump not zero     ;A9F: 3A C4 	
		ORI	0E0h                       ;AA1: F9 E0 	Optionally Blank Hundreds (ORI)
		STR	RE                       ;AA3: 5E 	
		LDI	17h	;RD=1017           ;AA4: F8 17 	
		PLO	RD                       ;AA6: AD 	
		LDN	RD                       ;AA7:  D 	
		BNZ	LAC7    ;Jump not zero     ;AA8: 3A C7 	
		DEC	RE                       ;AAA: 2E 	
		ORI	0E0h                       ;AAB: F9 E0 	Optionally Blank Tens  (ORI)
		STR	RE                       ;AAD: 5E 	
		LDI	15h	;RD=1015           ;AAE: F8 15 	
		PLO	RD                       ;AB0: AD 	
		LDN	RD                       ;AB1:  D 	
		BNZ	LACF    ;Jump not zero     ;AB2: 3A CF 	
		DEC	RE                       ;AB4: 2E 	
		ORI	0E0h                       ;AB5: F9 E0 	Optionally Blank Units (ORI)
		STR	RE                       ;AB7: 5E 	
		LDI	3Eh	;RD=103E           ;AB8: F8 3E 	
		PLO	RD                       ;ABA: AD 	
		LDN	RD                       ;ABB:  D 	
		BNZ	LAD7	;Jump not zero     ;ABC: 3A D7 	
		DEC	RE                       ;ABE: 2E 	
		ORI	0E0h                       ;ABF: F9 E0 	Optionally Blank Inches (ORI)
		STR	RE                       ;AC1: 5E 	
		BR	LADF                       ;AC2: 30 DF 	
		
LAC4		ORI	0F0h                       ;AC4: F9 F0 	Save non-zero Hundreds
		STR	RE                       ;AC6: 5E 	
LAC7		LDI	17h    ;RD=1017            ;AC7: F8 17 	
		PLO	RD                       ;AC9: AD 	
		DEC	RE                       ;ACA: 2E 	
		LDN	RD                       ;ACB:  D 	
		ORI	0F0h                       ;ACC: F9 F0 	Save non-zero Tens
		STR	RE                       ;ACE: 5E 	
LACF		LDI	15h    ;RD=1015            ;ACF: F8 15 	
		PLO	RD                       ;AD1: AD 	
		DEC	RE                       ;AD2: 2E 	
		LDN	RD                       ;AD3:  D 	
		ORI	0F0h                       ;AD4: F9 F0 	Save non-zero Units
		STR	RE                       ;AD6: 5E 	
LAD7		LDI	3Eh    ;RD=103E            ;AD7: F8 3E 	
		PLO	RD                       ;AD9: AD 	
		DEC	RE                       ;ADA: 2E 	
		LDN	RD                       ;ADB:  D 	
		ORI	0F0h                       ;ADC: F9 F0 	Save non-zero Inches
		STR	RE                       ;ADE: 5E 	
LADF		LDI	3Fh	;RD=103F           ;ADF: F8 3F 	
		PLO	RD                       ;AE1: AD 	
		DEC	RE                       ;AE2: 2E 	
		LDN	RD                       ;AE3:  D 	
		ORI	0F0h                       ;AE4: F9 F0 	Save non-zero Sixteenths
		STR	RE                       ;AE6: 5E 	
		
		GHI	R4                       ;AE7: 94 	
		ANI	0FBh                       ;AE8: FA FB 	
		PHI	R4	;Clear bit R4.1.2
		LBR	PRINT_RE                       ;AEB: C0  8 61	

EX_KEY_DIVW_P	GHI	R4                       ;AEE: 94 	
		ORI	 8                         ;AEF: F9  8 	
		PHI	R4	;Set bit R4.1.3
		LBR	PRINT_DIVIDE                  ;AF2: C0  8 2A PRINT %_
		
		.DB	0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh

		;.org	0xAFD
INIT		LDI	 0	
		PLO	R4	;R4=??00  Clear Flags
		LDI	10h	;RD=103B
		PHI	RD
		SEX	RD	;X=D
		LDI	3Bh	;4 variables of 7 bytes each
		PLO	RD	;INIT MEMORY WITH:
INIT_LP1	LDI	 0      ;V	MEMORY
		STXD		;00  103B  1034  102D  1026
		STXD		;00  103A  1033  102C  1025
		LDI	0E0h	;E0  1039  1032  102B  1024
		STXD		;E0  1038  1031  102A  1023
		STXD		;E0  1037  1030  1029  1022
		STXD		;E0  1036  102F  1028  1021
		STXD		;F0  1035  102E  1027  1020
		LDI	0F0h
		STXD
		GLO	RD
		XRI	1Fh	;EXIT AT 101F
		BZ	INIT_EX1
		BR	INIT_LP1
INIT_EX1	LDI	0Eh	;R1=0EFD (ISR_SUB)
		PHI	R1
		LDI	0FDh
		PLO	R1	
		LDI	10h	;R2=103D (ISR STACK)
		PHI	R2
		LDI	3Dh
		PLO	R2	
		
DO_CLEAR	LDI	 9	;R3=0933 (SEP_R3)  PRINT_ACC
		PHI	R3
		LDI	33h
		PLO	R3	
		LDI	 6	;R8=0647 (SEP_R8)
		PHI	R8
		LDI	47h
		PLO	R8	
		LDI	 6	;R9=060C (SEP_R9)
		PHI	R9
		LDI	0Ch
		PLO	R9	
		LDI	 3	;R7=03F9 (SEP_R7)
		PHI	R7
		LDI	0F9h
		PLO	R7	
		LDI	 2	;R6=02E1 (SEP_R6)
		PHI	R6
		LDI	0E1h
		PLO	R6	
		LDI	TBL_LIMIT%256	;0ECh	;RB=??EC
		PLO	RB	
		LDI	TBL_ADJUST%256	;0EEh	;RC=??EE
		PLO	RC	
		LDI	0
		PHI	R4	;R4=0000 Clear Flags
		PHI	R5	;R5=00??
		PHI	RB	;RB=00EC
		PHI	RC	;RC=00EE
		GLO	R4
		ANI	40h	;Clear all flags except Printer ERROR
		PLO	R4	;R4.0 = R4.0 AND 40
		LDI	10h
		PHI	RD	;RD=101F
		PHI	RE	;RE=10??
		LBR	START0
	
		
		.DB	0FFh,0FFh,0FFh,0FFh

				;Routine 1 of the ADD Routine
				;Convert the compound OPERAND number at M(1012):M(1011):M(1010):M(100F):M(100E)
				;To an 18 bit binary integer number of sixteeths of an ich at M(101A):M(101B):M(101C)
				;
				;First, convert 3 nibbles to 16 bit binary
				;1010 00 UNITS		to	;1017 00 ADD: Running Sum (high 8 bits)
				;1011 00 TENS			;1018 00 ADD: Running Sum (low 8 bits)
				;1012 00 HUNDREDS
									
EX_ADD_R1	LDI	16h	;RE=1016           ;B60: F8 16	---- SUM = M(1012) * 100 ----
		PLO	RE                       ;B62: AE 	
		LDI	12h	;RD=1012           ;B63: F8 12 	
		PLO	RD                       ;B65: AD 	
		SEX	RE	;X=E               ;B66: EE 	
		LDN	RD	
		STXD            ;M(1016)=M(1012)	;Multipy M(1012) by 100
		LDI	100
		STXD		;M(1015)=100
		LDI	0
		STR	RE	;M(1014)=0
		PLO	RF	;RF.0=0
		SHR		;DF=0
		MULTIPY                       ;B71: D8 	
		LDI	14h     ;RD=1014           ;B72: F8 14 	
		PLO	RD	                   ;B74: AD 	
		LDI	17h	;RE=1017           ;B75: F8 17 	
		PLO	RE                       ;B77: AE 	
		LDA	RD                       ;B78: 4D 	
		STR	RE	;M(1017)=M(1014)	;Save running sum
		INC	RE                       ;B7A: 1E 	
		LDN	RD                       ;B7B:  D 	
		STR	RE	;M(1018)=M(1015)		++++ SUM = M(1012) * 100 ++++
		
		LDI	16h	;RE=1016           ;B7D: F8 16 	---- SUM = SUM + M(1011) * 10 ----
		PLO	RE                       ;B7F: AE 	
		LDI	11h	;RD=1011           ;B80: F8 11 	
		PLO	RD                       ;B82: AD 	
		LDN	RD                       ;B83:  D 	
		STXD		;M(1016)=M(1011)   ;B84: 73 	
		LDI	10      ;M(1015)=10        ;B85: F8  A 	
		STXD                               ;B87: 73 	
		LDI	 0                         ;B88: F8  0 	
		STR	RE                       ;B8A: 5E 	
		PLO	RF	;RF.0=0            ;B8B: AF 	
		SHR		;DF=0              ;B8C: F6 	
		MULTIPY                            ;B8D: D8 	
		LDI	15h	;RD=1015           ;B8E: F8 15 	
		PLO	RD                       ;B90: AD 	
		LDI	18h	;RE=1018           ;B91: F8 18 	
		PLO	RE                       ;B93: AE 	
		LDN	RD                       ;B94:  D 	
		ADD                                ;B95: F4 	
		STXD		;M(1018)=M(1018)+M(1015)
		DEC	RD                       ;B97: 2D 	
		LDN	RD                       ;B98:  D 	
		ADC                                ;B99: 74 	
		STR	RE	;M(1017)=M(1017)+M(1014)+DF	++++ SUM = SUM + M(1011) * 10 ++++
		
		LDI	10h	;RD=1010           ;B9B: F8 10 	---- SUM = SUM + M(1010) ----
		PLO	RD                       ;B9D: AD 	
		LDI	18h	;RE=1018           ;B9E: F8 18 	
		PLO	RE                       ;BA0: AE 	
		LDN	RD                       ;BA1:  D 	
		ADC                                ;BA2: 74 	
		STXD            ;M(1018)=M(1018)+M(1010)
		LDI	 0                         ;BA4: F8  0 	
		ADC                                ;BA6: 74 	
		STR	RE	;M(1017)=M(1017)+0+DF	        ++++ SUM = SUM + M(1010) ++++

								;Multiply this SUM by 192, to convert feet to Sixteenths of an inch.
		LDI	17h	;RD=1017           ;BA8: F8 17 	
		PLO	RD                       ;BAA: AD 	
		LDI	15h	;RE=1015           ;BAB: F8 15 	
		PLO	RE                       ;BAD: AE 	
		LDA	RD	;D=M(1017) Running SUM high 8 bits
		STR	RE	;M(1015)=M(1017) Running SUM high 8 bits    ;M(1015):M(1016) become the 16 bit Multiplier
		INC	RE                       ;BB0: 1E 	
		LDN	RD                       ;BB1:  D 	
		STR	RE	;M(1016)=M(1018) Running SUM low 8 bits
		
		LDI	14h	;M(1014)=0
		PLO	RD                       ;BB5: AD 	
		LDI	 0                         ;BB6: F8  0 	
		STR	RD                       ;BB8: 5D 	
		PLO	RF	;RF.0=0            ;BB9: AF 	
		LDI	17h                        ;BBA: F8 17 	
		PLO	RD	;RD=1017           ;BBC: AD 	
		LDI	192                        ;BBD: F8 C0 	
		STR	RD						;M(1017)=192 is the 8 bit Multiplier
		SHR             ;DF=0              ;BC0: F6 	
		
EAR1_LOOP	LDI	14h                        ;BC1: F8 14 	
		PLO	RE                       ;BC3: AE 	
		LDN	RE	;D=M(1014)         ;BC4:  E 		;M(1014):M(1015):M(1016) become the 18 bit Product
		SHRC                               ;BC5: 76 	
		STR	RE                       ;BC6: 5E 	
		INC	RE                       ;BC7: 1E 	
		LDN	RE                       ;BC8:  E 	
		SHRC                               ;BC9: 76 	
		STR	RE                       ;BCA: 5E 	
		INC	RE                       ;BCB: 1E 	
		LDN	RE                       ;BCC:  E 	
		SHRC                               ;BCD: 76 	
		STR	RE                       ;BCE: 5E 	
		BDF	EAR1_D3                    ;BCF: 33 D3 	
		BR	EAR1_DA                    ;BD1: 30 DA 	
EAR1_D3		INC	RE                       ;BD3: 1E 	
		LDN	RE                       ;BD4:  E 	
		DEC	RE                       ;BD5: 2E 	
		DEC	RE                       ;BD6: 2E 	
		DEC	RE                       ;BD7: 2E 	
		ADD                                ;BD8: F4 	
		STR	RE                       ;BD9: 5E 	
EAR1_DA		INC	RF                       ;BDA: 1F 	
		GLO	RF                       ;BDB: 8F 	
		XRI	11h                       ;BDC: FB 11 	;11 shifts = 10 bits because the largest number 999 is only 3E7 in hex (10 bits)
		BZ	EAR1_E2                    ;BDE: 32 E2 	
		BR	EAR1_LOOP                  ;BE0: 30 C1 	;M(1014):M(1015):M(1016) become the 18 bit Product
		
EAR1_E2		SEX	RD                       ;BE2: ED 	;Save Product
		LDI	14h	;RD=1014           ;BE3: F8 14 	    M(101A):M(101B):M(101C) = M(1014):M(1015):M(1016)
		PLO	RD                       ;BE5: AD 	
		LDI	1Ah	;RE=101A           ;BE6: F8 1A 	
		PLO	RE                       ;BE8: AE 	
		LDXA                               ;BE9: 72 	
		STR	RE	;M(101A)=M(1014)
		INC	RE                       ;BEB: 1E 	
		LDXA                               ;BEC: 72 	
		STR	RE    ;M(101B)=M(1015)   ;BED: 5E 	
		INC	RE                       ;BEE: 1E 	
		LDX                                ;BEF: F0 	
		STR	RE	;M(101C)=M(1016)


		LDI	16h	;RD=1016           ;BF1: F8 16 	
		PLO	RD                       ;BF3: AD 	
		LDI	0Fh	;RE=100F           ;BF4: F8  F 	
		PLO	RE                       ;BF6: AE 	
		LDN	RE                       ;BF7:  E 	
		STXD            ;M(1016)=M(100F) INCHES
		LDI	16                         ;BF9: F8 10 	Multiply by 16 to get 16th's of an inch
		STXD            ;M(1015)=16        ;BFB: 73 	
		LDI	 0                         ;BFC: F8  0 	
		STR	RD    ;M(1014)=0         ;BFE: 5D 	
		PLO	RF	;RF.0=0            ;BFF: AF 	
		SHR             ;DF=0              ;C00: F6 	
		MULTIPY                            ;C01: D8 	M(1015)=Inches * 16  (1 byte is wide enough for result)
		
		LDI	1Ch	;RD=101C           ;C02: F8 1C 	;ADD INCHES * 16 TO RUNNING SUM At M(101A):M(101B):M(101C)
		PLO	RD                       ;C04: AD 	
		LDI	15h	;RE=1015           ;C05: F8 15 	
		PLO	RE                       ;C07: AE 	
		LDN	RE                       ;C08:  E 	
		ADD                                ;C09: F4 	
		STXD                               ;C0A: 73 	
		LDI	 0                         ;C0B: F8  0 	
		ADC                                ;C0D: 74 	
		STXD                               ;C0E: 73 	
		LDI	 0                         ;C0F: F8  0 	
		ADC                                ;C11: 74 	
		STR	RD                       ;C12: 5D 	

		LDI	1Ch                         ;C13: F8 1C ;ADD Sixteenths TO RUNNING SUM At M(101A):M(101B):M(101C)	
		PLO	RD                       ;C15: AD 	
		LDI	0Eh	;RE=100E Sixteenths
		PLO	RE                       ;C18: AE 	
		LDN	RE                       ;C19:  E 	
		ADD                                ;C1A: F4 	
		STXD                               ;C1B: 73 	
		LDI	 0                         ;C1C: F8  0 	
		ADC                                ;C1E: 74 	
		STXD                               ;C1F: 73 	
		LDI	 0                         ;C20: F8  0 	
		ADC                                ;C22: 74 	
		STR	RD                       ;C23: 5D 	
		
;++++++++++++  M(101A):M(101B):M(101C) = Binary Total of OPERAND Compound Number ++++++++++++
		
		
					;Convert ACC to Binary
					
		NOP                                ;C24: C4 	Multiply ACC hundreds by 100
		SEX	RE                       ;C25: EE 	
		LDI	16h                         ;C26: F8 16 	
		PLO	RE                       ;C28: AE 	
		LDI	 4                         ;C29: F8  4 	
		PLO	RD                       ;C2B: AD 	
		LDN	RD                       ;C2C:  D 	
		ANI	0Fh                         ;C2D: FA  F 	
		STXD                               ;C2F: 73 	
		LDI	100                         ;C30: F8 64 	
		STXD                               ;C32: 73 	
		LDI	 0                         ;C33: F8  0 	
		STR	RE                       ;C35: 5E 	
		PLO	RF	;RF.0=0            ;C36: AF 	
		SHR		;DF=0              ;C37: F6 	
		MULTIPY	          ;C38: D8 	
		
		LDI	14h                        ;C39: F8 14 	Save ACC 100's to running total
		PLO	RD                       ;C3B: AD 	
		LDI	17h                        ;C3C: F8 17 	
		PLO	RE                       ;C3E: AE 	
		LDA	RD                       ;C3F: 4D 	
		STR	RE                       ;C40: 5E 	
		INC	RE                       ;C41: 1E 	
		LDN	RD                       ;C42:  D 	
		STR	RE                       ;C43: 5E 	
		
		LDI	16h                        ;C44: F8 16 	Multiply ACC tens by 10
		PLO	RE                       ;C46: AE 	
		LDI	 3                         ;C47: F8  3 	
		PLO	RD                       ;C49: AD 	
		LDN	RD                       ;C4A:  D 	
		ANI	0Fh                         ;C4B: FA  F 	
		STXD                               ;C4D: 73 	
		LDI	10                         ;C4E: F8  A 	
		STXD                               ;C50: 73 	
		LDI	 0                         ;C51: F8  0 	
		STR	RE                       ;C53: 5E 	
		PLO	RF	;RF.0=0            ;C54: AF 	
		SHR             ;DF=0              ;C55: F6 	
		MULTIPY                            ;C56: D8
		 	
		LDI	15h	;RD=1015           ;C57: F8 15 ADD ACC 10'S to running total	
		PLO	RD                       ;C59: AD 	
		LDI	18h	;RE=1018           ;C5A: F8 18 	
		PLO	RE                       ;C5C: AE 	
		LDN	RD                       ;C5D:  D 	
		ADD                                ;C5E: F4 	
		STXD                               ;C5F: 73 	
		DEC	RD                       ;C60: 2D 	
		LDN	RD                       ;C61:  D 	
		ADC                                ;C62: 74 	
		STR	RE                       ;C63: 5E 	
		
		LDI	 2	;RD=1002           ;C64: F8  2 	Add ACC Units to running total
		PLO	RD                       ;C66: AD 	
		LDI	18h	;RE=1018           ;C67: F8 18 	
		PLO	RE                       ;C69: AE 	
		LDN	RD                       ;C6A:  D 	
		ANI	0Fh                         ;C6B: FA  F 	
		ADC                                ;C6D: 74 	
		STXD                               ;C6E: 73 	
		LDI	 0                         ;C6F: F8  0 	
		ADC                                ;C71: 74 	
		STR	RE                       ;C72: 5E 	
		
		LDI	17h	;RD=1017           ;C73: F8 17 Multiply ACC Feet by 192	
		PLO	RD                       ;C75: AD 	
		LDI	15h	;RE=1015           ;C76: F8 15 	
		PLO	RE                       ;C78: AE 	
		LDA	RD                       ;C79: 4D 	
		STR	RE                       ;C7A: 5E 	
		INC	RE                       ;C7B: 1E 	
		LDN	RD                       ;C7C:  D 	
		STR	RE                       ;C7D: 5E 	
		LDI	14h                         ;C7E: F8 14 	
		PLO	RD                       ;C80: AD 	
		LDI	 0                         ;C81: F8  0 	
		STR	RD                       ;C83: 5D 	
		PLO	RF                       ;C84: AF 	
		LDI	17h                         ;C85: F8 17 	
		PLO	RD                       ;C87: AD 	
		LDI	192                         ;C88: F8 C0 	
		STR	RD                       ;C8A: 5D 	
		SHR                                ;C8B: F6 	
		
LC8C_LOOP	LDI	14h                         ;C8C: F8 14 	
		PLO	RE                       ;C8E: AE 	
		LDN	RE                       ;C8F:  E 	
		SHRC                          ;C90: 76 	
		STR	RE                       ;C91: 5E 	
		INC	RE                       ;C92: 1E 	
		LDN	RE                       ;C93:  E 	
		SHRC                          ;C94: 76 	
		STR	RE                       ;C95: 5E 	
		INC	RE                       ;C96: 1E 	
		LDN	RE                       ;C97:  E 	
		SHRC                          ;C98: 76 	
		STR	RE                       ;C99: 5E 	
		BDF	LC9E                         ;C9A: 33 9E 	
		BR	LCA5                         ;C9C: 30 A5 	
LC9E		INC	RE                       ;C9E: 1E 	
		LDN	RE                       ;C9F:  E 	
		DEC	RE                       ;CA0: 2E 	
		DEC	RE                       ;CA1: 2E 	
		DEC	RE                       ;CA2: 2E 	
		ADD                                ;CA3: F4 	
		STR	RE                       ;CA4: 5E 	
LCA5		INC	RF                       ;CA5: 1F 	
		GLO	RF                       ;CA6: 8F 	
		XRI	11h                      ;CA7: FB 11 	
		BZ	LCAD                         ;CA9: 32 AD 	
		BR	LC8C_LOOP                  ;CAB: 30 8C 	
		
LCAD		SEX	RD                       ;CAD: ED 	Move 18 bit ACC Sum to M(101D):M(101E):M(101F)
		LDI	14h                         ;CAE: F8 14 	
		PLO	RD                       ;CB0: AD 	
		LDI	1Dh                         ;CB1: F8 1D 	
		PLO	RE                       ;CB3: AE 	
		LDXA                               ;CB4: 72 	
		STR	RE                       ;CB5: 5E 	
		INC	RE                       ;CB6: 1E 	
		LDXA                               ;CB7: 72 	
		STR	RE                       ;CB8: 5E 	
		INC	RE                       ;CB9: 1E 	
		LDX                                ;CBA: F0 	
		STR	RE                       ;CBB: 5E 	
		
		LDI	16h                         ;CBC:     Multiply ACC Inches by 16
		PLO	RD                       ;CBE: AD 	
		LDI	 1                         ;CBF: F8  1 	
		PLO	RE                       ;CC1: AE 	
		LDN	RE                       ;CC2:  E 	
		ANI	0Fh                         ;CC3: FA  F 	
		STXD                               ;CC5: 73 	
		LDI	16                         ;CC6: F8 10 	
		STXD                               ;CC8: 73 	
		LDI	 0                         ;CC9: F8  0 	
		STR	RD                       ;CCB: 5D 	
		PLO	RF	;RF.0=0            ;CCC: AF 	
		SHR		;DF=0              ;CCD: F6 	
		MULTIPY                        ;CCE: D8 	

		LDI	1Fh                         ;CCF:   Add ACC Inches * 16 to 18 bit ACC Sum at M(101D):M(101E):M(101F)
		PLO	RD                       ;CD1: AD 	
		LDI	15h                         ;CD2: F8 15 	
		PLO	RE                       ;CD4: AE 	
		LDN	RE                       ;CD5:  E 	
		ADD                                ;CD6: F4 	
		STXD                               ;CD7: 73 	
		LDI	 0                         ;CD8: F8  0 	
		ADC                                ;CDA: 74 	
		STXD                               ;CDB: 73 	
		LDI	 0                         ;CDC: F8  0 	
		ADC                                ;CDE: 74 	
		STR	RD                       ;CDF: 5D 	
		
		LDI	1Fh                         ;		 Add ACC Sixteenths to 18 bit ACC Sum at M(101D):M(101E):M(101F)
		PLO	RD                       ;CE2: AD 	
		LDI	 0                         ;CE3: F8  0 	
		PLO	RE                       ;CE5: AE 	
		LDN	RE                       ;CE6:  E 	
		ANI	0Fh                         ;CE7: FA  F 	
		ADD                                ;CE9: F4 	
		STXD                               ;CEA: 73 	
		LDI	 0                         ;CEB: F8  0 	
		ADC                                ;CED: 74 	
		STXD                               ;CEE: 73 	
		LDI	 0                         ;CEF: F8  0 	
		ADC                                ;CF1: 74 	
		STR	RD                       ;CF2: 5D 	FINAL ACC SUM M(101D):M(101E):M(101F)
		
		SEX	RE                       ;CF3: EE 	   M(1017):M(1018):M(1019)=000000
		LDI	19h                         ;CF4: F8 19 	
		PLO	RE                       ;CF6: AE 	
		LDI	0                         ;CF7: F8  0 	
		STXD                               ;CF9: 73 	
		STXD                               ;CFA: 73 	
		STR	RE                       ;CFB: 5E
		PLO	RA	;Set RA.0 Bit Count = 0
						
							;Divide OPERAND by ACC
				;X=E
LCFD_LOOP	LDI	1Fh	;RE=101F           ;CFD: F8 1F 	24 BIT SUBTRACT RF = RD - RE
		PLO	RE                       ;CFF: AE 	
		LDI	19h	;RD=1019           ;D00: F8 19 	RD -> M(1017):M(1018):M(1019) [000000]
		PLO	RD                       ;D02: AD 	RE -> M(101D):M(101E):M(101F) (FINAL SUM OF ACC) (DIVIDE BY)
		LDI	10h	;RF=1016           ;D03: F8 10 	RF -> M(1014):M(1015):M(1016) Difference
		PHI	RF                       ;D05: BF 	
		LDI	16h                        ;D06: F8 16 	
		PLO	RF                       ;D08: AF 	
		LDN	RD                       ;D09:  D 	
		SM                                 ;D0A: F7 	
		STR	RF                       ;D0B: 5F 	
		DEC	RF                       ;D0C: 2F 	
		DEC	RD                       ;D0D: 2D 	
		DEC	RE                       ;D0E: 2E 	
		LDN	RD                       ;D0F:  D 	
		SMB                                ;D10: 77 	
		STR	RF                       ;D11: 5F 	
		DEC	RF                       ;D12: 2F 	
		DEC	RD                       ;D13: 2D 	
		DEC	RE                       ;D14: 2E 	
		LDN	RD                       ;D15:  D 	
		SMB                                ;D16: 77 	
		STR	RF                       ;D17: 5F 	
		
		BNF	LD22	;Jump if DF=0 ie Result of 24 bit subtraction was MINUS and created a borrow.
		LDA	RF                       ;D1A: 4F   24bit RD = RF
		STR	RD                       ;D1B: 5D 	M(1017):M(1018):M(1019) = M(1014):M(1015):M(1016)
		INC	RD                       ;D1C: 1D 	
		LDA	RF                       ;D1D: 4F 	
		STR	RD                       ;D1E: 5D 	
		INC	RD                       ;D1F: 1D 	
		LDN	RF                       ;D20:  F 	
		STR	RD                       ;D21: 5D 
								
								;Shift the DF bit into 48 bit register.
LD22		LDI	1Ch	;RE=101C           ;D22: F8 1C    RE -> M(101A):M(101B):M(101C) (FINAL SUM OF OPERAND)
		PLO	RE                       ;D24: AE 	
		LDN	RE	;M(101C)           ;D25:  E 	
		SHLC	   ;Shift left through Carry  ;D26: 7E 	
		STXD                               ;D27: 73      SHLC	M(1017):M(1018):M(1019):M(101A):M(101B):M(101C)
		LDN	RE    ;M(101B)           ;D28:  E 	
		SHLC	                           ;D29: 7E 	
		STXD                               ;D2A: 73 	
		LDN	RE    ;M(101A)           ;D2B:  E 	
		SHLC	                           ;D2C: 7E 	
		STXD                               ;D2D: 73 	
		LDN	RE    ;M(1019)           ;D2E:  E 	
		SHLC	                           ;D2F: 7E 	
		STXD                               ;D30: 73 	
		LDN	RE    ;M(1018)           ;D31:  E 	
		SHLC	                           ;D32: 7E 	
		STXD                               ;D33: 73 	
		LDN	RE    ;M(1017)           ;D34:  E 	
		SHLC	                           ;D35: 7E 	
		STR	RE                       ;D36: 5E
		
		INC	RA	;Increment RA.0 Bit Counter
		GLO	RA	;Test RA.0 Bit Counter = 25
		XRI	25                         ;D39: FB 19 	
		BZ	LD40                       ;D3B: 32 40 	
		LBR	LCFD_LOOP                  ;D3D: C0  C FD	
		
LD40		LDN	RE                       ;D40:  E 	SHRC	M(1017):M(1018):M(1019)
		SHR                                ;D41: F6 	
		STR	RE                       ;D42: 5E 	
		INC	RE                       ;D43: 1E 	
		LDN	RE                       ;D44:  E 	
		SHRC                               ;D45: 76 	
		STR	RE                       ;D46: 5E 	
		INC	RE                       ;D47: 1E 	
		LDN	RE                       ;D48:  E 	
		SHRC                               ;D49: 76 	
		STR	RE                       ;D4A: 5E 	
		LBR	LF56                       ;D4B: C0  F 56 Jump to test for >999 overflow error
								;M(101A):M(101B):M(101C) - 0003E7h  (999)
		
LD4E		LDI	1Dh	;RE=101D           ;D4E: F8 1D 	 RD->1017
		PLO	RE                       ;D50: AE 	
		LDA	RD                       ;D51: 4D 	 24 bit M(RE)=M(RD)
		STR	RE                       ;D52: 5E 	    M(101D):M(101E):M(101F) = M(1017):M(1018):M(1019)
		INC	RE                       ;D53: 1E 	
		LDA	RD                       ;D54: 4D 	
		STR	RE                       ;D55: 5E 	
		INC	RE                       ;D56: 1E 	
		LDN	RD                       ;D57:  D 	
		STR	RE                       ;D58: 5E 	
		
		LDI	1Bh	;RD=101B           ;D59: F8 1B 	16 bit M(RE)=M(RD)
		PLO	RD                       ;D5B: AD 	M(1016):M(1017) = M(101B):M(101C)
		LDI	16h	;RE=1016           ;D5C: F8 16 	
		PLO	RE                       ;D5E: AE 	
		LDA	RD                       ;D5F: 4D 	
		STR	RE                       ;D60: 5E 	
		INC	RE                       ;D61: 1E 	
		LDN	RD                       ;D62:  D 	
		STR	RE                       ;D63: 5E 	
		
		INC	RE	;RE=1018           ;D64: 1E 	M(1018):M(1019) = 0064  (100 decimal)
		LDI	 0                         ;D65: F8  0 	
		STR	RE                       ;D67: 5E 	
		INC	RE                       ;D68: 1E 	
		LDI	100                        ;D69: F8 64 	
		STR	RE                       ;D6B: 5E 	
		
		LDI	14h	;RE=1014           ;D6C: F8 14 	M(1014):M(1015) = 0000
		PLO	RE                       ;D6E: AE 	
		LDI	 0                         ;D6F: F8  0 	
		STR	RE                       ;D71: 5E 	
		INC	RE                       ;D72: 1E 	
		STR	RE                       ;D73: 5E 	
		PLO	RF	;RF.0=0            ;D74: AF 	
		DIVIDE                             ;D75: D9 	Get Hundreds Digit
		
		LDI	17h	;RE=1017           ;D76: F8 17 	M(1004) = M(1017)  Save Hundreds
		PLO	RE                       ;D78: AE 	
		LDI	 4	;RD=1004 ACC Feet Hundreds
		PLO	RD                       ;D7B: AD 	
		LDN	RE                       ;D7C:  E 	
		STR	RD                       ;D7D: 5D 	
		
		LDI	14h	;RD=1014           ;D7E: F8 14 	
		PLO	RD                       ;D80: AD 	
		LDI	16h	;RE=1016           ;D81: F8 16 	Fetch the Remainder of previous division
		PLO	RE                       ;D83: AE 	16 bit M(RE)=M(RD)
		LDA	RD                       ;D84: 4D 	M(1016):M(1017) = M(1014):M(1015)
		STR	RE                       ;D85: 5E 	
		INC	RE                       ;D86: 1E 	
		LDN	RD                       ;D87:  D 	
		STR	RE                       ;D88: 5E 	
		
		LDI	 0                         ;D89: F8  0 	M(1014):M(1015) = 0000
		STR	RD                       ;D8B: 5D 	
		DEC	RD                       ;D8C: 2D 	
		STR	RD                       ;D8D: 5D 	
		
		INC	RE                       ;D8E: 1E 	M(1018):M(1019) = 000A  (10 decimal)
		STR	RE                       ;D8F: 5E 	
		PLO	RF	;RF.0=0            ;D90: AF 	
		INC	RE                       ;D91: 1E 	
		LDI	10                         ;D92: F8  A 	
		STR	RE                       ;D94: 5E 	
		DIVIDE                             ;D95: D9 	Get Tens
		
		LDI	17h     ;RE=1017           ;D96: F8 17 	M(1003) = M(1017)  Save Tens
		PLO	RE                       ;D98: AE 	
		LDI	 3      ;RD=1003 ACC Feet Tens
		PLO	RD                       ;D9B: AD 	
		LDN	RE                       ;D9C:  E 	
		STR	RD                       ;D9D: 5D 	
		
		LDI	15h	;RE=1015            ;D9E: F8 15 M(1002) = M(1015)  Save Units (Remainder of division)
		PLO	RE                       ;DA0: AE 	
		DEC	RD	;RD=1002 ACC Feet Units
		LDN	RE                       ;DA2:  E 	
		STR	RD                       ;DA3: 5D 	
		
		DEC	RD	;RD=1001 ACC Inches		M(1001) = E0 (Blank Inches)
		LDI	0E0h                       ;DA5: F8 E0 	
		STR	RD                       ;DA7: 5D 	
		DEC	RD	;RD=1000 ACC Sixteenths		M(1000) = E0 (Blank Sixteenths)
		STR	RD                       ;DA9: 5D 	
		
		LDI	 4	;RD=1004 ACC Feet Hundreds	Blank Leading Zeros for Hundreds and Tens
		PLO	RD                       ;DAC: AD 	
		LDN	RD                       ;DAD:  D 	
		LBNZ	LDC0                       ;DAE: CA  D C0 Test Feet Hundreds for Zero
		LDI	0E0h                         ;DB1: F8 E0  Blank if Zero
		STR	RD                       ;DB3: 5D 	
		DEC	RD                       ;DB4: 2D 	
		LDN	RD                       ;DB5:  D 	
		LBNZ	LDC0                       ;DB6: CA  D C0 Test Feet Tens for Zero	
		LDI	0E0h                         ;DB9: F8 E0  Blank if Zero	
		STR	RD                       ;DBB: 5D 	
		LBR	LDC0                       ;DBC: C0  D C0
		
		.DB	0
								;Save Remainder
LDC0		SEX	RE	;X=E               ;DC0: EE 	24 bit copy M(RE) = M(RD)
		LDI	1Ah	;RE=101A			    M(101A):M(101B):M(101C) = M(101D):M(101E):M(101F)
		PLO	RE                       ;DC3: AE 	
		LDI	1Dh	;RD=101D           ;DC4: F8 1D 	
		PLO	RD                       ;DC6: AD 	
		LDA	RD                       ;DC7: 4D 	
		STR	RE                       ;DC8: 5E 	
		INC	RE                       ;DC9: 1E 	
		LDA	RD                       ;DCA: 4D 	
		STR	RE                       ;DCB: 5E 	
		INC	RE                       ;DCC: 1E 	
		LDN	RD                       ;DCD:  D 	
		STR	RE                       ;DCE: 5E 	
		
		LDI	19h	;RE=1019           ;DCF: F8 19 	M(1017):M(1018):M(1019) = 000000
		PLO	RE                       ;DD1: AE 	
		LDI	 0                         ;DD2: F8  0 	
		STXD                               ;DD4: 73 	
		STXD                               ;DD5: 73 	
		STR	RE                       ;DD6: 5E 	
		PLO	RA	;Clear RA.0 Bit Counter
		
		LDI	1Fh	;RE=101F           ;DD8: F8 1F 	M(101D):M(101E):M(101F) = 0000C0 (192 Decimal)
		PLO	RE                       ;DDA: AE 	
		LDI	192                        ;DDB: F8 C0 	
		STXD                               ;DDD: 73 	
		LDI	 0                         ;DDE: F8  0 	
		STXD                               ;DE0: 73 	
		STR	RE                       ;DE1: 5E 	
		NOP                                ;DE2: C4 	
								;24 Bit Divide 
				;X=E		
LDE3_LOOP	LDI	1Fh	;RE=101F           ;DE3: F8 1F 	24 BIT SUBTRACT RF = RD - RE
		PLO	RE                       ;DE5: AE 	
		LDI	19h	;RD=1019           ;DE6: F8 19 	RD -> M(1017):M(1018):M(1019) [000000]
		PLO	RD                       ;DE8: AD 	RE -> M(101D):M(101E):M(101F) (DIVIDE BY)
		LDI	10h	;RF=1016           ;DE9: F8 10 	RF -> M(1014):M(1015):M(1016) Difference
		PHI	RF                       ;DEB: BF 	
		LDI	16h                        ;DEC: F8 16 	
		PLO	RF                       ;DEE: AF 	
		LDN	RD                       ;DEF:  D 	
		SM                                 ;DF0: F7 	
		STR	RF                       ;DF1: 5F 	
		DEC	RF                       ;DF2: 2F 	
		DEC	RD                       ;DF3: 2D 	
		DEC	RE                       ;DF4: 2E 	
		LDN	RD                       ;DF5:  D 	
		SMB                                ;DF6: 77 	
		STR	RF                       ;DF7: 5F 	
		DEC	RF                       ;DF8: 2F 	
		DEC	RD                       ;DF9: 2D 	
		DEC	RE                       ;DFA: 2E 	
		LDN	RD                       ;DFB:  D 	
		SMB                                ;DFC: 77 	
		STR	RF                       ;DFD: 5F 	
		
		LBNF	LE09	;Jump if DF=0 ie Result of 24 bit subtraction was MINUS and created a borrow.
		LDA	RF                       ;E01: 4F 	24bit RD = RF
		STR	RD                       ;E02: 5D 	  M(1017):M(1018):M(1019) = M(1014):M(1015):M(1016)
		INC	RD                       ;E03: 1D 	
		LDA	RF                       ;E04: 4F 	
		STR	RD                       ;E05: 5D 	
		INC	RD                       ;E06: 1D 	
		LDN	RF                       ;E07:  F 	
		STR	RD                       ;E08: 5D 
			
								;Shift the DF bit into 48 bit register.
LE09		LDI	1Ch	;RE=101C           ;E09: F8 1C    RE -> M(101A):M(101B):M(101C)
		PLO	RE                       ;E0B: AE 	
		LDN	RE                       ;E0C:  E 	
		SHLC                               ;E0D: 7E 	
		STXD                               ;E0E: 73 	SHLC	M(1017):M(1018):M(1019):M(101A):M(101B):M(101C)
		LDN	RE                       ;E0F:  E 	
		SHLC                               ;E10: 7E 	
		STXD                               ;E11: 73 	
		LDN	RE                       ;E12:  E 	
		SHLC                               ;E13: 7E 	
		STXD                               ;E14: 73 	
		LDN	RE                       ;E15:  E 	
		SHLC                               ;E16: 7E 	
		STXD                               ;E17: 73 	
		LDN	RE                       ;E18:  E 	
		SHLC                               ;E19: 7E 	
		STXD                               ;E1A: 73 	
		LDN	RE                       ;E1B:  E 	
		SHLC                               ;E1C: 7E 	
		STR	RE                       ;E1D: 5E 	
		
		INC	RA	;Increment RA.0 Bit Counter
		GLO	RA    ;Test RA.0 Bit Counter = 25
		XRI	25
		BZ	LE27                         ;E22: 32 27 	
		LBR	LDE3_LOOP                  ;E24: C0  D E3
		
LE27		LDN	RE                       ;E27:  E 	SHRC	M(1017):M(1018):M(1019)
		SHR                                ;E28: F6 	
		STR	RE                       ;E29: 5E 	
		INC	RE                       ;E2A: 1E 	
		LDN	RE                       ;E2B:  E 	
		SHRC                               ;E2C: 76 	
		STR	RE                       ;E2D: 5E 	
		INC	RE                       ;E2E: 1E 	
		LDN	RE                       ;E2F:  E 	
		SHRC                               ;E30: 76 	
		STR	RE                       ;E31: 5E 	
		
		LDI	18h	;RD=1018           ;E32: F8 18 16 Bit copy M(RE) = M(RD)
		PLO	RD                       ;E34: AD 	M(1016):M(1017) = M(1018):M(1019)
		LDI	16h	;RE=1016           ;E35: F8 16 	
		PLO	RE                       ;E37: AE 	
		LDA	RD                       ;E38: 4D 	
		STR	RE                       ;E39: 5E 	
		INC	RE                       ;E3A: 1E 	
		LDN	RD                       ;E3B:  D 	
		STR	RE                       ;E3C: 5E 	
		
		LDI	16                         ;E3D: F8 10 	M(1018):M(1019) = 0010  (16 decimal)
		STR	RD                       ;E3F: 5D 	
		DEC	RD                       ;E40: 2D 	
		LDI	 0                         ;E41: F8  0 	
		STR	RD                       ;E43: 5D 	
		
		LDI	14h	;RD=1014           ;E44: F8 14 	M(1014):M(1015) = 0000
		PLO	RD                       ;E46: AD 	
		LDI	 0                         ;E47: F8  0 	
		STR	RD                       ;E49: 5D 	
		INC	RD                       ;E4A: 1D 	
		STR	RD                       ;E4B: 5D 	
		PLO	RF	;RF.0 = 0          ;E4C: AF 	
		DIVIDE                             ;E4D: D9 	Get Inches (With Sixteenths Remainder)
		
		LDI	17h	;RD=1017           ;E4E: F8 17 	M(103E) = M(1017)  Save Inches
		PLO	RD                       ;E50: AD 	
		LDI	3Eh	;RE=103E           ;E51: F8 3E 	
		PLO	RE                       ;E53: AE 	
		LDN	RD                       ;E54:  D 	
		STR	RE                       ;E55: 5E 	

		DEC	RD                       ;E56: 2D 	Fetch remainder Sixteenths
		DEC	RD                       ;E57: 2D 	M(103F) = M(1015)  Save Sixteenths
		INC	RE                       ;E58: 1E 	
		LDN	RD                       ;E59:  D 	
		STR	RE                       ;E5A: 5E 	
		
		LDI	14h	;RD=1014           ;E5B: F8 14 	M(1014):M(1015) = 0000
		PLO	RD                       ;E5D: AD 	
		LDI	 0                         ;E5E: F8  0 	
		STR	RD                       ;E60: 5D 	
		INC	RD                       ;E61: 1D 	
		STR	RD                       ;E62: 5D 	
		
		LDI	18h	;RD=1018           ;E63: F8 18 	M(1018):M(1019) = 0064  (100 decimal)
		PLO	RD                       ;E65: AD 	
		LDI	 0                         ;E66: F8  0 	
		STR	RD                       ;E68: 5D 	
		INC	RD                       ;E69: 1D 	
		LDI	100                         ;E6A: F8 64 	
		STR	RD                       ;E6C: 5D 	
		
		LDI	1Bh	;RD=101B           ;E6D: F8 1B 	16 Bit copy M(RE) = M(RD)
		PLO	RD                       ;E6F: AD 	 M(1016):M(1017) = M(101B):M(101C)
		LDI	16h     ;RE=1016           ;E70: F8 16 	
		PLO	RE                       ;E72: AE 	
		LDA	RD                       ;E73: 4D 	
		STR	RE                       ;E74: 5E 	
		INC	RE                       ;E75: 1E 	
		LDN	RD                       ;E76:  D 	
		STR	RE                       ;E77: 5E 	
		LDI	 0                         ;E78: F8  0 	
		PLO	RF                       ;E7A: AF 	
		DIVIDE                             ;E7B: D9 	GET Hundreds
		
		LDI	17h	;RD=1017           ;E7C: F8 17 	M(101C) = M(1017)  Save Hundreds
		PLO	RD                       ;E7E: AD 	
		LDI	1Ch	;RE=101C           ;E7F: F8 1C 	
		PLO	RE                       ;E81: AE 	
		LDN	RD                       ;E82:  D 	
		STR	RE                       ;E83: 5E 
			
		LDI	15h     ;RE=1015           ;E84: F8 15 	Fetch remainder Tens*10+Units
		PLO	RE                       ;E86: AE 	M(1017) = M(1015)  
		LDI	17h     ;RD=1017           ;E87: F8 17 	
		PLO	RD                       ;E89: AD 	
		LDN	RE                       ;E8A:  E 	
		STR	RD                       ;E8B: 5D 	
		
		DEC	RF	;RF does nothing here
		
		LDI	14h	;RD=1014           ;E8D: F8 14 	M(1014):M(1015) = 0000
		PLO	RD                       ;E8F: AD 	
		LDI	 0                         ;E90: F8  0 	
		STR	RD                       ;E92: 5D 	
		INC	RD                       ;E93: 1D 	
		STR	RD                       ;E94: 5D
		
		INC	RD                       ;E95: 1D     M(1016) = 00
		STR	RD                       ;E96: 5D 	
							; 	M(1016):M(1017) = remainder Tens*10+Units
		
		LDI	18h	;RD=1018           ;E97: F8 18 	M(1018):M(1019) = 000A  (10 decimal)
		PLO	RD                       ;E99: AD 	
		LDI	 0                         ;E9A: F8  0 	
		STR	RD                       ;E9C: 5D 	
		INC	RD                       ;E9D: 1D 	
		LDI	10                         ;E9E: F8  A 	
		STR	RD                       ;EA0: 5D 	
		LDI	 0                         ;EA1: F8  0 	
		PLO	RF	;RF.0=0             ;EA3: AF 	
		DIVIDE                       	;EA4: D9 	
						;Result left in M(1017) Tens
						;		M(1015) Units
		GHI	R4                       ;EA5: 94 	
		ORI	 4                         ;EA6: F9  4 	
		PHI	R4	;Set bit R4.1.2
		LBR	LF84                       ;EA9: C0  F 84	
		
EX_KEY_F8_EAC	GHI	R4                       ;EAC: 94 	
		ANI	 8	;Test bit R4.1.3 
		LBZ	UPDATE_LED
		
		LDI	15h	;RD=1015           ;EB2: F8 15 	
		PLO	RD                       ;EB4: AD 	
		LDI	 0	;RE=1000 ACC       ;EB5: F8  0 	
		PLO	RE                       ;EB7: AE 	
		LDA	RD	;D=M(1015), RD=1016  ;EB8: 4D 	
		ORI	0F0h                       ;EB9: F9 F0 	
		STR	RE	;M(1000)=D OR F0   ;EBB: 5E    (SIXTEENTHS)	
		INC	RD	;RD=1017           ;EBC: 1D 	
		INC	RE	;RE=1001           ;EBD: 1E 	
		LDN	RD	;D=M(1017)         ;EBE:  D 	
		BZ	SKIP_ZERO_EC4              ;EBF: 32 C4 	
		ORI	0F0h                       ;EC1: F9 F0 	
		STR	RE	;M(1001)=D OR F0   ;EC3: 5E 	(INCHES)
SKIP_ZERO_EC4	ORI	0E0h                       ;EC4: F9 E0 	
		STR	RE	;M(1001)=D OR E0 (IF D=0)   ;EC6: 5E 	
		INC	RE	;RE=1002           ;EC7: 1E 	
		LDI	0E0h                       ;EC8: F8 E0 	
		STR	RE	;M(1002)=E0 FU     ;ECA: 5E 	(FEET UNITS)
		INC	RE	;RE=1003           ;ECB: 1E 	
		STR	RE	;M(1003)=E0 FT     ;ECC: 5E 	(FEET TENS) 	
		INC	RE	;RE=1004           ;ECD: 1E 	
		STR	RE	;M(1004)=E0 FH     ;ECE: 5E 	(FEET HUNDREDS)
		LDI	 0                         ;ECF: F8  0 	
		INC	RE	;RE=1005           ;ED1: 1E 	
		STR	RE    ;M(1005)=0         ;ED2: 5E 	(OVERFLOW)
		INC	RE	;RE=1006           ;ED3: 1E 	
		STR	RE    ;M(1006)=0         ;ED4: 5E 	(SIGN)
		GHI	R4                       ;ED5: 94 	
		ANI	0F7h                         ;ED6: FA F7 	
		PHI	R4	;Clear bit R4.1.3
		LBR	PRINT_RE                       ;ED9: C0  8 61
	
				;PATCHED CODE, after key has been shifted into acc, clear this bit and update the led display	
KS_RETURN_PATCH	GLO	R4                       ;EDC: 84 	
		ANI	07Fh                       ;EDD: FA 7F 	
		PLO	R4	;Clear bit R4.0.7 Prevents re-entry of ADDITION and SUBTRACTION Routines until a new Number key has been entered
		LBR	UPDATE_LED

				;NO OVERFLOW
EX_KEY_ADD_OK	GLO	R4                       ;EE3: 84 	
		ANI	80h	;Test bit R4.0.7 Prevents re-entry of ADDITION and SUBTRACTION Routines until a new Number key has been entered
		LBNZ	UPDATE_LED
		GLO	R4                       ;EE9: 84 	
		ORI	80h                        ;EEA: F9 80 	
		PLO	R4	;Set bit R4.0.7
		LBR	PRINT_PLUS                   ;EED: C0  8  0  PRINT +_
		
				;NO OVERFLOW
EX_KEY_SUB_OK	GLO	R4                       ;EF0: 84 	
		ANI	80h	;Test bit R4.0.7 Prevents re-entry of ADDITION and SUBTRACTION Routines until a new Number key has been entered
		LBNZ	UPDATE_LED
		GLO	R4                       ;EF6: 84 	
		ORI	80h                        ;EF7: F9 80 	
		PLO	R4	;Set bit R4.0.7
		LBR	PRINT_MINUS                   ;EFA: C0  8  E PRINT -_

;M103D	STACK FOR D
;M103C	STACK FOR XP

ISR_SUB		NOP                                ;EFD: C4 	
		STR	R2	;Save D to Stack @103D
		DEC	R2                       ;EFF: 22 	
		SAV		;T -> M(RX) @103C
		GLO	R4
		ORI	40h
		PLO	R4	;Set Bit R4.0.6 in R4.0 to indicate Printer NOT Reader
		
		INC	R2                       ;F05: 12 	
		LDN	R2	;Restore D from Stack
		DEC	R2                       ;F07: 22 	
		DIS		;RETURN WITH INTERRUPT DISABLED

		ANI	0EFh			;DEAD CODE ??  or is it called on SEP R1 ?
		PLO	R4    ;Clear bit R4.0.4
		RET                             ;If called, how is R1 re-initialized?

EX_KEY_ADD_PT	LDI	 5                         ;F0D: F8  5 	
		PLO	RD	;RD=1005 OVER FLOW?
		LDN	RD                       ;F10:  D 	
		BNZ	OVERFLOW_ERROR	;Jump over flow
		LDI	0Ch                         ;F13: F8  C 	
		PLO	RD	;RD=100C
		LDN	RD                       ;F16:  D 	
		BNZ	OVERFLOW_ERROR	;Jump over flow  ;F17: 3A 1C 	
		LBR	EX_KEY_ADD_OK             ;F19: C0  E E3	



OVERFLOW_ERROR	LDI	10h	;RF=101F (TEMP FOR OUTPUT)  ;F1C: F8 10 	
		PHI	RF                       ;F1E: BF 	
		LDI	1Fh                         ;F1F: F8 1F 	
		PLO	RF                       ;F21: AF 	
		SEX	RF	;X=F                ;F22: EF 	
		REQ                                ;F23: 7A 	
		
				;BLANK LED DISPLAY
		SEQ                                ;F24: 7B 	
		OUT	7                     ;F25: 67 	
		REQ                                ;F26: 7A 	
		DEC	RF                       ;F27: 2F 	
		LDI	0Fh                        ;F28: F8  F 	
		STR	RF                       ;F2A: 5F 	
		OUT	7                     ;F2B: 67 	
		DEC	RF                       ;F2C: 2F 	
		OUT	7                     ;F2D: 67 	
		DEC	RF                       ;F2E: 2F 	
		OUT	7                     ;F2F: 67 	
		DEC	RF                       ;F30: 2F 	
		OUT	7                     ;F31: 67 	
		DEC	RF                       ;F32: 2F 	
		OUT	7                     ;F33: 67 	
		DEC	RF                       ;F34: 2F 	
		OUT	7                     ;F35: 67 	
		DEC	RF                       ;F36: 2F 	
		OUT	7                     ;F37: 67 	
		DEC	RF                       ;F38: 2F 	
		LDI	0Bh                        ;F39: F8  B 	
		STR	RF                       ;F3B: 5F 	
		OUT	7                     ;F3C: 67 	
		DEC	RF                       ;F3D: 2F 	
					
					;LOOP HERE UNTIL CLEAR ENTRY IS PRESSED
WAIT_KEY_CE	SEQ                                ;F3E: 7B 	
		B3	DO_CLEAR_P                 ;F3F: 36 44 	
		REQ                                ;F41: 7A 	
		BR	WAIT_KEY_CE                ;F42: 30 3E 	
DO_CLEAR_P	LBR	DO_CLEAR                   ;F44: C0  B 27	

		
ADD_OPR_ACC_PT	LDI	 5	;RD=1005           ;F47: F8  5 	
		PLO	RD                       ;F49: AD 	
		LDN	RD                       ;F4A:  D 	
		BNZ	OVERFLOW_ERROR             ;F4B: 3A 1C 	
		LDI	0Ch	;RD=100C           ;F4D: F8  C 	
		PLO	RD                       ;F4F: AD 	
		LDN	RD                       ;F50:  D 	
		BNZ	OVERFLOW_ERROR             ;F51: 3A 1C 	
		LBR	ADD_OPR_ACC                ;F53: C0  2 A8	



LF56		LDI	1Ch	;RD=101C           ;F56: F8 1C 	RD - 0003E7h  (999)
		PLO	RD                       ;F58: AD 	M(101A):M(101B):M(101C) - 0003E7h  (999)
		LDN	RD                       ;F59:  D 	
		DEC	RD                       ;F5A: 2D 	
		SDI	0E7h                     ;F5B: FD E7 	
		LDN	RD                       ;F5D:  D 	
		DEC	RD                       ;F5E: 2D 	
		SDBI	3                        ;F5F: 7D  3 	
		LDN	RD                       ;F61:  D 	
		SDBI	0                        ;F62: 7D  0 	
		LBNF	OVERFLOW_ERROR             ;F64: CB  F 1C  JUMP if >999
		LDI	17h	;RD=1017           ;F67: F8 17 	
		PLO	RD                       ;F69: AD 	
		LBR	LD4E                       ;F6A: C0  D 4E
		
		.DB	0,0,0
		
		
;
;SHORT KEY SCAN OF 2 KEYS ONLY
;
SHORT_SCAN	OUT	6	;OUT6 = JUNK (must strobe something to be read on EF1/EF3)
		B1	GOP_KEYF8                         ;F71: 34 7E 	
		SEQ                                ;F73: 7B 	
		B3	GOP_KEYF11                      ;F74: 36 81 	
		REQ                                ;F76: 7A 	
		GHI	RA                       ;F77: 9A 	
		ANI	0F7h                         ;F78: FA F7 	
		PHI	RA	;Clear bit RA.1.3
		NOP                                ;F7B: C4 	
		BR	SHORT_SCAN                    ;F7C: 30 70 	
GOP_KEYF8	LBR	EX_KEY_F8                     ;F7E: C0  6 63	
GOP_KEYF11	LBR	DO_CLEAR                     ;F81: C0  B 27	


LF84		GLO	R4                         ;F84: 84 	
		ANI	0FEh                         ;F85: FA FE 	
		PLO	R4	;Clear Bit R4.0.0
		GHI	RA                       ;F88: 9A 	
		ANI	0B5h                         ;F89: FA B5 	
		PHI	RA	;Clear bits RA.1.1 , RA.1.3 , RA.1.16
		LBR	PRINT___                       ;F8C: C0  8 53	
		
		.DB	0,0
		
EX_KEY_SUB_PT	LDI	 5	;RD=1005           ;F91: F8  5 	
		PLO	RD                       ;F93: AD 	
		LDN	RD                       ;F94:  D 	
		BNZ	OVERFLOW_ERROR	;Jump on over flow ;F95: 3A 1C 	
		LDI	0Ch     ;RD=100C           ;F97: F8  C 	
		PLO	RD                       ;F99: AD 	
		LDN	RD                       ;F9A:  D 	
		BNZ	OVERFLOW_ERROR ;Jump on over flow ;F9B: 3A 1C 	
		LBR	EX_KEY_SUB_OK              ;F9D: C0  E F0	
		
EX_KEY_MUL_PT	LDI	 5	;RD=1005            ;FA0: F8  5 	
		PLO	RD                       ;FA2: AD 	
		LDN	RD                       ;FA3:  D 	
		BNZ	OVERFLOW_ERROR                         ;FA4: 3A 1C 	
		LDI	0Ch	;RD=100C           ;FA6: F8  C 	
		PLO	RD                       ;FA8: AD 	
		LDN	RD                       ;FA9:  D 	
		BNZ	OVERFLOW_ERROR                         ;FAA: 3A 1C 	
		LBR	PRINT_TIMES                       ;FAC: C0  8 1C	PRINT X_

EX_KEY_DIVW_PT	LDI	 5	;RD=1005           ;FAF: F8  5 	
		PLO	RD                       ;FB1: AD 	
		LDN	RD                       ;FB2:  D 	
		BNZ	OVERFLOW_ERROR                         ;FB3: 3A 1C 	
		LDI	0Ch                         ;FB5: F8  C 	
		PLO	RD                       ;FB7: AD 	
		LDN	RD                       ;FB8:  D 	
		BNZ	OVERFLOW_ERROR                         ;FB9: 3A 1C 	
		LBR	EX_KEY_DIVW_P               ;FBB: C0  A EE	

				;Memory store		
MEM_STORE_PT	LDI	 5                         ;FBE: F8  5 	
		PLO	RD	;RD=1005 (ACC) Check over flow     ;FC0: AD 	
		LDN	RD                       ;FC1:  D 	
		BNZ	OVERFLOW_ERROR            ;FC2: 3A 1C 	
		LDI	0Ch	;RD=100C           ;FC4: F8  C 	
		PLO	RD                       ;FC6: AD 	
		LDN	RD                       ;FC7:  D 	
		BNZ	OVERFLOW_ERROR            ;FC8: 3A 1C 	
LFCA		LBR	MEM_STORE_GKEY             ;FCA: C0  7 1D	
		
		.DB	0,0,0			;FILL END OF ROM WITH 0's
		.DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		.DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

		.END
		