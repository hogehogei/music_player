/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/

#include "diskio.h"



/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
	DSTATUS stat = STA_NOINIT;
	bool result = false;

	// Put your code here
	result = s_SDC.initialize( &s_SDC_Drv );

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

	if( (offset + count) > 512 ){
		return RES_ERROR;
	}

	bool result = s_SDC.readSector( buff, sector, offset, count );

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
			result = s_SDC.writeSector_Initiate( sc, 512 );

		} else {
			// Finalize write process
			result = s_SDC.writeSector_Finalize();
		}
	} else {
		// Send data to the disk
		result = s_SDC.writeSector( buff, sc );
	}

	return result ? RES_OK : RES_ERROR;
}

