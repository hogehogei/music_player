
#include "input.hpp"
#include "main.h"
#include "gpio.h"

ChatteringFilter::ChatteringFilter( uint32_t on_cnt, uint32_t off_cnt, bool on_level, bool initial_level )
: 
    mk_OnJdgCnt( on_cnt ),
    mk_OffJdgCnt( off_cnt ),
    m_Cnt( 0 ),
    mk_OnLevel( on_level ),
    m_InputSignal( initial_level ),
    m_Sts( initial_level ),
    m_RisingEdg( false ),
    m_FallingEdg( false )
{}

ChatteringFilter::~ChatteringFilter() 
{}

void ChatteringFilter::update( bool input_signal )
{
    // 前回立ち上がり・立ち下がりエッジがあった場合にのみONになるように
    // 毎回OFFで初期化
    m_RisingEdg = false;
    m_FallingEdg = false;

    if( input_signal == mk_OnLevel ){
        if( m_InputSignal == false ){
            m_Cnt = 0;
        }

        if( m_Cnt < mk_OnJdgCnt ){
            ++m_Cnt;
            if( m_Cnt >= mk_OnJdgCnt ){
                m_Sts = true;
                m_RisingEdg = true;
            }
        }
    }
    else {
        if( m_InputSignal == true ){
            m_Cnt = 0;
        }

        if( m_Cnt < mk_OffJdgCnt ){
            ++m_Cnt;
            if( m_Cnt >= mk_OffJdgCnt ){
                m_Sts = false;
                m_FallingEdg = true;
            }
        }
    }

    m_InputSignal = input_signal;
}

bool ChatteringFilter::isOn() const
{
    return m_Sts == true;
}

bool ChatteringFilter::isOff() const
{
    return m_Sts == false;
}

bool ChatteringFilter::isRisingEdg() const
{
    return m_RisingEdg;
}

bool ChatteringFilter::isFallingEdg() const
{
    return m_FallingEdg;
}



UI::UI()
:   m_PlayPause_SW( sk_PlayPause_OnCnt, sk_PlayPause_OffCnt, sk_PlayPause_OnLevel, false ),
    m_Up_SW( sk_Up_OnCnt, sk_Up_OffCnt, sk_Up_OnLevel, false ),
    m_Down_SW( sk_Down_OnCnt, sk_Down_OffCnt, sk_Down_OnLevel, false ),
    m_Right_SW( sk_Right_OnCnt, sk_Right_OffCnt, sk_Right_OnLevel, false ),
    m_Left_SW( sk_Left_OnCnt, sk_Left_OffCnt, sk_Left_OnLevel, false )
{}

UI::~UI()
{}

void UI::update()
{
    bool input_signal = false;              // 入力信号一時保管

    // 再生・停止スイッチ更新
    input_signal = HAL_GPIO_ReadPin( SW_PLAY_PAUSE_GPIO_Port, SW_PLAY_PAUSE_Pin );
    m_PlayPause_SW.update( input_signal );

    // 上スイッチ更新
    input_signal = HAL_GPIO_ReadPin( SW_UP_GPIO_Port, SW_UP_Pin );
    m_Up_SW.update( input_signal );

    // 下スイッチ更新
    input_signal = HAL_GPIO_ReadPin( SW_DOWN_GPIO_Port, SW_DOWN_Pin );
    m_Down_SW.update( input_signal );

    // 右スイッチ更新
    input_signal = HAL_GPIO_ReadPin( SW_RIGHT_GPIO_Port, SW_RIGHT_Pin );
    m_Right_SW.update( input_signal );

    // 左スイッチ更新
    input_signal = HAL_GPIO_ReadPin( SW_LEFT_GPIO_Port, SW_LEFT_Pin );
    m_Left_SW.update( input_signal );
}

bool UI::playPause_SW() const
{
    return m_PlayPause_SW.isOn();
}

bool UI::up_SW() const
{
    return m_Up_SW.isOn();
}

bool UI::down_SW() const
{
    return m_Down_SW.isOn();
}

bool UI::right_SW() const
{
    return m_Right_SW.isOn();
}

bool UI::left_SW() const
{
    return m_Left_SW.isOn();
}
