
#ifndef SD_CARD_HPP
#define SD_CARD_HPP

#include "I_SDC_Drv_SPI.hpp"

/**
 * @brief   SDカード読み書きクラス
 *          SDカードの読み書きを実施する。
 *
 *          読み書きのためのバッファを最小限にしていることが特徴。
 *          そのため、読み書きの速度を犠牲にしていたり、読み書き手順が少し煩雑。
 *          petit_fs を使うことを想定しているため、上位でバッファを持たせるなどして煩雑さをラップする思想。
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
    bool Initialize( I_SDC_Drv_SPI* driver );

    /**
     * @brief   SDカード 読み込み
     * @param [out] dst         書き込み先
     * @param [in]  sector      読み出すセクタ開始位置
     * @param [in]  offset      読み込み位置offset[byte]
     * @param [in]  len         読み込みデータ長[byte]
     * @retval  true    読み出し成功
     * @ratval  false   読み出し失敗
     **/
    bool Read( uint8_t* dst, uint32_t sector, uint32_t offset, uint32_t len );

    /**
     * @brief   SDカード書き込み開始
     *          Write() で必要なデータを書き込み、WriteFinalize() をコールすることで書き込みが完了する。
     * @param [in]  sector      書き込みセクタ開始位置
     * @retval  true    処理成功
     * @retval  false   処理失敗
     **/
    bool WriteInitiate( uint32_t sector );

    /**
     * @brief   SDカード書き込み
     * @param [in]  data        書き込みデータへのポインタ
     * @param [in]  len         書き込みデータ長[byte]
     * @retval  true    処理成功
     * @retval  false   処理失敗
     **/
    bool Write( const uint8_t* data, uint32_t len );

    /**
     * @brief   書き込み終了処理
     * @retval  true    処理成功
     * @retval  false   処理失敗
     **/
    bool WriteFinalize();

    /**
     * @brief   状態取得
     * @retval  true    読み書きが可能
     * @retval  false   状態が不正なため読み書きできない。Reset()をコールして再初期化することで読み書き可能になる。
     **/
    bool State() const;

    static constexpr uint32_t sk_SectorSize         = 512;      //! セクタサイズ

private:

    class Progress
    {
    public:
        Progress();
        uint32_t addr;
        uint32_t remain_offset;
        uint32_t remain_sector;
        uint32_t remain_bytes_cursec;
        uint32_t remain_len;
    };


    uint8_t initialize_SDv2();
    uint8_t initialize_SDv1_or_MMCv3();
    uint8_t sendCmd( uint8_t cmd, uint32_t arg );
    uint8_t sendCmdRetry( uint8_t cmd, uint32_t arg, uint16_t retry_cnt );

    bool readInitiate( Progress* progress, uint32_t sector, uint32_t offset, uint32_t len );
    bool nextSectorReadPreparation( Progress* progress );
    bool waitReadDataPacket();
    bool nextSectorWritePreparation();
    void advanceNextSector( Progress* progress );

    void busyWait();
    void ignoreRead( uint32_t cnt );
    void zeroWrite( uint32_t cnt );

    I_SDC_Drv_SPI*  m_SDC_Drv;                  //! SDカード通信ドライバ
    Progress        m_W_Progress;               //! 書き込み処理 進捗管理
    bool            m_SDC_State;
    bool            m_IsWriteOpeProcessing;     //! 書き込み処理開始フラグ
    uint8_t         m_CardType;
};

#endif      // SD_CARD_HPP
