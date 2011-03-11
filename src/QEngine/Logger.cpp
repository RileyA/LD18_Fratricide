// Copyright Riley Adams 2010

// This file is part of QEngine.

// QEngine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// QEngine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with QEngine.  If not, see <http://www.gnu.org/licenses/>.


#include "stdafx.h"
#include "QEngine.h"
#include "TimeManager.h"
#include "Logger.h"

namespace QE
{
	Logger::Logger() : mActive(true)
	{
		// open up the file
		mLogFile.open("QEngine.log");
	}

	Logger::~Logger()
	{
		// close the file
		if(mLogFile.is_open())
		{
			mLogFile.close();
		}
	}

	void Logger::logMessage(String message)
	{
		if(mActive)
		{
			// the timestamp


            // sorta hack-y but I'm too lazy to correct it for right now...
            #ifdef LINUX
                clock_t totalTime = TimeManager::getSingletonPtr()->getTime();
                int hrs = static_cast<int>(floor(static_cast<float>(totalTime/(3600.0f)))); totalTime = totalTime%(3600);
                int mins = static_cast<int>(floor(static_cast<float>(totalTime/(60.0f)))); totalTime = totalTime%(60);
                int secs = static_cast<int>(floor(static_cast<float>(totalTime/(1.0f)))); totalTime = totalTime%(1);
            #else
			// sorry for all the static_casts here, but floor only takes floats,
			// and VS bitches about float->int if I don't explicitly cast it....
                clock_t totalTime = TimeManager::getSingletonPtr()->getTime();
                int hrs = static_cast<int>(floor(static_cast<float>(totalTime/(3600.0f*CLOCKS_PER_SEC)))); totalTime = totalTime%(3600*CLOCKS_PER_SEC);
                int mins = static_cast<int>(floor(static_cast<float>(totalTime/(60.0f*CLOCKS_PER_SEC)))); totalTime = totalTime%(60*CLOCKS_PER_SEC);
                int secs = static_cast<int>(floor(static_cast<float>(totalTime/(1.0f*CLOCKS_PER_SEC)))); totalTime = totalTime%(1*CLOCKS_PER_SEC);
            #endif

			std::stringstream timeStamp (std::stringstream::in | std::stringstream::out);
			if(hrs<=9)
				timeStamp<<"0";
			timeStamp<<hrs<<":";
			if(mins<=9)
				timeStamp<<"0";
			timeStamp<<mins<<":";
			if(secs<=9)
				timeStamp<<"0";
			timeStamp<<secs<<":";

			// print the message and timestamp (in "hh:mm:ss: [message]" form)
			String timeStampStr;
			timeStamp>>timeStampStr;

			std::cout<<timeStampStr<<" "<<message<<"\n";

			// only write to the file if it's still open...
			if(mLogFile.is_open())
			{
				mLogFile<<timeStampStr<<" "<<message<<"\n";
			}
		}
	}

	void Logger::supressOutput()
	{
		mActive = false;
	}

	void Logger::supressFileOutput()
	{
		// close the file (if it's still open)
		if(mLogFile.is_open())
		{
			mLogFile.close();
		}
	}

}
