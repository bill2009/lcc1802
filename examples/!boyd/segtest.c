//segtest.c testing segment drive on the Boyd LEDS
unsigned char test[]={15,15,15,15,5,0,8,1};
void main()
{
	asm(" req\n seq\n"
		" sex 3\n out 7\n db 0b11010000\n req\n" //Data coming,NA,no decode, no shutdown
		" ldad r11,_test\n"
		" ldad r10,8\n"
		" sex 11\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" out 7\n"
		" br $\n");
}

//#include "olduino.c" //for the delay routine
