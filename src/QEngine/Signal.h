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


#ifndef SIGNAL_H
#define SIGNAL_H

#include "QEngine.h"
#include "stdafx.h"
#include "FastDelegate.h"
#include "MessageAny.h"

namespace QE
{
	class Slot;

	class QENGINE_API Signal
	{
	public:

		Signal(){};

		~Signal()
		{
			removeAllListeners();
		}

		void fire(Message* msg);

		void addListener(Slot* slot)
		{
			mListeners.push_back(slot);
		}

		void removeListener(Slot* slot);

		void removeAllListeners();

		void operator += (Slot* slot)
		{
			mListeners.push_back(slot);
		}

		void operator -= (Slot* slot);

	protected:

		std::vector<Slot*> mListeners;

	};
}

#endif