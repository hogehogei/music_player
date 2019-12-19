
#ifndef MUSIC_FILE_READER_HPP
#define MUSIC_FILE_READER_HPP

#include "petit_fs/PetitFs.hpp"

struct Chunk
{
    static constexpr int sk_ChunkSize = 32;
    uint8_t data[sk_ChunkSize];
    int length;
};

class MusicFileReader
{
private:

    class ReadBuffer
    {
    public:

        ReadBuffer();
        ~ReadBuffer() noexcept;

        bool IsEnd() const;
        bool GetChunk( Chunk* chunk );

    private:

        bool readNewSector();
        bool isBufferEmpty() const;
        void readCurrentChunk( Chunk* chunk );

        static constexpr int sk_SectorSize = 512;

        uint8_t  m_Buffer[sk_SectorSize];
        uint32_t m_Length;
        uint32_t m_ReadIndex;
        bool     m_IsEOF;
    };

public:

    MusicFileReader();
    ~MusicFileReader() noexcept;

    bool OpenFile( const char* filepath );
    //bool IsOpenedFile() const;
    bool IsEOF() const;
    bool GetChunk( Chunk* chunk );

private:

    ReadBuffer          m_Buffer;
};

#endif      // MUSIC_FILE_READER_HPP