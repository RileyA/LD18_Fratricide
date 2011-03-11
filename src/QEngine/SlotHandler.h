#ifndef SLOTHANDLER_H
#define SLOTHANDLER_H

#include "QEngine.h"
#include "stdafx.h"
#include "FastDelegate.h"
#include "MessageAny.h"
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


#include "Signal.h"
#include "Slot.h"

namespace QE
{
	class QENGINE_API SlotHandler
	{
	public:

		typedef void (SlotHandler::*HandlerMemberFunction)(Message*);

		SlotHandler(){};
		virtual ~SlotHandler(){};

		void createSlot(String name,HandlerMemberFunction function)
		{
			mSlots[name] = Slot(fastdelegate::MakeDelegate(this,function));
		}

		void createSlot(String name,Delegate dlg)
		{
			mSlots[name] = Slot(dlg);
		}

		Slot& getSlot(String name)
		{
			return mSlots[name];
		}

		bool hasSlot(String name)
		{
			if(mSlots.find(name)!=mSlots.end())
				return true;
			else
				return false;
		}

	protected:

		std::map<String,Slot> mSlots;

	};
}

#endif