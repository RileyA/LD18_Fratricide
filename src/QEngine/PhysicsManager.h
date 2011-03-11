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


#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "stdafx.h"
//#include "CoreThread.h"
#include "QEngine.h"

#include "RenderedObject.h"
#include "PhysicsObject.h"

// TODO: Needs documentation overhaul!!

class NxPhysicsSDK;
class NxScene;
class NxActor;
class NxCookingInterface;
class UserAllocator;

class btDynamicsWorld;
class btRigidBody;
class btCollisionShape;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
struct btDbvtBroadphase;
class btSequentialImpulseConstraintSolver;

namespace Ogre
{
	class Mesh;
};

namespace QE
{
	struct SimpleUserData
	{
		SimpleUserData()
		{
			data = 0;
			type = "nullptr";
		}
		SimpleUserData(void* dat,String tp)
		{
			data = dat;
			type = tp;
		}
		void* data;
		String type;
	};

	struct RaycastReport
	{
		RaycastReport()
		{
			hit = false;
		}
		RaycastReport(Vector3 n,Vector3 h,SimpleUserData* dat)
		{
			normal = n;
			hitP = h;
			hit = true;
			data = dat;
		}
		Vector3 normal;
		Vector3 hitP;
		bool hit;
		SimpleUserData* data;

	};
	/** Manages the PhysX system, acts as the physics wrapper. */
	class QENGINE_API PhysicsManager
	{
	public:

		PhysicsManager();
		~PhysicsManager();

        void setupPhysX();

		// Releases PhysX
		void ReleasePhysX();

		// Resets NX
		void ResetPhysX();

		// Begins the physics simulation
		void StartPhysX(float delta);

		// waits for calculations to finish performing various functions while waiting
		void GetPhysXResults();

		void update(float delta);

		// create a static trimesh

		PhysicsObject* createStaticTrimesh(RenderedObject* object,Vector3 pos);

		PhysicsObject* createStaticTrimesh(String mesh,Vector3 pos);

		PhysicsObject* createCube(Vector3 scale,Vector3 pos);

		PhysicsObject* createConvexHull(RenderedObject* object,Vector3 pos);

		PhysicsObject* createConvexHull(String meshname,Vector3 pos,Vector3 scale=Vector3(1,1,1));

		PhysicsObject* createSphere(float radius,Vector3 pos);


		//NxCCDSkeleton* PhysicsManager::createCCDSkeleton(Ogre::MeshPtr mesh,Ogre::Vector3 loc,Ogre::Quaternion quat);

		bool isEnabled();

		btDynamicsWorld* getScene();

		float getInterpolation();

		Vector3 getLocalGravity(Vector3 pos);

		Vector3 getGravity();

		void setGravity(Vector3 grav);

		std::vector<void*> sphereOverlap(float radius,Vector3 pos,short int group,short int mask);
		std::vector<void*> actorOverlap(PhysicsObject* actor,short int group,short int mask);
		//void sphereOverlap();

		/** Performs a raycast, returning the surface normal that it hit */
		RaycastReport raycastSimple(Vector3 pos,Vector3 dir,short group=32767,short mask=32767,Real dist=10000.f);


	private:

		//NxPhysicsSDK* mPhysicsSDK;
		//NxScene* mScene;

		btDynamicsWorld* mDynamicsWorld;
		//btCollisionWorld* mDynamicsWorld;
		btDefaultCollisionConfiguration* mCollisionConfiguration;
		btCollisionDispatcher* mDispatcher;
		btDbvtBroadphase* mBroadphase;
		btSequentialImpulseConstraintSolver* mSolver;

		//NxCookingInterface* mCooking;
		//UserAllocator* mAllocator;
		bool started;
		btRigidBody* dynamicActor;

		// This is sort of redundant but having a vector of all the objects might be helpful at some point
		std::vector<PhysicsObject*> mObjects;

		// Cache the collision shapes for reuse...
		//std::vector<btCollisionShape*> mShapes;
		std::map<std::string,btCollisionShape*> mShapes;


		float mAccumulation;
		float mInterpolation;
		float mTimeStep;

		Vector3 mGlobalGravity;

		// quick little test cube
		//PhysicsObjectManager* pomgr;
		//NxVec3 current_gravity;

	};
}

#endif
