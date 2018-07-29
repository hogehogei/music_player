
#ifndef SDC_DRV_SPI_HPP
#define SDC_DRV_SPI_HPP

#include "I_SDC_Drv_SPI.hpp"

class SDC_Drv_SPI : public I_SDC_Drv_SPI
{
public:
    
    /**
     * @brief   コンストラクタ
     **/
    SDC_Drv_SPI();

    /**
     * @brief   デストラクタ
     **/
    virtual ~SDC_Drv_SPI() noexcept {}

    /**
     * @brief   chipselect Hi
     **/
    virtual void CS_Hi() override;

    /**
     * @brief   chipselect Lo
     **/
    virtual void CS_Lo() override;

    /**
     * @brief   データ送信
     *          SPIでデータ送信する
     * @param [in]  data        送信データ
     * @param [in]  len         送信データ長
     * @retval  true            送信処理実行。バックグラウンドで送信する実装も考えられるので
     *                          処理が完遂したことを確認するには sendTransEnd() をコールすること。
     * @retval  false           送信処理失敗。トランザクションが開始されていないか、他の処理でブロッキング中であることも考えられる。
     **/
    virtual bool send( const uint8_t* data, uint32_t len ) override;

    /**
     * @brief   データ送信処理終了確認
     * @retval  true            送信処理完了
     * @retval  false           送信処理未完了
     **/
    virtual bool sendTransEnd() override;

    /**
     * @brief   データ受信
     *          SPIでデータ受信する
     * @param [out] data        受信データの格納先
     * @param [in]  len         受信データ長
     * @retval  true            受信処理実行。バックグラウンドで送信する実装も考えられるので
     *                          処理が完遂したことを確認するには transEnd() をコールすること。
     * @retval  false           受信処理失敗。トランザクションが開始されていないか、他の処理でブロッキング中であることも考えられる。
     **/
    virtual bool recv( uint8_t* data, uint32_t len ) override;

    /**
     * @brief   データ受信処理終了確認
     * @retval  true            受信処理完了
     * @retval  false           受信処理未完了
     **/
    virtual bool recvTransEnd() override;
};

#endif      // SDC_DRV_SPI_HPP
