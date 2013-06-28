
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

// Wiznet W5100 Op Codes
#define WIZNET_WRITE_OPCODE 0xF0
#define WIZNET_READ_OPCODE 0x0F
// Wiznet W5100 Register Addresses
#define MR   0x0000   // Mode Register
#define GAR  0x0001   // Gateway Address: 0x0001 to 0x0004
#define SUBR 0x0005   // Subnet mask Address: 0x0005 to 0x0008
#define SAR  0x0009   // Source Hardware Address (MAC): 0x0009 to 0x000E
#define SIPR 0x000F   // Source IP Address: 0x000F to 0x0012
#define RTR  0x0017   //Retry Timeout Register
#define RMSR 0x001A   // RX Memory Size Register
#define TMSR 0x001B   // TX Memory Size Register
//olduino definitions for spi constants follow
#define sck 5
#define miso 15	//really ef3
#define mosi 7
#define wizss 4	//slave select for the ethernet adapter

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
  xferspi(WIZNET_WRITE_OPCODE);   // Send Wiznet W5100 Write OpCode
  xferspi((addr & 0xFF00)>>8);  // Send Wiznet W5100 Address High Byte
  xferspi(addr & 0x00FF);  // Send Wiznet W5100 Address Low Byte
  xferspi(data);			// Send the data byte
  disablechip();	// make CS pin not active
}
unsigned char SPI_Read(unsigned int addr)
{
  unsigned char value; //data returned from spi transmission
  enablechip();   	// Activate the CS pin
  xferspi(WIZNET_READ_OPCODE);   // Send Wiznet W5100 Write OpCode
  xferspi((addr & 0xFF00)>>8);  // Send Wiznet W5100 Address High Byte
  xferspi(addr & 0x00FF);  // Send Wiznet W5100 Address Low Byte
  value=xferspi(0x00);	// Send Dummy transmission to read the data
  disablechip();	// make CS pin not active
  return(value);
}

void W5100_Init(void)
{
  // Ethernet Setup
  unsigned char mac_addr[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  //was 0x00,0x16,0x36,0xDE,0x58,0xF6};
  unsigned char ip_addr[] = {192,168,1,181};
  unsigned char sub_mask[] = {255,255,255,0};
  unsigned char gtw_addr[] = {192,168,1,1};
  SPI_Write(MR,0x80);   // setting bit 7 of the mode register does a software reset of the w5100
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

  // Setting the Wiznet W5100 RX and TX Memory Size, we use 2KB for Rx/Tx 4 channels
  printf("Setting Wiznet RMSR and TMSR\n\n");
  SPI_Write(RMSR,0x55);
  SPI_Write(TMSR,0x55);
  printf("Done Wiznet W5100 Initialization!\n");
}

void setup(){
  printf("Wiznet 5100 pingmastery begins here!\n");
        W5100_Init(); //initialize the wiznet chip

}
void loop(){

}

void main(void){
	setup();
	printf("loop..");
  // Loop forever
  for(;;){
	  loop();
  }

}
#include <olduino.c>
#include <nstdlib.c>