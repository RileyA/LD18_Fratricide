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
#include "EditorCameraManager.h"
#include "Ogre.h"
#include "InputManager.h"

namespace QE
{	
	unsigned int EditorCameraManager::count = 0;

	EditorCameraManager::EditorCameraManager()
	{
		mInputManager = 0;
		mInitialized = false;
	}
	EditorCameraManager::~EditorCameraManager()
	{
		deinitialize();
	}
	void EditorCameraManager::initialize(Ogre::SceneManager* smgr,Ogre::Camera* cam)
	{
		++count;// so that if more than one is created the node names don't overlap
		mSceneManager = smgr;
		mCameraNode = mSceneManager->createSceneNode("EditorCameraManager_CameraNode"+Ogre::StringConverter::toString(count));
		mCameraRotNode = mSceneManager->createSceneNode("EditorCameraManager_CameraRotNode"+Ogre::StringConverter::toString(count));
		mCameraPosNode = mSceneManager->createSceneNode("EditorCameraManager_CameraPosNode"+Ogre::StringConverter::toString(count));

		mCameraPosNode->addChild(mCameraRotNode);
		mCameraRotNode->addChild(mCameraNode);

		mSceneManager->getRootSceneNode()->addChild(mCameraPosNode);

		mCamera = cam;

		mCameraNode->attachObject(mCamera);

		focalPoint = Vector3(0,0,0);

		mCameraPosNode->setPosition(0,0,50);
		
		Ogre::Vector3 fp = Ogre::Vector3(focalPoint.x,focalPoint.y,focalPoint.z);
		Ogre::Plane pln(Ogre::Vector3(0,1,0),0);
		Ogre::Vector3 fpP = pln.projectVector(fp);
		mCameraRotNode->lookAt(fpP,Ogre::Node::TS_WORLD);

		mCameraPosNode->_update(true,false);
		mCameraRotNode->_update(true,false);
		mCameraNode->lookAt(fp,Ogre::Node::TS_WORLD);

		mInitialized = true;
	}
	void EditorCameraManager::deinitialize()
	{
		if(mInitialized)
		{
			mCameraNode->detachAllObjects();
			mCameraNode->getCreator()->destroySceneNode(mCameraRotNode);
			mCameraNode->getCreator()->destroySceneNode(mCameraPosNode);
			mCameraNode->getCreator()->destroySceneNode(mCameraNode);
			mInitialized = false;
		}
	}
	void EditorCameraManager::update()
	{
		if(mInputManager)
		{
			Ogre::Vector3 fp = Ogre::Vector3(focalPoint.x,focalPoint.y,focalPoint.z);

			if(mInputManager->isMouseButtonDown(2))
			{
				if(mInputManager->isKeyDown("KC_LSHIFT"))
				{
					Real dist = Ogre::Vector3(mCameraPosNode->_getDerivedPosition()-fp).length();
					float factor = 0.002f * dist;
					Ogre::Vector3 fm = mCamera->getDerivedRight()*static_cast<Real>(mInputManager->getMouseX())*-factor+mCamera->getDerivedUp()*static_cast<Real>(mInputManager->getMouseY())*factor + fp;
					focalPoint = Vector3(fm.x,fm.y,fm.z);
					fp = fm;
					mCameraPosNode->translate(mCamera->getDerivedRight()*static_cast<Real>(mInputManager->getMouseX())*-factor+mCamera->getDerivedUp()*static_cast<Real>(mInputManager->getMouseY())*factor);
				}
				else
				{
					Real dist = Ogre::Vector3(mCameraPosNode->_getDerivedPosition()-fp).length();
					float factor = 0.004f * dist;
					Ogre::Vector3 right = mCamera->getDerivedRight();
					Ogre::Vector3 motion = right*static_cast<Real>(mInputManager->getMouseX())*-factor+mCamera->getDerivedUp()*static_cast<Real>(mInputManager->getMouseY())*factor + (mCameraPosNode->_getDerivedPosition()-fp);//+mCamera->getDerivedUp()*mInputManager->getMouseY()*factor
					motion.normalise();
					motion*=dist;
					mCameraPosNode->setPosition(motion+fp);
				}
			}

			Ogre::Plane pln(Ogre::Vector3(0,1,0),0);
			Ogre::Vector3 fpP = pln.projectVector(fp);
			mCameraRotNode->lookAt(fpP,Ogre::Node::TS_WORLD);

			mCameraPosNode->_update(true,false);
			mCameraRotNode->_update(true,false);

			mCameraNode->lookAt(fp,Ogre::Node::TS_WORLD);

			if(mInputManager->getMouseZ()!=0)
			{
				Ogre::Vector3 focal = mCameraPosNode->getPosition() - fp;
				float dist = focal.length();
				float change = static_cast<float>(mInputManager->getMouseZ()/20);

				float rate = 1;
				if(dist<5)
				{
					rate = dist/50;
				}
				if(dist<=2&&change>1)
				{
					rate = 0;
				}

				if(rate*change>dist)
				{

				}
				else
				{
					focal -= (focal/dist)*rate*change;
				}

				mCameraPosNode->setPosition(fp+focal);
			}
		}
	}
}