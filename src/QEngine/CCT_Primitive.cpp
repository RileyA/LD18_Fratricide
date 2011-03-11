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
#include "CCT_Primitive.h"


// NOTE! This is some of the ugliest code ever to exist , it was originally written and rewritten about 30 times on a tight deadline for a game with PhysX like 2 years ago,
// then really horribly adapted to Bullet. It works pretty darn well as a character controller that can rotate dynamically with gravity, but don't expect the code to make any sense.


// NOTE: This is completely autonomous of the full rig, up vector and stuff are irrelevant...

namespace QE
{
	Ogre::Vector3 NxVector3(const btVector3& vect)
	{
		return Ogre::Vector3(vect.x(),vect.y(),vect.z());
	}
	NxVec3 Vector3Nx(Ogre::Vector3 vect)
	{
		return NxVec3(vect.x,vect.y,vect.z);
	}
	NxVec3 QVector3Nx(Vector3 vect)
	{
		return NxVec3(vect.x,vect.y,vect.z);
	}
	CCTPrimitive::CCTPrimitive(float radius_,float increment_,float skinWidth_,const btVector3& pos,btDynamicsWorld* scene)
	{
		Scene = scene;
		radius = radius_;
		increment = increment_;
		skinWidth = skinWidth_;

		/*NxBodyDesc bodyDesc;
		bodyDesc.setToDefault();
		NxActorDesc actorDesc;
		actorDesc.setToDefault();

		NxSphereShapeDesc sphereDesc;
		sphereDesc.setToDefault();
		sphereDesc.radius = radius_;
		sphereDesc.group = 15;

		actorDesc.shapes.pushBack(&sphereDesc);
		actorDesc.body            = &bodyDesc;
		actorDesc.density = 10.0f;
		actorDesc.globalPose.t    = pos; //position at the origin.

		act = Scene->createActor(actorDesc);

		act->getShapes()[0]->setFlag(NX_SF_DISABLE_RAYCASTING,true);

		NxVec3 wtf = act->getGlobalPosition();

		act->raiseBodyFlag(NX_BF_KINEMATIC);
		act->setGroup(15);*/

		sphShape = new btSphereShape(radius);
		act = new btRigidBody(0.f,0,sphShape,btVector3(0,0,0));
		// it is a kinematic and shouldn't react to any collisions itself

		// cast up??
		dynamic_cast<btDiscreteDynamicsWorld*>(scene)->addRigidBody(act,QE::COL_KINEMATICS,QE::COL_KINEMATICS|QE::COL_DYNAMICS|QE::COL_STATICS);
		// tell bullet that it is kinematic...
		act->setCollisionFlags(act->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

		// sphere for overlap/sweep checking and the like
		sph = NxSphere(pos,radius+skinWidth);


		// default to enabled...
		enabled = true;
		position = pos;
		upDir = NxVec3(0,1,0);
	}
	CCTPrimitive::~CCTPrimitive()
	{
		delete sphShape;
		// does nothing for the moment
	}
	// just an overlap check, 'teleports' the primitive to the given pos
	// can more or less be used as a cheap overlap check function
	bool CCTPrimitive::setPosition(const btVector3& pos,bool set)
	{
		NxVec3 old = sph.center;
		sph.center = pos;
		

		OverlapResultCallback2 overlapCall = OverlapResultCallback2();
		overlapCall.m_collisionFilterGroup = QE::COL_STATICS;
		overlapCall.m_collisionFilterMask = QE::COL_STATICS|QE::COL_DYNAMICS;

		//OverlapResultCallback overlapCall(sph.center,sph.center+btVector3(0.000001f,0.000001f,0.000001f));


		// stationary sweep test... (basically an overlap test, there may be a better alternative to this but oh well for now...)
		//Scene->convexSweepTest(&btSphereShape(sph.radius),btTransform(btQuaternion(0,0,0,1),sph.center),btTransform(btQuaternion(0,0,0,1),sph.center+btVector3(0.000001f,0.000001f,0.000001f)),overlapCall);
		//Scene->convexSweepTest(&btSphereShape(sph.radius),btTransform(btQuaternion(0,0,0,1),sph.center),btTransform(btQuaternion(0,0,0,1),sph.center+btVector3(0.000001f,0.000001f,0.000001f)),overlapCall);
		btSphereShape sphsh = btSphereShape(sph.radius);
		sphsh.setMargin(0.f);
		btCollisionObject* bcobj = new btCollisionObject();
		bcobj->setWorldTransform(btTransform(btQuaternion(0,0,0,1),pos));
		bcobj->setCollisionShape(&sphsh);
		Scene->contactTest(bcobj,overlapCall);
		delete bcobj;
		//unsigned int numOverlap = Scene->overlapSphereShapes(sph,NX_ALL_SHAPES,0,NULL,NULL,0|1|2|5,0,true);//-1 -> !15 || 0|1|2|5
		if(overlapCall.hits<=0)
		{
			if(set)
			{
				position = pos;
				act->setWorldTransform(btTransform(btQuaternion(0,0,0,1),btVector3(pos.x(),pos.y(),pos.z())));
			}
			if(!set)
			{
				sph.center = old;
			}
			return true;
		}
		else
		{
			//std::cout<<"failed: "<<overlapCall.hits<<"\n";
			//std::cout<<"Hmm: "<<numOverlap<<"\n";
			sph.center = old;
			return false;
		}
	}

	// gets the furthest move position posible with a given vector
	moveStruct CCTPrimitive::move(const btVector3& directiona,bool slide,bool lowerSW)
	{
		// grab a copy...
		btVector3 direction = directiona;
		if(direction.isZero())
		{
			return moveStruct(position,btVector3(0,0,0),btVector3(0,0,0),btVector3(0,0,0),NULL);
		}
		if(lowerSW)
		{
			skinWidth = 0.0125f;
			sph.radius = radius + skinWidth;
			increment = 0.00125f;
		}

		// finite...??
		//if(direction.length2()<10000000.f)
		//{
			//std::cout<<"fixing...\n";
		//	direction.setX(0.0001f);
		//	direction.setY(0.0001f);
		//	direction.setZ(0.0001f);
		//}
		// okay... yay for the third time I've had to write one of these types of function but oh well, to work...


		NxVec3 originalPos = position;
		
		//NxU32 flags = NX_SF_STATICS|NX_SF_DYNAMICS;

		//NxSweepQueryHit results[2];
		//NxSweepQueryHit result;
		NxVec3 incr = direction;
		incr.normalize();
		incr*=increment;

		btCollisionWorld::ClosestConvexResultCallback closeCall(position,direction);
		closeCall.m_collisionFilterGroup = QE::COL_STATICS;
		closeCall.m_collisionFilterMask = QE::COL_STATICS|QE::COL_DYNAMICS;

		btSphereShape tempShape = btSphereShape(radius);
		tempShape.setMargin(0.f);

		Scene->convexSweepTest(&tempShape,btTransform(btQuaternion(0,0,0,1),sph.center),btTransform(btQuaternion(0,0,0,1),sph.center+direction),closeCall);

		// since actor sweep didn't allow activegroup filtering...
		//double ab2 = clock();

		

		//unsigned int nrH = Scene->linearCapsuleSweep(NxCapsule(NxSegment(position,position*1.0000001f),radius),direction,flags,NULL,1,&result,NULL,0|1|2|5,0);//act->linearSweep(direction,flags,NULL,1,&result,NULL);
			


		/*double bc2 = clock();
		double cd2 = (bc2-ab2)/1000;
		if(cd2>0.004)
		{
			std::cout<<cd2<<" : "<<" AHCRAP2!"<<"\n";
			std::cout<<"dir: "<<direction.x<<" "<<direction.y<<" "<<direction.z<<"\n";

			std::cout<<"dir: "<<direction.x<<" "<<direction.y<<" "<<direction.z<<"\n";

			//std::cout<<nrLoops<<" LOOPS!\n";
		}*/

		NxVec3 resultPos;
		NxVec3 changeVec;
		btCollisionObject* acto = 0;
		if(closeCall.hasHit())
		{
			NxVec3 svct = closeCall.m_hitNormalWorld;//result.normal;
			if(slide)
			{
				//std::cout<<"normal = "<<svct.x<<" "<<svct.y<<" "<<svct.z<<"\n";
			}

			float ap = closeCall.m_closestHitFraction;
			btCollisionObject* spT = closeCall.m_hitCollisionObject;
			acto = spT;
			changeVec = direction*ap;
			resultPos = direction*ap + originalPos;
		}
		else
		{
			//std::cout<<"NOOOOOOOO = "<<direction.x()<<" "<<direction.y()<<" "<<direction.z()<<"\n";
			//std::cout<<"No obstructions!\n";
			changeVec = direction;
			resultPos = direction + position;
		}
		bool finished = false;

		float change = 0.0f;
		float change2 = 0.0f;

		int a = -1;
		int b = -1;

		NxVec3 rp2 = resultPos;
		NxVec3 incr2;
		bool secondary = false;
		if(closeCall.m_hitCollisionObject&&slide)
		{
			secondary = true;
			incr2 = closeCall.m_hitNormalWorld;
			incr2.normalize();
			incr2*=increment;

		}

		sph.radius = radius+skinWidth+increment/2;//PRIMITIVE_OVERLAP_STANDOFF;//radius+skinWidth+increment/2;
		// loop until the secondary capsule is no longer penetrating
		//unsigned long int nrLoops = 0;
		///moveStruct mv = cctp[b]->move(total,slidingE);
		/// 0.169 seconds???

				//if(changeVec.isZero())
				//{

				//}

		while(!finished)
		{
			//nrLoops++;
			if(!setPosition(resultPos,false)&&a<20)//total guess here... remove !secondary if it kills it...
			{
				if(Ogre::Math::Abs(change+incr.length())>=Ogre::Math::Abs(changeVec.length()))//+incr.magnitude()*2
				{
					resultPos = originalPos;
					changeVec = NxVec3(0,0,0);
					finished = true;
				}
				else
				{
					a++;
					resultPos-=incr;
					change+=incr.length();
				}
			}
			else
			{
				finished = true;
				if(changeVec.isZero())
				{

				}
				else
				{
					changeVec -= (changeVec/changeVec.length())*change;
				}
				if(a>=20)
				{

					resultPos = originalPos;

				}
			}
			if(secondary)
			{
				if(!setPosition(rp2,false)&&b<20)
				{
						b++;
						rp2+=incr2;
						change2+=incr2.length();
				}
				else
				{
					resultPos = rp2;
					changeVec = (changeVec)+(incr2*static_cast<Real>(b));
					finished = true;
				}
			}
			else
			{

			}
		}

		//if(nrLoops>40)
		//{
		//}
		if(lowerSW)
		{
			skinWidth = 0.00125f;
			increment = 0.000125f;
			sph.radius = radius + skinWidth;
		}

		//std::cout<<"?: "<<resultPos.x()<<" "<<resultPos.y()<<" "<<resultPos.z()<<"\n";

		sph.radius = radius + skinWidth;
		return moveStruct(resultPos,resultPos-originalPos,closeCall.m_hitPointWorld,closeCall.m_hitNormalWorld,acto);
	};
}