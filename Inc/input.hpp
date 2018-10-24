
#include "stdint.h"

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
     **/
    void update();

    /**
     * @brief   再生・停止スイッチ取得
     * @retval  true    スイッチON
     * @retval  false   スイッチOFF
     **/
    bool playPause_SW() const;

    /**
     * @brief   上スイッチ取得
     * @retval  true    スイッチON
     * @retval  false   スイッチOFF
     **/
    bool up_SW() const;
    
    /**
     * @brief   下スイッチ取得
     * @retval  true    スイッチON
     * @retval  false   スイッチOFF
     **/
    bool down_SW() const;
    
    /**
     * @brief   右スイッチ取得
     * @retval  true    スイッチON
     * @retval  false   スイッチOFF
     **/
    bool right_SW() const;
    
    /**
     * @brief   左スイッチ取得
     * @retval  true    スイッチON
     * @retval  false   スイッチOFF
     **/
    bool left_SW() const;

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

