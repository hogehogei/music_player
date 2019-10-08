
#ifndef SDC_DRV_SPI_HPP
#define SDC_DRV_SPI_HPP

#include "sdcard/I_SDC_Drv_SPI.hpp"

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
    virtual ~SDC_Drv_SPI() noexcept;

    /**
     * @brief   SDカードドライバ低速初期化
     */
    virtual bool InitSlowSpeed() override;

    /**
     * @brief   SDカードドライバ高速初期化
     */
    virtual bool InitFastSpeed() override;

    /**
     * @brief   SDカードセレクト
     **/
    virtual bool Select() override;

    /**
     * @brief   SDカード開放
     **/
    virtual void Release() override;

    /**
     * @brief   データ送信
     *          SPIでデータ送信する
     * @param [in]  data        送信データ
     * @param [in]  len         送信データ長
     * @retval  true            送信処理実行。バックグラウンドで送信する実装も考えられるので
     *                          処理完了するまで待つには flush() をコールすること。
     * @retval  false           送信処理失敗。
     **/
    virtual bool send( const uint8_t* data, uint32_t len ) override;

    /**
     * @brief   データ受信
     *          SPIでデータ受信する
     * @param [out] data        受信データの格納先
     * @param [in]  len         受信データ長
     * @retval  true            受信処理成功
     * @retval  false           受信処理失敗
     **/
    virtual bool recv( uint8_t* data, uint32_t len ) override;


    /**
     * @brief   書き込みフラッシュ
     *          書き込みバッファ等を用いた実装の場合、この関数をコールすることで
     *          書き込み待ちのデータを実際のデバイスに書き込むことを保証する。
     * @retval  true    フラッシュ成功
     * @retval  false   フラッシュ失敗　書き込み処理に失敗した
     **/
    virtual bool flush() override;

private:

    bool waitReady();
};

#endif      // SDC_DRV_SPI_HPP
