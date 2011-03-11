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
#include "QuantaCharacterController.h"
#include "CCT_Compound.h"
#include "PhysicsManager.h"

namespace QE
{
	QCharacterController::QCharacterController(PhysicsManager* pmgr, Vector3 startPos) : CharacterController(pmgr,startPos),mGracePeriod(0)
	{
		mTimeStep = 1.0f/100.0f;
		mAccumulation = 0.0f;
		mInterpolation = 0.0f;
		q_1 = Quaternion(Ogre::Quaternion::IDENTITY);
		q_2 = Quaternion(Ogre::Quaternion::IDENTITY);
		setPlayerSpeed(4.f,7.5f,0.5f);
		setPlayerGravitySpeed(0.9f*2.f,1.5f*2.f,2.0f*2.f);
		mPlayerSpeedInterpolation = 0;
		mPlayerGravityInterpolation = 0;

		mCrouching = false;
		mCrouched = false;
		mCrouchLevel = 0.3f;
		mInMidair = true;
		mMidairTime = 0;
		mMidairTimeBase = 0;
		mJumpFactor = 0;
		mInAirLast = true;
		mOffset = 0;
		mIsJumping = false;
		mGroundImpactSpd = 0.0f;
		mGroundImpact = 0.0f;
		mHittingGround = false;
		mImpactLevel = 0.0f;

		mBobSwap = false;
		mBobPeriod = 0.0f;
		mBobLR = false;
		//mJumpDir = NxVec3(0,0,0);
	}


	QCharacterController::~QCharacterController()
	{
		deinitialize();
	}

	void QCharacterController::setMove(bool setting)
	{
		mController->move_ = setting;
	}

	void QCharacterController::setMoveVector(Vector3 move)
	{
		moveV = move;
	}

	void QCharacterController::update(float delta)
	{
		mAccumulation += delta;

		while(mAccumulation>=mTimeStep)
		{
			btVector3 upVectorFinal = QVector3Nx(mPhysicsManager->getLocalGravity(Vector3(NxVector3(mController->getPosition()))));//gmgr->gvmgr->getGravityLocalCCT(NxVector3(mController->getPosition()));
			NxVec3 GravityVector = upVectorFinal;
			upVectorFinal.normalize();
			upVectorFinal*=-1;

			bool testing = false;

			NxVec3 opo = NxVec3(0,0,0);
			NxVec3 fpo = NxVec3(0,0,0);

			float abbc = mPhysicsManager->getLocalGravity(NxVector3(mController->getPosition()+mController->getUpVector())).length();//gmgr->gvmgr->getGravityLocalCCT(NxVector3(mController->getPosition()+mController->getUpVector())).magnitude();
			q_1 = q_2;

			if(mController->getUpVector()!=upVectorFinal&&Ogre::Degree(NxVector3(mController->getUpVector()).angleBetween(NxVector3(upVectorFinal)))>Ogre::Degree(0.075f)&&abbc>0.2f)
			{
				testing = true;

				Ogre::Vector3 up1 = NxVector3(mController->getUpVector());

				//std::cout<<"pos1: "<<mController->cctp[0]->getPosition().x()<<"\n";
				mController->setUpVector(upVectorFinal,mTimeStep);
				//std::cout<<"pos2: "<<mController->cctp[0]->getPosition().x()<<"\n";

				Ogre::Vector3 up2 = NxVector3(mController->getUpVector());
				Ogre::Quaternion q2 = up1.getRotationTo(up2,Ogre::Vector3::UNIT_X);
				q_2 = Quaternion(q2*q_1.toOgre());
			}

			//dnode02->setOrientation(Ogre::Quaternion::IDENTITY);
			//dnode02->setDirection(NxVector3(mController->getUpVector()),Ogre::SceneNode::TS_LOCAL,Ogre::Vector3::UNIT_Y);

			opo = mController->position;

			bool mMidair = false;// = inAir();
			float playerSpeed = 0;

			if(mController->move_)
			{
				Ogre::Vector3 v3 = Ogre::Vector3(moveV.x,moveV.y,moveV.z);
				Ogre::Plane pln = Ogre::Plane(NxVector3(mController->getUpVector()),0);
				pln.projectVector(v3);
				v3.normalise();
				moveV = Ogre::Vector3(moveV.x,moveV.y,moveV.z);
				/// run = 6.225
				/// walk = 2.225
				float spdd = mPlayerSpeedStart*(1-mPlayerSpeedInterpolation)+mPlayerSpeedTop*(mPlayerSpeedInterpolation);//7.0f;//2.225f*(1-runTimer) + 6.225*runTimer

				if(abbc<0.2)
				{
					mController->move(NxVec3(moveV.x,moveV.y,moveV.z)*mTimeStep*spdd,true,mTimeStep,true);
				}
				else
				{
					//std::cout<<"Try move....\n";
					mController->move(NxVec3(moveV.x,moveV.y,moveV.z)*mTimeStep*spdd,true,mTimeStep);
				}
				fpo = mController->position;
				playerSpeed = Ogre::Math::Abs((fpo-opo).length());
				//playerMotionDir = NxVector3(fpo-opo);
				//std::cout<<"Speed: "<<playerSpeed/(gmgr->tmgr->getTimeSpeed()/60.0f)<<"\n";
			}
			else
			{
				playerSpeed = 0;
				//playerMotionDir = Ogre::Vector3(0,0,0);
			}

			mMidair = inAir();

			float lastSpeed = mPlayerSpeedLast;
			mPlayerSpeedLast = playerSpeed;

			if(mPlayerSpeedLast>lastSpeed)
			{
				mPlayerSpeedInterpolation+=(mTimeStep/mPlayerSpeedAccel);
				mGracePeriod = 0;
			}
			else if((mPlayerSpeedLast<mPlayerSpeedStart*mTimeStep||mHittingGround)&&!mMidair)
			{
				// we give a little bit of padding so if a gravity issue ir something stops it for a frame
				// it doesn't slow abruptly.
				++mGracePeriod;
				if(mGracePeriod>=2)
				{
					mPlayerSpeedInterpolation=0;
				}
			}
			else
			{
				mGracePeriod = 0;
			}

			if(mMidair)
			{
				//mPlayerSpeedLast = 0;
			}

			if(mPlayerSpeedInterpolation<0)
			{
				mPlayerSpeedInterpolation = 0;
			}
			if(mPlayerSpeedInterpolation>1)
			{
				mPlayerSpeedInterpolation = 1;
			}


			NxVec3 gravityCont = QVector3Nx(mPhysicsManager->getLocalGravity(NxVector3(mController->getPosition()+mController->getUpVector())))*0.9f;//gmgr->gvmgr->getGravityLocalCCT(NxVector3(mController->getPosition()+mController->getUpVector()))*0.9f;
			NxVec3 gcN = gravityCont;
			gcN.normalize();
			if(NxVector3(gcN).angleBetween(Ogre::Vector3(0,1,0))<Ogre::Degree(1))
			{
				gravityCont = NxVec3(0,1,0)*gravityCont.length();
			}
			if(NxVector3(gcN).angleBetween(Ogre::Vector3(0,-1,0))<Ogre::Degree(1))
			{
				gravityCont = NxVec3(0,-1,0)*gravityCont.length();
			}

				//||NxVector3(gravityCont).angleBetween(Ogre::Vector3(0,-1,0))<Ogre::Degree(1))
			if(mIsJumping)
			{
				gravityCont = gravityCont - gravityCont*mJumpFactor;
				mJumpFactor -= 1.f*static_cast<Real>(mTimeStep);
				if(mJumpFactor<0)
				{
					mJumpFactor = 0;
					mIsJumping = false;
				}

			}
			NxVec3 ogp = mController->getPosition();

			float ff = 1.0;
			if(testing )
			{
				ff=0.5f*(1-mController->mUpInterp)+(mController->mUpInterp*1.0f);
				mPlayerGravityInterpolation = mPlayerGravityInterpolation/5;
			}

			if(abbc>0.2f)
			{
				mController->move(ff*((gravityCont*(mPlayerGravityBase*(1-mPlayerGravityInterpolation)+mPlayerGravityTerminal*(mPlayerGravityInterpolation)))*mTimeStep),false,mTimeStep);
			}

			NxVec3 pogp = mController->getPosition();

			float lastGravity = mPlayerGravityLast;
			mPlayerGravityLast = btVector3(ogp-pogp).length();

			if(mMidair&&!mHittingGround)
			{
				mGroundImpact = mPlayerGravityLast/mTimeStep*1.45f;
			}

			if(mPlayerGravityLast>lastGravity)
			{
				mPlayerGravityInterpolation+=(mTimeStep/mPlayerGravityAccel);
			}
			else if(mPlayerGravityLast<mPlayerGravityBase*mTimeStep)
			{
				mPlayerGravityInterpolation=0;
			}

			if(mPlayerGravityInterpolation<0)
			{
				mPlayerGravityInterpolation = 0;
			}
			if(mPlayerGravityInterpolation>1)
			{
				mPlayerGravityInterpolation = 1;
			}


			if(mIsJumping&&mJumpFactor>1)
			{
				NxVec3 fgp = mController->getPosition();
				Ogre::Vector3 dirT = NxVector3(fgp-ogp);
				dirT.normalise();
				Ogre::Vector3 upVC = NxVector3(gravityCont*mTimeStep*ff);
				upVC.normalise();
				if(!dirT.directionEquals(upVC,Ogre::Radian(Ogre::Degree(45.0f))))
				{
					mJumpFactor = 1;
				}
			}
			//Ogre::Vector3 pMoDir = NxVector3(opo - player->position);

			// takes gravity into account
			//Ogre::Vector3 velocity = pMoDir/deltaTT;
			//float vel[] = {velocity.x,velocity.y,velocity.z};
			//alListenerfv(AL_VELOCITY,vel);
			if(mCrouching)
			{
				if(mCrouched)
				{
					float oldCrouch = mController->offset;
					mCrouchLevel+=mTimeStep*1.f;
					if(mCrouchLevel>mOffset)
					{
						mCrouchLevel = mOffset;
						mCrouched = false;
						mCrouching = false;
					}
					mController->offset = mCrouchLevel;
					if(!mController->setPosition(mController->getPosition(),false,mController->getUpVector(),false))
					{
						mController->offset = oldCrouch;
						mCrouchLevel = oldCrouch;
					}
					else
					{

					}
				}
				else
				{
					float oldCrouch = mController->offset;
					mCrouchLevel-=mTimeStep*0.5f;
					if(mCrouchLevel<0.06f)
					{
						mCrouchLevel = 0.06f;
						mCrouched = true;
						mCrouching = false;
					}
					mController->offset = mCrouchLevel;
					if(!mController->setPosition(mController->getPosition(),false,mController->getUpVector(),false))
					{
						mController->offset = oldCrouch;
						mCrouchLevel = oldCrouch;
					}
					else
					{

					}
				}
			}


			if(mInAirLast2&&!mHittingGround)
			{
				mHittingGround = true;
				if(mGroundImpact>0.05&&false)
				{
				mGroundImpactSpd = mGroundImpact;
				mGroundImpact/=40.f;
				}
				else
				{
					mHittingGround = false;
					mGroundImpactSpd = 0.f;
					mGroundImpact = 0.f;
				}
				//mGroundImpact
			}

			if(mImpactLevel>0)
			{
				//float recoverSpeed = 1.0f;

				mImpactLevel-=mTimeStep*1.6f;
				if(mImpactLevel<0)
				{
					mImpactLevel = 0;
				}
			}

			if(mHittingGround)
			{
				float spdC = 1.0f;
				spdC = 0.9f*((mGroundImpact/(mGroundImpactSpd/40)))+(0.095f*(1-(mGroundImpact/(mGroundImpactSpd/40))));
				mImpactLevel+=mTimeStep*mGroundImpactSpd*spdC;
				if(mImpactLevel>0.8f)
				{
					mImpactLevel = 0.8f;
				}
				mGroundImpact-=mTimeStep*(mGroundImpactSpd*0.5f)*spdC;
				if(mGroundImpact<0)
				{
					mGroundImpact = 0;
					mHittingGround = false;
				}
			}


			if(mBobSwap)
			{
				if(!mMidair)
					mBobPeriod+=mPlayerSpeedLast*1.2f;
			}
			else
			{
				if(!mMidair)
					mBobPeriod-=mPlayerSpeedLast*1.2f;
			}
			if(abs(mBobPeriod)>=1.0f)
			{
				if(mBobPeriod>0)
				{
					mBobPeriod = 0.99f;
				}
				else
				{
					mBobPeriod = -0.99f;
					mBobLR = !mBobLR;
					if(mBobLRPeriod>0)
					{
						mBobLRPeriod = 1.99f;
					}
					else
					{
						mBobLRPeriod = -1.99f;
					}

				}
				mBobSwap = !mBobSwap;
			}

			NxVec3 upvv = mController->getUpVector();
			upvv*=(mImpactLevel+mBobPeriod*0.f);
			Vector3 right1 = Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->getCamera("mMainCam")->getDerivedRight();


			if(mBobLR)
			{
				if(!mMidair)
					mBobLRPeriod -= mPlayerSpeedLast*1.2f;
			}
			else
			{
				if(!mMidair)
					mBobLRPeriod += mPlayerSpeedLast*1.2f;
			}

			if(mBobLRPeriod>0)
			{
				right1*=(0.0625f*((mBobLRPeriod)));
			}
			else
			{
				right1*=(0.0625f*((mBobLRPeriod)));
			}
			right1*=0;

			p_2 = p_1;
			//p_1 = Vector3(mController->getEyePosition().x(),mController->getEyePosition().y(),mController->getEyePosition().z());
			p_1 = Vector3(mController->getEyePosition().x()-upvv.x()-right1.x,mController->getEyePosition().y()-upvv.y()-right1.y,mController->getEyePosition().z()-upvv.z()-right1.z);
			// if our speed has gone up then increment the interpolation thingy

			mAccumulation -= mTimeStep;
		}

		mInterpolation = mAccumulation/mTimeStep;
	}

	void QCharacterController::initialize()
	{
		mController = new CCTCompound(5,1.029f,0.4f,mPhysicsManager->getScene(),QVector3Nx(mStartPos));
		mOffset = mController->offset;
		mController->move_ = false;
	}

	void QCharacterController::deinitialize()
	{
		delete mController;
		mController = 0;
	}

	Vector3 QCharacterController::getPosition()
	{
		return (p_1*mInterpolation)+(p_2*(1-mInterpolation));
	}

	Quaternion QCharacterController::getOrientation()
	{
		return Quaternion(Ogre::Quaternion::Slerp(mInterpolation,q_1.toOgre(),q_2.toOgre()));
	}

	void QCharacterController::setQueriesEnabled(bool set)
	{
		if(set)
		{
			mController->enableQs();
		}
		else
		{
			mController->disableQs();
		}
	}

	void QCharacterController::setPlayerSpeed(float start,float top,float accel)
	{
		mPlayerSpeedStart = start;
		mPlayerSpeedTop = top;
		mPlayerSpeedAccel = accel;
	}

	void QCharacterController::setPlayerGravitySpeed(float base,float terminal,float accel)
	{
		mPlayerGravityBase = base;
		mPlayerGravityTerminal = terminal;
		mPlayerGravityAccel = accel;
	}

	bool QCharacterController::jump(float factor)
	{
			if(!mInMidair&&mJumpFactor<0.775f&&!mHittingGround&&mImpactLevel<=0)//&&!isJumping
			{
				mJumpFactor = factor;
				mIsJumping = true;
				//mJumpDir = mController->getUpVector();
				return true;
			}

			return false;
	}

	void QCharacterController::crouchToggle(unsigned int _switch)
	{
		if(!_switch)
		{
			mCrouching = true;
		}
		else if(_switch==1)
		{
			if(mCrouched)
			{
				mCrouching = true;
			}
		}
		else if(_switch==2)
		{
			if(!mCrouched)
			{
				mCrouching = true; 
			}
		}
	}

	bool QCharacterController::inAir()
	{
		Ogre::Quaternion quat = Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->getCamera("mMainCam")->getDerivedOrientation();
		btQuaternion qu;
		qu.setX(quat.x);
		qu.setY(quat.y);
		qu.setZ(quat.z);
		qu.setW(quat.w);

		mInAirLast2 = false;

		OverlapResultCallback2 overlapCall = OverlapResultCallback2();
		overlapCall.m_collisionFilterGroup = QE::COL_STATICS;
		overlapCall.m_collisionFilterMask = QE::COL_STATICS|QE::COL_DYNAMICS;
		// just a guess... but half extents of the PhysX one...?
		btBoxShape sphsh = btBoxShape(btVector3(0.25f,(mController->cctp[0]->radius*0.6525f),0.25f));
		btCollisionObject* bcobj = new btCollisionObject();
		btVector3 boxPosition = mController->cctp[0]->getPosition()-0.5f*mController->cctp[0]->radius*mController->upVector;
		bcobj->setWorldTransform(btTransform(qu,boxPosition));
		bcobj->setCollisionShape(&sphsh);
		this->mPhysicsManager->getScene()->contactTest(bcobj,overlapCall);
		delete bcobj;

		//unsigned int a = 5;//mController->Scene->overlapOBBShapes(NxBox(mController->cctp[0]->getPosition()-0.5f*mController->cctp[0]->radius*mController->upVector,NxVec3(0.25f,mController->cctp[0]->radius*0.6525f,0.25f),NxMat33(qu)),NX_ALL_SHAPES,0,NULL,NULL,0|1|2|5,0,true);
		bool tInAir = true;
		if(overlapCall.hits>0)
		{
			tInAir = false;
			mInAirLast2 = false;
		}
		if(!tInAir&&mInAirLast)
		{
			mMidairTimeBase = clock()/1000.0f;
		}
		else if(!tInAir)
		{
			mMidairTime = clock()/1000.0f;
			if(mMidairTime-mMidairTimeBase>0.05f&&mInMidair)
			{
				mInMidair = false;
				mInAirLast2 = true;
			}
		}
		else
		{
			mInMidair = true;
			mMidairTimeBase = 0;
			mMidairTime = 0;
		}

		mInAirLast = tInAir;

		return tInAir;
	}

	void QCharacterController::resetImpactLevel()
	{
		mImpactLevel = 0.f;
	}

	Real QCharacterController::getSpeed()
	{
		return mPlayerSpeedLast;
	}

	Vector3 QCharacterController::getSegmentPosition(int segment)
	{
		return NxVector3(mController->cctp[segment]->getPosition());
	}


}
