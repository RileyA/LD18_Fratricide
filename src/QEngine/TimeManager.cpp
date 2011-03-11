#include "stdafx.h"
#include "QEngine.h"
#include "TimeManager.h"

namespace QE
{
	// doesn't do anything
	TimeManager::TimeManager():mLastTime(0)
	{
	    #ifdef LINUX
            timeval time_tod;
            gettimeofday(&time_tod, NULL);
            mLastTime=time_tod.tv_sec+(time_tod.tv_usec/1000000.0);
            mStartSec = time_tod.tv_sec;
            mStartTime = mLastTime;
        #endif
    }

	// doesn't do anything
	TimeManager::~TimeManager(){}

	void TimeManager::setTimeSpeed(float speed)
	{
		// set the speed
		mTimeSpeed = speed;
	}

	float TimeManager::getTimeSpeed()
	{
		// return the speed
		return mTimeSpeed;
	}

	double TimeManager::getTimeDecimal()
	{
		// divide by clocks per second and cast to double
        #ifdef LINUX
            timeval time_tod;
            gettimeofday(&time_tod, NULL);
            return time_tod.tv_sec+(time_tod.tv_usec/1000000.0)-mStartTime;
        #else
            return static_cast<double>(clock())/static_cast<double>(CLOCKS_PER_SEC);
		#endif
	}

	long TimeManager::getTime()
	{
        #ifdef LINUX
            timeval time_tod;
            gettimeofday(&time_tod, NULL);
            return static_cast<long>(time_tod.tv_sec-mStartSec);
	    #else
            return clock();
		#endif
	}

	void TimeManager::update()
	{
        #ifdef LINUX
            timeval time_tod;
            gettimeofday(&time_tod, NULL);
            double t1=time_tod.tv_sec+(time_tod.tv_usec/1000000.0);

            mDeltaTime = t1 - mLastTime;

            mLastTime = t1;
        #else
            double currentTime = static_cast<double>(clock())/static_cast<double>(CLOCKS_PER_SEC);
            mDeltaTime = currentTime - mLastTime;
            mLastTime = currentTime;
        #endif


		mFrameTimes.push_back(mDeltaTime);

		double total = 0;
		unsigned int nrFr = 0;
		double smP = 0.125;// the smoothing period .1 of a second

		bool dn = false;
		while(!dn)
		{
			total = 0;
			nrFr = 0;
			for(unsigned int p = 0;p<mFrameTimes.size();++p)
			{
				total+=mFrameTimes[p];
				++nrFr;
			}
			if(total>smP)
			{
				std::vector<double>::iterator iter = mFrameTimes.begin();
				mFrameTimes.erase(iter);
			}
			else
			{
				dn = true;
				break;
			}
		}

		if(nrFr>1)
		{
			mSmoothedDeltaTime = total/nrFr;
		}
		else
		{
			mSmoothedDeltaTime = mDeltaTime;
		}
	}

	double TimeManager::getDeltaTime()
	{
		return mSmoothedDeltaTime*mTimeSpeed;
	}

}
