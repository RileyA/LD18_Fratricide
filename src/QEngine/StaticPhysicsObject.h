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


#ifndef STATICPHYSICSOBJECT_H
#define STATICPHYSICSOBJECT_H

#include "stdafx.h"
#include "QEngine.h"

#include "LogicObject.h"
#include "RenderedObject.h"
#include "PhysicsObject.h"

namespace QE
{

	class ScriptManager;

	class QENGINE_API StaticPhysicsObject : public LogicObject
	{
	public:

		StaticPhysicsObject(QEngine* engine,String bucket,String name,String mesh,String pmesh="NULL",Vector3 pos=Vector3(0,0,0));

		virtual ~StaticPhysicsObject();

		virtual void update(double delta);

		virtual void deinitialize();

		virtual void setPosition(Vector3 pos);

		RenderedObject* getRenderedObject();

		static StaticPhysicsObject* toStaticPhysicsObject(LogicObject* obj);

		static StaticPhysicsObject* createStaticPhysicsObject(QEngine* engine,String bucket,String name,String mesh,String pMesh);

		static void registerClass(ScriptManager* scriptMgr);

		void yaw(Real angle);
		void pitch(Real angle);
		void roll(Real angle);

	protected:

		RenderedObject* mObject;
		PhysicsObject* mPhysics;

	};
}

#endif