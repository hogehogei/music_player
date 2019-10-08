/*
 * PetitFs.cpp
 *
 *  Created on: 2019/07/20
 *      Author: hogehogei
 */

#include "petit_fs/PetitFs.hpp"

namespace petitfs
{
	FileSys& FileSys::Instance()
	{
		static FileSys s_Instance;
		return s_Instance;
	}

	FRESULT FileSys::Mount()
	{
		return pf_mount( &m_FatFileSys );
	}

	FRESULT FileSys::OpenFile( const char* filepath )
	{
		return pf_open( filepath );
	}

	FRESULT FileSys::OpenFile( const FileInfo& fileinfo )
	{
		return pf_open( fileinfo.Name() );
	}

	FRESULT FileSys::Read( uint8_t* dst, uint16_t byte_to_read, uint16_t* read_len )
	{
		UINT len;
		FRESULT result = pf_read( dst, static_cast<UINT>(byte_to_read), &len );
		*read_len = len;

		return result;
	}

	FRESULT FileSys::Write( const uint8_t* src, uint16_t byte_to_write, uint16_t* write_len )
	{
		UINT len;
		FRESULT result = pf_write( src, static_cast<UINT>(byte_to_write), &len );
		*write_len = len;

		return result;
	}

	FRESULT FileSys::Seek( uint32_t offset )
	{
		return pf_lseek( offset );
	}

	Dir FileSys::OpenDirectory( const char* filepath )
	{
		return Dir( filepath );
	}

	Dir FileSys::OpenDirectory( const FileInfo& fileinfo )
	{
		return Dir( fileinfo.Name() );
	}


	Dir::Dir( const char* filepath )
		: m_Directory()
	{
		m_Status = pf_opendir( &m_Directory, filepath );
	}

	FRESULT Dir::Status() const
	{
		return m_Status;
	}

	bool Dir::IsOK() const
	{
		return m_Status == FR_OK;
	}

	FileInfo Dir::NextEntry()
	{
		FILINFO info;
		FRESULT result = pf_readdir( &m_Directory, &info );

		if( result != FR_OK || info.fname[0] == '\0' ){
			return FileInfo();
		}

		return FileInfo( info );
	}

	bool Dir::Reset()
	{
		return pf_readdir( &m_Directory, nullptr );
	}

	FileInfo::FileInfo()
		: m_FileInfo(),
		  m_IsValid( false )
	{
	}

	FileInfo::FileInfo( const FILINFO& info )
		: m_FileInfo( info ),
		  m_IsValid( true )
	{
	}

	bool FileInfo::IsValid() const
	{
		return m_IsValid;
	}

	bool FileInfo::IsDirectory() const
	{
		return (m_FileInfo.fattrib & AM_DIR) == AM_DIR;
	}

	bool FileInfo::IsFile() const
	{
		return !IsDirectory();
	}

	const char* FileInfo::Name() const
	{
		return m_FileInfo.fname;
	}
}

