
#ifndef INPUT_HPP
#define INPUT_HPP

#include "stdint.h"
#include "ChatteringFilter.hpp"

/**
 * @brief       ユーザインターフェースクラス
 **/


class UI
{
public:

    /**
     * @brief   コンストラクタ
     **/
    UI();

    /**
     * @brief   デストラクタ
     **/
    ~UI() noexcept;

    /**
     * @brief       ユーザインターフェース入力更新
     *              外部信号を取得し、入力状態を更新する。
     *              タイマ割り込み等で定期的に呼び出すこと。
     **/
    void update();

    /**
     * @brief   再生・停止スイッチ取得
     * @retval  true    スイッチON
     * @retval  false   スイッチOFF
     **/
    bool playPause_SW() const;

    /**
     * @brief   再生・停止スイッチ押下
     *          スイッチが押された瞬間にONになる
     * @retval  true    スイッチを押した瞬間
     * @retval  false   それ以外
     **/
    bool playPause_SW_Down() const;

    /**
     * @brief   再生・停止スイッチリリース
     *          スイッチが離された瞬間にONになる
     * @retval  true    スイッチを離した瞬間
     * @retval  false   それ以外
     **/
    bool playPause_SW_Up() const;

    /**
     * @brief   上スイッチ取得
     * @retval  true    スイッチON
     * @retval  false   スイッチOFF
     **/
    bool up_SW() const;

    /**
     * @brief   上スイッチ押下
     *          スイッチが押された瞬間にONになる
     * @retval  true    スイッチを押した瞬間
     * @retval  false   それ以外
     **/
    bool up_SW_Down() const;

    /**
     * @brief   上スイッチリリース
     *          スイッチが離された瞬間にONになる
     * @retval  true    スイッチを離した瞬間
     * @retval  false   それ以外
     **/
    bool up_SW_Up() const;
    
    /**
     * @brief   下スイッチ取得
     * @retval  true    スイッチON
     * @retval  false   スイッチOFF
     **/
    bool down_SW() const;

    /**
     * @brief   下スイッチ取得
     *          スイッチが押された瞬間にONになる
     * @retval  true    スイッチを押した瞬間
     * @retval  false   それ以外
     **/
    bool down_SW_Down() const;

    /**
     * @brief   下スイッチリリース
     *          スイッチが離された瞬間にONになる
     * @retval  true    スイッチを離した瞬間
     * @retval  false   それ以外
     **/
    bool down_SW_Up() const;
    
    /**
     * @brief   右スイッチ取得
     * @retval  true    スイッチON
     * @retval  false   スイッチOFF
     **/
    bool right_SW() const;

    /**
     * @brief   右スイッチ取得
     *          スイッチが押された瞬間にONになる
     * @retval  true    スイッチを押した瞬間
     * @retval  false   それ以外
     **/
    bool right_SW_Down() const;

    /**
     * @brief   右スイッチリリース
     *          スイッチが離された瞬間にONになる
     * @retval  true    スイッチを離した瞬間
     * @retval  false   それ以外
     **/
    bool right_SW_Up() const;
    
    /**
     * @brief   左スイッチ取得
     * @retval  true    スイッチON
     * @retval  false   スイッチOFF
     **/
    bool left_SW() const;

    /**
     * @brief   左スイッチ取得
     *          スイッチが押された瞬間にONになる
     * @retval  true    スイッチを押した瞬間
     * @retval  false   それ以外
     **/
    bool left_SW_Down() const;

    /**
     * @brief   左スイッチリリース
     *          スイッチが離された瞬間にONになる
     * @retval  true    スイッチを離した瞬間
     * @retval  false   それ以外
     **/
    bool left_SW_Up() const;

private:

    static const uint32_t sk_PlayPause_OnCnt        = 5U;       //! 再生・停止スイッチON閾値
    static const uint32_t sk_PlayPause_OffCnt       = 5U;       //! 再生・停止スイッチOFF閾値
    static const bool sk_PlayPause_OnLevel          = true;     //! 再生・停止スイッチONレベル
    static const uint32_t sk_Up_OnCnt          = 5U;       //! 上スイッチON閾値
    static const uint32_t sk_Up_OffCnt         = 5U;       //! 上スイッチOFF閾値
    static const bool sk_Up_OnLevel            = true;     //! 上スイッチONレベル
    static const uint32_t sk_Down_OnCnt        = 5U;       //! 下スイッチON閾値
    static const uint32_t sk_Down_OffCnt       = 5U;       //! 下スイッチOFF閾値
    static const bool sk_Down_OnLevel          = true;     //! 下スイッチONレベル
    static const uint32_t sk_Right_OnCnt       = 5U;       //! 右スイッチON閾値
    static const uint32_t sk_Right_OffCnt      = 5U;       //! 右スイッチOFF閾値
    static const bool sk_Right_OnLevel         = true;     //! 右スイッチONレベル
    static const uint32_t sk_Left_OnCnt        = 5U;       //! 左スイッチON閾値
    static const uint32_t sk_Left_OffCnt       = 5U;       //! 左スイッチOFF閾値
    static const bool sk_Left_OnLevel          = true;     //! 左スイッチONレベル

    ChatteringFilter    m_PlayPause_SW;     //! 再生・停止スイッチ判定
    ChatteringFilter    m_Up_SW;            //! 上スイッチ判定
    ChatteringFilter    m_Down_SW;          //! 下スイッチ判定
    ChatteringFilter    m_Right_SW;         //! 右スイッチ判定
    ChatteringFilter    m_Left_SW;          //! 左スイッチ判定
};

#endif      // INPUT_HPP
