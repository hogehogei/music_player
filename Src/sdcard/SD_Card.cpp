
#include "SD_Card.hpp"
#include <limits>
#include <algorithm>

constexpr uint8_t k_CMD0 = (0x40 + 0);
constexpr uint8_t k_CMD8 = (0x40 + 8);

constexpr uint32_t k_ACMD41_HCS = (1UL << 30);
constexpr uint32_t k_ACMD41_CCS = (0x04);

// read() / write() を引数 sector から byte に変換するときに
// sector がとりうることのできる最大値
constexpr uint32_t k_MaxSector_ByteAccess = (std::numeric_limits<uint32_t>::max() / sk_SectorSize);

SD_Card::SD_Card()
 : m_SDC_Drv( nullptr ),
   m_WriteLen( 0 ),
   m_IsWriteOpeProcessing( false )
{}

SD_Card::~SD_Card()
{}


bool SD_Card::initialize( I_SDC_Drv_SPI* driver )
{
    // ドライバインターフェースを設定
    if( !driver ){
        return false;
    }
    m_SDC_Drv = driver;

    // CSをHレベルに設定
    m_SDC_Drv->CS_Hi();
    // DIはハードでプルアップしているので処理なし

    // 1ms以上待つ
    HAL_Delay( sk_SDC_InitDelay_ms );

    for( int i = 0; i < sk_SDC_InitSCLK; ++i ){
        // SPI 初期化クロック送信
        uint8_t t = 0;
        m_SDC_Drv->send( &t, 1 );
    }

    // CSをLレベルに設定
    m_SDC_Drv->CS_Lo();
    
    uint8_t type = sk_SDC_Type_None;
    // CMD0 初期化コマンド送信
    if( sendCmd( k_CMD0, 0 ) == K_RES_InIdleState ){
        // CMD8 SDv2 専用コマンドを送信して SDv2 かそれ以外かを確かめる
        if( sendCmd( k_CMD8, 0x1AA ) == k_RES_InIdleState ){
            // SDv2
            type = initialize_SDv2();
        }
        else {
            // CMD8 がリジェクトされたので SDv1/MMC の初期化を試す
            type = initialize_SDv1_or_MMCv3();
        }
    }

    // CSをHレベルに設定
    m_SDC_Drv->CS_Hi();

    return type == sk_SDC_Type_None ? false : true;
}

bool SD_Card::read( uint8_t* dst, uint32_t sector, uint32_t offset, uint32_t len )
{
    if( len == 0 ){
        return true;
    }
    if( !dst || offset >= sk_SectorSize ){
        return false;
    }
    // アクセス単位がbyte単位の場合に、上限値を超えたらエラー
    if( !(m_CardType & sk_SDC_Block) &&
        (sector > k_MaxSector_ByteAccess)) ){
        return false;
    }

    // @todo len > sk_SectorSize の場合はマルチブロックリードのほうが良い

    bool result = true;
    // 開始アドレスを算出
    uint32_t addr = m_CardType & sk_SDC_Block ? sector : sector * sk_SectorSize;
    // 総読み込みセクタを算出　1セクタより小さい値の場合でも、1セクタは読み込む
    uint32_t remain_sector  = ((len + offset - 1) / sk_SectorSize) + 1;
    uint32_t remain_secbyte = 0;
    uint32_t remain_offset  = offset;
    uint32_t remain_read    = len;

    // 3つの場合に分けて考える
    // step1: offset だけ先頭データを読み飛ばす
    // step2: 必要なデータをコピーする
    // step3: 必要なデータをすべて読み終わった後、最終セクタの余ったデータを読み飛ばす
    while( result && remain_sector > 0 ){
        // 今回のセクタのデータを読み終わったら、次のセクタを読む
        if( remain_secbyte == 0 ){
            if( sendCmd( k_CMD17, addr ) == k_RES_OK ){
                remain_secbyte = sk_SectorSize;
                --remain_sector;
            }
            else {
                result = false;
                break;
            }
        }

        if( remain_offset > 0 ){
            // step1
            result = ignoreRead( offset );
            remain_secbyte -= offset;
            remain_offset = 0;
        }
        else if( remain_read > 0 ){
            // step2
            uint8_t size = std::min( remain_read, remain_secbyte );
            result = m_SDC_Drv->read( dst, size );
            dst += size;
            remain_secbyte -= size;
            remain_read -= size;
        }
        else if( remain_secbyte > 0 ){
            // step3 
            result = ignoreRead( remain_secbyte );
            remain_secbyte = 0;
        }
        else {
            result = false;
        }
    }

    return result;
}

uint8_t SD_Card::initialize_SDv2()
{
    uint8_t buf[4];
    uint8_t type = sk_SDC_Type_None;

    // CMD8が通ったので、コマンドの追加引数を受信
    m_SDC_Drv->recv( buf, sizeof(buf) );
    while( !m_SDC_Drv->recvTransEnd() );

    // 下位12bit が0x1AAなら、SDCv2 で2.7V～3.6Vで動作可能
    if( buf[2] == 0x01 && buf[3] == 0xAA ){
        // 初期化コマンド送信　1秒間リトライする
        uint8_t res = sendCmdRetry( k_ACMD41, k_ACMD41_HCS, sk_SDC_TimeOut_ms );

        // OCR読み出しコマンド送信
        if( (res == k_RES_OK) && (sendCmd( k_CMD58, 0 ) == k_RES_OK) ){
            // OCR受信
            m_SDC_Drv->recv( buf, sizeof(buf) );
            while( !m_SDC_Drv->recvTransEnd() );

            if( buf[0] & k_ACMD41_CCS ){
                type = sk_SDC_Type_SD2 | sk_SDC_Block;      // アクセスはブロック単位
            }
            else {
                type = sk_SDC_Type_SD2;                     // アクセスはbyte単位
            }
        }
    }

    return type;
}

uint8_t SD_Card::initialize_SDv1_or_MMCv3()
{
    uint8_t type = sk_SDC_Type_None;
    uint8_t cmd = 0;

    // ACMD41 SDC専用の初期化コマンドを試す
    if( sendCmd( k_ACMD41, 0 ) <= k_RES_InIdleState ){
        // ACMD41が通った。CMD8は通らず SDv2 ではなかったので、SDv1
        cmd = k_ACMD41;
        type = sk_SDC_Type_SD1;
    }
    else {
        // ACMD41がリジェクトされたので、MMCと仮定
        cmd = k_CMD1;
        type = sk_SDC_Type_MMC;
    }

    // 初期化コマンド送信
    uint8_t res = sendCmdRetry( cmd, 0, sk_SDC_TimeOut_ms );
    // セクタサイズを512byteに固定
    if( (res != k_RES_OK) || (sendCmd( k_CMD16, sk_SectorSize ) != k_RES_OK) ){
        type = sk_SDC_Type_None;
    }

    return type;
}

uint8_t SD_Card::sendCmd( uint8_t cmd, uint32_t arg )
{
    uint8_t buf[6];
    uint8_t res;

    // byte0 はコマンド
    // byte1～4に引数（ビッグエンディアン）
    buf[0] = cmd;
    buf[1] = (arg >> 24) & 0xFF;
    buf[2] = (arg >> 16) & 0xFF;
    buf[3] = (arg >>  8) & 0xFF;
    buf[4] = arg & 0xFF;
    // byte5 ダミーのCRC7  SPIの場合はCRCチェックはなくてもよい（ダミーデータは必要）
    buf[5] = 0x01;
    // CMD0, CMD8 のCRC7は必要なので、決め打ち
    if( cmd == k_CMD0 ){
        buf[5] = 0x95;
    }
    if( cmd == k_CMD8 ){
        buf[5] = 0x87;
    }

    m_SDC_Drv->send( buf, sizeof(buf) );
    while( !m_SDC_Drv->sendTransEnd() );

    // コマンドレスポンスを待つ
    uint8_t retry = 10;
    do {
        res = m_SDC_Drv->recv( &res, 1 );
        while( !m_SDC_Drv->recvTransEnd() );

    } while( (res & 0x80) && --retry );

    return res;
}

uint8_t SD_Card::sendCmdRetry( uint8_t cmd, uint32_t arg, uint16_t retry_cnt )
{
    uint8_t res = k_RES_OK;

    // 設定された回数コマンド送信をリトライ
    // リトライの間隔は 1ms
    for( uint16_t i = 0; i < retry_cnt; ++i ){
        res = sendCmd( cmd, arg );
        if( res == k_RES_OK ){
           break;
        }
        HAL_Delay( 1 );             // 1ms待つ
    }

    return res;
}

