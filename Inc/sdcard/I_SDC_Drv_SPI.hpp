
#ifndef I_SDC_DRV_SPI_HPP
#define I_SDC_DRV_SPI_HPP

#include <stdint.h>

/**
 * @brief   SPI SDカード通信インターフェースクラス
 *          SPIを用いてSDカードと通信をする際に使用する機能のインターフェース
 **/
class I_SDC_Drv_SPI
{
public:
    virtual ~I_SDC_Drv_SPI() noexcept {}

    /**
     * @brief   SDカードセレクト
     **/
    virtual bool Select() = 0;

    /**
     * @brief   SDカード開放
     **/
    virtual void Release() = 0;

    /**
     * @brief   データ送信
     *          SPIでデータ送信する
     * @param [in]  data        送信データ
     * @param [in]  len         送信データ長
     * @retval  true            送信処理実行。バックグラウンドで送信する実装も考えられるので
     *                          処理完了するまで待つには flush() をコールすること。
     * @retval  false           送信処理失敗。
     **/
    virtual bool send( const uint8_t* data, uint32_t len ) = 0;

    /**
     * @brief   データ受信
     *          SPIでデータ受信する
     * @param [out] data        受信データの格納先
     * @param [in]  len         受信データ長
     * @retval  true            受信処理成功
     * @retval  false           受信処理失敗
     **/
    virtual bool recv( uint8_t* data, uint32_t len ) = 0;


    /**
     * @brief   書き込みフラッシュ
     *          書き込みバッファ等を用いた実装の場合、この関数をコールすることで
     *          書き込み待ちのデータを実際のデバイスに書き込むことを保証する。
     * @retval  true    フラッシュ成功
     * @retval  false   フラッシュ失敗　書き込み処理に失敗した
     **/
    virtual bool flush() = 0;
};

#endif      // I_SDC_DRV_SPI_HPP
