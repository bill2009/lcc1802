
/*****************************************************************************
//  File Name    : olduinoserver4W5500.c
//  Version      : 4.4
//  Description  : olduino LED server
//  Author       : WJR with thanks to RWB
//  Target       : Olduino
//  Compiler     : LCC1802
//  IDE          : TextPad
//  Programmer   : Olduino bootloader via avrdude
//  Adaptated    : 17 May 2013 by Bill Rowe - WJR for the olduino platform
//  Revised 	 : 22 Aug 2015 for the W5500
*****************************************************************************/
//15-08-22 sending images insead of text status, logging ips as they're seen.
#define	nofloats			//not using floating point
#include <nstdlib.h> //for printf etc.
#include <cpu1802spd4port7.h> //defines processor type, speed, and host port
#include <olduino.h> //for digitalRead, digitalWrite, delay
#include <hspi2.h> //hardware spi header
#include "w5500data.h"  //wiznet definitions
#include "w5500code.h"  //wiznet code definitions
#include "vcfmwdemo.h"
#define MAX_BUF 512
unsigned char buf[MAX_BUF];			//memory buffer for incoming & outgoing data
int ledmode=0;	//0=off, 1=on
union IPaddr cmdip,oldip; //most recent address where a command came from, current session ip
int pages=0,sessions=0;
char strbuf[16];
void sendip(unsigned char * ip){
	send0s(itoa(ip[0],strbuf));
	send0s(".");
	send0s(itoa(ip[1],strbuf));
	sendlit(".");
	send0s(itoa(ip[2],strbuf));
	sendlit(".");
	send0s(itoa(ip[3],strbuf));
}
void sendCip(){
	sendlit("Last command from: ");
	sendip(cmdip.c);
	sendlit("<p>");

}
void sendform(){
	int sendrc;
	pages+=1;
	sendrc=sendlit(hdr); 	// Now Send the HTTP Response first part
	sendlit("Pages Served: "); send0s(itoa(pages,strbuf)); sendlit("<p>");
	if (cmdip.c[0]!=0)
		sendCip();
	if (ledmode==1){
		sendconst(gifon); //http://bit.ly/1LAGSm5
		asm("	seq\n"); //insurance
	}else{
		sendconst(gifoff); //http://bit.ly/1EiEYnx
		asm("	req\n"); //insurance
	}
	sendrc=sendlit(postform); 	// Now Send the "POST" form
	sendrc=sendlit(getform); 	// Now Send the "GET" form
	sendrc=sendlit(olduinolink); 	// Now Send the link to the wordpress page
	sendrc=sendlit(trlr); 	// Now Send the rest of the page
}

void handlepost(){
	if (ledmode==1){
		ledmode=0;
		asm(" req\n"); //Q led off
	} else {
		ledmode=1;
		asm(" seq\n"); //Q led on
	}
	cmdip.l=getip();
	sendform();
	if (cmdip.l!=oldip.l){
		tweetflag=1;//indicate need for tweet
		oldip.l=cmdip.l;
	}
}
void handlesession(){	//handle a session once it's established
	unsigned int rsize,strncmpval;
	unsigned int tries=10;
	rsize=wizGetCtl16(SnRX_RSR); //get the size of the received data
	while(rsize==0 && tries-->0){
		delay(20);
		printf("re-size ");
		rsize=wizGetCtl16(SnRX_RSR); //retry size of the received data
	}
//	printf("**rsz=%d\n",rsize);
	if (rsize>0){
		if (recv0(buf,min(16,rsize))>0){ //get enough characters to distinguish the request
//			printf("%s\n",buf);
  			if (strncmp((char *)buf,"POST /",6)==0){
  				handlepost(); //toggle LED, send the form
			}
			else if (strncmp((char *)buf,"GET /favicon",12)==0){
  				sendnak(); //no favicon here
			}
  			else if (strncmp((char *)buf,"GET /",5)==0){
 				sendform(); //send the form
			}
  			else{
				printf("\nmystery meat\n");
				printf("%s\n",buf);
 				sendform(); //initialize game, send the form
			}
		}
		if (rsize>0) flush(rsize);	//get rid of the received data
	}
	wizCmd(CR_DISCON);// Disconnect the connection- we're done here
	sessions++;
}
void tweetmachine(){
//	printf ("\nTweetmachine entry sez tweetstate=%d,tweetflag=%d\n",tweetstate,tweetflag);
	switch (tweetstate){
		case 0: //pretend we're initializing
			if (tweetflag){
				tweetstate=1;
			}
			break;
		case 1: //pretend we're ready
			printf("TWEET %d.%d.%d.%d\n",cmdip.c[0],cmdip.c[1],cmdip.c[2],cmdip.c[3]);
			tweetflag=0;
			tweetstate=2;
			break;
		case 2: //pretend we're done
			tweetstate=0;
			break;
		}
//	printf ("\nTweetmachine exit sez tweetstate=%d,tweetflag=%d\n",tweetstate,tweetflag);
}
void main(void){
	unsigned char socket0status;
    unsigned char ip_addr[] = {192,168,0,182};//{169,254,180,2};//{10,0,0,180};//
    unsigned int idles=0; //count of times we've been waiting for a client
    oldip.l=0;cmdip.l=0;
    printf("Olduino Blinkenlights on W5500 VCFMW\n");
	delay(500);
    wiz_Init(ip_addr); //initialize the wiznet chip
	while(1){  // Loop forever
		socket0status=wizGetCtl8(SnSR); //socket 0 status
		switch (socket0status){
			case SOCK_CLOSED: //initial condition
				idles=0;
				socket0_init();	//initialize socket 0
				break;
			case SOCK_ESTABLISHED: //someone wants to talk to the server
				idles=0;
				handlesession();
				break;
			//following are cases where we have to reset and reopen the socket
			case SOCK_LISTEN: //waiting for a client to talk to the server
				idles++;
				if (idles>100){//if we've been idle for a bit
					tweetmachine(); //safe to spend some time
				}
				break;
			case SOCK_FIN_WAIT:
			case SOCK_CLOSING: case SOCK_TIME_WAIT:
			case SOCK_CLOSE_WAIT: case SOCK_LAST_ACK:
				idles=0;
				wizCmd(CR_CLOSE);
				break;
		}
		delay(10);
	}
}

#include <olduino.c>
#include <nstdlib.c>
#include <hspi2.c>
#include "w5500code.c"
/* aug 27
8:25:38.794> re-size re-size re-size TWEET 37.227.198.57
re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size 8:29:36.791> re-size re-size re-size re-size TWEET 69.159.24.47
re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-9:27:41.023> size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size TWEET 75.62.148.26
re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size 9:58:24.081> re-size re-size re-size re-size re-size TWEET 173.14.209.33
10:03:50.498> re-size TWEET 184.58.160.143
re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size 10:06:22.882> re-size re-size TWEET 144.130.194.161
re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-10:09:21.158> size re-size re-size re-size re-size re-size TWEET 75.67.208.86
re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size 10:13:43.865> re-size re-size re-size re-size re-size TWEET 129.42.208.174
10:41:52.232>
10:41:52.232> mystery meat
10:41:52.232> HEAD / HTTP/1.1
10:41:52.232>
10:42:03.027> re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size re-size TWEET 72.226.50.40
re-size
*/