
#ifndef     TIMER_HPP_DEFINED
#define     TIMER_HPP_DEFINED

#include <cstdint>

class MsCounter
{
public:

    MsCounter();
    ~MsCounter() noexcept;

    bool IsInUse() const;
    void Use();
    void Unuse();
    uint32_t Count() const;
    void Increment();
    void Reset();

private:

    bool m_IsInUse;
    uint32_t m_Count;
};

class TimerManager
{
public:
    ~TimerManager() noexcept;
    TimerManager( const TimerManager& ) = delete;
    TimerManager& operator=( const TimerManager& ) = delete;

    static TimerManager& Instance();
 
    MsCounter* GetMsCounter();
    void UpdateMsTimers();

private:

    TimerManager();

    static TimerManager s_Manager;
    static constexpr uint32_t sk_CounterNum = 5;
    MsCounter m_Counters[sk_CounterNum];
};

class MsTimer
{
public:

    MsTimer();
    ~MsTimer() noexcept;

    MsTimer( const MsTimer& ) = delete;
    MsTimer& operator=( const MsTimer& ) = delete;
    
    bool IsValid() const;
    bool IsElapsed( uint32_t time ) const;
    void Reset();

private:

    MsCounter* m_Counter;
};

#endif      // TIMER_HPP_DEFINED
