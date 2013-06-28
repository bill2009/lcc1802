/*****************************************************************************
//  File Name    : wiznetweb.c
//  Version      : 1.0
//  Description  : AVRJazz Mega328 and Wiznet W5100 Web Server
//  Author       : RWB
//  Target       : AVRJazz Mega328 Board
//  Compiler     : AVR-GCC 4.3.2; avr-libc 1.6.6 (WinAVR 20090313)
//  IDE          : Atmel AVR Studio 4.17
//  Programmer   : AVRJazz Mega328 STK500 v2.0 Bootloader
//               : AVR Visual Studio 4.17, STK500 programmer
//  Last Updated : 20 July 2010
*****************************************************************************/
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
// AVRJazz Mega328 SPI I/O
#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_CS   PORTB2
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
// Debugging Mode, 0 - Debug OFF, 1 - Debug ON
#define _DEBUG_MODE      0
#if _DEBUG_MODE
  #define BAUD_RATE 19200
#endif
// Define W5100 Socket Register and Variables Used
uint8_t sockreg;
#define MAX_BUF 512
uint8_t buf[MAX_BUF];
int tempvalue;
uint8_t ledmode,ledeye,ledsign;
#if _DEBUG_MODE
void uart_init(void)
{
  UBRR0H = (((F_CPU/BAUD_RATE)/16)-1)>>8;		// set baud rate
  UBRR0L = (((F_CPU/BAUD_RATE)/16)-1);
  UCSR0B = (1<<RXEN0)|(1<<TXEN0); 				// enable Rx & Tx
  UCSR0C=  (1<<UCSZ01)|(1<<UCSZ00);  	        // config USART; 8N1
}
void uart_flush(void)
{
  unsigned char dummy;
  while (UCSR0A & (1<<RXC0)) dummy = UDR0;
}
int uart_putch(char ch,FILE *stream)
{
   if (ch == '\n')
    uart_putch('\r', stream);
   while (!(UCSR0A & (1<<UDRE0)));
   UDR0=ch;
   return 0;
}
int uart_getch(FILE *stream)
{
   unsigned char ch;
   while (!(UCSR0A & (1<<RXC0)));
   ch=UDR0;  

   /* Echo the Output Back to terminal */
   uart_putch(ch,stream);       

   return ch;
}
void ansi_cl(void)
{
  // ANSI clear screen: cl=\E[H\E[J
  putchar(27);
  putchar('[');
  putchar('H');
  putchar(27);
  putchar('[');
  putchar('J');
}
void ansi_me(void)
{
  // ANSI turn off all attribute: me=\E[0m
  putchar(27);
  putchar('[');
  putchar('0');
  putchar('m');
}
#endif
void SPI_Write(uint16_t addr,uint8_t data)
{
  // Activate the CS pin
  SPI_PORT &= ~(1<<SPI_CS);
  // Start Wiznet W5100 Write OpCode transmission
  SPDR = WIZNET_WRITE_OPCODE;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address High Bytes transmission
  SPDR = (addr & 0xFF00) >> 8;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address Low Bytes transmission
  SPDR = addr & 0x00FF;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));   

  // Start Data transmission
  SPDR = data;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // CS pin is not active
  SPI_PORT |= (1<<SPI_CS);
}
unsigned char SPI_Read(uint16_t addr)
{
  // Activate the CS pin
  SPI_PORT &= ~(1<<SPI_CS);
  // Start Wiznet W5100 Read OpCode transmission
  SPDR = WIZNET_READ_OPCODE;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address High Bytes transmission
  SPDR = (addr & 0xFF00) >> 8;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  // Start Wiznet W5100 Address Low Bytes transmission
  SPDR = addr & 0x00FF;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));   

  // Send Dummy transmission for reading the data
  SPDR = 0x00;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));  

  // CS pin is not active
  SPI_PORT |= (1<<SPI_CS);
  return(SPDR);
}
void W5100_Init(void)
{
  // Ethernet Setup
  unsigned char mac_addr[] = {0x00,0x16,0x36,0xDE,0x58,0xF6};
  unsigned char ip_addr[] = {192,168,2,10};
  unsigned char sub_mask[] = {255,255,255,0};
  unsigned char gtw_addr[] = {192,168,2,1};
  // Setting the Wiznet W5100 Mode Register: 0x0000
  SPI_Write(MR,0x80);            // MR = 0b10000000;
  // Setting the Wiznet W5100 Gateway Address (GAR): 0x0001 to 0x0004
  SPI_Write(GAR + 0,gtw_addr[0]);
  SPI_Write(GAR + 1,gtw_addr[1]);
  SPI_Write(GAR + 2,gtw_addr[2]);
  SPI_Write(GAR + 3,gtw_addr[3]);
  // Setting the Wiznet W5100 Source Address Register (SAR): 0x0009 to 0x000E
  SPI_Write(SAR + 0,mac_addr[0]);
  SPI_Write(SAR + 1,mac_addr[1]);
  SPI_Write(SAR + 2,mac_addr[2]);
  SPI_Write(SAR + 3,mac_addr[3]);
  SPI_Write(SAR + 4,mac_addr[4]);
  SPI_Write(SAR + 5,mac_addr[5]);
  // Setting the Wiznet W5100 Sub Mask Address (SUBR): 0x0005 to 0x0008
  SPI_Write(SUBR + 0,sub_mask[0]);
  SPI_Write(SUBR + 1,sub_mask[1]);
  SPI_Write(SUBR + 2,sub_mask[2]);
  SPI_Write(SUBR + 3,sub_mask[3]);
  // Setting the Wiznet W5100 IP Address (SIPR): 0x000F to 0x0012
  SPI_Write(SIPR + 0,ip_addr[0]);
  SPI_Write(SIPR + 1,ip_addr[1]);
  SPI_Write(SIPR + 2,ip_addr[2]);
  SPI_Write(SIPR + 3,ip_addr[3]);    

  // Setting the Wiznet W5100 RX and TX Memory Size (2KB),
  SPI_Write(RMSR,NET_MEMALLOC);
  SPI_Write(TMSR,NET_MEMALLOC);
}
void close(uint8_t sock)
{
   if (sock != 0) return;

   // Send Close Command
   SPI_Write(S0_CR,CR_CLOSE);
   // Waiting until the S0_CR is clear
   while(SPI_Read(S0_CR));
}
void disconnect(uint8_t sock)
{
   if (sock != 0) return;
   // Send Disconnect Command
   SPI_Write(S0_CR,CR_DISCON);
   // Wait for Disconecting Process
   while(SPI_Read(S0_CR));
}
uint8_t socket(uint8_t sock,uint8_t eth_protocol,uint16_t tcp_port)
{
    uint8_t retval=0;
    if (sock != 0) return retval;

    // Make sure we close the socket first
    if (SPI_Read(S0_SR) == SOCK_CLOSED) {
      close(sock);
    }
    // Assigned Socket 0 Mode Register
    SPI_Write(S0_MR,eth_protocol);

    // Now open the Socket 0
    SPI_Write(S0_PORT,((tcp_port & 0xFF00) >> 8 ));
    SPI_Write(S0_PORT + 1,(tcp_port & 0x00FF));
    SPI_Write(S0_CR,CR_OPEN);                   // Open Socket
    // Wait for Opening Process
    while(SPI_Read(S0_CR));
    // Check for Init Status
    if (SPI_Read(S0_SR) == SOCK_INIT)
      retval=1;
    else
      close(sock);

    return retval;
}
uint8_t listen(uint8_t sock)
{
   uint8_t retval = 0;
   if (sock != 0) return retval;
   if (SPI_Read(S0_SR) == SOCK_INIT) {
     // Send the LISTEN Command
     SPI_Write(S0_CR,CR_LISTEN);

     // Wait for Listening Process
     while(SPI_Read(S0_CR));
     // Check for Listen Status
     if (SPI_Read(S0_SR) == SOCK_LISTEN)
       retval=1;
     else
       close(sock);
    }
    return retval;
}
uint16_t send(uint8_t sock,const uint8_t *buf,uint16_t buflen)
{
    uint16_t ptr,offaddr,realaddr,txsize,timeout;   

    if (buflen <= 0 || sock != 0) return 0;
#if _DEBUG_MODE
    printf("Send Size: %d\n",buflen);
#endif
    // Make sure the TX Free Size Register is available
    txsize=SPI_Read(SO_TX_FSR);
    txsize=(((txsize & 0x00FF) << 8 ) + SPI_Read(SO_TX_FSR + 1));
#if _DEBUG_MODE
    printf("TX Free Size: %d\n",txsize);
#endif
    timeout=0;
    while (txsize < buflen) {
      _delay_ms(1);
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
uint16_t recv(uint8_t sock,uint8_t *buf,uint16_t buflen)
{
    uint16_t ptr,offaddr,realaddr;   	

    if (buflen <= 0 || sock != 0) return 1;   

    // If the request size > MAX_BUF,just truncate it
    if (buflen > MAX_BUF)
      buflen=MAX_BUF - 2;
    // Read the Rx Read Pointer
    ptr = SPI_Read(S0_RX_RD);
    offaddr = (((ptr & 0x00FF) << 8 ) + SPI_Read(S0_RX_RD + 1));
#if _DEBUG_MODE
    printf("RX Buffer: %x\n",offaddr);
#endif	

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
    _delay_us(5);    // Wait for Receive Process

    return 1;
}
uint16_t recv_size(void)
{
  return ((SPI_Read(S0_RX_RSR) & 0x00FF) << 8 ) + SPI_Read(S0_RX_RSR + 1);
}
int strindex(char *s,char *t)
{
  uint16_t i,n;

  n=strlen(t);
  for(i=0;*(s+i); i++) {
    if (strncmp(s+i,t,n) == 0)
      return i;
  }
  return -1;
}
ISR(TIMER0_OVF_vect)
{
  static unsigned char tenms=1;
  tenms++;                  // Read ADC every 20 x 10ms = 200 milisecond
  if (tenms >= 20) {
    cli();                                // Disable Interupt
    // Select the LED Mode here
    if (ledmode == 1) {
      if (ledeye <= 0) ledeye=0x01;
      if (ledsign == 0) {
	PORTD=ledeye;
	ledeye=ledeye << 1;
	if (ledeye >= 0x80) ledsign=1;
      } else {
	PORTD=ledeye;
	ledeye=ledeye >> 1;
	if (ledeye <= 0x01) ledsign=0;
      }
    } else {
      if (ledsign == 0) {
	PORTD=0x00;
	ledsign=1;
      } else {
	PORTD=0xFF;
	ledsign=0;
      }
    }
    // Set ADMUX Channel for LM35DZ Input
    ADMUX=0x01;
    // Start conversion by setting ADSC on ADCSRA Register
    ADCSRA |= (1<<ADSC);
    // wait until convertion complete ADSC=0 -> Complete
    while (ADCSRA & (1<<ADSC));
    // Get the ADC Result
    tempvalue = ADCW;
    // ADC = (Vin x 1024) / Vref, Vref = 1 Volt, LM35DZ Out = 10mv/C
    tempvalue = (int)(tempvalue) / 10.24;

    tenms=1;	 

    sei();                            // Enable Interupt
  }
  // Start counter from 0x94, overflow at 10 mSec
  TCNT0=0x94;
}
#if _DEBUG_MODE
// Assign I/O stream to UART
FILE uart_str = FDEV_SETUP_STREAM(uart_putch, uart_getch, _FDEV_SETUP_RW);
#endif
int main(void){
  uint8_t sockstat;
  uint16_t rsize;
  char radiostat0[10],radiostat1[10],temp[4];
  int getidx,postidx;

  // Reset Port D
  DDRD = 0xFF;       // Set PORTD as Output
  PORTD = 0x00;	     

#if _DEBUG_MODE
  // Define Output/Input Stream
  stdout = stdin = &uart_str;
  // Initial UART Peripheral
  uart_init();
  // Clear Screen
  ansi_me();
  ansi_cl();
  ansi_me();
  ansi_cl();
  uart_flush();
#endif
  // Initial ATMega386 ADC Peripheral
  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);
  // Free running ADC Mode
  ADCSRB = 0x00;
  // Initial the AVR ATMega328 SPI Peripheral
  // Set MOSI (PORTB3),SCK (PORTB5) and PORTB2 (SS) as output, others as input
  SPI_DDR = (1<<PORTB3)|(1<<PORTB5)|(1<<PORTB2);
  // CS pin is not active
  SPI_PORT |= (1<<SPI_CS);
  // Enable SPI, Master Mode 0, set the clock rate fck/2
  SPCR = (1<<SPE)|(1<<MSTR);
  SPSR |= (1<<SPI2X);  

  // Initial ATMega368 Timer/Counter0 Peripheral
  TCCR0A=0x00;                  // Normal Timer0 Operation
  TCCR0B=(1<<CS02)|(1<<CS00);   // Use maximum prescaller: Clk/1024
  TCNT0=0x94;                   // Start counter from 0x94, overflow at 10 mSec
  TIMSK0=(1<<TOIE0);            // Enable Counter Overflow Interrupt
  sei();                        // Enable Interrupt

  // Initial the W5100 Ethernet
  W5100_Init();
  // Initial variable used
  sockreg=0;
  tempvalue=0;
  ledmode=1;
  ledeye=0x01;                  // Initial LED Eye Variables
  ledsign=0;
#if _DEBUG_MODE
  printf("WEB Server Debug Mode\n\n");
#endif

  // Loop forever
  for(;;){
    sockstat=SPI_Read(S0_SR);
    switch(sockstat) {
     case SOCK_CLOSED:
        if (socket(sockreg,MR_TCP,TCP_PORT) > 0) {
	  // Listen to Socket 0
	  if (listen(sockreg) <= 0)
	    _delay_ms(1);
#if _DEBUG_MODE
          printf("Socket Listen!\n");
#endif
	}
	break;
     case SOCK_ESTABLISHED:
	// Get the client request size
        rsize=recv_size();
#if _DEBUG_MODE
	printf("Size: %d\n",rsize);
#endif
	if (rsize > 0) {
	  // Now read the client Request
	  if (recv(sockreg,buf,rsize) <= 0) break;
#if _DEBUG_MODE
  	  printf("Content:\n%s\n",buf);
#endif
          // Check the Request Header
	  getidx=strindex((char *)buf,"GET /");
	  postidx=strindex((char *)buf,"POST /");
	  if (getidx >= 0 || postidx >= 0) {
#if _DEBUG_MODE
	    printf("Req. Check!\n");
#endif
            // Now check the Radio Button for POST request
	    if (postidx >= 0) {
	      if (strindex((char *)buf,"radio=0") > 0)
	        ledmode=0;
	      if (strindex((char *)buf,"radio=1") > 0)
	        ledmode=1;
            }
#if _DEBUG_MODE
	    printf("Req. Send!\n");
#endif
	    // Create the HTTP Response	Header
	    strcpy_P((char *)buf,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"));
	    strcat_P((char *)buf,PSTR("<html><body><span style=\"color:#0000A0\">\r\n"));
	    strcat_P((char *)buf,PSTR("<h1>Embedded Web Server</h1>\r\n"));
	    strcat_P((char *)buf,PSTR("<h3>AVRJazz Mega328 and WIZ811MJ</h3>\r\n"));
	    strcat_P((char *)buf,PSTR("<p><form method=\"POST\">\r\n"));
	    // Now Send the HTTP Response
	    if (send(sockreg,buf,strlen((char *)buf)) <= 0) break;

	    // Create the HTTP Temperature Response
	    sprintf((char *)temp,"%d",tempvalue);        // Convert temperature value to string
	    strcpy_P((char *)buf,PSTR("<strong>Temp: <input type=\"text\" size=2 value=\""));
	    strcat((char *)buf,temp);
	    strcat_P((char *)buf,PSTR("\"> <sup>O</sup>C\r\n"));									

	    if (ledmode == 1) {
	      strcpy(radiostat0,"");
	      strcpy_P(radiostat1,PSTR("checked"));
	    } else {
	      strcpy_P(radiostat0,PSTR("checked"));
	      strcpy(radiostat1,"");
	    }
            // Create the HTTP Radio Button 0 Response
	    strcat_P((char *)buf,PSTR("<p><input type=\"radio\" name=\"radio\" value=\"0\" "));
	    strcat((char *)buf,radiostat0);
	    strcat_P((char *)buf,PSTR(">Blinking LED\r\n"));
	    strcat_P((char *)buf,PSTR("<br><input type=\"radio\" name=\"radio\" value=\"1\" "));
	    strcat((char *)buf,radiostat1);
	    strcat_P((char *)buf,PSTR(">Scanning LED\r\n"));
 	    strcat_P((char *)buf,PSTR("</strong><p>\r\n"));
	    strcat_P((char *)buf,PSTR("<input type=\"submit\">\r\n"));
	    strcat_P((char *)buf,PSTR("</form></span></body></html>\r\n"));
            // Now Send the HTTP Remaining Response
	    if (send(sockreg,buf,strlen((char *)buf)) <= 0) break;
          }
	  // Disconnect the socket
	  disconnect(sockreg);
        } else
	  _delay_us(10);    // Wait for request
	break;
      case SOCK_FIN_WAIT:
      case SOCK_CLOSING:
      case SOCK_TIME_WAIT:
      case SOCK_CLOSE_WAIT:
      case SOCK_LAST_ACK:
        // Force to close the socket
	close(sockreg);
#if _DEBUG_MODE
	printf("Socket Close!\n");
#endif
	break;
    }
  }
  return 0;
}
