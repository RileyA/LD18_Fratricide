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

#include "ScriptManager.h"
#include "ChaiWrapper.h"


namespace QE
{
	StaticPhysicsObject::StaticPhysicsObject(QEngine* engine,String bucket,String name,String mesh,String pmesh,Vector3 pos):LogicObject(engine,bucket,name)
	{
		mType = "StaticPhysicsObject";
		mObject = mEngine->getGraphicsManager()->createRenderedObject(name,mesh);
		mObject->attachToRoot();

		if(pmesh!="NULL")
		{
			//then we have a separate physics mesh...
			mPhysics = mEngine->getPhysicsManager()->createStaticTrimesh(pmesh,pos);
		}
		else
		{
			mPhysics = mEngine->getPhysicsManager()->createStaticTrimesh(mObject,pos);
		}
	}

	StaticPhysicsObject::~StaticPhysicsObject()
	{
		if(mInitialized)
		{
			deinitialize();
		}
	}

	void StaticPhysicsObject::update(double delta)
	{
		// not much to do here actually... maybe update animations eventually?
	}

	void StaticPhysicsObject::deinitialize()
	{
		// TODO: make it kill the physics and rendered bits here...
	}

	void StaticPhysicsObject::setPosition(Vector3 pos)
	{
		mObject->setPosition(pos);
		mPhysics->setPosition(pos);
	}

	RenderedObject* StaticPhysicsObject::getRenderedObject()
	{
		return mObject;
	}


	StaticPhysicsObject* StaticPhysicsObject::toStaticPhysicsObject(LogicObject* obj)
	{
		return dynamic_cast<StaticPhysicsObject*>(obj);
	}

	void StaticPhysicsObject::registerClass(ScriptManager* scriptMgr)
	{
		scriptMgr->getWrapper()->getChai()->add(chaiscript::user_type<StaticPhysicsObject>(), "StaticPhysicsObject");
		scriptMgr->getWrapper()->getChai()->add(chaiscript::fun(&StaticPhysicsObject::setPosition), "setPosition");
		scriptMgr->getWrapper()->getChai()->add(chaiscript::fun(&StaticPhysicsObject::toStaticPhysicsObject), "toStaticPhysicsObject");
		scriptMgr->getWrapper()->getChai()->add(chaiscript::fun(&StaticPhysicsObject::createStaticPhysicsObject), "createStaticPhysicsObject");
	}

	StaticPhysicsObject* StaticPhysicsObject::createStaticPhysicsObject(QEngine* engine,String bucket,String name,String mesh,String pMesh)
	{
		return new StaticPhysicsObject(engine,bucket,name,mesh,pMesh);
	}

	void StaticPhysicsObject::yaw(Real angle)
	{
		mObject->yaw(angle);
		mPhysics->setOrientation(mObject->getOrientation());
	}

	void StaticPhysicsObject::pitch(Real angle)
	{
		mObject->pitch(angle);
		mPhysics->setOrientation(mObject->getOrientation());
	}

	void StaticPhysicsObject::roll(Real angle)
	{
		mObject->roll(angle);
		mPhysics->setOrientation(mObject->getOrientation());
	}


}