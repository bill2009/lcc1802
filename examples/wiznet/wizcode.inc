;void wiz_WriteNf(unsigned int offaddr,unsigned char *data, unsigned int N)
  ;Here I have the offset address in 12 and the data address in 13
  ;the count to transfer is at 16 bytes off the stack pointer
	lda2 R8,_SPINpayloadf 	;R8 will point to the payload area
  	lda2 memaddr,_PIN4	;address of parallel port state
  	ldn memaddr		;parallel port content
  	ani 0x7f		;set 
  	str R8\n"	//store it ahead of payload
  	inc R8\n	inc R8\n	inc R8\n	inc R8\n	inc R8\n" //step past payload
  	ori 0x80\n	str memaddr\n"				//store again with SS off
  	dec R8\n	dec R8\n	dec R8\n	dec R8\n"	//point to the opcode
  	ldi _WIZNET_WRITE_OPCODE\n	str	R8\n"	//store the write opcode
	dec R8\n");	//R8 now points to the work area
  asm("	ld2 R9,'O',sp,(16)\n");	//pick up the count parameter
  //at this point the work area is prepped and the count is in R9
  ;while(N>0){//for all the bytes to be transferred
  asm("	lbr $$whiletest\n");
  	asm("	sex R8\n"	//prepare for I/O from the work area
  	SPINpayload.realaddr=TXBUFADDR+(offaddr&TX_BUF_MASK);
  	SPINpayload.data=*data;
  	//p4hex(&SPINpayload.opcode);
  	enablewiz();   			// Activate the CS pin
  	//spiSendN(&SPINpayload.opcode,4);
  	asm("	lda2 memaddr,
  	disablewiz();				// make CS pin not active
  	offaddr++;data++;N--;
  }
}
