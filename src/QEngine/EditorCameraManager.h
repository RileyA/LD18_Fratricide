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


#ifndef EDITORCAMERAMANAGER_H
#define EDITORCAMERAMANAGER_H

#include "stdafx.h"
#include "CameraManager.h"
#include "Vector3.h"

namespace QE
{
	class EditorCameraManager 
		: public CameraManager
	{	
	public:

		EditorCameraManager();
		virtual ~EditorCameraManager();

		void initialize(Ogre::SceneManager* smgr,Ogre::Camera* cam);
		void deinitialize();

		void update();

		void setFocalPoint();

	protected:

		Ogre::SceneNode* mCameraRotNode;
		Ogre::SceneNode* mCameraPosNode;

		static unsigned int count;

		// I REALLY need a custom vector format, since if I use Ogre::Vector3 everywhere, it'll require people to include Ogre...
		Vector3 focalPoint;
	};
}

#endif