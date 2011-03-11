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
#include "QuantaCCT.h"
#include "Engine.h"
#include "LogicObject.h"
#include "Logger.h"

#include "Ogre.h"

namespace QE
{
	QuantaCCT::QuantaCCT(QEngine* engine,String bucket,String name,Vector3 pos):LogicObject(engine,bucket,name)
	{
		mController = new QCharacterController(mEngine->getPhysicsManager(),pos);
		mController->initialize();

		Robjs[0] = engine->getGraphicsManager()->createRenderedObject("Robj1","cctsphere.mesh",1);
		Robjs[1] = engine->getGraphicsManager()->createRenderedObject("Robj2","cctsphere.mesh",1);
		Robjs[2] = engine->getGraphicsManager()->createRenderedObject("Robj3","cctsphere.mesh",1);
		Robjs[3] = engine->getGraphicsManager()->createRenderedObject("Robj4","cctsphere.mesh",1);
		Robjs[4] = engine->getGraphicsManager()->createRenderedObject("Robj5","cctsphere.mesh",1);
		Robjs[0]->attachToRoot();
		Robjs[1]->attachToRoot();
		Robjs[2]->attachToRoot();
		Robjs[3]->attachToRoot();
		Robjs[4]->attachToRoot();
	}

	QuantaCCT::~QuantaCCT()
	{
		if(mInitialized)
		{
			deinitialize();
		}
	}

	void QuantaCCT::update(double delta)
	{
		mController->update(static_cast<Real>(delta));
		Robjs[0]->setPosition(mController->getSegmentPosition(0));
		Robjs[1]->setPosition(mController->getSegmentPosition(1));
		Robjs[2]->setPosition(mController->getSegmentPosition(2));
		Robjs[3]->setPosition(mController->getSegmentPosition(3));
		Robjs[4]->setPosition(mController->getSegmentPosition(4));
	}

	void QuantaCCT::deinitialize()
	{
		Logger::getSingletonPtr()->logMessage("Character Controller Destroyed.");
		delete mController;
		mController = 0;
	}

	Vector3 QuantaCCT::getPosition()
	{
		return mController->getPosition();
	}

	Quaternion QuantaCCT::getOrientation()
	{
		return mController->getOrientation();
	}

	void QuantaCCT::setQueriesEnabled(bool set)
	{
		mController->setQueriesEnabled(set);
	}

	void QuantaCCT::setMoveVector(Vector3 move)
	{
		mController->setMoveVector(move);
	}

	void QuantaCCT::setMove(bool move)
	{
		mController->setMove(move);
	}

	bool QuantaCCT::jump(float str)
	{
		return mController->jump(str);
	}

	void QuantaCCT::toggleCrouch(unsigned int sw)
	{
		mController->crouchToggle(sw);
	}

	void QuantaCCT::resetImpact()
	{
		mController->resetImpactLevel();
	}

	Real QuantaCCT::getPlayerSpeed()
	{
		return mController->getSpeed();
	}

	void QuantaCCT::setPlayerSpeed(Real base,Real top, Real acel)
	{
		mController->setPlayerSpeed(base,top,acel);
	}


	void QuantaCCT::setDebugVisualization(bool visible)
	{
		Robjs[0]->setVisible(visible);
		Robjs[1]->setVisible(visible);
		Robjs[2]->setVisible(visible);
		Robjs[3]->setVisible(visible);
		Robjs[4]->setVisible(visible);
	}

}