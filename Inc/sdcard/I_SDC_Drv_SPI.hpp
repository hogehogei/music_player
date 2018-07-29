
#ifndef SDC_DRV_SPI_HPP
#define SDC_DRV_SPI_HPP

#include <stdint.h>

// コマンドレスポンス一覧
// 0以外はエラーを表している
constexpr uint8_t k_RES_OK          = 0; 
constexpr uint8_t k_RES_InIdleState = (1 << 0);
constexpr uint8_t k_RES_EraseReset  = (1 << 1);
constexpr uint8_t k_RES_IllegalCmd  = (1 << 2);
constexpr uint8_t k_RES_Cmd_CRCErr  = (1 << 3);
constexpr uint8_t k_RES_EraseSeqErr = (1 << 4);
constexpr uint8_t k_RES_AddrErr     = (1 << 5);
constexpr uint8_t k_RES_ParamErr    = (1 << 6);

/**
 * @brief   SPI SDカード通信インターフェースクラス
 *          SPIを用いてSDカードと通信をする際に使用する機能のインターフェース
 **/
class I_SDC_Drv_SPI
{
public:
    virtual ~I_SDC_Drv_SPI() noexcept {}

    /**
     * @brief   chipselect Hi
     **/
    virtual void CS_Hi() = 0;

    /**
     * @brief   chipselect Lo
     **/
    virtual void CS_Lo() = 0;

    /**
     * @brief   データ送信
     *          SPIでデータ送信する
     * @param [in]  data        送信データ
     * @param [in]  len         送信データ長
     * @retval  true            送信処理実行。バックグラウンドで送信する実装も考えられるので（DMAとか？）
     *                          処理が完遂したことを確認するには sendTransEnd() をコールすること。
     * @retval  false           送信処理失敗。トランザクションが開始されていないか、他の処理でブロッキング中であることも考えられる。
     **/
    virtual bool send( const uint8_t* data, uint32_t len ) = 0;

    /**
     * @brief   データ送信処理終了確認
     * @retval  true            送信処理完了
     * @retval  false           送信処理未完了
     **/
    virtual bool sendTransEnd() = 0;

    /**
     * @brief   データ受信
     *          SPIでデータ受信する
     * @param [out] data        受信データの格納先
     * @param [in]  len         受信データ長
     * @retval  true            受信処理実行。バックグラウンドで送信する実装も考えられるので（DMAとか？）
     *                          処理が完遂したことを確認するには transEnd() をコールすること。
     * @retval  false           受信処理失敗。トランザクションが開始されていないか、他の処理でブロッキング中であることも考えられる。
     **/
    virtual bool recv( uint8_t* data, uint32_t len ) = 0;

    /**
     * @brief   データ受信処理終了確認
     * @retval  true            受信処理完了
     * @retval  false           受信処理未完了
     **/
    virtual bool recvTransEnd() = 0;
};

#endif      // SDC_DRV_SPI_HPP
