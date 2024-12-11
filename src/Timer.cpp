#include "Timer.h"


Timer::Timer(uint8_t ValueToLoad, uint32_t DecrementRate) 
	: m_CurrentValue(ValueToLoad), m_DecementRate(DecrementRate)
{
}

void Timer::SetTimerValue(uint8_t value)
{
	m_CurrentValue = value;
}

uint8_t Timer::GetTimerValue() { return m_CurrentValue; }

void Timer::Decrement()
{
	while (true)
	{
		if (m_CurrentValue != 0)
		{
			--m_CurrentValue;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_DecementRate));
	}
	
}