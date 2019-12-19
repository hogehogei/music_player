
#include <limits>
#include "MusicPlayer.hpp"
#include "UARTOut.hpp"
#include "Timer.hpp"

MusicPlayer::MusicPlayer( const char* root_directory_path )
    : m_IsPlaying( false ),
      m_IsGood( false ),
      m_VolumeRight( VS1053_Drv::sk_DefaultVolume ),
      m_VolumeLeft( VS1053_Drv::sk_DefaultVolume ),
      m_PlayList( root_directory_path )
{
    if( !m_MusicDriver.Initialize() ){
        exlib::sout << "VS1053 driver initialization failed!\n";
        return;
    }

    m_IsGood = true;

}

MusicPlayer::~MusicPlayer()
{}

void MusicPlayer::Start()
{
    if( !m_IsGood ){
        return;
    }
    const char* play_filename = m_PlayList.CurrentFileName();
    if( play_filename == nullptr ){
        exlib::sout << "Invalid filename.\n";
        m_IsGood = false;
        return;
    }

    if( !m_FileReader.OpenFile( play_filename ) ){
        exlib::sout << "Music player start failed!\n";
        m_IsGood = false;
        return;
    }

    m_IsPlaying = true;
    exlib::sout << "Music player start succeeded!\n";
}


void MusicPlayer::Stop()
{
    constexpr uint32_t sk_PlayingCancelOperationTimeout = 1000;       // 再生中断処理タイムアウト[ms]
    constexpr int sk_FeedDataMaxCountForWaitCancelling = 64;          // キャンセル時に送信するデータのMAX値[x 32byte]
    constexpr int sk_EndFillByteSendNum = 2052;
    
    // 再生中止フラグを立てる
    m_IsPlaying = false;
    // SM_CANCEL をセット
    m_MusicDriver.WriteSCI( VS1053_Drv::SCI_MODE, 0x0808 );

    bool is_cancel_complete= false;
    {
        MsTimer timer;
        uint16_t sci_mode_reg;
        int feeddata_cnt = 0;
        while(1){
            m_MusicDriver.ReadSCI( VS1053_Drv::SCI_MODE, &sci_mode_reg );
            if( (sci_mode_reg & 0x0008) != 0x0008 ){
                is_cancel_complete = true;
                exlib::sout << "Stop playing. Cancel Complete.\n";
                break;
            }
            if( timer.IsElapsed( sk_PlayingCancelOperationTimeout ) ){
                exlib::sout << "Stop playing timeout.\n";
                break;
            }
            if( feeddata_cnt >= sk_FeedDataMaxCountForWaitCancelling ){
                exlib::sout << "Stop playing feed datamax.\n";
                break;
            }
            
            feedData();
            ++feeddata_cnt;
        }
    }


    if( !is_cancel_complete ){
        exlib::sout << "Soft Reset.\n";
        m_MusicDriver.SoftReset();
    }
    
    sendEndFillBytes( sk_EndFillByteSendNum );
}

void MusicPlayer::PauseResume()
{
    if( m_IsPlaying ){
        exlib::sout << "PauseResume: StopPlaying.\n";
        m_IsPlaying = false;
    }
    else {
        exlib::sout << "PauseResume: StartPlaying.\n";
        m_IsPlaying = true;
    }
}

void MusicPlayer::ReStart()
{
}

void MusicPlayer::NextFile()
{
    m_PlayList.NextEntry();
    Stop();
    Start();
}

void MusicPlayer::PrevFile()
{
}

void MusicPlayer::VolumeUp()
{
    m_VolumeLeft = m_VolumeLeft > 0 ? m_VolumeLeft - 1 : 0;
    m_VolumeRight = m_VolumeRight > 0 ? m_VolumeRight - 1 : 0;

    m_MusicDriver.SetVolume( m_VolumeLeft, m_VolumeRight );
}

void MusicPlayer::VolumeDown()
{
    m_VolumeLeft = m_VolumeLeft < std::numeric_limits<uint8_t>::max() ? m_VolumeLeft + 1 : std::numeric_limits<uint8_t>::max();
    m_VolumeRight = m_VolumeRight < std::numeric_limits<uint8_t>::max() ? m_VolumeRight + 1 : std::numeric_limits<uint8_t>::max();

    m_MusicDriver.SetVolume( m_VolumeLeft, m_VolumeRight );
}

bool MusicPlayer::Update()
{
    if( !m_IsGood ){
        //exlib::sout << "Music player is invalid.\n";
        return false;
    }
    if( !m_IsPlaying ){
        //exlib::sout << "Music player has not started.\n";
        return true;
    }

    feedData();
    playNextFile_IfReachEOF();

    return true;
}

void MusicPlayer::ShowStatus()
{
    uint16_t hdat0, hdat1;
    m_MusicDriver.ReadSCI( VS1053_Drv::SCI_HDAT0, &hdat0 );
    m_MusicDriver.ReadSCI( VS1053_Drv::SCI_HDAT1, &hdat1 );
    exlib::sout << "HDAT0 : " << hdat0 << " \n";
    exlib::sout << "HDAT1 : " << hdat1 << " \n";

    uint16_t clockf;
    m_MusicDriver.ReadSCI( VS1053_Drv::SCI_CLOCKF, &clockf );
    exlib::sout << "CLOCKF :" << clockf << " \n";    
}

void MusicPlayer::feedData()
{
    if( !m_MusicDriver.IsBusy() ){
        //exlib::sout << "Update Music player.\n";
        if( m_FileReader.GetChunk( &m_ReadBuffer ) ){
            //exlib::sout << "Send music data.\n";
            m_MusicDriver.WriteSDI( m_ReadBuffer.data, m_ReadBuffer.length );
        }
    }
}

void MusicPlayer::sendEndFillBytes( const uint32_t send_count )
{
    uint8_t end_fill_byte = m_MusicDriver.ReadEndFillByte();
 
    for( uint32_t i = 0; i < send_count; ++i ){
        m_MusicDriver.WriteSDI( &end_fill_byte, 1 );
    }
}

void MusicPlayer::playNextFile_IfReachEOF()
{
    constexpr int sk_EndFillByteSendNum = 2052;
    constexpr int sk_EndFillByteCheckCancel = 32;
    constexpr int sk_SendEndFillBytesMaxForWaitEnding = 64;     // 送信終了時に送信するデータのMAX値[x 32byte]

    if( m_FileReader.IsEOF() ){
        exlib::sout << "EOF detected. Next file searching.\n";
        sendEndFillBytes( sk_EndFillByteSendNum );
        // SM_CANCEL をセット
        m_MusicDriver.WriteSCI( VS1053_Drv::SCI_MODE, 0x0808 );

        int end_fill_bytes_sendcnt = 0;
        uint16_t sci_mode_reg;
        bool is_ending_succeed = false;
        while(1){
            sendEndFillBytes( sk_EndFillByteCheckCancel );
            m_MusicDriver.ReadSCI( VS1053_Drv::SCI_MODE, &sci_mode_reg );
            if( (sci_mode_reg & 0x0008) != 0x0008 ){
                is_ending_succeed = true;
                break;
            }
            ++end_fill_bytes_sendcnt;
            if( end_fill_bytes_sendcnt >= sk_SendEndFillBytesMaxForWaitEnding ){
                break;
            }
        }

        if( !is_ending_succeed ){
            m_MusicDriver.SoftReset();
        }

        m_PlayList.NextEntry();
        Start();
    }
}