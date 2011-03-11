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


#ifndef STATE_H
#define STATE_H

#include "QEngine.h"

#include "SlotHandler.h"
#include "SignalHandler.h"

namespace QE
{
	class ParasiticState;

	/** Virtual base for engine states */
	class QENGINE_API State : public SignalHandler, public SlotHandler
	{
	public:

		virtual ~State();
		virtual void initialize() = 0;
		virtual bool update() = 0;
		virtual void processInput() = 0;
		virtual void deinitialize() = 0;
		virtual bool continueState();

		virtual void setInputEnabled(bool enable);
		virtual bool isInputEnabled();

		/** Lets the state manager tell a state to shutdown. */
		void forceShutdown();

	protected:

		bool mIsStarted;
		bool mIsFinished;
		bool mInputEnabled;
		std::vector<ParasiticState*> mParasites;

	};
}

#endif
