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


#ifndef LOGICRIGIDBODY_H
#define LOGICRIGIDBODY_H

#include "stdafx.h"
#include "QEngine.h"

#include "LogicObject.h"
#include "RenderedObject.h"
#include "PhysicsObject.h"

namespace QE
{

	class ScriptManager;

	class QENGINE_API RigidBody : public LogicObject
	{
	public:

		RigidBody(QEngine* engine,String bucket,String name,String mesh,String pmesh="NULL",Vector3 pos=Vector3(0,0,0));

		virtual ~RigidBody();

		virtual void update(double delta);

		virtual void deinitialize();

		virtual void setPosition(Vector3 pos);

		static RigidBody* toRigidBody(LogicObject* obj);

		static RigidBody* createRigidBody(QEngine* engine,String bucket,String name,String mesh,String pMesh);

		static void registerClass(ScriptManager* scriptMgr);

	protected:

		RenderedObject* mObject;
		PhysicsObject* mPhysics;

	};
}

#endif