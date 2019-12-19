
#ifndef     MUSIC_PLAYER_HPP_DEFINED
#define     MUSIC_PLAYER_HPP_DEFINED

#include "MusicFileReader.hpp"
#include "VS1053.hpp"
#include "PlayList.hpp"

class MusicPlayer
{
public:

    MusicPlayer( const char* root_directory_path );
    ~MusicPlayer() noexcept;

    void Start();
    void Stop();
    void PauseResume();
    void ReStart();
    void NextFile();
    void PrevFile();
    void VolumeUp();
    void VolumeDown();

    bool Update();
    void ShowStatus();

private:

    void feedData();
    void sendEndFillBytes( const uint32_t send_count );
    void playNextFile_IfReachEOF();
    
    bool m_IsPlaying;
    bool m_IsGood;
    uint8_t m_VolumeRight;
    uint8_t m_VolumeLeft;
    
    Chunk           m_ReadBuffer;
    VS1053_Drv      m_MusicDriver;
    MusicFileReader m_FileReader;
    PlayList        m_PlayList;
};

#endif    // MUSIC_PLAYER_HPP_DEFINED
