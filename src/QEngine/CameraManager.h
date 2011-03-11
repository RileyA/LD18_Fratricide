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


#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "stdafx.h"
#include "QEngine.h"

namespace Ogre
{
	class SceneManager;
	class SceneNode;
	class Camera;
	class Vector3;
}

namespace QE
{
	class InputManager;

	class QENGINE_API CameraManager
	{
	public:

		virtual ~CameraManager(){}

		virtual void initialize(Ogre::SceneManager* smgr,Ogre::Camera* cam) = 0;
		virtual void deinitialize() = 0;

		virtual void update() = 0;

		void setInputManager(InputManager* imgr)
		{
			mInputManager = imgr;
		}

		InputManager* getInputManager()
		{
			return mInputManager;
		}

		Ogre::Camera* getCamera()
		{
			return mCamera;
		}

		Ogre::SceneNode* getCameraNode()
		{
			return mCameraNode;
		}


	protected:

		InputManager* mInputManager;
		bool mInitialized;

		Ogre::SceneNode* mCameraNode;
		Ogre::SceneManager* mSceneManager;
		Ogre::Camera* mCamera;
	};
}

#endif