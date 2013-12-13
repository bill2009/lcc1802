//hspi.c - routines for hardware spi on olduino
unsigned char spixfer(unsigned char value){
	//this code depends on the argument being in Reg 12 and returned value in reg 15
	asm("	glo 12\n" //get the char to be sent
		"	dec 2\n"  //make a work area
		"	str 2\n"  //save the character
		"	out 6\n"  //this loads the shift register and starts the outboard clock
		"	dec 2\n"
		"	sex 2\n"  //delay to allow outbound shift to complete
		"	inp 6\n"  //read the shift register
		"	plo 15\n" //leave it in 15
		"	inc 2\n"  //restore the stack
		"	cretn\n");
	//warning the return below is NOT executed. It just prevents a compiler warning
	//the cretn inside the asm block above returns the correct value from the spi transfer
	//sorry.
	return 0;
}
void spisend(unsigned char value){ //this is for output only
	//this code depends on the argument being in Reg 12
	asm("	glo 12\n" //get the char to send
		"	dec 2\n"  //make a work area
		"	str 2\n"  //place the outbound char
		"	out 6\n"); //this loads the MOSR and starts the outboard clock
	//there needs to be 2 instructions before the next spi accesee - return is 12 or so
}
void spiReceiveN(unsigned char * buffer, unsigned int n){//receive n bytes over spi
		asm("	sex 12\n" //point X to the buffer
			"	dec 12\n" //back off so the first OUT will leave us in the 1st position
			"	lbr $$spiSendLoop\n" //branch around any alignment gap
			"	align 16\n" //make sure jumps are on page
			"$$spiSendLoop:\n" //we will do this N times
			"	out 6\n"  //this dends out garbage and clocks in the 1st character
			"	dec 13\n" //decrement the byte count
			"	sex 12\n" //extra instruction to allow the shift to finish
			"	inp 6\n"  //this reads the nth byte into the nth buffer location
			"	glo 13\n" //check bottom byte of counter
			"	bnz $$spiSendLoop\n" //back for more if needed
			"	ghi 13\n" //check high byte of counter if necessary
			"	bnz $$spiSendLoop\n"
			"	sex 2\n"  //reset X register
		);
} //that's it

