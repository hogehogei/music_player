
#ifndef CHATTERING_FILTER_HPP
#define CHATTERING_FILTER_HPP

#include <stdint.h>

/**
 * @brief       チャタリングフィルタ処理クラス
 *              チャタリング・ノイズを除去するため、指定したON判定/OFF判定回数だけ入力レベルが連続した場合に
 *              レベルを確定するための処理を行う
 **/
class ChatteringFilter
{
public:

    /**
     * @brief   コンストラクタ
     * @param [in]      on_cnt          ON判定回数
     * @param [in]      off_cnt         OFF判定回数
     * @param [in]      on_level        ON判定レベル（HI or LO）
     * @param [in]      initial_level   初期レベル
     **/
    ChatteringFilter( uint32_t on_cnt, uint32_t off_cnt, bool on_level, bool initial_level );

    /**
     * @brief   デストラクタ
     **/
    ~ChatteringFilter() noexcept;

    /**
     * @brief   チャタリングフィルタ処理
     *          入力信号を引数で与え、フィルタ処理後の値を判定する
     * @param [in]      input_signal    入力信号レベル(ON or OFF)
     **/
    void update( const bool input_signal );

    /**
     * @brief   ON判定取得
     * @retval  true    ONレベル
     * @retval  false   OFFレベル
     **/
    bool isOn() const;

    /**
     * @brief   OFF判定取得
     * @retval  true    OFFレベル
     * @retval  false   ONレベル
     **/
    bool isOff() const;

    /**
     * @brief   立ち上がりエッジ取得
     *          前回更新時に立ち上がりエッジがあったかどうか取得する
     *          フラグは update() をコールするたび内部で自動判断し操作される。
     * @retval  true    立ち上がりエッジ
     * @retval  false   立ち上がりエッジなし
     **/
    bool isRisingEdg() const;

    /**
     * @brief   立ち下がりエッジ取得
     *          前回更新時に立ち下がりエッジがあったかどうか取得する
     *          フラグは update() をコールするたび内部で自動判断し操作される。
     * @retval  true    立ち下がりエッジ
     * @retval  false   立ち下がりエッジなし
     **/
    bool isFallingEdg() const;

private:

    const uint32_t  mk_OnJdgCnt;            //! ON判定閾値
    const uint32_t  mk_OffJdgCnt;           //! OFF判定閾値
    uint32_t        m_Cnt;                  //! 判定カウンタ
    const bool      mk_OnLevel;             //! ONレベル
    bool            m_InputSignal;          //! 前回入力信号
    bool            m_Sts;                  //! レベル判定値
    bool            m_RisingEdg;            //! 前回立ち上がりエッジフラグ
    bool            m_FallingEdg;           //! 前回立ち下がりエッジフラグ
};

#endif      // CHATTERING_FILTER_HPP
