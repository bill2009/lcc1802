//fakeloader simulates 1802 load mode in run for 1806
#define nofloats
void main(){
	asm(" 		b4 run\n"			//bypass bootloader if IN pressed
		" 		ldAD 14,0x800\n"	//starting address
		" 		sex 14\n"			//in X register
		"noEF4:	bn4 noEF4\n"		//loop til IN pressed
		"		inp 6\n"			//load memory
		"   	nop\n"
		"		out 7\n"			//and echo
		"yEF4:	b4 yEF4\n"			//wait til switch released
		"		br noEF4\n"			//back for more
		"run:	lbr 0x800\n");	//finally - off we go
}
