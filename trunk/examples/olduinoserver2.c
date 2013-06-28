
/*****************************************************************************
//  File Name    : olduinoserver.c
//  Version      : 2.0
//  Description  : olduino web server
//  Author       : WJR with thanks to RWB
//  Target       : Olduino
//  Compiler     : LCC1802
//  IDE          : TextPad
//  Programmer   : Olduino bootloader via avrdude
//  Adaptated    : 17 May 2013 by Bill Rowe - WJR for the olduino platform
*****************************************************************************/
#include <nstdlib.h> //for printf etc.
#include <olduino.h> //for digitalRead, digitalWrite, delay
#include <fastspi.h> //fastspi header
#include "wiznet.h"  //wiznet definitions

#define disablewiz() asm("\tseq\n"); //high level on Q disables the w5100 from the spi bus
#define enablewiz()  asm("\treq\n");

#define MAX_BUF 512
unsigned char buf[MAX_BUF];			//memory buffer for incoming & outgoing data
int ledmode=3;	//0=off, 1=on, else=no change
enum reqtypes {get,post,favicon,unknown};
int reqtype=unknown;
int cmdip=182; //last part of most recent address where a command came from

void SPI_Write(unsigned int addr,unsigned char data)
{
  enablewiz();   			// Activate the CS pin
  xferspif2(WIZNET_WRITE_OPCODE);   // Send Wiznet W5100 Write OpCode
  xferspif2(addr >>8); 		// Send Wiznet W5100 Address High Byte
  xferspif2(addr & 0x00FF);	// Send Wiznet W5100 Address Low Byte
  xferspif2(data);			// Send the data byte
  disablewiz();				// make CS pin not active
}
unsigned char SPI_Read(unsigned int addr)
{
  unsigned char value; 	//data returned from spi transmission
  enablewiz();   		// Activate the CS pin
  xferspif2(WIZNET_READ_OPCODE);   // Send Wiznet W5100 Write OpCode
  xferspif2(addr >>8);	// Send Wiznet W5100 Address High Byte
  xferspif2(addr & 0x00FF);  // Send Wiznet W5100 Address Low Byte
  value=xferspif2(0x00);	// Send Dummy transmission to read the data
  disablewiz();			// make CS pin inactive
  return(value);
}

void W5100_Init(void){// Ethernet Setup
  unsigned char mac_addr[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  unsigned char ip_addr[] = {192,168,1,182};
  unsigned char sub_mask[] = {255,255,255,0};
  unsigned char gtw_addr[] = {192,168,1,1};
  SPI_Write(MR,0x80);   // setting bit 7 of the mode register does a software reset of the w5100
  delay(1);
  //set the wiznet gateway address register
  SPI_Write(GAR + 0,gtw_addr[0]); SPI_Write(GAR + 1,gtw_addr[1]);
  SPI_Write(GAR + 2,gtw_addr[2]); SPI_Write(GAR + 3,gtw_addr[3]);
  delay(1);
  // Set the Wiznet W5100 MAC address - Source Address Register (SAR): 0x0009 to 0x000E
  SPI_Write(SAR + 0,mac_addr[0]); SPI_Write(SAR + 1,mac_addr[1]);
  SPI_Write(SAR + 2,mac_addr[2]); SPI_Write(SAR + 3,mac_addr[3]);
  SPI_Write(SAR + 4,mac_addr[4]); SPI_Write(SAR + 5,mac_addr[5]);
  delay(1);
  // Set the Wiznet W5100 Sub Mask Address (SUBR): 0x0005 to 0x0008
  SPI_Write(SUBR + 0,sub_mask[0]); SPI_Write(SUBR + 1,sub_mask[1]);
  SPI_Write(SUBR + 2,sub_mask[2]); SPI_Write(SUBR + 3,sub_mask[3]);
  delay(1);
  // Set the Wiznet W5100 IP Address (SIPR): 0x000F to 0x0012
  SPI_Write(SIPR + 0,ip_addr[0]); SPI_Write(SIPR + 1,ip_addr[1]);
  SPI_Write(SIPR + 2,ip_addr[2]); SPI_Write(SIPR + 3,ip_addr[3]);
  delay(1);
  // Set the Wiznet W5100 RX and TX Memory Size, we use 2KB for Rx/Tx 4 channels
  SPI_Write(RMSR,0x55);
  SPI_Write(TMSR,0x55);
  printf("Done Wiznet W5100 Initialization on IP address %d.%d.%d.%d\n\n",SPI_Read(SIPR + 0),SPI_Read(SIPR + 1),SPI_Read(SIPR + 2),SPI_Read(SIPR + 3));
}

void socket0_init(){ //initialize socket 0 for http server
	SPI_Write(S0_MR,MR_TCP);	//set mode register to tcp
	SPI_Write(S0_PORT,((TCP_PORT & 0xFF00) >> 8 ));	//set tcp port to 0050
	SPI_Write(S0_PORT + 1,(TCP_PORT & 0x00FF));
	SPI_Write(S0_CR,CR_OPEN);                   // Open Socket
	delay(10);
    SPI_Write(S0_CR,CR_LISTEN);					//listen to socket
}

void disconnect0(){
   SPI_Write(S0_CR,CR_DISCON); 	//send disconnect command
   while(SPI_Read(S0_CR));		// Wait for Disconecting Process
}

void close0(){

   SPI_Write(S0_CR,CR_CLOSE);    // Send Close Command
   while(SPI_Read(S0_CR));   	// Wait until the S0_CR is clear
}

unsigned int recv0(unsigned char *buf,unsigned int buflen){
    unsigned int ptr,offaddr,realaddr;

    if (buflen <= 0) return 1;

    if (buflen > MAX_BUF)	// If the request size > MAX_BUF,just truncate it
        buflen=MAX_BUF - 2;
    ptr = SPI_Read(S0_RX_RD);     // Read the Rx Read Pointer
    offaddr = (((ptr & 0x00FF) << 8 ) + SPI_Read(S0_RX_RD + 1));

    while(buflen) {//receive individual bytes into the buffer
      buflen--;
      realaddr=RXBUFADDR + (offaddr & RX_BUF_MASK);
      *buf = SPI_Read(realaddr);
      offaddr++;
      buf++;
    }
    *buf='\0';        // terminate string

    // Increase the S0_RX_RD value, so it point to the next receive
    SPI_Write(S0_RX_RD,(offaddr >> 8) );
    SPI_Write(S0_RX_RD + 1,(offaddr & 0x00FF));

    SPI_Write(S0_CR,CR_RECV);	// Now Send the RECV command
    delay(5);    				// Wait for Receive Process

    return 1;
}
unsigned int recv_size(void){
  return ((SPI_Read(S0_RX_RSR) & 0x00FF) << 8 ) + SPI_Read(S0_RX_RSR + 1);
}

unsigned int send0(unsigned char *buf,unsigned int buflen){
    unsigned int ptr,offaddr,realaddr,txsize,timeout;

    if (buflen <= 0) return 0;
    // Make sure the TX Free Size Register is available
    txsize=SPI_Read(SO_TX_FSR);
    txsize=(((txsize & 0x00FF) << 8 ) + SPI_Read(SO_TX_FSR + 1));
#if _DEBUG_MODE
    printf("Send Size: %d\n",buflen);
    printf("TX Free Size: %d\n",txsize);
#endif
    timeout=0;
    while (txsize < buflen) {
      delay(1);
     txsize=SPI_Read(SO_TX_FSR);
     txsize=(((txsize & 0x00FF) << 8 ) + SPI_Read(SO_TX_FSR + 1));
     // Timeout for approx 1000 ms
     if (timeout++ > 1000) {
#if _DEBUG_MODE
       printf("TX Free Size Error!\n");
#endif
       // Disconnect the connection
       disconnect0();
       return 0;
     }
   }

   // Read the Tx Write Pointer
   ptr = SPI_Read(S0_TX_WR);
   offaddr = (((ptr & 0x00FF) << 8 ) + SPI_Read(S0_TX_WR + 1));

    while(buflen) {
      buflen--;
      // Calculate the real W5100 physical Tx Buffer Address
      realaddr = TXBUFADDR + (offaddr & TX_BUF_MASK);
      // Copy the application data to the W5100 Tx Buffer
      SPI_Write(realaddr,*buf);
      offaddr++;
      buf++;
    }

    // Increase the S0_TX_WR value, so it point to the next transmit
    SPI_Write(S0_TX_WR,(offaddr & 0xFF00) >> 8 );
    SPI_Write(S0_TX_WR + 1,(offaddr & 0x00FF));

    // Now Send the SEND command
    SPI_Write(S0_CR,CR_SEND);

    // Wait for Sending Process
    while(SPI_Read(S0_CR));

    return 1;
}

void report(char * where){
	//return;
	printf(" at %s: wiznet MR=%x, S0_SR=%x, S0_MR=%x, S0_PORT=%x %x\n", where, SPI_Read(MR), SPI_Read(S0_SR), SPI_Read(S0_MR), SPI_Read(S0_PORT), SPI_Read(S0_PORT+1));
}
void sendack(){
	int sendrc;
	static unsigned char ack[]="HTTP/1.0 200 OK\r\n\r\n";
	sendrc=send0(ack,strlen((char *)ack)); 	// Now Send the HTTP Response
}
int send0s(unsigned char* what){
	return send0(what,strlen((char *)what));
}
void sendstuff(){
	int sendrc;
	static unsigned char hdr[]="HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"
						"<html><body><span style=\"color:#0000A0\">\r\n"
						"<h1><center>Olduino 1802 Web Server</center></h1>\r\n"
						"<p><form method=\"POST\">\r\n";
	static unsigned char rpart1[]="<p><strong>"
						"<input type=\"radio\" name=\"rd\" value=\"0\" ";
	static unsigned char rpart2[]=">LED OFF\r\n"
						"<br><input type=\"radio\" name=\"rd\" value=\"1\" ";
	static unsigned char trlr[]=">LED ON\r\n"
						"</strong><p>\r\n"
						"<input type=\"submit\">\r\n"
						"</form></span></body></html>\r\n";
	static unsigned char checked[]="checked";
	static unsigned char unchecked[]=" ";

	printf("sendstuff sees ledmode=%d\n",ledmode);
	sendrc=send0s(hdr); 	// Now Send the HTTP Response first part
	sendrc=send0s(rpart1); 	// Now Send the radio button 1st part
	if (ledmode==0) send0s(checked); else send0s(unchecked);
	sendrc=send0s(rpart2); 	// Now Send the radio button 2nd part
	if (ledmode==1) send0s(checked); else send0s(unchecked);
	sendrc=send0s(trlr); 	// Now Send the rest of the page
}
int strindex(char *s,char *t)
{
  unsigned int i,n;

  n=strlen(t);
  for(i=0;*(s+i); i++) {
    if (strncmp(s+i,t,n) == 0)
      return i;
  }
  return -1;
}
void analyzeinput(){
	int reqmode=3; //mode request zero=off, 1=on, else =no request
	printf("input from %d.%d.%d.%d\n\n",SPI_Read(S0_DIPR + 0),SPI_Read(S0_DIPR + 1),SPI_Read(S0_DIPR + 2),SPI_Read(S0_DIPR + 3));
	printf("analyzing:\n%s\n",buf);
  	if (strindex((char *)buf,"POST /")>=0) reqtype=post;
	if (strindex((char *)buf,"GET /")>=0) reqtype=get;
	if (strindex((char *)buf,"GET /favicon")>=0) reqtype=favicon;

	if (reqtype==post){
	    printf("POST received\n");
		// Now check the Radio Button for POST request
	  if (strindex((char *)buf,"rd=0") > 0)
		reqmode=0;
	  if (strindex((char *)buf,"rd=1") > 0)
		reqmode=1;
	}
	else if (reqtype==get)
		printf("GET received.\n");
	else if (reqtype==favicon)
		printf("favicon ignored\n");
	else
		printf("unknown input\n");

	if (reqmode==0){
		printf("setting led off\n");
		ledmode=0;
		digitalWrite(0,LOW);
	} else{
		if (reqmode==1){
			printf("setting led ON\n");
			ledmode=1;
			digitalWrite(0,HIGH);
		}else{
			printf("No led action request\n");
		}
	}
}

void sendresponse(){
	switch (reqtype){
		case get: case post:
			printf("sending form \n");
			sendstuff();
			break;
		case favicon:
			printf("sending ack\n");
			sendack();
			break;
	}
}
void handlesession(){	//handle a session once it's established
	unsigned int rsize;
	rsize=recv_size();
	printf("rsz=%d\n",rsize);
	if (rsize>0){
		if (recv0(buf,rsize)>0){
			analyzeinput();
			sendresponse();
			disconnect0();
		}
	}
}


void server_loop(){
  	int socketstatus;
  	socketstatus=SPI_Read(S0_SR);
  	switch (socketstatus){
	  	case SOCK_CLOSED: //initial condition
	  	printf("SC\n");
			socket0_init();	//initialize socket 0
	  		break;
	  	case SOCK_ESTABLISHED: //someone wants to talk to the server
	  	printf("SX\n");
	  		handlesession();
	  		break;
	  	//following are cases where we have to reset and reopen the socket
      	case SOCK_FIN_WAIT: case SOCK_CLOSING: case SOCK_TIME_WAIT:
      	case SOCK_CLOSE_WAIT: case SOCK_LAST_ACK:
		  	printf("CS\n");
      		close0();
      		break;
	}
}

void main(void){
	int s0sr,rsize,recvrc,sendrc;
	ledmode=1;
	digitalWrite(0,HIGH);
	delay(1000);
	printf("\nOlduino Web Server v2.1\n");
    W5100_Init(); //initialize the wiznet chip
	//report("initialized");
	while(1){  // Loop forever
		server_loop();
		delay(100);
	}
}
#include <olduino.c>
#include <nstdlib.c>
#include "fastspi.c"
