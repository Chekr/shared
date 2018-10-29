#ifndef _TIMER_H_
#pragma once
#define _TIMER_H_

#include <chrono>

namespace Timer
{
	using std::chrono::steady_clock;

	typedef std::chrono::time_point<steady_clock> TimePoint;

	class Timer
	{
		public:
			Timer()
			{
				m_offset = steady_clock::duration::zero();
				m_isRunning = false;
			}
			void Start()
			{
				if (!m_isRunning)
				{
					m_startTime = steady_clock::now();

					m_isRunning = true;
				}
			}
			void Stop()
			{
				if (m_isRunning)
				{
					m_offset += (steady_clock::now() - m_startTime);

					m_isRunning = false;
				}
			}
			void ResetAndStop()
			{
				m_offset = steady_clock::duration::zero();
				m_startTime = steady_clock::now();
				m_isRunning = false;
			}
			double GetElapsedTimeInSeconds()
			{
				std::chrono::duration<double> elapsedTime = steady_clock::duration::zero();

				if (m_isRunning)
				{
					elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(steady_clock::now() - m_startTime);
				}

				return (elapsedTime + m_offset).count();
			}

			bool IsRunning()
			{
				return m_isRunning;
			}

		private:
			std::chrono::duration<double> m_offset;
			TimePoint m_startTime;
			bool m_isRunning;
	};
}

#endif
