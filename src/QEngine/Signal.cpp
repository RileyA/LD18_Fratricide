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

#include "Signal.h"
#include "Slot.h"

namespace QE
{
	void Signal::operator -= (Slot* slot)
	{
		for(unsigned int i=0;i<mListeners.size();++i)
		{
			if(mListeners[i]==slot)
			{
				mListeners[i]->stopListeningTo(this);
				mListeners.erase(mListeners.begin()+i);
				break;
			}
		}
	}

	void Signal::removeListener(Slot* slot)
	{
		for(unsigned int i=0;i<mListeners.size();++i)
		{
			if(mListeners[i]==slot)
			{
				mListeners[i]->stopListeningTo(this);
				mListeners.erase(mListeners.begin()+i);
				break;
			}
		}
	}

	void Signal::removeAllListeners()
	{
		for(unsigned int i=0;i<mListeners.size();++i)
		{
			mListeners[i]->stopListeningTo(this);
		}
		mListeners.clear();
	}

	void Signal::fire(Message* msg)
	{
		for(unsigned int i=0;i<mListeners.size();++i)
		{
			// I would use a getter here but getDelegate()(msg) looked funny,
			// and it might be slightly more efficient this way.
			mListeners[i]->slot(msg);
		}
	}
}