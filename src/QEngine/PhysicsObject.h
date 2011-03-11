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


#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "stdafx.h"
#include "QEngine.h"

class btRigidBody;
class btCollisionObject;
class btDynamicsWorld;

namespace QE
{

	/** A wrapper class for NxActor */
	class QENGINE_API PhysicsObject
	{
	public:

		PhysicsObject(btRigidBody* actor,btDynamicsWorld* dynWorld);
		PhysicsObject(btCollisionObject* actor,btDynamicsWorld* dynWorld);
		~PhysicsObject();

		// all the force position etc functions will be wrapped eventually...

		btRigidBody* getActor();

		void setPosition(Vector3 pos);

		void setOrientation(Quaternion ori);

		Vector3 getPosition();

		Quaternion getOrientation();

		void set2D(bool d2);

		void setKinematic(bool kine);

		void update(bool frame,double interpolation,Vector3 gravity);

		void setUserData(void* data);

		void addForce(Vector3 dir,Vector3 pos=Vector3(0,0,0));

		void addImpulse(Vector3 dir,Vector3 pos=Vector3(0,0,0));

		void setVelocity(Vector3 dir);

		Vector3 getVelocity();

		void setMass(float mass);

		void wakeup();

		void setLinearFactor(Vector3 fact);
		void setAngularFactor(Vector3 fact);

		void setCollisionGroup(short group);
		void setCollisionMask(short mask);

		void* getUserData();

		/** Returns whether or not the object will be deleted next frame */
		bool readyForDelete();

		/** Internal function for killing a physics object
            @remarks This actually just tells the manager to delete it at the next available frame,
                        rather than immediately deleting it. */
		void _kill();

	protected:

		btDynamicsWorld* mDynamicsWorld;
		btRigidBody* mActor;
		btCollisionObject* mSActor;

		bool mRb;

		Vector3 mPos[2];
		float mInterpolation;
		Quaternion mOrient[2];

		bool mReadyForDelete;

		Vector3 gravLast;

	};
}

#endif
