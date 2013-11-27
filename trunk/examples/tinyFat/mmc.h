/*
	This is a trimmed down version of the SD/MMC access routines
	made by Lars Pontoppidan, Aske Olsson, Pascal Dufour and Ingo Korb
	converted from C++ to C for LCC1802 by Bill Rowe November 2013

	DISCLAIMER:
	The author is in no way responsible for any problems or damage caused
	by using this code. Use at your own risk.

	LICENSE:
	This code is distributed under the GNU Public License
	which can be found at http://www.gnu.org/licenses/gpl.txt
*/

#ifndef MMC_H
#define MMC_H
#include <nstdint.h>
#include "SD_defines.h"
#include "HW_1802_defines.h"
#define SD_SS 7	//slave select pin for sd card
const uint16_t BYTESPERSECTOR = 512;
const uint16_t SD_INIT_TIMEOUT = 2000;			/** init timeout ms */
const uint16_t SD_ERASE_TIMEOUT = 10000;		/** erase timeout ms */
const uint16_t SD_READ_TIMEOUT = 600;			/** read timeout ms */
const uint16_t SD_WRITE_TIMEOUT = 600;			/** write time out ms */
const uint16_t SD_READ_RETRIES = 5;

/* Disk Status Bits (byte) */
const byte STA_NOINIT = 0x01; 	/* Drive not initialized */
const byte STA_NODISK = 0x02; 	/* No medium in the drive */
const byte STA_PROTECT = 0x04; 	/* Write protected */


/* Results of Disk Functions */
typedef enum {
  RES_OK = 0,		/* 0: Successful */
  RES_ERROR,		/* 1: R/W Error */
  RES_WRPRT,		/* 2: Write Protected */
  RES_NOTRDY,		/* 3: Not Ready */
  RES_PARERR		/* 4: Invalid Parameter */
}
DRESULT;

/* Will be set to DISK_ERROR if any access on the card fails */
enum diskstates
{
  DISK_CHANGED = 0,
  DISK_REMOVED,
  DISK_OK,
  DISK_ERROR
};

//namespace mmc

	byte mmc_initialize(byte speed);
	byte mmc_readSector(byte *buffer, unsigned long sector);
	byte mmc_writeSector(const byte *buffer, uint32_t sector);
	uint8_t mmc_cardCommand(uint8_t cmd, uint32_t arg) ;
	void mmc_setSSpin(const uint8_t _pin);

	static	regtype	*mmc_P_SS, *mmc_P_MISO, *mmc_P_MOSI, *mmc_P_SCK;
	static	regsize	mmc_B_SS, mmc_B_MISO, mmc_B_MOSI, mmc_B_SCK;
	static	uint8_t	mmc__card_type;
	static	uint8_t	mmc__errorCode;

#endif // __MMC_H__
