#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Timer.h"

namespace air
{
	CTimer::CTimer()	
	{
		__int64 rate;
		QueryPerformanceFrequency((LARGE_INTEGER*)&rate);	// 每秒执行的 Count 数
		m_sec_per_count = 1.0 / (double)rate;				// 执行 1 Count 耗费的秒数
		m_start_tick = 0;
		m_pause = false;
	}

	CTimer::~CTimer()		
	{

	}

	void CTimer::reset()
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_start_tick);
	}

	void CTimer::pause(bool flag)
	{
		m_pause = flag;
	}

	double CTimer::pass_ms()
	{
		return pass_s() * 1000;
	}

	double CTimer::pass_s()
	{
		if (0 == m_start_tick || m_pause)
			return 0.0;

		__int64 cur_tick;
		QueryPerformanceCounter((LARGE_INTEGER*)&cur_tick);
		double pass = (double)(cur_tick - m_start_tick) * m_sec_per_count;	// 经过的秒数
		return pass;
	}

	double CTimer::pass_m()
	{
		return pass_s() / 60.0;
	}

	double CTimer::pass_h()
	{
		return pass_s() / 3600.0;
	}
}