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


#ifndef LOGGER_H
#define LOGGER_H

#include "stdafx.h"
#include "QEngine.h"

namespace QE
{
	/** Singleton Logger class, outputs to the console and to a QEngine.log file.
	@remarks
		This is a very simple class at the moment, further functionality may be added at a later date.*/
	class QENGINE_API Logger
	{
	public:

		/** Gets a pointer to the Logger */
		static Logger* getSingletonPtr()
		{
			static Logger lgr;
			return &lgr;
		}

		/** Writes a message to the console and log file. */
		void logMessage(String message);

		/** Tells the Logger to stop outputting all messages */
		void supressOutput();

		/** Tells the Logger to stop outputting to the QEngine.log file */
		void supressFileOutput();

	private:

		Logger();
		~Logger();
		Logger(Logger const&);
		Logger& operator=(Logger const&);

		std::ofstream mLogFile;
		bool mActive;

	};
}

#endif
