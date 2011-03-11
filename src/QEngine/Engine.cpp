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
#include "Engine.h"
#include "TimeManager.h"
#include "GraphicsManager.h"

#include "LogicConsole.h"

#include "OgreRenderWindow.h"

namespace QE
{
	QEngine::QEngine()
		:mEndState(false),
		mInputManager(0),
		mKill(0)
	{
		UglyHack::gEngine = this;
		mTimeManager = TimeManager::getSingletonPtr();
		mLogger = Logger::getSingletonPtr();
		mLogger->logMessage("QEngine Object Created.");
	}

	QEngine::~QEngine()
	{
		// just in case, we run this again.
		deinitialize();
		mLogger->logMessage("QEngine Object Destroyed.");
	}

	void QEngine::initialize(bool graphical)
	{
		mGraphical = graphical;
		mGraphicsManager = 0;
		mAudioManager = 0;
		mInputManager = 0;

		// here we start up the essential subsystems...
		if(mGraphical)
		{
			mGraphicsManager = new GraphicsManager();
			mGraphicsManager->initialize();
		}

		mScriptManager = new ScriptManager(this);
		mScriptManager->initialize();


		if(mGraphical)
		{
			mInputManager = new InputManager();
			mGraphicsManager->getCameraManager()->setInputManager(mInputManager);
		}

		mLogicManager = new LogicManager();
		mLogicManager->initialize();

		mNetworkManager = new NetworkManager();

		if(mGraphical)
		{
			// also no audio, this may get its own option later, but a server probs won't need audio...
			mAudioManager = new AudioManager();
			mAudioManager->initialize();

			size_t handle = 0;
			mGraphicsManager->getWindow()->getCustomAttribute("WINDOW", &handle);
			mInputManager->initialize(handle);
			mInputManager->activateGUI(mGraphicsManager->getGUIManager());
			mInputManager->setWindowSize(static_cast<unsigned int>(mGraphicsManager->getWindow()->getWidth()),static_cast<unsigned int>(mGraphicsManager->getWindow()->getHeight()));

			// Set up the scripting console
			LogicConsole* console = new LogicConsole(this,"Default","ScriptingConsole");
			console->setVisible(false);
		}

		// note that we do not init the PhysicsManager here, some states (simple menus for
		// instance) will not need physics, so it is set up on a state-by-state basis.
		mPhysicsManager = new PhysicsManager();

		mLogger->logMessage("QEngine Initialized.");
	}

	void QEngine::start()
	{
		// I'll just leave this here...
		srand(time(0));

		mLogger->logMessage("QEngine Started...");

		mKill = false;

		// State Management
		while(!mStates.empty())
		{
			mEndState = false;

			// start up the state
			mStates[0]->_sendEngine(this);
			mStates[0]->initialize();
			mLogger->logMessage("Initialized State.");

			// loop while until the state returns false
			while(mStates[0]->continueState()&&!mEndState)
			{
				// update the subsystems
				mTimeManager->update();
				mLogicManager->update(mTimeManager->getDeltaTime());

				if(mPhysicsManager->isEnabled())
				{
					mPhysicsManager->update(static_cast<Real>(mTimeManager->getDeltaTime()));
				}

				if(mGraphical)
				{

					if(mInputManager->isKeyDown("KC_ESCAPE")||mKill)
					{
						mKill = true;
						break;
					}

					if(mInputManager->isKeyDown("KC_END")||mEndState)
					{
						endCurrentState();
					}


					mAudioManager->update();
					mGraphicsManager->update();
					mInputManager->update();
				}

				//mNetworkManager->update(); //No networking needed for LD...
			}

			mLogicManager->_endState();

			// Shut it down and erase it from the vector
			mStates[0]->deinitialize();
			if(mKill)
			{
				for(unsigned int i=0;i<mStates.size();++i)
				{
					delete mStates[i];
				}
				mStates.clear();
				break;
			}
			else
			{
				delete mStates.front();
				mStates.erase(mStates.begin());
			}

			if(mGraphical)
			{
				mGraphicsManager->_endState();
			}

		}

		// Out of states, so we shut the whole thing down.
		mLogger->logMessage("QEngine Finishing...");
		deinitialize();
		mLogger->logMessage("QEngine Deinitialized.");
	}

	TimeManager* QEngine::getTimeManager()
	{
		// return the pointer
		return mTimeManager;
	}

	Logger* QEngine::getLogger()
	{
		// return the pointer
		return mLogger;
	}

	GraphicsManager* QEngine::getGraphicsManager()
	{
		// return the pointer
		return mGraphicsManager;
	}

	PhysicsManager* QEngine::getPhysicsManager()
	{
		return mPhysicsManager;
	}

	LogicManager* QEngine::getLogicManager()
	{
		return mLogicManager;
	}

	AudioManager* QEngine::getAudioManager()
	{
		return mAudioManager;
	}

	NetworkManager* QEngine::getNetworkManager()
	{
		return mNetworkManager;
	}

	void QEngine::addState(GameState* state)
	{
		// push it into the vector...
		mStates.push_back(state);
	}

	void QEngine::deinitialize()
	{
		// delete all the subsystems
		if(mInputManager)
		{
			delete mInputManager;
			mInputManager = 0;
		}

		if(mAudioManager)
		{
			delete mAudioManager;
			mAudioManager = 0;
		}

		if(mLogicManager)
		{
			delete mLogicManager;
			mLogicManager = 0;
		}

		if(mGraphicsManager)
		{
			delete mGraphicsManager;
			mGraphicsManager = 0;
		}

		if(mPhysicsManager)
		{
			delete mPhysicsManager;
			mPhysicsManager = 0;
		}

		if(mScriptManager)
		{
			delete mScriptManager;
			mScriptManager = 0;
		}

		if(mNetworkManager)
		{
			delete mNetworkManager;
			mNetworkManager = 0;
		}
	}

	InputManager* QEngine::getInputManager()
	{
		return mInputManager;
	}

	ScriptManager* QEngine::getScriptManager()
	{
		return mScriptManager;
	}

	GameState* QEngine::getCurrentState()
	{
		return mStates[0];
	}

	void QEngine::endCurrentState()
	{
		mEndState = true;
	}

	void QEngine::shutdown()
	{
		mKill = true;	
	}

}
#ifdef QENGINE_EXPORTS
namespace UglyHack
{
	// ugly ugly _UGLY_ global, but it's an easy way of giving chaiscript access from any function
	QE::QEngine* gEngine;

	QE::QEngine* getEngine()
	{
		return gEngine;
	}
}
#endif
