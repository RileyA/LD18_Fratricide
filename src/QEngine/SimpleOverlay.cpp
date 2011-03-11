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
#include "StaticPhysicsObject.h"
#include "Engine.h"
#include "LogicObject.h"
#include "Logger.h"

#include "OgreOverlayManager.h"
#include "OgreOverlayElement.h"
#include "OgreRenderWindow.h"
#include "OgreMaterialManager.h"
#include "OgreMaterial.h"
#include "OgrePass.h"
#include "OgreTechnique.h"

#include "SimpleOverlay.h"

namespace QE
{
	SimpleOverlay::SimpleOverlay(QEngine* engine,String bucket,String name)
		:LogicObject(engine,bucket,name)
	{
		mOverlay = Ogre::OverlayManager::getSingletonPtr()->getByName("HUD");
		mOverlay->show();
		tx = 3000.f;
		ty = 3000.f;
		Real w = mEngine->getGraphicsManager()->getWindow()->getWidth();
		Real h = mEngine->getGraphicsManager()->getWindow()->getHeight();
		Ogre::MaterialPtr ptr = Ogre::MaterialManager::getSingletonPtr()->getByName("EscherTile");
		ptr->getBestTechnique()->getPass(0)->getTextureUnitState(0)->setTextureUScale(1024.f/w);
		ptr->getBestTechnique()->getPass(0)->getTextureUnitState(0)->setTextureVScale(1024.f/h);
		//Ogre::OverlayManager::getSingletonPtr()->getOverlayElement("HUDPanel")->
	}

	SimpleOverlay::~SimpleOverlay()
	{
		if(mInitialized)
		{
			deinitialize();
		}
	}

	void SimpleOverlay::update(double delta)
	{
		Ogre::OverlayManager::getSingletonPtr()->getOverlayElement("Jump2")->setPosition(0.495f+static_cast<float>(mEngine->getInputManager()->getMouseX())/tx,0.495f+static_cast<float>(mEngine->getInputManager()->getMouseY())/ty);
	}

	void SimpleOverlay::deinitialize()
	{

	}

	void SimpleOverlay::setThreshold(Real x,Real y)
	{
		tx = x;
		ty = y;
	}


}