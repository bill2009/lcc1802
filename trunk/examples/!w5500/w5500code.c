//14-11-26 routines to address the wiznet w5500
//14-11-27 switched to single read & write routines
void wizWrite(unsigned int addr, unsigned char opcode, /*unsigned char*/ void * data, unsigned int len){
//variable length write to the wiznet W5500 common or socketN areas
//opcode is xxxx x100
// xxxxx is 00000 for common area, 00001 for socket 0 register, 00010 for socket 0 tx buffer
  enablewiz();   			// Activate the CS pin
  spiSendN((unsigned char *)&addr,2);
  spiSend(opcode);   		// Send Wiznet Write OpCode
  spiSendN(data,len);		//Send the payload
  disablewiz();				// make CS pin not active
}

void wizRead(unsigned int addr,unsigned char opcode, unsigned char * data, unsigned int len){
//variable length read from the wiznet w5500 common or socketN areas
//opcode is xxxx x000
// xxxxx is 00000 for common area, 00001 for socket 0 register, 00011 for socket 0 rx buffer
  enablewiz();   			// Activate the CS pin
  spiSendN((unsigned char *)&addr,2);
  spiSend(opcode);   		// Send Wiznet Read OpCode
  spiReceiveN(data,len);	// Send the data byte
  disablewiz();				// make CS pin not active
}
void wizCmd(unsigned char cmd){ //send a command to socket 0 and wait for completion
	wizWrite(SnCR,WIZNET_WRITE_S0R,&cmd,1); //send the command
	while(wizRead(SnCR,WIZNET_READ_S0R,&cmd,1),cmd); //wait til command completes
}
void wizSetCtl8(unsigned int ctlreg, unsigned char val){//write to a socket 0 control register
	wizWrite(ctlreg, WIZNET_WRITE_S0R,&val,1);
}
void wiz_Init(unsigned char ip_addr[]){// Ethernet Setup
  unsigned char mac_addr[] = {0xDE, 0xAD, 0xBE, 0xE5, 0xFE, 0xED};
  unsigned char sub_mask[] = {255,255,255,0};
  unsigned char gtw_addr[] = {192,168,0,1};
  unsigned char readback_ip[] = {1,2,3,4};
  unsigned char bsz0=0, bsz2=2, bsz4=4;	//
  wizWrite(GAR,WIZNET_WRITE_COMMON,gtw_addr,4);//set the wiznet gateway address register
  delay(1);
  wizWrite(SHAR,WIZNET_WRITE_COMMON,mac_addr,6);// Set the MAC address - Source Address Register
  delay(1);
  wizWrite(SUBR,WIZNET_WRITE_COMMON,sub_mask,4);// Set the Wiznet W5100 Sub Mask Address
  delay(1);
  wizWrite(SIPR,WIZNET_WRITE_COMMON,ip_addr,4);// Set the Wiznet W5100 IP Address
  delay(1);
  wizWrite(SnRX_BSZ,WIZNET_WRITE_S0R,&bsz4,1); //set receive buffer for soscket 0 to 4K
  wizWrite(SnTX_BSZ,WIZNET_WRITE_S0R,&bsz4,1); //set transmit buffer for soscket 0 to 4K
  wizWrite(SnRX_BSZ,WIZNET_WRITE_S7R,&bsz0,1); //set receive buffer for soscket 7 to 0K
  wizWrite(SnTX_BSZ,WIZNET_WRITE_S7R,&bsz0,1); //set transmit buffer for soscket 7 to 0K

  wizRead(SIPR,WIZNET_READ_COMMON,readback_ip,4); //read back the IP to make sure it "took"
  printf("Done Wiznet W5500 Initialization on IP address %d.%d.%d.%d\n\n",readback_ip[0],readback_ip[1],readback_ip[2],readback_ip[3]);
}
void socket0_init(){ //initialize socket 0 for http server
	unsigned char tcpmode=MR_TCP, cropen=CR_OPEN, crlisten=CR_LISTEN, intclr=255, crreadback=255,crclose=CR_CLOSE;
	unsigned char tcpport[]={0,80}; //port 80 for tcp
	wizCmd(CR_CLOSE); //make sure port is closed
	wizSetCtl8(SnIR,0xFF); //reset interrupt reg
	wizSetCtl8(SnMR,MR_TCP); //set mode register to tcp
	wizWrite(SnPORT,WIZNET_WRITE_S0R,tcpport,2);	//set tcp port to 0050
	wizCmd(CR_OPEN); //open the port
	wizCmd(CR_LISTEN); //listen for a conection
}


long getip(){ //retrieve the requester's ip and return it as a long
	union IPaddr thisip;
	wizRead(SnDIPR,WIZNET_READ_S0R,thisip.c,4);
	return thisip.l;
}

unsigned int recv_size(void){
  union WReg rsz; //spot to hold the received size
  wizRead(SnRX_RSR,WIZNET_READ_S0R,rsz.c,2);
  return rsz.i;
}
unsigned int txfree_size(void){
  union WReg fsz; //spot to hold the free size
  wizRead(SnTX_FSR,WIZNET_READ_S0R,fsz.c,2);
  return fsz.i;
}
unsigned int txwr_loc(void){
  union WReg txwr; //spot to hold the buffer pointer
  wizRead(SnTX_WR,WIZNET_READ_S0R,txwr.c,2);
  return txwr.i;
}
unsigned int wizGetCtl16(unsigned int ctlregaddr){
  union WReg regval; //spot to hold the register contents
  wizRead(ctlregaddr,WIZNET_READ_S0R,regval.c,2);
  return regval.i;
}
void wizSetCtl16(unsigned int ctlregaddr, unsigned int ctlregvalue){
  union WReg regval; //spot to hold the register contents
  regval.i=ctlregvalue;
  wizRead(ctlregaddr,WIZNET_READ_S0R,regval.c,2);
}


#define sendlit(x) send0((unsigned char*)x,sizeof(x)-1)
#define sendconst(x) send0((unsigned char*)x,sizeof(x)-1)

unsigned int send0(unsigned char *buf,unsigned int buflen){
    unsigned int timeout,txsize,txfree;
    unsigned char crsend=CR_SEND,crreadback;
	union WReg txwr;
	//printf("send0 %d\n",buflen);
    if (buflen <= 0) return 0;
    // Make sure the TX Free Size Register shows enough room
    txfree=txfree_size();
	//printf("free %d\n",txfree);
    timeout=0;
    while (txfree < buflen) {
      delay(1);
     txfree=txfree_size();
     // Timeout for approx 1000 ms
     if (timeout++ > 1000) {
       printf("TX Free Size Error!\n");
       // Disconnect the connection
		wizCmd(CR_DISCON);
//       disconnect0();
       return 0;
     }
   }


   txwr.i=wizGetCtl16(SnTX_WR); //txwr_loc(); // Read the Tx Write Pointer
   wizWrite(txwr.i,WIZNET_WRITE_S0TX,buf, buflen);
   txwr.i+=buflen;
   wizWrite(SnTX_WR,WIZNET_WRITE_S0R,txwr.c,2);

	wizCmd(CR_SEND); // Now Send the SEND command

    return 1;
}
void sendnak(){
	sendlit("HTTP/1.1 404 Not Found\r\n\r\n"); 	// Now Send the HTTP Response
}

void sendack(){
	sendlit("HTTP/1.0 200 OK\r\n\r\n"); 	// Now Send the HTTP Response
}
unsigned int rxrd_loc(void){
  union WReg rxrd; //spot to hold the buffer pointer
  wizRead(SnRX_RD,WIZNET_READ_S0R,rxrd.c,2);
  return rxrd.i;
}


unsigned int recv0(unsigned char *buf,unsigned int buflen){
    union WReg rxrd;
    unsigned char crreadback,crrecv=CR_RECV;

    if (buflen <= 0) return 1;

    if (buflen > MAX_BUF)	// If the request size > MAX_BUF,just truncate it
        buflen=MAX_BUF - 2;
    rxrd.i = rxrd_loc();     // Read the Rx Read Pointer
	wizRead(rxrd.i,WIZNET_READ_S0RX,buf,buflen); //read the data
    *(buf+buflen)='\0';        // terminate string
    wizWrite(SnRX_RD,WIZNET_WRITE_S0R,rxrd.c,2); //update the read pointer

    // Now Send the RECEIVE command to update the buffer pointer
	wizCmd(CR_RECV);
    return 1;
}

void flush(unsigned int rsize){ //this just gets rid of data that i don't want to process
	union WReg rxrd;
    unsigned char crreadback,crrecv=CR_RECV;
	if (rsize>0){
  		wizRead(SnRX_RD,WIZNET_READ_S0R,rxrd.c,2); //retrieve read data pointer
  		rxrd.i+=rsize; //update receive buffer pointer
  		wizWrite(SnRX_RD,WIZNET_WRITE_S0R,rxrd.c,2); //replace read data pointer
		wizCmd(CR_RECV);

	}
}
