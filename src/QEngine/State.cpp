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

#include "State.h"
#include "ParasiticState.h"

namespace QE
{
	State::~State()
	{
		for(unsigned int i = 0; i<mParasites.size(); ++i)
		{
			if(mParasites[i])
			{
				delete mParasites[i];
				mParasites[i] = 0;
			}
		}
		mParasites.clear();
	}

	bool State::continueState()
	{
		if(mInputEnabled)
		{
			processInput();
		}
		return update();
	}

	void State::setInputEnabled(bool enable)
	{
		mInputEnabled = enable;
	}

	bool State::isInputEnabled()
	{
		return mInputEnabled;
	}

	void State::forceShutdown()
	{
		mIsFinished = true;
	}
}