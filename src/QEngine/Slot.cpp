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

#include "Slot.h"
#include "Signal.h"

namespace QE
{
	void Slot::stopAllListening()
	{
		for(unsigned int i=0;i<mSignals.size();++i)
		{
			mSignals[i]->removeListener(this);
		}
		mSignals.clear();
	}

	void Slot::stopListeningTo(Signal* signal)
	{
		for(unsigned int i=0;i<mSignals.size();++i)
		{
			if(mSignals[i]==signal)
			{
				mSignals[i]->removeListener(this);
				mSignals.erase(mSignals.begin()+i);
				break;
			}
		}
	}

	void Slot::operator -= (Signal* signal)
	{
		for(unsigned int i=0;i<mSignals.size();++i)
		{
			if(mSignals[i]==signal)
			{
				mSignals[i]->removeListener(this);
				mSignals.erase(mSignals.begin()+i);
				break;
			}
		}
	}
}