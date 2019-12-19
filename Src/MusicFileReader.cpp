
#include "MusicFileReader.hpp"
#include <algorithm>
#include "UARTOut.hpp"

MusicFileReader::ReadBuffer::ReadBuffer()
 : m_Length( 0 ),
   m_ReadIndex( 0 ),
   m_IsEOF( false )
{}

MusicFileReader::ReadBuffer::~ReadBuffer() 
{}

bool MusicFileReader::ReadBuffer::IsEnd() const
{
    return m_IsEOF && isBufferEmpty();
}

bool MusicFileReader::ReadBuffer::GetChunk( Chunk* chunk )
{
    if( isBufferEmpty() ){
        //exlib::sout << "Buffer empty. Read new sector.\n";
        if( !readNewSector() ){
            //exlib::sout << "Read new sector failed.\n";
        }
    }

    if( IsEnd() ){
        exlib::sout << "IsEND detected.\n";
        return false;
    }

    readCurrentChunk( chunk );

    return true;
}

bool MusicFileReader::ReadBuffer::readNewSector()
{
    petitfs::FileSys& filesys = petitfs::FileSys::Instance();
    uint16_t readlen = 0;

    if( filesys.Read( m_Buffer, sk_SectorSize, &readlen ) != FR_OK ){
        m_IsEOF  = true;
        m_Length = 0;
        m_ReadIndex = 0;
        return false;
    }

    //exlib::sout << "Read sector size : " << static_cast<uint16_t>(readlen) << " \n";
    if( readlen == 0 ){
        exlib::sout << "EOF detected\n";
        m_IsEOF = true;
    }
    m_Length    = readlen;
    m_ReadIndex = 0;

    return true;
}

bool MusicFileReader::ReadBuffer::isBufferEmpty() const
{
    return m_ReadIndex >= m_Length;
}

void MusicFileReader::ReadBuffer::readCurrentChunk( Chunk* chunk )
{
    // ChunkSize分 か 最終データのどちらかまでコピー
    uint32_t copylast = std::min( m_ReadIndex + Chunk::sk_ChunkSize, m_Length );
    std::copy( &m_Buffer[m_ReadIndex], &m_Buffer[copylast], chunk->data );

    chunk->length = copylast - m_ReadIndex;
    m_ReadIndex = copylast;
}

MusicFileReader::MusicFileReader()
{}

MusicFileReader::~MusicFileReader()
{}

bool MusicFileReader::OpenFile( const char* filepath )
{
    return petitfs::FileSys::Instance().OpenFile( filepath ) == FR_OK;
}

bool MusicFileReader::IsEOF() const
{
    return m_Buffer.IsEnd();
}

bool MusicFileReader::GetChunk( Chunk* chunk )
{
    return m_Buffer.GetChunk( chunk );
}