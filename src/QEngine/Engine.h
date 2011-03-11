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


#ifndef ENGINE_H
#define ENGINE_H

#include "stdafx.h"
#include "QEngine.h"

#include "GameState.h"
#include "TimeManager.h"
#include "Logger.h"

#include "GraphicsManager.h"
#include "ScriptManager.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "LogicManager.h"
#include "AudioManager.h"
#include "NetworkManager.h"

namespace QE
{
	/** This is the main interface between the host application and QEngine.
		This also serves as the state machine for the engine. */
	class QENGINE_API QEngine
	{
	public:

		QEngine();
		~QEngine();

		/** Starts up the engine */
		void initialize(bool graphical=true);

		/** Tells the engine to start running */
		void start();

		/** Returns a pointer to the TimeManager */
		TimeManager* getTimeManager();

		/** Returns a pointer to the Logger  */
		Logger* getLogger();

		/** Returns a pointer to the Graphics Manager  */
		GraphicsManager* getGraphicsManager();

		/** Returns a pointer to the PhysicsManager */
		PhysicsManager* getPhysicsManager();

		/** Returns a pointer to the LogicManager */
		LogicManager* getLogicManager();

		/** Returns a pointer to the InputManager */
		InputManager* getInputManager();

		/** Returns a pointer to the ScriptManager */
		ScriptManager* getScriptManager();

		/** Returns a pointer to the AudioManager */
		AudioManager* getAudioManager();
		
		/** Returns a pointer to the NetworkManager */
		NetworkManager* getNetworkManager();

		/** Adds a GameState to the state stack (states will be executed in the order they are added). */
		void addState(GameState* state);

		GameState* getCurrentState();

		/** Forces the current state to end */
		void endCurrentState();

		/** Tells the engine to shut down immediately*/
		void shutdown();

	private:

		/** Shuts down the main systems, called after all states are finished */
		void deinitialize();

		/** The stack of game states, when told to start, the engine will go
			through all of these then call Deinitialize() */
		std::vector<GameState*> mStates;

		TimeManager* mTimeManager;
		Logger* mLogger;
		GraphicsManager* mGraphicsManager;
		ScriptManager* mScriptManager;
		PhysicsManager* mPhysicsManager;
		InputManager* mInputManager;
		LogicManager* mLogicManager;
		AudioManager* mAudioManager;
		NetworkManager* mNetworkManager;

		bool mEndState;
		bool mKill;
		bool mGraphical;
	};

}

#ifdef QENGINE_EXPORTS
namespace UglyHack
{
	// ugly ugly _UGLY_ global, but it's an easy way of giving chaiscript access from any function
	extern QE::QEngine* gEngine;// = 0;
	 QE::QEngine* getEngine();
}
#endif

#endif
