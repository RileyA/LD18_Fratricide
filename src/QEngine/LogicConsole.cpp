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

#include "LogicConsole.h"

#include "GUIManager.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"

#include "ScriptManager.h"
#include "ChaiWrapper.h"

namespace QE
{
	LogicConsole::LogicConsole(QEngine* engine,String bucket,String name)
		:LogicObject(engine,bucket,name),mTildaDown(false)
	{
		mConsole = new Console("ingameConsole.layout",engine);
		mEngine->getGraphicsManager()->getGUIManager()->registerComponent(mConsole,false);
	}

	LogicConsole::~LogicConsole()
	{
		
	}

	void LogicConsole::deinitialize()
	{

	}

	void LogicConsole::update(double delta)
	{
		if(mEngine->getInputManager()->isKeyDown("KC_GRAVE")&&!mTildaDown)
		{
			bool enabled = !isVisible();
			setVisible(enabled);
			mEngine->getCurrentState()->setInputEnabled(!enabled);
			mTildaDown = true;
		}
		else if(!mEngine->getInputManager()->isKeyDown("KC_GRAVE"))
		{
            mTildaDown = false;
		}

		if(isVisible()&&mEngine->getInputManager()->isKeyDown("KC_RETURN"))
		{
			enterScript();
		}
	}

	void LogicConsole::enterScript()
	{
		mConsole->enterCode();
	}

	void LogicConsole::setVisible(bool visible)
	{
		mConsole->setVisible(visible);
		if(visible)
		{
			mConsole->focus();
		}
	}

	bool LogicConsole::isVisible()
	{
		return mConsole->getVisible();
	}

	void LogicConsole::registerClass(ScriptManager* scriptMgr)
	{
		scriptMgr->getWrapper()->getChai()->add(chaiscript::user_type<LogicConsole>(), "LogicConsole");
		//scriptMgr->getWrapper()->getChai()->add(chaiscript::fun(&LogicConsole::setPosition), "setPosition");
		//scriptMgr->getWrapper()->getChai()->add(chaiscript::fun(&LogicConsole::toStaticPhysicsObject), "toStaticPhysicsObject");
	}
}