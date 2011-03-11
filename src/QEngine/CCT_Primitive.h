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


#ifndef CCTPRIMITIVE_H
#define CCTPRIMITIVE_H

#include "stdafx.h"
#include "QEngine.h"
//#include <NxPhysics.h>
#include "Ogre.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "PhysicsMasks.h"

// NOTE! This is some of the ugliest code ever to exist , it was originally written and rewritten about 30 times on a tight deadline for a game with PhysX like 2 years ago,
// then really horribly adapted to Bullet. It works pretty darn well as a character controller that can rotate dynamically with gravity, but don't expect the code to make any sense.


typedef btVector3 NxVec3;

namespace QE
{
	class NxSphere
	{
	public:
		NxSphere()
		{
			radius = 1.f;
			center = btVector3(0,0,0);
		}
		NxSphere(const btVector3& pos,Real rad)
		{
			radius = rad;
			center = pos;
		}

		btVector3 center;
		Real radius;
	};

	class NxCapsule
	{
	public:

		NxCapsule(Real hei,Real rad)
		{
			radius = rad;
			height = hei;
		}

		NxCapsule()
		{
			height = 1.f;
			radius = 1.f;
		}

		Real radius;
		Real height;
		btVector3 up;
	};
	// legacy nxvec->ogrevec conversion, someday I'd like to convert all of this to QEvects...
	Ogre::Vector3 NxVector3(const btVector3& vect);

	btVector3 Vector3Nx(Ogre::Vector3 vect);

	btVector3 QVector3Nx(Vector3 vect);

	struct	OverlapResultCallback : public btCollisionWorld::ConvexResultCallback
	{
		OverlapResultCallback(const btVector3&	convexFromWorld,const btVector3&	convexToWorld)
		:m_convexFromWorld(convexFromWorld),
		m_convexToWorld(convexToWorld)
		{
			// don't collide with kinematics (i.e. the character controller pieces)
			m_collisionFilterGroup = COL_STATICS;//???
			m_collisionFilterMask = COL_STATICS|COL_DYNAMICS;
		}

		// default constructor
		OverlapResultCallback()
		:m_convexFromWorld(btVector3(0,0,0)),
		m_convexToWorld(btVector3(0,0,0))
		{
			m_collisionFilterGroup = COL_STATICS;//???
			m_collisionFilterMask = COL_STATICS|COL_DYNAMICS;
		}

		btVector3	m_convexFromWorld;
		btVector3	m_convexToWorld;

		
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult,bool normalInWorldSpace)
		{
			btAssert(convexResult.m_hitFraction <= m_closestHitFraction);

			m_closestHitFraction = convexResult.m_hitFraction;
			return convexResult.m_hitFraction;
		}
	};

	struct	OverlapResultCallback2 : public btCollisionWorld::ContactResultCallback
	{
		unsigned int hits;

		OverlapResultCallback2()
		{
			hits = 0;
			m_collisionFilterGroup = COL_STATICS;//???
			m_collisionFilterMask = COL_STATICS|COL_DYNAMICS;
		}

		virtual	btScalar addSingleResult(btManifoldPoint& cp,	const btCollisionObject* colObj0,int partId0,int index0,const btCollisionObject* colObj1,int partId1,int index1)
		{
			if(cp.getDistance()<=0)
			{
			++hits;
			}
			return 0.f;
		}
	};


	struct	OverlapResultCallback3 : public btCollisionWorld::ContactResultCallback
	{
		unsigned int hits;

		OverlapResultCallback3()
		{
			hits = 0;
			m_collisionFilterGroup = COL_STATICS;//???
			m_collisionFilterMask = COL_STATICS|COL_DYNAMICS;
		}

		btCollisionObject* obj0;
		btCollisionObject* obj1;

		virtual	btScalar addSingleResult(btManifoldPoint& cp,	const btCollisionObject* colObj0,int partId0,int index0,const btCollisionObject* colObj1,int partId1,int index1)
		{
			if(cp.getDistance()<=0)
			{
			++hits;
			obj0 = const_cast<btCollisionObject*>(colObj0);
			obj1 = const_cast<btCollisionObject*>(colObj1);
			}
			return 0.f;
		}
	};

	struct	OverlapResultCallback4 : public btCollisionWorld::ContactResultCallback
	{
		unsigned int hits;

		OverlapResultCallback4()
		{
			hits = 0;
			m_collisionFilterGroup = COL_STATICS;//???
			m_collisionFilterMask = COL_STATICS|COL_DYNAMICS;
		}

		btCollisionObject* obj0;
		btCollisionObject* obj1;

		std::vector<void*> mObjectData;

		virtual	btScalar addSingleResult(btManifoldPoint& cp,	const btCollisionObject* colObj0,int partId0,int index0,const btCollisionObject* colObj1,int partId1,int index1)
		{
			if(cp.getDistance()<=0)
			{
				++hits;
				obj0 = const_cast<btCollisionObject*>(colObj0);
				obj1 = const_cast<btCollisionObject*>(colObj1);
				mObjectData.push_back(obj1->getUserPointer());
			}
			return 0.f;
		}
	};

	struct	SweepClosestCall : public btCollisionWorld::ClosestConvexResultCallback
	{
	public:
		SweepClosestCall(const btVector3& rayFrom,const btVector3& rayTo)
			:btCollisionWorld::ClosestConvexResultCallback(rayFrom,rayTo)
		{
			// do nada...
		}
		// default
		SweepClosestCall()
			:btCollisionWorld::ClosestConvexResultCallback(btVector3(0,0,0),btVector3(0,1,0))
		{
			
		}
		// nothin' else!
	};

	// stores top 5 hits
	struct	ConvexCollisionsCallback : public btCollisionWorld::ConvexResultCallback
	{
		ConvexCollisionsCallback()
		{
			m_hitCollisionObject[0]=0;
			m_hitCollisionObject[1]=0;
			m_hitCollisionObject[2]=0;
			m_hitCollisionObject[3]=0;
			m_hitCollisionObject[4]=0;
		}

		ConvexCollisionsCallback(const btVector3&	convexFromWorld,const btVector3&	convexToWorld)
		:m_convexFromWorld(convexFromWorld),
		m_convexToWorld(convexToWorld)
		//m_hitCollisionObject(0)
		{
			m_hitCollisionObject[0]=0;
			m_hitCollisionObject[1]=0;
			m_hitCollisionObject[2]=0;
			m_hitCollisionObject[3]=0;
			m_hitCollisionObject[4]=0;
		}

		btVector3	m_convexFromWorld;//used to calculate hitPointWorld from hitFraction
		btVector3	m_convexToWorld;

		btVector3	m_hitNormalWorld;
		btVector3	m_hitPointWorld;
		btCollisionObject*	m_hitCollisionObject[5];
		
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult,bool normalInWorldSpace)
		{
//caller already does the filter on the m_closestHitFraction
			btAssert(convexResult.m_hitFraction <= m_closestHitFraction);
						
			m_closestHitFraction = convexResult.m_hitFraction;

			btCollisionObject* temp;
			btCollisionObject* temp2;

			temp = m_hitCollisionObject[1];
			m_hitCollisionObject[1] = m_hitCollisionObject[0];
			m_hitCollisionObject[0] = convexResult.m_hitCollisionObject;

			temp2 = m_hitCollisionObject[2];
			m_hitCollisionObject[2] = temp;

			temp = m_hitCollisionObject[3];
			m_hitCollisionObject[3] = temp2;

			m_hitCollisionObject[4] = temp;


			m_hitPointWorld = convexResult.m_hitPointLocal;
			return convexResult.m_hitFraction;
		}
	};


	struct moveStruct
	{
	public:
		moveStruct(const btVector3& fp,const btVector3& cp,const btVector3& pt,const btVector3& norm,btCollisionObject* ac)
		{
			finalPos = NxVector3(fp);
			change = NxVector3(cp);
			//hit = h;
			act = ac;
			point = NxVector3(pt);
			normal = NxVector3(norm);
		}
		Vector3 finalPos;
		Vector3 change;
		Vector3 point;
		Vector3 normal;
		//SweepClosestCall hit;
		btCollisionObject* act;/// maybe collision object???
	};

	// The basic CCT primitive, a sphere that can slide and do all that wonderful CCT crap, to be compounded into the main character controller
	class CCTPrimitive
	{
	public:

		// constructor
		CCTPrimitive(float radius_,float increment_,float skinWidth_,const btVector3& pos,btDynamicsWorld* scene);
		// destructor
		~CCTPrimitive();

		// gets the position
		NxVec3 getPosition()
		{
			return position;
		}

		// sets the position and returns whether or not the volume check passed, (if not then the move was not made) has optional final setting of positions so that it can be used as just an overlap style check
		bool setPosition(const btVector3& pos,bool set);

		// tests a move in a given direction, and goves the result vector, does NOT use sliding, for that one must use several such calls
		// This sweeps then does volume checks and increments backwards in order to maintain askinWidth so that the sweep shape isn't killed by numeric instability
		// passes back the hit query if one exists
		moveStruct move(const btVector3& direction,bool slide,bool lowerSW=false);

		// returns the radius (the radius can't be changed in simulation)
		float getRadius()
		{
			return radius;
		}

		// gets the skinwidth, this can be changed mid simulation, but it isn't exactly reccomended...
		float getSkinWidth()
		{
			return skinWidth;
		}

		// This changes the skinWidth, and returns whether or not it succeeded (based on a volume check)
		bool setSkinWidth(float newWidth)
		{
			//nada so far
			return false;
		}

		// gets the increment for the recursive thingy, smaller is slower but more accurate
		float getIncrement()
		{
			return increment;
		}

		void setIncrement(float inc)
		{
			increment = inc;
		}

	//private:

		// stuff for defining the object and its properties.. (that was vague..)
		float increment;
		float radius;
		float skinWidth;

		NxVec3 position;

		// spherical kinematic actor, used for sweeps
		btRigidBody* act;
		btSphereShape* sphShape;
		// sphere shape, not actually a physical object, but it is used for volume testing (i.e. Scene->overlapSphereShapes(sph,blah,blah,etc);)
		NxSphere sph;

		// so that certain primitives can be disbaled for crouching etc etc...
		bool enabled;

		btDynamicsWorld* Scene;

		NxVec3 upDir;


	};
}

#endif
