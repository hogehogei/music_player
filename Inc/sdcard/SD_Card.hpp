
#ifndef SD_CARD_HPP
#define SD_CARD_HPP

#include "I_SDC_Drv_SPI.hpp"

/**
 * @brief   SDカード読み書きクラス
 *          SDカードの読み書きを実施する。
 **/
class SD_Card
{
public:
    
    /**
     * @brief   コンストラクタ
     **/
    SD_Card();

    /**
     * @brief   デストラクタ
     **/
    ~SD_Card() noexcept;


    /**
     * @brief   SDカード初期化
     * @param [in]  driver      SDカードインターフェース
     *                          このクラスではメモリ解放責務は負わない。
     * @retval  true    初期化成功
     * @ratval  false   初期化失敗
     **/
    bool initialize( I_SDC_Drv_SPI* driver );

    /**
     * @brief   SDカード 読み込み
     * @param [in]  dst         書き込み先
     * @param [in]  sector      読み出すセクタ開始位置
     * @param [in]  offset      読み込み位置offset[byte]
     * @param [in]  len         読み込みデータ長[byte]
     * @retval  true    読み出し成功
     * @ratval  false   読み出し失敗
     **/
    bool read( uint8_t* dst, uint32_t sector, uint32_t offset, uint32_t len );

    /**
     * @brief   SDカード 書き込み開始
     *          書き込み開始コマンドを送信する
     *          本関数で書き込み開始セクタと書き込み長を指定し、writeSector() で書き込みし、writeSector_Finalize() でフラッシュする。
     *          セクタ単位の書き込みのため、余った領域は0埋めしてセクタ書き込みされる。
     *          例）900byte書き込み = 900byteデータ書き込み + 124byte 0書き込み
     *          writeSector_Finalize() がコールされるまで、書き込みは完了しない。
     * 
     * @param [in]  sector      書き込み開始セクタ
     * @param [in]  len         書き込み長[byte]
     * @retval  true    処理成功
     * @retval  false   処理失敗
     **/
    bool write_Initiate( uint32_t sector, uint32_t len );

    /**
     * @brief   SDカード書き込み
     * @param [in]  data        書き込みデータへのポインタ
     * @param [in]  len         書き込みデータ長[byte]
     * @retval  true    処理成功
     * @retval  false   処理失敗
     **/
    bool write( const uint8_t* data, uint32_t len );

    /**
     * @brief   SDカード 書き込み終了処理
     * @retval  true    処理成功
     * @retval  false   処理失敗
     **/
    bool write_Finalize();

private:

    uint8_t initialize_SDv2();
    uint8_t initialize_SDv1_or_MMCv3();

    /**
     * @brief   コマンド送信
     * @param [in]  cmd         送信コマンド
     * @param [in]  arg         送信コマンド引数
     * @return                  コマンドレスポンス一覧の値を返す
     **/
    uint8_t sendCmd( uint8_t cmd, uint32_t arg );
    void sendCmdRetry( uint8_t cmd, uint32_t arg, uint16_t retry_cnt );

    I_SDC_Drv_SPI*  m_SDC_Drv;                  //! SDカード通信ドライバ
    uint32_t        m_WriteLen;                 //! 書き込みデータ長記憶用
                                                //! writeSector_Initiate() で書き込みデータ長を記憶し
                                                //! writeSector() が呼ばれるたびに減算される。
    bool            m_IsWriteOpeProcessing;     //! 書き込み処理開始フラグ
    
    static constexpr uint8_t sk_SDC_InitDelay_ms    = 3;        //! SDC初期化ウェイト[ms]
    static constexpr uint8_t sk_SDC_InitSCLK        = 10;       //! SDC初期化カウント[x8 clock]
    static constexpr uint16_t sk_SDC_TimeOut_ms     = 1000;     //! SDCコマンドタイムアウト[ms]
};

#endif      // SD_CARD_HPP
