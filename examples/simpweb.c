
/*****************************************************************************
//  File Name    : wiznetping.c
//  Version      : 1.0
//  Description  : Wiznet W5100
//  Author       : RWB
//  Target       : Olduino
//  Compiler     : LCC1802
//  IDE          : Atmel AVR Studio 4.17
//  Programmer   : Olduino bootloader via avrdude
//  Adaptation   : 17 May 2013 by Bill Rowe - WJR
*****************************************************************************/
#include <nstdlib.h> //for printf etc.
#include <olduino.h> //for digitalRead, digitalWrite, delay
//fastspi header
#include <fastspi.h>
#define spiroutine1 xferspif2
#define spiroutine2 xferspif2
#define disableroutine() asm("\tseq\n"); //disablechip()
#define enableroutine()  asm("\treq\n"); //enablechip()
#define filler //printf("d=%x, p4=%x\n",data,PIN4);//delay(5);
// Wiznet W5100 Op Code
#define WIZNET_WRITE_OPCODE 0xF0
#define WIZNET_READ_OPCODE 0x0F
// Wiznet W5100 Register Addresses
#define MR         0x0000      // Mode Register
#define GAR        0x0001      // Gateway Address: 0x0001 to 0x0004
#define SUBR       0x0005      // Subnet mask Address: 0x0005 to 0x0008
#define SAR        0x0009      // Source Hardware Address (MAC): 0x0009 to 0x000E
#define SIPR       0x000F      // Source IP Address: 0x000F to 0x0012
#define RMSR       0x001A      // RX Memory Size Register
#define TMSR       0x001B      // TX Memory Size Register
#define S0_MR	   0x0400      // Socket 0: Mode Register Address
#define S0_CR	   0x0401      // Socket 0: Command Register Address
#define S0_IR	   0x0402      // Socket 0: Interrupt Register Address
#define S0_SR	   0x0403      // Socket 0: Status Register Address
#define S0_PORT    0x0404      // Socket 0: Source Port: 0x0404 to 0x0405
#define SO_TX_FSR  0x0420      // Socket 0: Tx Free Size Register: 0x0420 to 0x0421
#define S0_TX_RD   0x0422      // Socket 0: Tx Read Pointer Register: 0x0422 to 0x0423
#define S0_TX_WR   0x0424      // Socket 0: Tx Write Pointer Register: 0x0424 to 0x0425
#define S0_RX_RSR  0x0426      // Socket 0: Rx Received Size Pointer Register: 0x0425 to 0x0427
#define S0_RX_RD   0x0428      // Socket 0: Rx Read Pointer: 0x0428 to 0x0429
#define TXBUFADDR  0x4000      // W5100 Send Buffer Base Address
#define RXBUFADDR  0x6000      // W5100 Read Buffer Base Address
// S0_MR values
#define MR_CLOSE	  0x00    // Unused socket
#define MR_TCP		  0x01    // TCP
#define MR_UDP		  0x02    // UDP
#define MR_IPRAW	  0x03	  // IP LAYER RAW SOCK
#define MR_MACRAW	  0x04	  // MAC LAYER RAW SOCK
#define MR_PPPOE	  0x05	  // PPPoE
#define MR_ND		  0x20	  // No Delayed Ack(TCP) flag
#define MR_MULTI	  0x80	  // support multicating
// S0_CR values
#define CR_OPEN          0x01	  // Initialize or open socket
#define CR_LISTEN        0x02	  // Wait connection request in tcp mode(Server mode)
#define CR_CONNECT       0x04	  // Send connection request in tcp mode(Client mode)
#define CR_DISCON        0x08	  // Send closing reqeuset in tcp mode
#define CR_CLOSE         0x10	  // Close socket
#define CR_SEND          0x20	  // Update Tx memory pointer and send data
#define CR_SEND_MAC      0x21	  // Send data with MAC address, so without ARP process
#define CR_SEND_KEEP     0x22	  // Send keep alive message
#define CR_RECV          0x40	  // Update Rx memory buffer pointer and receive data
// S0_SR values
#define SOCK_CLOSED      0x00     // Closed
#define SOCK_INIT        0x13	  // Init state
#define SOCK_LISTEN      0x14	  // Listen state
#define SOCK_SYNSENT     0x15	  // Connection state
#define SOCK_SYNRECV     0x16	  // Connection state
#define SOCK_ESTABLISHED 0x17	  // Success to connect
#define SOCK_FIN_WAIT    0x18	  // Closing state
#define SOCK_CLOSING     0x1A	  // Closing state
#define SOCK_TIME_WAIT	 0x1B	  // Closing state
#define SOCK_CLOSE_WAIT  0x1C	  // Closing state
#define SOCK_LAST_ACK    0x1D	  // Closing state
#define SOCK_UDP         0x22	  // UDP socket
#define SOCK_IPRAW       0x32	  // IP raw mode socket
#define SOCK_MACRAW      0x42	  // MAC raw mode socket
#define SOCK_PPPOE       0x5F	  // PPPOE socket
#define TX_BUF_MASK      0x07FF   // Tx 2K Buffer Mask:
#define RX_BUF_MASK      0x07FF   // Rx 2K Buffer Mask:
#define NET_MEMALLOC     0x05     // Use 2K of Tx/Rx Buffer
#define TCP_PORT         80       // TCP/IP Port
//olduino definitions for spi constants follow
#define sck 5
#define miso 15	//really ef3
#define mosi 7
// slave select is on Q #define wizss 4	//slave select for the ethernet adapter
// Define W5100 Socket Register and Variables Used
unsigned char sockreg;
#define MAX_BUF 512
unsigned char buf[MAX_BUF];
//int tempvalue;
//uint8_t ledmode,ledeye,ledsign;

void enablechip () { //pull cs low
  setqoff();//digitalWrite(wizss,LOW);
}
 void disablechip () {
  setqon();//digitalWrite(wizss,HIGH);
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
unsigned char xferspi2(unsigned int value){
  int i;
  for(i=0;i<8;i++){
    digitalWrite(mosi,(value&0x80));      //by setting mosi for each bit
    value=(value<<1)|digitalRead(miso);;
    out(1,PIN4);              //then pulsing the clock
  }
  return value;
}


void SPI_Write(unsigned int addr,unsigned char data)
{
  unsigned char value;
  enableroutine();   	// Activate the CS pin
  spiroutine1(WIZNET_WRITE_OPCODE);   // Send Wiznet W5100 Write OpCode
  spiroutine1(addr >>8);// was((addr & 0xFF00)>>8);  // Send Wiznet W5100 Address High Byte
  spiroutine1(addr & 0x00FF);  // Send Wiznet W5100 Address Low Byte
  spiroutine2(data);			// Send the data byte
  disableroutine();	// make CS pin not active
}
unsigned char SPI_Read(unsigned int addr)
{
  unsigned char value; //data returned from spi transmission
  unsigned char data=0; //dummy
  enableroutine();   	// Activate the CS pin
  spiroutine1(WIZNET_READ_OPCODE);   // Send Wiznet W5100 Write OpCode
  spiroutine1(addr >>8);// ((addr & 0xFF00)>>8);  // Send Wiznet W5100 Address High Byte
  spiroutine1(addr & 0x00FF);  // Send Wiznet W5100 Address Low Byte
  value=spiroutine2(0x00);	// Send Dummy transmission to read the data
  disableroutine();	// make CS pin not active
  return(value);
}

void W5100_Init(void)
{
  // Ethernet Setup
  unsigned char mac_addr[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  //was 0x00,0x16,0x36,0xDE,0x58,0xF6};
  unsigned char ip_addr[] = {192,168,1,182};
  unsigned char sub_mask[] = {255,255,255,0};
  unsigned char gtw_addr[] = {192,168,1,1};
  SPI_Write(MR,0x80);   // setting bit 7 of the mode register does a software reset of the w5100
  printf("MR=%x\n",SPI_Read(MR));
  delay(1);

  printf("Setting Gateway Address %d.%d.%d.%d\n",gtw_addr[0],gtw_addr[1],
  gtw_addr[2],gtw_addr[3]);
  SPI_Write(GAR + 0,gtw_addr[0]);
  SPI_Write(GAR + 1,gtw_addr[1]);
  SPI_Write(GAR + 2,gtw_addr[2]);
  SPI_Write(GAR + 3,gtw_addr[3]);
  delay(1);
  // Setting the Wiznet W5100 Source Address Register (SAR): 0x0009 to 0x000E
  printf("Setting Source Address %x:%x:%x:%x:%x:%x\n",mac_addr[0],mac_addr[1],
          mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
  SPI_Write(SAR + 0,mac_addr[0]);
  SPI_Write(SAR + 1,mac_addr[1]);
  SPI_Write(SAR + 2,mac_addr[2]);
  SPI_Write(SAR + 3,mac_addr[3]);
  SPI_Write(SAR + 4,mac_addr[4]);
  SPI_Write(SAR + 5,mac_addr[5]);
  delay(1);
  // Set the Wiznet W5100 Sub Mask Address (SUBR): 0x0005 to 0x0008
  printf("Setting Subnet Mask  %d.%d.%d.%d\n",sub_mask[0],sub_mask[1],sub_mask[2],sub_mask[3]);
  SPI_Write(SUBR + 0,sub_mask[0]);
  SPI_Write(SUBR + 1,sub_mask[1]);
  SPI_Write(SUBR + 2,sub_mask[2]);
  SPI_Write(SUBR + 3,sub_mask[3]);
  delay(1);
  // Setting the Wiznet W5100 IP Address (SIPR): 0x000F to 0x0012
  printf("Setting IP Address %d.%d.%d.%d\n",ip_addr[0],ip_addr[1],ip_addr[2],ip_addr[3]);
  SPI_Write(SIPR + 0,ip_addr[0]);
  SPI_Write(SIPR + 1,ip_addr[1]);
  SPI_Write(SIPR + 2,ip_addr[2]);
  SPI_Write(SIPR + 3,ip_addr[3]);
  delay(1);
  printf("Reading SIPR: %d.%d.%d.%d\n\n",SPI_Read(SIPR + 0),SPI_Read(SIPR + 1),SPI_Read(SIPR + 2),SPI_Read(SIPR + 3));
  printf("Reading SUBR: %d.%d.%d.%d\n\n",SPI_Read(SUBR + 0),SPI_Read(SUBR + 1),SPI_Read(SUBR + 2),SPI_Read(SUBR + 3));
  printf("Reading GAR: %d.%d.%d.%d\n\n",SPI_Read(GAR + 0),SPI_Read(GAR + 1),SPI_Read(GAR + 2),SPI_Read(GAR + 3));
  // Setting the Wiznet W5100 RX and TX Memory Size, we use 2KB for Rx/Tx 4 channels
  printf("Setting Wiznet RMSR and TMSR\n\n");
  SPI_Write(RMSR,0x55);
  SPI_Write(TMSR,0x55);
  printf("Done Wiznet W5100 Initialization!\n");
}

void disconnect(unsigned char sock)
{
   if (sock != 0) return; // Send Disconnect Command
   SPI_Write(S0_CR,CR_DISCON); // Wait for Disconecting Process
   while(SPI_Read(S0_CR));
}

unsigned int recv(unsigned char sock,unsigned char *buf,unsigned int buflen)
{
    unsigned int ptr,offaddr,realaddr;

    if (buflen <= 0 || sock != 0) return 1;

    // If the request size > MAX_BUF,just truncate it
    if (buflen > MAX_BUF)
      buflen=MAX_BUF - 2;
    // Read the Rx Read Pointer
    ptr = SPI_Read(S0_RX_RD);
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
unsigned int recv_size(void)
{
  return ((SPI_Read(S0_RX_RSR) & 0x00FF) << 8 ) + SPI_Read(S0_RX_RSR + 1);
}
unsigned int send(unsigned char sock,const unsigned char *buf,unsigned int buflen)
{
    unsigned int ptr,offaddr,realaddr,txsize,timeout;

    if (buflen <= 0 || sock != 0) return 0;
    printf("Send Size: %d\n",buflen);
    // Make sure the TX Free Size Register is available
    txsize=SPI_Read(SO_TX_FSR);
    txsize=(((txsize & 0x00FF) << 8 ) + SPI_Read(SO_TX_FSR + 1));
#if _DEBUG_MODE
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
       disconnect(sock);
       return 0;
     }
   }

   // Read the Tx Write Pointer
   ptr = SPI_Read(S0_TX_WR);
   offaddr = (((ptr & 0x00FF) << 8 ) + SPI_Read(S0_TX_WR + 1));
#if _DEBUG_MODE
    printf("TX Buffer: %x\n",offaddr);
#endif

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
void dump(int addr){
	int i;
	printf("%x=", addr);
	for (i=addr;i<addr+32;i++){
		if ((i%8)==0) printf("%x=",i);
		printf("%x ",SPI_Read(addr));
		if ((i%8)==7) printf("\n");
	}
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

void main(void){
	int s0sr,rsize,recvrc,sendrc;
	delay(1000);
	digitalWrite(6,HIGH); //take wiznet out of hard reset
	report("begin");
	//while(1);
	SPI_WRITE(MR,0x80);//11110000 00000000 00000000 10000000 00001111 00000000 00000000 00000000
	report("post reset");
    W5100_Init(); //initialize the wiznet chip
	report("after init");
	SPI_Write(S0_MR,MR_TCP);
	SPI_Write(S0_PORT,((TCP_PORT & 0xFF00) >> 8 ));
	SPI_Write(S0_PORT + 1,(TCP_PORT & 0x00FF));
	report("after socket init");
	SPI_Write(S0_CR,CR_OPEN);                   // Open Socket
	report("after socket open");
    SPI_Write(S0_CR,CR_LISTEN);
	report("after listen");
	printf("loop..");
  // Loop forever
  for(;;){
	  report("loop");
	  s0sr=SPI_Read(S0_SR);
	  if (s0sr=SOCK_ESTABLISHED){
	  	rsize=recv_size();
	  	if (rsize>0){
			recvrc=recv(sockreg,buf,rsize);
			printf("rsize=%d, recv=%d\n", rsize, recvrc);
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
#include "fastspi.c"
