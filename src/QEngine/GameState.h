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


#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "stdafx.h"
#include "QEngine.h"
#include "State.h"
#include "ParasiticState.h"

namespace QE
{
	class QEngine;

	/** A game state, for example, the main menu would be a state, as would gameplay and 'editor mode' or somthing like that. */
	class QENGINE_API GameState : public State
	{
	public:

		/** Destructor */
        ~GameState()
		{

		}

		/** Internal, passes a pointer to the engine to the state, this is called
		by the engine before every state inits, so there is no need to pass pointers by other means...*/
		void _sendEngine(QEngine* engine)
		{
			mEngine = engine;
		}

	protected:

		// Pointer to the engine itself, gives the state access to everything
		QEngine* mEngine;
		

	};
}

#endif
