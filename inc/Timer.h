#pragma once
#include <atomic>
#include <stdint.h>
#include <chrono>
#include <thread>
#include <iostream>
//##########################################

#define TIMER_MAX_VALUE 255
#define TIMER_MIN_VALUE 0
//##########################################

class Timer
{
private:
	std::atomic<uint8_t> m_CurrentValue;
	uint32_t m_DecementRate;
public:
	//@param ValueToLoad The initial Value that will be loaded inside the timer Register.
	//@param DecrementRate The rate by which timer decrements in times per second (Hz).
	Timer(uint8_t ValueToLoad , uint32_t DecrementRate);
	void SetTimerValue(uint8_t value);
	uint8_t GetTimerValue();
	void Decrement();
};


