
#include "input.hpp"
#include "main.h"
#include "gpio.h"

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

bool UI::playPause_SW_Down() const
{
    return m_PlayPause_SW.isRisingEdg();
}

bool UI::playPause_SW_Up() const
{
    return m_PlayPause_SW.isFallingEdg();
}

bool UI::up_SW() const
{
    return m_Up_SW.isOn();
}

bool UI::up_SW_Down() const
{
    return m_Up_SW.isRisingEdg();
}

bool UI::up_SW_Up() const
{
    return m_Up_SW.isFallingEdg();
}

bool UI::down_SW() const
{
    return m_Down_SW.isOn();
}

bool UI::down_SW_Down() const
{
    return m_Down_SW.isRisingEdg();
}

bool UI::down_SW_Up() const
{
    return m_Down_SW.isFallingEdg();
}

bool UI::right_SW() const
{
    return m_Right_SW.isOn();
}

bool UI::right_SW_Down() const
{
    return m_Right_SW.isRisingEdg();
}

bool UI::right_SW_Up() const
{
    return m_Right_SW.isFallingEdg();
}

bool UI::left_SW() const
{
    return m_Left_SW.isOn();
}

bool UI::left_SW_Down() const
{
    return m_Left_SW.isRisingEdg();
}

bool UI::left_SW_Up() const
{
    return m_Left_SW.isFallingEdg();
}
