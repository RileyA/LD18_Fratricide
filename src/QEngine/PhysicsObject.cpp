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

#include "PhysicsObject.h"

#include "btBulletDynamicsCommon.h"

namespace QE
{
	PhysicsObject::PhysicsObject(btRigidBody* actor,btDynamicsWorld* dynWorld)
        :mReadyForDelete(0)
	{
		mSActor = 0;
		mDynamicsWorld = dynWorld;
		mActor = actor;
		mRb = true;
		mPos[0] = Vector3(mActor->getWorldTransform().getOrigin().x(),
				mActor->getWorldTransform().getOrigin().y(),
				mActor->getWorldTransform().getOrigin().z());
		mPos[1] = mPos[0];
		mOrient[0] = Quaternion(1,0,0,0);
		mOrient[1] = Quaternion(1,0,0,0);
		gravLast = Vector3(0,0,0);
	}
	PhysicsObject::PhysicsObject(btCollisionObject* actor,btDynamicsWorld* dynWorld)
        :mReadyForDelete(0)
	{
		mActor = 0;
		mDynamicsWorld = dynWorld;
		mSActor = actor;
		mRb = false;
		mPos[0] = Vector3(mSActor->getWorldTransform().getOrigin().x(),
				mSActor->getWorldTransform().getOrigin().y(),
				mSActor->getWorldTransform().getOrigin().z());
		mPos[1] = mPos[0];
		mOrient[0] = Quaternion(1,0,0,0);
		mOrient[1] = Quaternion(1,0,0,0);
	}

	PhysicsObject::~PhysicsObject()
	{
		/// the physics world _should_ do this on its own??
		//mDynamicsWorld->removeRigidBody(mActor);
		//delete mActor;
		mActor = 0;
	}

	btRigidBody* PhysicsObject::getActor()
	{
		return mActor;
	}

	void PhysicsObject::setPosition(Vector3 pos)
	{
		if(mRb)
		{
			btTransform trans = mActor->getCenterOfMassTransform();
			trans.setOrigin(btVector3(pos.x,pos.y,pos.z));
			mActor->setCenterOfMassTransform(trans);//setGlobalPosition(NxVec3(pos.x,pos.y,pos.z));
		}
		else
		{
			btTransform trans = mSActor->getWorldTransform();
			trans.setOrigin(btVector3(pos.x,pos.y,pos.z));
			mSActor->setWorldTransform(trans);//setGlobalPosition(NxVec3(pos.x,pos.y,pos.z));
		}
		mPos[0] = pos;
		mPos[1] = pos;
	}

	void PhysicsObject::setOrientation(Quaternion ori)
	{
		if(mRb)
		{
			btTransform trans = mActor->getCenterOfMassTransform();
			trans.setRotation(btQuaternion(ori.x,ori.y,ori.z,ori.w));
			mActor->setCenterOfMassTransform(trans);//setGlobalPosition(NxVec3(pos.x,pos.y,pos.z));
		}
		else
		{
			btTransform trans = mSActor->getWorldTransform();
			trans.setRotation(btQuaternion(ori.x,ori.y,ori.z,ori.w));
			mSActor->setWorldTransform(trans);//setGlobalPosition(NxVec3(pos.x,pos.y,pos.z));
		}
	}

	Vector3 PhysicsObject::getPosition()
	{
		// pos[0] is the 'current' pos, [1] is the previous
		return mPos[0]*mInterpolation+mPos[1]*(1-mInterpolation);//Vector3(mActor->getGlobalPosition().x,mActor->getGlobalPosition().y,mActor->getGlobalPosition().z);
	}

	Quaternion PhysicsObject::getOrientation()
	{
		return mOrient[0];
	}

	void PhysicsObject::update(bool frame,double interpolation,Vector3 gravity)
	{
		mInterpolation = interpolation;
		if(mRb)
		{
			if(frame&&!mActor->isStaticObject())
			{
				if(gravLast!=gravity)
				{
					mActor->activate(true);
					mActor->setGravity(btVector3(gravity.x,gravity.y,gravity.z));
				}
				
				gravLast = gravity;

				//std::cout<<"grav: "<<gravity.x<<" "<<gravity.y<<" "<<gravity.z<<"\n";

				//mActor->applyGravity();
				//mActor->//addForce(NxVec3(gravity.x,gravity.y,gravity.z)*2,NX_ACCELERATION,true);
				mPos[1] = mPos[0];
				mPos[0] = Vector3(mActor->getCenterOfMassPosition().x(),mActor->getCenterOfMassPosition().y(),mActor->getCenterOfMassPosition().z());
				mOrient[1] = mOrient[0];
				mOrient[0] = Quaternion(mActor->getOrientation().x(),
					mActor->getOrientation().y(),
					mActor->getOrientation().z(),
					mActor->getOrientation().w());
			}
		}
	}

	void PhysicsObject::setUserData(void* data)
	{
		if(mActor)
		{
			mActor->setUserPointer(data);
		}
		else if(mSActor)
		{
			mSActor->setUserPointer(data);
		}
	}

	void* PhysicsObject::getUserData()
	{
		if(mActor)
		{
			return mActor->getUserPointer();
		}
		else if(mSActor)
		{
			return mSActor->getUserPointer();
		}
	}

	bool PhysicsObject::readyForDelete()
	{
        return mReadyForDelete;
	}

	void PhysicsObject::_kill()
	{
        mReadyForDelete = true;
	}

	void PhysicsObject::set2D(bool d2)
	{
		if(d2)
		{
			mActor->setAngularFactor(btVector3(0,1,0));
			mActor->setLinearFactor(btVector3(1,0,1));
		}
		else
		{
			mActor->setAngularFactor(btVector3(1,1,1));
			mActor->setLinearFactor(btVector3(1,1,1));
		}
	}

	void PhysicsObject::addForce(Vector3 dir,Vector3 pos)
	{
		mActor->applyForce(btVector3(dir.x,dir.y,dir.z),btVector3(pos.x,pos.y,pos.z));
	}

	void PhysicsObject::addImpulse(Vector3 dir,Vector3 pos)
	{
		mActor->applyImpulse(btVector3(dir.x,dir.y,dir.z),btVector3(pos.x,pos.y,pos.z));
	}

	void PhysicsObject::setVelocity(Vector3 dir)
	{
		mActor->setLinearVelocity(btVector3(dir.x,dir.y,dir.z));
	}

	Vector3 PhysicsObject::getVelocity()
	{
		return Vector3(mActor->getLinearVelocity().x(),
			mActor->getLinearVelocity().y(),
			mActor->getLinearVelocity().z());
	}

	void PhysicsObject::setMass(float mass)
	{
		mDynamicsWorld->removeRigidBody(mActor);
		btVector3 localInertia(0,0,0);
		mActor->getCollisionShape()->calculateLocalInertia(10.f,localInertia);
		mActor->setMassProps(10.f,localInertia);
		mDynamicsWorld->addRigidBody(mActor);
	}

	void PhysicsObject::setLinearFactor(Vector3 fact)
	{
		mActor->setLinearFactor(btVector3(fact.x,fact.y,fact.z));
	}

	void PhysicsObject::setAngularFactor(Vector3 fact)
	{
		mActor->setAngularFactor(btVector3(fact.x,fact.y,fact.z));
	}

	void PhysicsObject::wakeup()
	{
		mActor->setActivationState(DISABLE_DEACTIVATION);
	}

	void PhysicsObject::setCollisionGroup(short group)
	{
		mActor->getBroadphaseProxy()->m_collisionFilterGroup = group;
	}

	void PhysicsObject::setCollisionMask(short mask)
	{
		mActor->getBroadphaseProxy()->m_collisionFilterMask = mask;
	}

	void PhysicsObject::setKinematic(bool kine)
	{
		if(mActor)
		{
			if(kine)
			{
				mDynamicsWorld->removeRigidBody(mActor);
				mActor->setMassProps(0,btVector3(0,0,0));
				mActor->setCollisionFlags(mActor->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
				mActor->setActivationState(DISABLE_DEACTIVATION);
				mDynamicsWorld->addRigidBody(mActor);
			}
			else
			{
				mDynamicsWorld->removeRigidBody(mActor);
				btVector3 localInertia(0,0,0);
				mActor->getCollisionShape()->calculateLocalInertia(10.f,localInertia);
				mActor->setMassProps(10.f,localInertia);
				mActor->setCollisionFlags(mActor->getCollisionFlags() ^ btCollisionObject::CF_KINEMATIC_OBJECT);
				mActor->setActivationState(DISABLE_DEACTIVATION);
				mDynamicsWorld->addRigidBody(mActor);
			}
		}
	}




}
