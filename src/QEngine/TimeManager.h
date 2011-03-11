#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include "QEngine.h"

#ifdef LINUX
    #include <sys/time.h>
#endif

namespace QE
{
	/**	Singleton class for keeping track of time and the speed of time.*/
	class QENGINE_API TimeManager
	{
	public:

		/** Gets a pointer to this singleton */
		static TimeManager* getSingletonPtr()
		{
			static TimeManager tmgr;
			return &tmgr;
		}

		/** Set the speed of time
		@remarks
			Cannot be negative at this point, reversing time may be supported eventually.
		@param speed The desired speed of time (1 is normal, 0 is paused) */
		void setTimeSpeed(float speed);

		/** Get the speed of time
		@returns The current speed of time */
		float getTimeSpeed();

		/** Gets the current time in seconds as a double
		@returns The current time (in seconds) */
		double getTimeDecimal();

		/** Gets the current time as a long int
		@returns The current time (in ms) */
		long getTime();

		/** The global change in time from the last frame time (more sensitive subsystems
		might need to implement a delta timer thing of their own). */
		double getDeltaTime();

		/** Pretty much just updates the delta time. */
		void update();

	protected:

		float mTimeSpeed;
		double mLastTime;
		double mStartTime;
		unsigned int mStartSec;
		double mDeltaTime;
		double mSmoothedDeltaTime;
		std::vector<double> mFrameTimes;

	private:

		TimeManager();
		TimeManager(TimeManager const&);
		TimeManager& operator=(TimeManager const&);
		~TimeManager();

	};
}

#endif
