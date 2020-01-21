#ifndef REGULATOR_H
#define REGULATOR_H


class Regulator
{
private:
	double m_timeToWait;
	double m_nextUpdateTime;

public:
	Regulator(double secondsBetweenTrigger, double startTime = 0.0)
	{
		UpdateSecondsBetweenTrigger(secondsBetweenTrigger);
		m_nextUpdateTime = startTime;
	}

	void UpdateSecondsBetweenTrigger(double secondsBetweenTrigger)
	{
		if(secondsBetweenTrigger < 0.0)
        {
            secondsBetweenTrigger = 0.0;
        }

        m_timeToWait = secondsBetweenTrigger;

        if (IsZero(static_cast<float>(secondsBetweenTrigger)))
        {
            m_timeToWait = 0.0;
        }
        else if (secondsBetweenTrigger < 0)
        {
            m_timeToWait = -1.0;
        }	
	}

	bool IsReady(double currentTimeInSeconds)
	{
		if (m_timeToWait == 0.0) { return true; } // No need for "IsZero()" is set explicitly
		if (m_timeToWait < 0.0) { return false; }

		if (currentTimeInSeconds >= m_nextUpdateTime)
		{
			m_nextUpdateTime = currentTimeInSeconds + m_timeToWait;

			return true;
		}

		return false;
	}
};



#endif
