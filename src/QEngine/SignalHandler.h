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


#ifndef SIGNALHANDLER_H
#define SIGNALHANDLER_H

#include "QEngine.h"
#include "stdafx.h"
#include "FastDelegate.h"
#include "MessageAny.h"
#include "Signal.h"

namespace QE
{
	class QENGINE_API SignalHandler
	{
	public:

		SignalHandler(){};
		virtual ~SignalHandler(){};

		void createSignal(String name)
		{
			mSignals[name] = Signal();
		}

		//void removeSignal(String name){} TODO

		bool hasSignal(String name)
		{
			if(mSignals.find(name)!=mSignals.end())
				return true;
			else
				return false;
		}

		Signal& getSignal(String name)
		{
			return mSignals[name];
		}

	protected:

		std::map<String,Signal> mSignals;

	};
}

#endif