
#include "Timer.hpp"
#include <limits>

TimerManager::TimerManager()
{}

TimerManager::~TimerManager()
{}

MsCounter* TimerManager::GetMsCounter()
{
    for( int i = 0; i < sk_CounterNum; ++i ){
        if( !m_Counters[i].IsInUse() ){
            return &m_Counters[i];
        }
    }

    return nullptr;
}

void TimerManager::UpdateMsTimers()
{
    for( MsCounter& counter : m_Counters ){
        if( counter.IsInUse() ){
            counter.Increment();
        }
    }
}

MsCounter::MsCounter()
    : m_IsInUse( false ),
      m_Count( 0 )
{}

MsCounter::~MsCounter()
{}

bool MsCounter::IsInUse() const
{
    return m_IsInUse;
}

void MsCounter::Use()
{
    m_IsInUse = true;
}

void MsCounter::Unuse()
{
    m_IsInUse = false;
}

uint32_t MsCounter::Count() const
{
    return m_Count;
}

void MsCounter::Increment()
{
    if( m_Count < std::numeric_limits<uint32_t>::max() ){
        ++m_Count;
    }
}

void MsCounter::Reset()
{
    m_Count = 0;
}


TimerManager TimerManager::s_Manager;

TimerManager& TimerManager::Instance()
{
    return s_Manager;
}


MsTimer::MsTimer()
    : m_Counter( TimerManager::Instance().GetMsCounter() )
{
    if( m_Counter ){
        m_Counter->Reset();
        m_Counter->Use();
    }
}

MsTimer::~MsTimer() noexcept
{
    if( m_Counter ){
        m_Counter->Reset();
        m_Counter->Unuse();
    }
}
    
bool MsTimer::IsValid() const
{
    return m_Counter != nullptr;
}

bool MsTimer::IsElapsed( uint32_t time ) const
{
    if( !m_Counter ){
        return true;
    }

    return m_Counter->Count() >= time;
}

void MsTimer::Reset()
{
    if( m_Counter ){
        m_Counter->Reset();
    }
}
