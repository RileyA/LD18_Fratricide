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


#ifndef DEFAULTCAMERAMANAGER_H
#define DEFAULTCAMERAMANAGER_H

#include "stdafx.h"
#include "CameraManager.h"
#include "QEngine.h"
#include "RenderedObject.h"

namespace QE
{
	class QENGINE_API DefaultCameraManager 
		: public CameraManager
	{	
	public:

		DefaultCameraManager();
		virtual ~DefaultCameraManager();

		void initialize(Ogre::SceneManager* smgr,Ogre::Camera* cam);
		void deinitialize();

		void update();

		Ogre::SceneNode* getCameraPositionNode();
		Ogre::SceneNode* getCameraRollNode();

		void lockMotion();
		void unlockMotion();

		void unlockRotation();
		void lockRotation();

		void setPitchLimits(Real up,Real down);
		void setPitchLimitUp(Real up);
		void setPitchLimitDown(Real down);

		Real getPitchLimitUp();
		Real getPitchLimitDown();

		void setPosition(Vector3 pos);
		void setPosition(Vector3 pos,Quaternion dir,Vector3 offset);
		void setOrientation(Quaternion ori);

		Vector3 getPosition();
		Quaternion getOrientation();

		Vector3 getDirection();
		Vector3 getRight();
		Vector3 getLeft();

		bool canSee(RenderedObject* obj);

		float getSensitivity();
		void setSensitivity(float sens);

	protected:

		// is the motion of the camera locked (i.e. it is attached to a character controller and shouldn't move under its own power)
		bool mMotionLocked;
		bool mRotationLocked;
		Ogre::SceneNode* mCameraRollNode;
		Ogre::SceneNode* mCameraPitchNode;
		Ogre::SceneNode* mCameraYawNode;
		Ogre::SceneNode* mCameraPosNode;

		Real mPitchLimitUp;
		Real mPitchLimitDown;
		Real mCurrentPitch;

		static unsigned int count;

		double lastTime;

		float mSensitivity;

		float mLastX;
		float mLastY;

		std::vector<Real> mMouseSmoothX;
		std::vector<Real> mMouseSmoothY;
		std::vector<Real> mMouseSmoothTime;

	};
}

#endif