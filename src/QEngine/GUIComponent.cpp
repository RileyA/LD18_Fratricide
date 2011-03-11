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

#include "GUIComponent.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

#include "Logger.h"

#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"

namespace QE
{
	GUIComponent::GUIComponent(String layoutFile,bool visible)
		:mVisible(visible),mRoot(MyGUI::LayoutManager::getInstance().load(layoutFile))
	{

	}

	GUIComponent::~GUIComponent()
	{
		MyGUI::LayoutManager::getInstance().unloadLayout(mRoot);
	}

	void GUIComponent::setVisible(bool visible)
	{
		mVisible = visible;
		mParent->setVisible(visible);
		mParent->setEnabled(visible);
	}

	bool GUIComponent::getVisible()
	{
		return mVisible;	
	}

	void GUIComponent::setTemporary(bool temporary)
	{
		mTemporary = temporary;
	}

	bool GUIComponent::isTemporary()
	{
		return mTemporary;
	}
}