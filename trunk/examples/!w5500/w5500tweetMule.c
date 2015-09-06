
/*****************************************************************************
//  File Name    : w5500mule.c
//  Version      : 1
//  Description  : wiznet w5500 test harness
//  Author       : WJR with thanks Karl Lunt & Wiznet
//  Target       : Olduino
//  Compiler     : LCC1802
//  IDE          : TextPad
//  Programmer   : Olduino bootloader via avrdude
//  Created		 : Nov 26, 2014
*****************************************************************************/
#define	nofloats			//not using floating point
#include <nstdlib.h> //for printf etc.
#include <cpu1802spd4port7.h>
#include <olduino.h> //for digitalRead, digitalWrite, delay
#include <hspi2.h> //hardware spi header
#include "w5500data.h"  //wiznet definitions
#include "w5500code.h"  //wiznet code definitions
union IPaddr thisip={182}; //the ip that a form/request came from
#define MAX_BUF 1024
unsigned char buf[MAX_BUF];			//memory buffer for incoming & outgoing data
#define LIB_DOMAIN "arduino-tweet.appspot.com"
#define mytoken "3436815045-F28y3lUJrVJCAWFENtUeiyV0PJfrt3e0r65aGCu"
#define mymsg "This is Olduino\'s first tweet!"
void sendreq(){//send a request
	char itoabuf[16];
	printf("sending tweet\n");
	sendlit("POST http://" LIB_DOMAIN "/update HTTP/1.0\n");
	sendlit("Content-Length: ");
	send0s(itoa(strlen(mymsg)+strlen(mytoken)+14,itoabuf));
	sendlit("\n\n");
	sendlit("token=");
	sendlit(mytoken);
	sendlit("&status=");
	sendlit(mymsg);sendlit("\n");

}
void getresp(){	//handle a session once it's established
	unsigned int rsize,strncmpval;
	unsigned int tries=500;
	printf("getting response\n");
	rsize=wizGetCtl16(SnRX_RSR); //get the size of the received data
	while(rsize==0 && tries-->0){
		delay(20);
		printf("re-size ");
		rsize=wizGetCtl16(SnRX_RSR); //retry size of the received data
	}
	printf("**rsz=%d\n",rsize);
	if (rsize>0){
		thisip.l=getip();
		if (recv0(buf,min(1023,rsize))>0){ //get some characters
			printf("%s\n",buf);
		}
	}
	printf("flushing %d\n",rsize);
  	if (rsize>0) flush(rsize);	//get rid of the received data
	wizCmd(CR_DISCON);// Disconnect the connection- we're done here
	printf("done\n>\n");
}


void wizWrite(unsigned int addr, unsigned char opcode, void * data, unsigned int len);
void socket0_client_init(){ //initialize socket 0 for http client
    unsigned char host_addr[]={173,194,196,141};//{24,156,153,25};//address for google destination
	wizCmd(CR_CLOSE); //make sure port is closed
	wizSetCtl8(SnIR,0xFF); //reset interrupt reg
	wizSetCtl8(SnMR,MR_TCP); //set mode register to tcp
	wizSetCtl16(SnPORT,1024); //set tcp port to 80
   	wizWrite(SnDIPR,WIZNET_WRITE_S0R,host_addr, 4); //write the outgoing dest ip address
   	wizSetCtl16(SnDPORT,80);//destination port
	wizCmd(CR_OPEN); //open the port
	wizCmd(CR_CONNECT); //try to make a conection
}

void main(void){
	unsigned char socket0status;
    unsigned char ip_addr[] = {192,168,1,182}; //{169,254,180,2}; //
	delay(100);
	printf("\nW5500 Test Mule Tweet\n");
	delay(500);
    wiz_Init(ip_addr); //initialize the wiznet chip
	socket0_client_init();

	while(1){  // Loop forever
		socket0status=wizGetCtl8(SnSR);
		printf("s0s=%cx ",socket0status);
		if (socket0status==SOCK_ESTABLISHED){ //we're connected
			sendreq();
			getresp();
			printf("stalling\n");
			while(1);//loop here
		}
		delay(100);
	}
}
#include <olduino.c>
#include <nstdlib.c>
#include <hspi2.c>
#include "w5500code.c"
