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


#ifndef SLOT_H
#define SLOT_H

#include "QEngine.h"
#include "stdafx.h"
#include "FastDelegate.h"
#include "MessageAny.h"

namespace QE
{
	class Signal;

	class QENGINE_API Slot
	{
	public:

		// the delegate itself
		Delegate slot;

		Slot(){}

		Slot(Delegate _slot)
		{
			slot = _slot;
		}

		~Slot()
		{
			stopAllListening();
		}

		void listenTo(Signal* signal)
		{
			mSignals.push_back(signal);
		}

		void stopListeningTo(Signal* signal);

		void stopAllListening();

		void operator += (Signal* signal)
		{
			mSignals.push_back(signal);
		}

		void operator -= (Signal* signal);

	protected:

		// Signals this slot is subscribed to
		std::vector<Signal*> mSignals;

	};

}

#endif