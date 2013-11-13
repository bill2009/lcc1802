//Feb 7 2013, changed digitalwrite to out4 instead of out5
//Feb 26 added digitalRead on ef3
void delay(unsigned int howlong){
	unsigned int i;
	for (i=1;i!=howlong;i++){
		oneMs();
	}
}
/*
void digitalWrite(unsigned char n, unsigned char hilo){ //set a bit in the output port on or off
    if (LOW==hilo){ //turn it off
        PIN4= PIN4 & ~ (1<<n); //with AND
    } else { //turn it on
        PIN4=PIN4|(1<<n); //with OR
    }
    OUT4(PIN4); //
}
*/
void olduinoincluder(){
	asm("\tinclude olduino.inc\n");
}
