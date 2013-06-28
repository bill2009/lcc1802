
/*****************************************************************************
//  File Name    : webserv.c
//  Version      : 1.0 - May 2013
//  Description  : Wiznet W5100
//  Author       : WJR with thanks to RWB
//  Target       : Olduino
//  Compiler     : LCC1802
//  IDE          : Textpad!
//  Programmer   : Olduino bootloader via avrdude
*****************************************************************************/
#include <nstdlib.h> //for printf etc.
#include <olduino.h> //for digitalRead, digitalWrite, delay
#include "wiznet.h" //wiznet defines

//olduino definitions for spi constants follow
#define sck 5
#define miso 15	//really ef3
#define mosi 7
#define wizss 4	//slave select for the ethernet adapter
#define MAX_BUF 512
unsigned char buf[MAX_BUF]; //buffer for incoming and outgoing data
unsigned int Qmode; //what to do with the Q led
unsigned int hitcount=0;

void enablechip () { //pull cs low
  digitalWrite(wizss,LOW);
}
 void disablechip () {
  digitalWrite(wizss,HIGH);
}


unsigned char xferspi(unsigned char value){
  int i;
  for(i=0;i<8;i++){
    digitalWrite(mosi,(value&0x80));      //by setting mosi for each bit
    value=(value<<1)|digitalRead(miso);;
    digitalWrite(sck,HIGH);              //then pulsing the clock
    digitalWrite(sck,LOW);
  }
  return value;
}


void SPI_Write(unsigned int addr,unsigned char data)
{
  unsigned char value;
  enablechip();   	// Activate the CS pin
  value=xferspi(WIZNET_WRITE_OPCODE);   // Send Wiznet W5100 Write OpCode
  value=xferspi((addr & 0xFF00)>>8);  // Send Wiznet W5100 Address High Byte
  value=xferspi(addr & 0x00FF);  // Send Wiznet W5100 Address Low Byte
  value=xferspi(data);			// Send the data byte
  disablechip();	// make CS pin not active
}
unsigned char SPI_Read(unsigned int addr)
{
  unsigned char value; //data returned from spi transmission
  enablechip();   	// Activate the CS pin
  value=xferspi(WIZNET_READ_OPCODE);   // Send Wiznet W5100 Write OpCode
  value=xferspi((addr & 0xFF00)>>8);  // Send Wiznet W5100 Address High Byte
  value=xferspi(addr & 0x00FF);  // Send Wiznet W5100 Address Low Byte
  value=value=xferspi(0x00);	// Send Dummy transmission to read the data
  disablechip();	// make CS pin not active
  return(value);
}

void W5100_Init(void)
{
  // Ethernet Setup
  unsigned char mac_addr[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  unsigned char ip_addr[] = {192,168,1,181};
  unsigned char sub_mask[] = {255,255,255,0};
  unsigned char gtw_addr[] = {192,168,1,1};
  SPI_Write(MR,0x80);   // setting bit 7 of the mode register does a software reset of the w5100
  delay(1);

  SPI_Write(GAR + 0,gtw_addr[0]);  SPI_Write(GAR + 1,gtw_addr[1]);
  SPI_Write(GAR + 2,gtw_addr[2]);  SPI_Write(GAR + 3,gtw_addr[3]);
  delay(1);
  // Setting the Wiznet W5100 Source Address Register (SAR): 0x0009 to 0x000E
  SPI_Write(SAR + 0,mac_addr[0]);  SPI_Write(SAR + 1,mac_addr[1]);
  SPI_Write(SAR + 2,mac_addr[2]);  SPI_Write(SAR + 3,mac_addr[3]);
  SPI_Write(SAR + 4,mac_addr[4]);  SPI_Write(SAR + 5,mac_addr[5]);
  delay(1);
  // Set the Wiznet W5100 Sub Mask Address (SUBR): 0x0005 to 0x0008
  SPI_Write(SUBR + 0,sub_mask[0]);  SPI_Write(SUBR + 1,sub_mask[1]);
  SPI_Write(SUBR + 2,sub_mask[2]);  SPI_Write(SUBR + 3,sub_mask[3]);
  delay(1);
  // Setting the Wiznet W5100 IP Address (SIPR): 0x000F to 0x0012
  SPI_Write(SIPR + 0,ip_addr[0]);  SPI_Write(SIPR + 1,ip_addr[1]);
  SPI_Write(SIPR + 2,ip_addr[2]);  SPI_Write(SIPR + 3,ip_addr[3]);
  delay(1);
  printf("IP Address set: %d.%d.%d.%d\n\n",SPI_Read(SIPR + 0),SPI_Read(SIPR + 1),SPI_Read(SIPR + 2),SPI_Read(SIPR + 3));
  // Setting the Wiznet W5100 RX and TX Memory Size, we use 2KB for Rx/Tx 4 channels
  SPI_Write(RMSR,0x55);
  SPI_Write(TMSR,0x55);
  printf("Done Wiznet W5100 Initialization!\n");
}

unsigned int recv0(unsigned char *buf,unsigned int buflen){
    unsigned int ptr,offaddr,realaddr;

    if (buflen <= 0) return 1;

    if (buflen > MAX_BUF) // If the request size > MAX_BUF,
      buflen=MAX_BUF - 2; //just truncate it

    ptr = SPI_Read(S0_RX_RD);     // Read the Rx Read Pointer
    offaddr = (((ptr & 0x00FF) << 8 ) + SPI_Read(S0_RX_RD + 1));
    printf("RX Buffer: %x\n",offaddr);

    while(buflen) {
      buflen--;
      realaddr=RXBUFADDR + (offaddr & RX_BUF_MASK);
      *buf = SPI_Read(realaddr);
      offaddr++;
      buf++;
    }
    *buf='\0';        // String terminated character

    // Increase the S0_RX_RD value, so it point to the next receive
    SPI_Write(S0_RX_RD,(offaddr & 0xFF00) >> 8 );
    SPI_Write(S0_RX_RD + 1,(offaddr & 0x00FF));

    // Now Send the RECV command
    SPI_Write(S0_CR,CR_RECV);
    delay(5);    // Wait for Receive Process

    return 1;
}

unsigned int recv_size0(){
  return ((SPI_Read(S0_RX_RSR) & 0x00FF) << 8 ) + SPI_Read(S0_RX_RSR + 1);
}

unsigned int send0(const unsigned char *buf,unsigned int buflen)
{
    unsigned int ptr,offaddr,realaddr,txsize,timeout;

    if (buflen <= 0) return 0;
    printf("Send Size: %d\n",buflen);
    // Make sure the TX Free Size Register is available
    txsize=SPI_Read(SO_TX_FSR);
    txsize=(((txsize & 0x00FF) << 8 ) + SPI_Read(SO_TX_FSR + 1));
    printf("TX Free Size: %d\n",txsize);
    timeout=0;
    while (txsize < buflen) {
      delay(1);
     txsize=SPI_Read(SO_TX_FSR);
     txsize=(((txsize & 0x00FF) << 8 ) + SPI_Read(SO_TX_FSR + 1));
     // Timeout for approx 1000 ms
     if (timeout++ > 1000) {
       printf("TX Free Size Error!\n");
       disconnect0();
       return 0;
     }
   }

   ptr = SPI_Read(S0_TX_WR); // Read the Tx Write Pointer
   offaddr = (((ptr & 0x00FF) << 8 ) + SPI_Read(S0_TX_WR + 1));
   printf("TX Buffer: %x\n",offaddr);

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

void disconnect0(){
   SPI_Write(S0_CR,CR_DISCON); // Wait for Disconecting Process
   while(SPI_Read(S0_CR));
}

void setup(){
        W5100_Init(); //initialize the wiznet chip

}

void sendstuff(){
	int sendrc;
	strcpy((char *)buf,"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"
						"<html><body><span style=\"color:#0000A0\">\r\n"
						"<h1><center>Olduino Web Server</center></h1>\r\n"
						"<h3>1802 Membership Card and Wiznet w5100</h3>\r\n"
						"<p><form method=\"POST\">\r\n");
	sendrc=send(sockreg,buf,strlen((char *)buf)); 	// Now Send the HTTP Response

	// Create the HTTP Temperature and radio button Response
	strcpy((char *)buf,"<strong>Temp: <input type=\"text\" size=2 value=\""
						"1802! "
						"\"> <sup>O</sup>C\r\n"
						"<p><input type=\"radio\" name=\"radio\" value=\"0\" "
						"radiostat0"
						">Q LED ON\r\n"
						"<br><input type=\"radio\" name=\"radio\" value=\"1\" "
						"radiostat1"
						">Q LED OFF\r\n"
						"</strong><p>\r\n"
						"<input type=\"submit\">\r\n"
						"</form></span></body></html>\r\n");
	sendrc=send(sockreg,buf,strlen((char *)buf)); // Now Send the HTTP Remaining Response
}

void server_loop(){
	int socketstatus;
  report("loop");
  socketstatus=SPI_Read(S0_SR);
  switch socketstatus{
	  case SOCK_CLOSED: //initial condition
	  	opensocket();
	  	listen();
	  	break;
	  case SOCK_ESTABLISHED: //someone wants to talk to the server
	  	handlesession();
	  	break();
	  //following are cases where we have to reset and reopen the socket
      case SOCK_FIN_WAIT: case SOCK_CLOSING: case SOCK_TIME_WAIT:
      case SOCK_CLOSE_WAIT: case SOCK_LAST_ACK:
      	closesocket();
      	break;
	}
}

void main(void){
	int s0sr,rsize,recvrc,sendrc;
  	printf("Wiznet 5100 webbmastery begins here!\n");
	report("begin");
	w5100init();
	report("after init");
	while(1){
		server_loop();
	}
	SPI_Write(S0_MR,MR_TCP);
	SPI_Write(S0_PORT,((TCP_PORT & 0xFF00) >> 8 ));
	SPI_Write(S0_PORT + 1,(TCP_PORT & 0x00FF));
	report("after socket init");
	SPI_Write(S0_CR,CR_OPEN);                   // Open Socket
	report("after socket init");
    SPI_Write(S0_CR,CR_LISTEN);
	report("after listen");
	printf("loop..");
  // Loop forever
  for(;;){
	  if (s0sr=SOCK_ESTABLISHED){
	  	rsize=recv_size();
	  	if (rsize>0){
			recvrc=recv(sockreg,buf,rsize);
			printf("recv(sockreg,buf,rsize)=%d\n");
			if (recvrc>0){
				printf("Content:\n%s\n",buf);
				sendstuff();
				disconnect(sockreg); // Disconnect the socket

			}
		}
	}
	delay(1000);
  }
}
#include <olduino.c>
#include <nstdlib.c>
