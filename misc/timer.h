#ifndef TIMER_H
#pragma once
#define TIMER_H

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
				m_isRunning = false;
				m_isReset = true;
			}
			void Start()
			{
				if (!m_isRunning)
				{
					m_startPoint = m_isReset ? steady_clock::now() : steady_clock::now() - (m_stopPoint - m_startPoint);
					m_isReset = false;
					m_isRunning = true;
				}
			}
			void Stop()
			{
				if (m_isRunning)
				{
					m_stopPoint = steady_clock::now();
					m_isRunning = false;
				}
			}

			void ResetAndStop()
			{
				m_isRunning = false;
				m_isReset = true;
			}

			double GetElapsedTimeInSeconds()
			{
				if(m_isReset)
				{
					return 0.0f;
				}
				
				if (m_isRunning)
				{
					return std::chrono::duration_cast<std::chrono::duration<double>>(steady_clock::now() - m_startPoint).count();
				}

				return std::chrono::duration_cast<std::chrono::duration<double>>(m_stopPoint - m_startPoint).count();
			}

			bool IsRunning()
			{
				return m_isRunning;
			}

		private:
			TimePoint m_startPoint;
			TimePoint m_stopPoint;
			bool m_isRunning;
			bool m_isReset;
	};
}

#endif
