
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

#define MAX_BUF 512
unsigned char buf[MAX_BUF];			//memory buffer for incoming & outgoing data
int ledmode=0;	//0=off, 1=on
union IPaddr cmdip,sessip,oldip; //most recent address where a command came from, current session ip
int pages=0,sessions=0;
char strbuf[16];
void sendip(unsigned char * ip){
	int pigs=3;
	printf("\n@%x\n",&pigs);
	printf("\n%x\n",strbuf);
	//printf("\n.0=%x\n",itoa(ip[0],strbuf));
	send0s(itoa(ip[0],strbuf));
	printf("\n1/4\n");
	sendlit(".");
	//printf("\n.1=%x\n",itoa(ip[0],strbuf));
	send0s(itoa(ip[1],strbuf));
	sendlit(".");
	printf("\n1/2\n");
	send0s(itoa(ip[2],strbuf));
	sendlit(".");
	send0s(itoa(ip[3],strbuf));
	printf("\n%x\n",strbuf);
}
void sendCip(){
	sendlit("Last command from: ");
	sendip(cmdip.c);
	sendlit("<p>");

}
void sendform(){
	int sendrc;
	static char hdr[]="HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"
						"<html><body><span style=\"color:#0000A0\">\r\n"
						"<h1><center>Olduino 1802 Blinkenlights 4.41</center></h1>\r\n";

	static char postform[]="<p><form method=\"POST\">\r\n"
						"<input type=\"submit\" value=\"Toggle LED\">\r\n"
						"</form>";
	static char getform[]="<p><form method=\"GET\">\r\n"
						"<input type=\"submit\" value=\"LED Status\">\r\n"
						"</form>";
	static unsigned char olduinolink[]="<a href=\"http://goo.gl/RYbPYC\">Olduino</a>: An Arduino for the First of Us<p>";
	static char trlr[]="</body></html>\r\n\r\n";

	printf(">SF\n");
	pages+=1;
	sendrc=sendlit(hdr); 	// Now Send the HTTP Response first part
	printf("0\n");
	sendlit("Pages Served: "); send0s(itoa(pages,strbuf)); sendlit("<p>");
	printf("1\n");
	if (cmdip.c[0]!=0)
		sendCip();
	printf("2\n");
	if (ledmode==1){
		sendlit("<img src=\"http://olduino.files.wordpress.com/2015/08/15-08-27-qon.jpg\">\r\n");
		asm("	seq\n"); //insurance
	}else{
		sendlit("<img src=\"http://olduino.files.wordpress.com/2015/08/15-08-27-qoff.jpg\">\r\n"); //
		asm("	req\n"); //insurance
	}
	printf("3\n");
	sendrc=sendlit(postform); 	// Now Send the "POST" form
	sendrc=sendlit(getform); 	// Now Send the "GET" form
	sendrc=sendlit(olduinolink); 	// Now Send the link to the wordpress page
	sendrc=sendlit(trlr); 	// Now Send the rest of the page
	printf("FS>\n");
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
		printf("IP %d.%d.%d.%d\n",cmdip.c[0],cmdip.c[1],cmdip.c[2],cmdip.c[3]);
		oldip.l=cmdip.l;
	}
}

void send405(){
	sendlit("HTTP/1.1 405 Method Not Allowed\r\n\r\n"); 	// Now Send the HTTP Response
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
	printf("**rsz=%d\n",rsize);
	if (rsize>0){
		sessip.l=getip();
		if (recv0(buf,min(16,rsize))>0){ //get enough characters to distinguish the request
			printf("%s\n",buf);
  			if (strncmp((char *)buf,"POST /",6)==0){
				printf("\np\n");
  				handlepost(); //toggle LED, send the form
			}
			else if (strncmp((char *)buf,"GET /favicon",12)==0){
			printf("\nf\n");
  				sendnak(); //no favicon here
			}
  			else if (strncmp((char *)buf,"GET /",5)==0){
			printf("\ng\n");
 				sendform(); //send the form
			}
  			else{
				printf("\nmystery meat\n%s\n",buf);
				printf("IP %d.%d.%d.%d\n",sessip.c[0],sessip.c[1],sessip.c[2],sessip.c[3]);
 				send405(); //disallow oddball requests
			}
		}
		printf("flushing %d\n",rsize);
		if (rsize>0) flush(rsize);	//get rid of the received data
	}
	printf("\nd\n");
	wizCmd(CR_DISCON);// Disconnect the connection- we're done here
	printf(">\n");
	sessions++;
}
void main(void){
	unsigned char socket0status;
    unsigned char ip_addr[] = {192,168,0,182};//{169,254,180,2};//{10,0,0,180};//
    unsigned int SFWs=0; //count of times we've seen SOCK_FIN_WAIT
#include "sadump.c"	//include standalone dump print
    printf("Olduino Blinkenlights on W5500 4.4\n");
	delay(500);
    wiz_Init(ip_addr); //initialize the wiznet chip
	while(1){  // Loop forever
		socket0status=wizGetCtl8(SnSR); //socket 0 status
		//if(0x14!=socket0status){
		//	printf("%cx ",socket0status);
		//}else{
		//	printf(".");
		//}
		switch (socket0status){
			case SOCK_CLOSED: //initial condition
				SFWs=0;
				socket0_init();	//initialize socket 0
				break;
			case SOCK_ESTABLISHED: //someone wants to talk to the server
				SFWs=0;
				handlesession();
				//printf("%d sessions, %d pages\n",sessions,pages);
				break;
			//following are cases where we have to reset and reopen the socket
			case SOCK_FIN_WAIT:
				printf("SOCK_FIN_WAIT:");
				if (++SFWs>2){
					printf(" lost patience, closing\n");
					wizCmd(CR_CLOSE);
				}else{
					printf(" ignoring\n");
				}
				break;
			case SOCK_CLOSING: case SOCK_TIME_WAIT:
			case SOCK_CLOSE_WAIT: case SOCK_LAST_ACK:
				SFWs=0;
				wizCmd(CR_CLOSE);
				break;
		}
		delay(20);
	}
}

#include <olduino.c>
#include <nstdlib.c>
#include <hspi2.c>
#include "w5500code.c"
