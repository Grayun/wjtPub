#ifndef timer_h_
#define timer_h_

namespace air 
{
	class CTimer
	{
	private:
		double	m_sec_per_count;	// CPU 执行 1 Count 耗费的秒数
		__int64	m_start_tick;		// 计时器启动时刻
		bool	m_pause;			// 计时器暂停标志

	public:
		CTimer();
		~CTimer();

		/*
			重置计时器起始计时时刻
		*/
		void reset();

		/*
			暂停 / 重启计时器
			flag：	true	-- 暂停计时
					false	-- 恢复计时
		*/
		void pause(bool flag);

		/*
			经过的毫秒 / 秒 / 分钟 / 时
		*/
		double pass_ms();
		double pass_s();
		double pass_m();
		double pass_h();

	private:
		CTimer(const CTimer&);
		CTimer& operator =(const CTimer&);
	};
}

#endif