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


#ifndef PARASITICSTATE_H
#define PARASITICSTATE_H

#include "QEngine.h"
#include "State.h"

namespace QE
{
	/** A state that runs on top of another (for example, a pause screen in gameplay, or maybe a 'benchmark' thing in a menu).
	@remarks
		Much the same as a game state, except it runs inside of a game state (the game state is responsible for updating it).
		Parasitic states can contain other parasitic states.
	*/
	class QENGINE_API ParasiticState : public State
	{
	public:

		/** Destructor */
		~ParasiticState()
		{
		}

		/** Activates or deactvates the state, generally when a parasitic state is active, the 'host' will alter its behavior somehow
		(i.e. gameplay stops non-essential updates and pauses while a pause screen is open).*/
		virtual void setActive(bool active)
		{
			mIsActive = active;
		}

		bool isActive()
		{
			return mIsActive;
		}

	protected:

		bool mIsActive;

	};
}

#endif
