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
#include "DefaultCameraManager.h"
#include "Ogre.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "OIS.h"
#include "OISManager.h"
#include "RenderedObject.h"

namespace QE
{
	unsigned int DefaultCameraManager::count = 0;

	DefaultCameraManager::DefaultCameraManager()
		:mMotionLocked(false),lastTime(0),mRotationLocked(false),mSensitivity(1.f)
	{
		mInputManager = 0;
	}
	DefaultCameraManager::~DefaultCameraManager()
	{
		deinitialize();
	}
	void DefaultCameraManager::initialize(Ogre::SceneManager* smgr,Ogre::Camera* cam)
	{
		++count;// so that if more than one is created the node names don't overlap
		mSceneManager = smgr;
		mCameraNode = mSceneManager->createSceneNode("DefaultCameraManager_CameraNode"+Ogre::StringConverter::toString(count));
		mCameraRollNode = mSceneManager->createSceneNode("DefaultCameraManager_CameraRollNode"+Ogre::StringConverter::toString(count));
		mCameraPitchNode = mSceneManager->createSceneNode("DefaultCameraManager_CameraPitchNode"+Ogre::StringConverter::toString(count));
		mCameraYawNode = mSceneManager->createSceneNode("DefaultCameraManager_CameraYawNode"+Ogre::StringConverter::toString(count));
		mCameraPosNode = mSceneManager->createSceneNode("DefaultCameraManager_CameraPosNode"+Ogre::StringConverter::toString(count));

		mCameraPosNode->addChild(mCameraRollNode);
		mCameraRollNode->addChild(mCameraYawNode);
		mCameraYawNode->addChild(mCameraPitchNode);
		mCameraPitchNode->addChild(mCameraNode);

		mCameraYawNode->yaw(Ogre::Radian(Ogre::Degree(180.f)));

		//cam->setFarClipDistance(300.f);

		mSceneManager->getRootSceneNode()->addChild(mCameraPosNode);

		setPitchLimits(80.0f,80.0f);
		mCurrentPitch = -79.0f;
		mCameraPitchNode->pitch(Ogre::Radian(Ogre::Degree(-79.f)));
		mCamera = cam;

		mCameraNode->attachObject(mCamera);
		mInitialized = true;
		mLastX = 0;
		mLastY = 0;
	}
	void DefaultCameraManager::deinitialize()
	{
		if(mInitialized)
		{
			mCameraNode->detachAllObjects();
			mInitialized = false;
		}
	}
	void DefaultCameraManager::update()
	{
		if(mInputManager)
		{
			// if we haven't locked the motion, then the camera can fly freely
			if(!mMotionLocked)
			{
				Ogre::Vector3 moveVector = Ogre::Vector3(0,0,0);
				if(mInputManager->isKeyDown("KC_UP"))
				{
					moveVector+=Ogre::Vector3(0,0,-1);
				}
				if(mInputManager->isKeyDown("KC_DOWN"))
				{
					moveVector+=Ogre::Vector3(0,0,1);
				}
				if(mInputManager->isKeyDown("KC_LEFT"))
				{
					moveVector+=Ogre::Vector3(-1,0,0);
				}
				if(mInputManager->isKeyDown("KC_RIGHT"))
				{
					moveVector+=Ogre::Vector3(1,0,0);
				}
				moveVector.normalise();
				mCameraPosNode->translate(mCamera->getDerivedOrientation()*moveVector*5.f*TimeManager::getSingletonPtr()->getDeltaTime());
			}

			if(!mRotationLocked)
			{
				// get the mouse state direct from the input manager
				const OIS::MouseState &ms = mInputManager->getOISManager()->getMouse()->getMouseState();

                float sensitivityVal = 0.016667*mSensitivity;//TimeManager::getSingletonPtr()->getDeltaTime();

                #ifdef LINUX
                    sensitivityVal = (0.016667f)*6.f;
                #endif

                // in case of an abrupt change use the last value??
                //if(!mMouseSmoothX.empty())
                //{
                //    if(Ogre::Math::Abs(ms.X.rel-mMouseSmoothX[mMouseSmoothX.size()-1])>100.f)
                //    {
                //        mMouseSmoothX.push_back(ms.X.rel-mMouseSmoothX[mMouseSmoothX.size()-1]);
                //    }
                //    else
                //    {
                //        mMouseSmoothX.push_back(ms.X.rel);
                //    }
                //}
                //else
                //{
                    mMouseSmoothX.push_back(ms.X.rel);
                //}

                //if(!mMouseSmoothY.empty()&&Ogre::Math::Abs(ms.Y.rel-mMouseSmoothY[mMouseSmoothY.size()-1])>100.f)
                //{
                //    mMouseSmoothY.push_back(ms.Y.rel-mMouseSmoothY[mMouseSmoothY.size()-1]);
                //}
                //else
                //{
                    mMouseSmoothY.push_back(ms.Y.rel);
                //}

				//mMouseSmoothY.push_back(ms.Y.rel);
				//std::cout<<"Mouse: "<<ms.X.abs<<" "<<ms.Y.abs<<"\n";//<<ms.X.rel<<" "<<ms.Y.rel<<" "
				mMouseSmoothTime.push_back(TimeManager::getSingletonPtr()->getTimeDecimal());

				Real xTotal = 0.0f;
				Real yTotal = 0.0f;

				for(int i = 0;i<mMouseSmoothTime.size();++i)
				{
					if((TimeManager::getSingletonPtr()->getTimeDecimal()-mMouseSmoothTime[i])>0.045f)
					{
						mMouseSmoothTime.erase(mMouseSmoothTime.begin()+i);
						mMouseSmoothX.erase(mMouseSmoothX.begin()+i);
						mMouseSmoothY.erase(mMouseSmoothY.begin()+i);
					}
					else
					{
						xTotal+=static_cast<Real>(mMouseSmoothX[i]);
						yTotal+=static_cast<Real>(mMouseSmoothY[i]);
					}
				}

				xTotal/=mMouseSmoothX.size();
				yTotal/=mMouseSmoothY.size();

				mCameraYawNode->yaw(Ogre::Radian(Ogre::Degree((xTotal)*(-3.25f)*sensitivityVal)));//-TimeManager::getSingletonPtr()->getDeltaTime()

				Real pitchChange = Ogre::Degree((yTotal)*(-3.25f)*sensitivityVal).valueDegrees();//*-TimeManager::getSingletonPtr()->getDeltaTime() a

				// limit the pitch
				if(mCurrentPitch+pitchChange>=mPitchLimitUp)
				{
					pitchChange = mPitchLimitUp-mCurrentPitch;
				}
				else if(mCurrentPitch+pitchChange<=-mPitchLimitDown)
				{
					pitchChange = -mPitchLimitDown-mCurrentPitch;
				}

				mCameraPitchNode->pitch(Ogre::Radian(Ogre::Degree(pitchChange)));
				mCurrentPitch+=pitchChange;

				//mLastX = ms.X.abs;
				//mLastY = ms.Y.abs;
			}
		}
	}

	void DefaultCameraManager::lockMotion()
	{
		mMotionLocked = true;
	}

	void DefaultCameraManager::unlockMotion()
	{
		mMotionLocked = false;
	}

	void DefaultCameraManager::unlockRotation()
	{
		mRotationLocked = false;
	}

	void DefaultCameraManager::lockRotation()
	{
		mRotationLocked = true;
	}

	Ogre::SceneNode* DefaultCameraManager::getCameraPositionNode()
	{
		return mCameraPosNode;
	}

	Ogre::SceneNode* DefaultCameraManager::getCameraRollNode()
	{
		return mCameraRollNode;
	}

	void DefaultCameraManager::setPitchLimits(Real up,Real down)
	{
		if(up>90.f)
		{
			up = 89.5f;
		}
		mPitchLimitUp = up;
		if(down>90.f)
		{
			down = 89.5f;
		}
		mPitchLimitDown = down;
	}

	void DefaultCameraManager::setPitchLimitUp(Real up)
	{
		if(up>90.f)
		{
			up = 89.5f;
		}
		mPitchLimitUp = up;
	}

	void DefaultCameraManager::setPitchLimitDown(Real down)
	{
		if(down>90.f)
		{
			down = 89.5f;
		}
		mPitchLimitDown = down;
	}

	Real DefaultCameraManager::getPitchLimitUp()
	{
		return mPitchLimitUp;
	}

	Real DefaultCameraManager::getPitchLimitDown()
	{
		return mPitchLimitDown;
	}

	void DefaultCameraManager::setPosition(Vector3 pos)
	{
		mCameraPosNode->setPosition(pos);
	}

	void DefaultCameraManager::setPosition(Vector3 pos,Quaternion dir,Vector3 offset)
	{
		mCameraPosNode->setPosition(pos+dir.toOgre()*offset);
	}

	void DefaultCameraManager::setOrientation(Quaternion ori)
	{
		mCameraPosNode->setOrientation(ori.toOgre());
	}

	Vector3 DefaultCameraManager::getPosition()
	{
		return mCamera->getDerivedPosition();
	}

	Quaternion DefaultCameraManager::getOrientation()
	{
		Ogre::Quaternion quat = mCamera->getDerivedOrientation();
		return Quaternion(quat);
	}

	Vector3 DefaultCameraManager::getDirection()
	{
		return mCamera->getDerivedDirection();
	}

	Vector3 DefaultCameraManager::getRight()
	{
		return mCamera->getDerivedRight();
	}

	Vector3 DefaultCameraManager::getLeft()
	{
		return -mCamera->getDerivedRight();
	}

	bool DefaultCameraManager::canSee(RenderedObject* obj)
	{
		Ogre::AxisAlignedBox aabb = obj->getEntity()->getBoundingBox();
		Ogre::Matrix4 mat4 = Ogre::Matrix4::IDENTITY;
		mat4.setTrans(obj->getPosition());
		aabb.transform(mat4);
		return mCamera->isVisible(aabb);
		//Ogre::AxisAlignedBox aabb = obj->
		//if(mCamera->isVisible(
	}

	float DefaultCameraManager::getSensitivity()
	{
		return mSensitivity;
	}

	void DefaultCameraManager::setSensitivity(float sens)
	{
		mSensitivity = sens;
	}

}
