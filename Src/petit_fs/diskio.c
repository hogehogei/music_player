/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "sdcard/SDC_Drv_SPI.hpp"
#include "sdcard/SD_Card.hpp"

static SD_Card s_SDC;					//! SD Card class
static SDC_Drv_SPI s_SDC_Drv;			//! SD Card driver

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
	DSTATUS stat = STA_NOINIT;
	bool result = false;

	// Put your code here
	result = s_SDC.Initialize( &s_SDC_Drv );

	if( result ){
		// 初期化成功
		stat = 0;
	}

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
	BYTE* buff,		/* Pointer to the destination object */
	DWORD sector,	/* Sector number (LBA) */
	UINT offset,	/* Offset in the sector */
	UINT count		/* Byte count (bit15:destination) */
)
{
	// Put your code here
	bool result = s_SDC.Read( buff, sector, offset, count );

	return result ? RES_OK : RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/

DRESULT disk_writep (
	BYTE* buff,		/* Pointer to the data to be written, NULL:Initiate/Finalize write operation */
	DWORD sc		/* Sector number (LBA) or Number of bytes to send */
)
{
	bool result = false;

	if (!buff) {
		if (sc) {
			// Initiate write process
			result = s_SDC.WriteInitiate( sc );

		} else {
			// Finalize write process
			result = s_SDC.WriteFinalize();
		}
	} else {
		// Send data to the disk
		result = s_SDC.Write( buff, sc );
	}

	return result ? RES_OK : RES_ERROR;
}

