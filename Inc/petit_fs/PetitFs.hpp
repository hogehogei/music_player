/*
 * PetitFs.hpp
 *
 *  Created on: 2019/07/20
 *      Author: hogehogei
 */

#ifndef DRV_PETIT_FS_PETITFS_HPP_
#define DRV_PETIT_FS_PETITFS_HPP_

#include <cstdint>
#include "petit_fs/pff.h"

namespace petitfs
{
	class FileInfo;
	class Dir;
	class FileSys
	{
	public:

		static FileSys& Instance();

		FRESULT Mount();
		FRESULT OpenFile( const char* filepath );
		FRESULT OpenFile( const FileInfo& fileinfo );
		FRESULT Read( uint8_t* dst, uint16_t byte_to_read, uint16_t* read_len );
		FRESULT Write( const uint8_t* src, uint16_t byte_to_write, uint16_t* write_len );
		FRESULT Seek( uint32_t offset );
		Dir OpenDirectory( const char* filepath );
		Dir OpenDirectory( const FileInfo& fileinfo );

	private:

		FATFS m_FatFileSys;
	};

	class Dir
	{
	public:

		Dir( const char* filepath );

		FRESULT Status() const;
		bool IsOK() const;
		FileInfo NextEntry();
		bool Reset();

	private:

		DIR m_Directory;
		FRESULT m_Status;
	};

	class FileInfo
	{
	public:

		FileInfo();
		bool IsValid() const;
		bool IsDirectory() const;
		bool IsFile() const;
		const char* Name() const;

	private:

		friend class Dir;
		FileInfo( const FILINFO& info );

		FILINFO m_FileInfo;
		bool m_IsValid;
	};
}


#endif /* DRV_PETIT_FS_PETITFS_HPP_ */
