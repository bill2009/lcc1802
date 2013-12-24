//MMC mule for testing SD/MMC card over spi
//Nov 22 2013, bill rowe
#include <nstdlib.h>
#include <olduino.h>
#define SDSS 7	//active low slave select for SD card
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long int uint32_t;
unsigned char spixfer(unsigned char movalue){//now using outboard clock
	asm("	glo 12\n"
		"	dec 2\n"
		"	str 2\n"
		"	out 6\n"
		"	dec 2\n"
		"	sex 2\n" "	sex 2\n" "	sex 2\n" "	sex 2\n" "	sex 2\n" "	sex 2\n" "	sex 2\n" "	sex 2\n"
		"	sex 2\n" "	sex 2\n" "	sex 2\n" "	sex 2\n" "	sex 2\n" "	sex 2\n" "	sex 2\n" "	sex 2\n"
		"	inp 6\n"
		"	plo 15\n"
		"	inc 2\n"
		"	cretn\n");
	//warning the return below is NOT executed. It just prevents a compiler warning
	//the cretn inside the asm block above returns the correct value from the spi transfer
	//sorry.
	return 0;
}
unsigned char spixferAsWas(unsigned char movalue){
	asm("	glo 12\n"
		"	dec 2\n"
		"	str 2\n"
		"	out 6\n"
		"	sex 3\n"
		"	out 2\n	db 00\n"
		"	out 2\n	db 00\n"
		"	out 2\n	db 00\n"
		"	out 2\n	db 00\n"
		"	out 2\n	db 00\n"
		"	out 2\n	db 00\n"
		"	out 2\n	db 00\n"
		"	out 2\n	db 00\n"
		"	sex 2\n"
		"	dec 2\n"
		"	inp 6\n"
		"	plo 15\n"
		"	inc 2\n"
		"	cretn\n");
	//warning the return below is NOT executed. It just prevents a compiler warning
	//the cretn inside the asm block above returns the correct value from the spi transfer
	//sorry.
	return 0;
}
void mmc_clock_and_release(void)
{
    uint8_t i;

    // SD cards require at least 8 final clocks
    for(i=0;i<10;i++)
        (void)spixfer(0xff);

    digitalWrite(SDSS,HIGH);   // release CS
}
void mmc_send_command(uint8_t command, uint16_t px, uint16_t py)
{
	unsigned char x; //dummy to hold return value
	uint16_t pz; //temp to hold transmitted bottom byte of py;
    digitalWrite(SDSS,LOW);  // enable CS
	printf(" sendcmd %x %x %x\n",command,px,py);
    x=spixfer(0xff);         // dummy byte

    x=spixfer(command | 0x40);

    x=spixfer(px>>8); // high byte of param x
    x=spixfer(px&0xff);  // low byte of param x

    x=spixfer(py>>8); // high byte of param y
    pz=py&0xff;
	printf("<%x,%x>",py,pz);
    x=spixfer(pz);  // low byte of param y
    x=spixfer(0x95);         // correct CRC for first command in SPI
                            // after that CRC is ignored, so no problem with
                            // always sending 0x95
    x=spixfer(0xff);         // ignore return byte
}
uint8_t mmc_get(void)
{
    uint16_t i = 0xff;
    uint8_t b = 0xff;

    while ((b == 0xff) && (--i))
    {
        b = spixfer(0xff);
    }
	if (b==0xff){
		printf("\nmmc_get timeout\n");
	}
    return b;
}


uint8_t mmc_init(void)
{
    unsigned int i,retval;
	digitalWrite(SDSS,HIGH)	;
    for(i=0;i<10;i++)           // send 80 clocks while card power stabilizes
        (void)spixfer(0xff);

    mmc_send_command(0,0,0);    // send CMD0 - reset card
  	retval=mmc_get();

    if (retval != 1)         // if no valid response code
    {
		printf("\nmmc init cmd 0 failed with code %d (should have been 1)\n");
       	mmc_clock_and_release();
       	return 1;                // card cannot be detected
    }

    //
    // send CMD1 until we get a 0 back, indicating card is done initializing
    //
    i = 0xff;                     // max timeout
    while ((spixfer(0xff) != 0) && (--i))  // wait for it
    {
         mmc_send_command(1,0,0);   // send CMD1 - activate card init
    }

    mmc_clock_and_release();        // clean up

    if (i == 0){                     // if we timed out above
    	printf("\nmmc cmd 1 failed\n");
       	return 2;                    // return failure code
	}

    return 0;
}


uint8_t mmc_datatoken(void)
{
    uint16_t i = 0xffff;
    uint8_t b = 0xff;

    while ((b != 0xfe) && (--i))
    {
        b = spixfer(0xff);
    }
    return b;
}


int mmc_readsector(uint32_t lba, uint8_t *buffer)
{
    uint16_t i,retval;
	printf("\nreadsector\n");
    // send read command and logical sector address
    mmc_send_command(17,(lba>>7) & 0xffff, (lba<<9) & 0xffff);
    //mmc_send_command(17,(lba>>16) & 0xffff, (lba) & 0xffff); //wjr
	retval=mmc_get();
	printf("\ngot rc %d\n",retval);

    if (mmc_datatoken() != 0xfe)    // if no valid token
    {
        mmc_clock_and_release();    // cleanup and
        return -1;                  // return error code
    }

    for (i=0;i<512;i++)             // read sector data
        *buffer++ = spixfer(0xff);

    (void)spixfer(0xff);                 // ignore dummy checksum
    (void)spixfer(0xff);                 // ignore dummy checksum

    mmc_clock_and_release();        // cleanup

    return 0;                       // return success
}
int isalpha(int c)
{
        return((c >='a' && c <='z') || (c >='A' && c <='Z'));
}
void dump(uint8_t * p, uint16_t len)
{
    int i,j;

    for (i=0;i<len/16;i++)
    {
        printf("%x  ",i*16);
        for (j=0;j<16;j++)
            printf("%cx ",p[i*16+j]);
        printf("  ");
        for (j=0;j<16;j++)
            printf("%c", isalpha(p[i*16+j]) ? p[i*16+j] : '.');
        printf("\n");
    }
}

void main(){
	unsigned int i,px,py,pz,retval[32];
    uint32_t sector = 0;
    uint8_t sectorbuffer[512];
    //py=0xdc00;
    //pz=py&0xff;
	//printf("<%x,%x>",py,pz);
	//while(1);
    //sector=0xee;
    //px=(sector>>7) & 0xffff;
    //py=(sector<<9) & 0xffff;
	//printf("%lx, %x, %x\n",sector,px,py);
   	//while(1);
	//retval[0]=mmc_readsector(0xee,sectorbuffer);    // read a data sector
	printf("MMC mule sez Hello World\n");
//while(1);
	printf("trying mmc_init\n");
	retval[0]=mmc_init();
	printf("\nreturn code is %d\n",retval[0]);

	if (retval[0]==0){
		printf("trying readsector %l\n",sector);
		retval[0]=mmc_readsector(sector,sectorbuffer);    // read a data sector
		printf("return code=%d\n",retval[0]);
		//dump(sectorbuffer,512);                 // dump sector contents
		sector=(0xed)+1+121+121+32;
		printf("trying readsector %lx\n",sector);
		retval[0]=mmc_readsector(sector,sectorbuffer);    // read a data sector
		printf("return code=%d\n",retval[0]);
		dump(sectorbuffer,512);                 // dump sector contents
	}

/*
	digitalWrite(SDSS,HIGH); //to get into MMC mode we send 80 clocks with SS high
	for (i=10;i!=0;i--){ 	//10 repeats
		retval[0]=spixfer(0xff);		//of clocks with MOSI high
	}
	digitalWrite(SDSS,LOW);	//now SS is active low
	retval[31]=spixfer(0xff);
	retval[0]=spixfer(0x40); //cmd0 - reset
	retval[1]=spixfer(0x00); //arg1
	retval[2]=spixfer(0x00); //arg2
	retval[3]=spixfer(0x00); //arg3
	retval[4]=spixfer(0x00); //arg4
	retval[5]=spixfer(0x95); //CRC
	retval[6]=spixfer(0xff); //waiting for response
	retval[7]=spixfer(0xff); //getting response
	retval[8]=spixfer(0xff); //
	retval[9]=spixfer(0xff); //
	retval[10]=spixfer(0xff); //
	retval[11]=spixfer(0xff); //
	retval[12]=spixfer(0xff); //
	retval[13]=spixfer(0xff); //
	for (i=0;i<9;i++){
		printf("%x ",retval[i]);
	}
	printf("\ntrying cmd 1: ");
	retval[31]=spixfer(0xff);
	retval[0]=spixfer(0x41); //cmd1 - initialization
	retval[1]=spixfer(0x00); //arg1
	retval[2]=spixfer(0x00); //arg2
	retval[3]=spixfer(0x00); //arg3
	retval[4]=spixfer(0x00); //arg4
	retval[5]=spixfer(0x95); //CRC
	retval[6]=spixfer(0xff); //waiting for response
	retval[7]=spixfer(0xff); //getting response
	retval[8]=spixfer(0xff); //
	retval[9]=spixfer(0xff); //
	retval[10]=spixfer(0xff); //
	retval[11]=spixfer(0xff); //
	retval[12]=spixfer(0xff); //
	retval[13]=spixfer(0xff); //
	retval[14]=spixfer(0xff); //
	retval[15]=spixfer(0xff); //
	for (i=0;i<16;i++){
		printf("%x ",retval[i]);
	}
	printf("\ntrying cmd 1(again): ");
	retval[31]=spixfer(0xff);
	retval[0]=spixfer(0x41); //cmd1 - initialization
	retval[1]=spixfer(0x00); //arg1
	retval[2]=spixfer(0x00); //arg2
	retval[3]=spixfer(0x00); //arg3
	retval[4]=spixfer(0x00); //arg4
	retval[5]=spixfer(0x95); //CRC
	retval[6]=spixfer(0xff); //waiting for response
	retval[7]=spixfer(0xff); //getting response
	retval[8]=spixfer(0xff); //
	retval[9]=spixfer(0xff); //
	retval[10]=spixfer(0xff); //
	retval[11]=spixfer(0xff); //
	retval[12]=spixfer(0xff); //
	retval[13]=spixfer(0xff); //
	retval[14]=spixfer(0xff); //
	retval[15]=spixfer(0xff); //
	for (i=0;i<16;i++){
		printf("%x ",retval[i]);
	}
*/
	printf("\n");
	printf("\nand we're done\n");
}

#include <nstdlib.c>
#include <olduino.c>
