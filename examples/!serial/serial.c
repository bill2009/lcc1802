// bit bang serial routines in C for the 1802 MS Card
// J. Kenneke jon@kenneke.com

// This uses Q for send, and EF3 for receive. See defines.

// Assuming a clock speed of 1.75 Mhz
// Serial is 8 N 1. 8 bits no parity and 1 stop bit.
// There is always a start bit

// Right now, the fastest I can get is 600 baud. The delay and
// shift routines are slow I think. The BITDELAY value is not
// linear, so that's why I think bit shift stuff is taking a long
// time. I'll look at the assemble at some point. Really need to
// get better at reading 1802 assembly.

// I've discovered there is a bit of function overhead with LCC, which
// is expected. The Macros save time and space. It's not the best way
// to code. But when there is a 40+ year-old processor involved, if ok.

// Based on code from http://www.romanblack.com/bitbangserial.htm

//Versions:

//	JKK	10/09/16	Initial program 600 baud
//	JKK 10/09/16.2	Attemptes at making things run faster
//	JKK	10/09/16.3	Now runs at 2400 Baud

// remember, baud == bits per second
#define BITDELAY 1
// For a MS Card that's set up for the standard RS232, leave this define
//#define BITPOS // comment out if output inverted. Your hardware may do this for you
#define TESTTIMING // uncomment to send a continuous 0x55 "U" for 'scope timing

#ifdef BITPOS		// These defines just make the code more readable for the polarity option
	#define SETQ asm(" seq\n") // turns Q on
	#define CLRQ asm(" req\n") // turns Q off
#else
	#define SETQ asm(" req\n")
	#define CLRQ asm(" seq\n")
#endif

//#define DELAYUS for(g=BITDELAY;g!=0;g--) // uS delay macro
//#define DELAYUS asm(" ldi 5\n plo 14\n dec 14\n glo 14\n bnz $-2\n"); //2+5*3=17 instruction delay=68 us
#define DELAYUS asm(" nop\n");
const char *outstring = "This is a test of this system. 1234567890\n\r";

//declarations
void send_serial_byte(char data);
void main(void);
void fakeit(){
	asm(" req\n"
		" nop\n"
		" lbr $+2\n"
		" glo r14\n"
		" lbr $+2\n"
		" glo 12\n"
		" shr\n"
		" plo 12\n"
		" nop\n"
		" seq\n");
}
void main(void) {
	int i,g;

#ifdef TESTTIMING // uncomment this define if you are looking at the timing on an oscilloscope
	// sends char 0x55 continuously. That is a "U" which has a nice 01010101 bit pattern
	// remember the start and stop bits. So it's ten bits total
	while(1) {	// do forever
		fakeit();//send_serial_byte(0x55);
	}
#else
	while(1) {	// do forever
		i=0;
		while (outstring[i]) {
				send_serial_byte(outstring[i]);
				i++;
		}
		for (g=1;g!=1000;g++) oneMs(); // wait roughly a second
	}
#endif
}

//---------------------------------------------------------
void send_serial_byte(char data)
{
  	// this manually sends a serial byte out Q.
	// Apologies for all the macros. I'm saving function overhead
	// for 9600 baud at 4 mhz i need a bit time of 104us=26 instructions
  	int i = 8;  // 8 data bits to send
	int g; // for macros

  	SETQ;							// send start bit SETQ macro
	DELAYUS; //17 inst  						// wait for bit delay macro

  	while(i) //LBR=1.5 inst                     // send 8 serial bits, LSB first
  	{
  		// invert and send data bit
asm(" glo 12\n"
    " shr\n"
    " plo 12\n"
    " bnf $$nobit\n"
    " seq\n"
    " br  $$done\n"
"$$nobit: req\n"
    " nop\n"
"$$done:\n"); //6 instructions=24 us at 4 mhz
    	i--;  //1 inst
    	DELAYUS; //17 inst			// wait for baud macro
  	} //while overhead is 2.5 inst //maybe 26.5 inst
	CLRQ;					// send stop bit macro
	DELAYUS;
	DELAYUS; 				// wait a couple of bits for safety macro
}

