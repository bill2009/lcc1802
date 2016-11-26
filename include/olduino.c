//olduino.c
//Feb 7 2013, changed digitalwrite to out4 instead of out5
//Feb 26 added digitalRead on ef3
//Nov 9 2014 added oneMsBN to soak up one ms adjusted for 4mhz clock
//16-11-26 removed OneMsBN in favour of oneMs in epilog
//cpu speed is set by LCC1802CPUSPEED assembler variable
void delay(unsigned int howlong){
	unsigned int i;
	for (i=1;i!=howlong;i++){
		oneMs();
	}
}
void olduinoincluder(){
	asm("\tinclude olduino.inc\n");
#ifdef __CPUSPEED4
	asm("LCC1802CPUSPEED set 4000000"); //for 4mhz 1802
#endif
}
