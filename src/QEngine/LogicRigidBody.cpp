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
#include "LogicRigidBody.h"
#include "Engine.h"
#include "LogicObject.h"
#include "Logger.h"

#include "ScriptManager.h"
#include "ChaiWrapper.h"


namespace QE
{
	RigidBody::RigidBody(QEngine* engine,String bucket,String name,String mesh,String pmesh,Vector3 pos):LogicObject(engine,bucket,name)
	{
		mType = "RigidBody";
		mObject = mEngine->getGraphicsManager()->createRenderedObject(name,mesh);
		mObject->attachToRoot();

		if(pmesh!="NULL")
		{
			//then we have a separate physics mesh...
			mPhysics = mEngine->getPhysicsManager()->createConvexHull(pmesh,pos);
		}
		else
		{
			mPhysics = mEngine->getPhysicsManager()->createConvexHull(mObject,pos);
		}
	}

	RigidBody::~RigidBody()
	{
		if(mInitialized)
		{
			deinitialize();
		}
	}

	void RigidBody::update(double delta)
	{
		mObject->setPosition(mPhysics->getPosition());
		// TODO: SHOULD BE INTERPOLATED ASAP!
		mObject->setOrientation(mPhysics->getOrientation());// AABB assert issue here
	}

	void RigidBody::deinitialize()
	{
		mEngine->getGraphicsManager()->deleteObject(mObject->getName());
		mPhysics->_kill();
        mInitialized = false;
		Logger::getSingletonPtr()->logMessage("Rigid Body Destroyed.");
	}

	void RigidBody::setPosition(Vector3 pos)
	{
		mObject->setPosition(pos);
		mPhysics->setPosition(pos);
	}

	RigidBody* RigidBody::toRigidBody(LogicObject* obj)
	{
		return dynamic_cast<RigidBody*>(obj);
	}

	void RigidBody::registerClass(ScriptManager* scriptMgr)
	{
		scriptMgr->getWrapper()->getChai()->add(chaiscript::user_type<RigidBody>(), "RigidBody");
		scriptMgr->getWrapper()->getChai()->add(chaiscript::fun(&RigidBody::setPosition), "setPosition");
		scriptMgr->getWrapper()->getChai()->add(chaiscript::fun(&RigidBody::toRigidBody), "toRigidBody");
		scriptMgr->getWrapper()->getChai()->add(chaiscript::fun(&RigidBody::createRigidBody), "createRigidBody");
	}

	RigidBody* RigidBody::createRigidBody(QEngine* engine,String bucket,String name,String mesh,String pMesh)
	{
		return new RigidBody(engine,bucket,name,mesh,pMesh);
	}


}
