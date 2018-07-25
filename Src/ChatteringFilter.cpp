
#include "ChatteringFilter.hpp"

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
        }
        if( m_Cnt >= mk_OnJdgCnt ){
            // 立ち上がりエッジか判定
            if( m_Sts == false ){
                m_RisingEdg = true;
            }
            m_Sts = true;
        }
    }
    else {
        if( m_InputSignal == true ){
            m_Cnt = 0;
        }

        if( m_Cnt < mk_OffJdgCnt ){
            ++m_Cnt;
        }
        if( m_Cnt >= mk_OffJdgCnt ){
            // 立ち下がりエッジか判定
            if( m_Sts == true ){
                m_FallingEdg = true;
            }
            m_Sts = false;
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
