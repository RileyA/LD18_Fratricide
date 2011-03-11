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

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"

#include "Console.h"
#include "Engine.h"

namespace QE
{
	//MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("ingameConsole.layout");
	Console::Console(String layout,QEngine* engine,bool visible)
		:GUIComponent(layout,visible),mEngine(engine),mVisible(visible)
	{
		for (MyGUI::VectorWidgetPtr::iterator iter=mRoot.begin(); iter!=mRoot.end(); ++iter)
		{
			if((*iter)->getName()=="_Main")
			{
				mParent = (*iter);
				break;
			}
		}

		mEdit = dynamic_cast<MyGUI::Edit*>(mParent);// since for now the edit is the only widget...
		unsigned int w = mEngine->getGraphicsManager()->getWindow()->getWidth();
		unsigned int h = mEngine->getGraphicsManager()->getWindow()->getHeight();
		mEdit->setSize(w-50,mEdit->getHeight());
		mEdit->setPosition(30,h-75);
	}

	Console::~Console()
	{
		
	}

	void Console::enterCode()
	{
		mEngine->getScriptManager()->runString(mEdit->getCaption());
		mEdit->setCaption("");
	}

	void Console::focus()
	{
		MyGUI::InputManager::getInstance().setKeyFocusWidget(mEdit);
	}


}