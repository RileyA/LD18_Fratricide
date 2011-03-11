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


#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "QEngine.h"
#include "stdafx.h"
#include "GUIManager.h"
#include "DefaultCameraManager.h"
#include "EditorCameraManager.h"

#include "Camera.h"
#include "ParticleSystem.h"
#include "RenderedObject.h"

// forward declare these...
namespace Ogre
{
	class Root;
	class Viewport;
	class RenderWindow;
	class Camera;
	class SceneManager;
}

namespace QE
{
	/** The GraphicsManager is responsible for all of the graphics and visuals, it wraps
	the relevant Ogre functionality, and also manages the MyGUI system. */
	class QENGINE_API GraphicsManager
	{
	public:

		/** Constructor */
		GraphicsManager();
		/** Destructor */
		~GraphicsManager();

		/** Starts the GraphicsManager up */
		void initialize();

		/** Stops and shuts Ogre down */
		void deinitialize();

		/** Renders a frame */
		void update();

		/** Returns the render window */
		Ogre::RenderWindow* getWindow();

		/** Returns the Ogre camera */
		Ogre::Camera* getCamera();

		/** Returns the GUI Manager */
		GUIManager* getGUIManager();

		/** Returns a pointer to the camera manager */
		CameraManager* getCameraManager();

		/** Sets the camera manager */
		void setCameraManager(CameraManager* cmgr);

		/** Creates a SceneNode object
			\param name the name of the SceneNode (should be unique)
			\param state if the node will be destroyed at the end of the current state */
		SceneNode* createSceneNode(std::string name,bool state = true);

		/** Creates a SceneNode object
			\param state if the node will be destroyed at the end of the current state */
		SceneNode* createSceneNode(bool state = true);

		/** Creates a RenderedObject
			\param name the name of the object (should be unique)
			\param name the name of the mesh to load
			\param state if the node will be destroyed at the end of the current state */
		RenderedObject* createRenderedObject(String name,std::string mesh,bool state = true);

		/** Creates a RenderedObject
			\param name the name of the mesh to load
			\param state if the node will be destroyed at the end of the current state */
		RenderedObject* createRenderedObject(String mesh,bool state = true);

		/** Creates a Camera
			\param name the name of the camera */
		Camera* createCamera(String name);

		/** Creates a ParticleSystem
			\param name the name of the system 
			\param system the name of the particle template
			\param state whether or not it will persist past the end of the current GameState */
		ParticleSystem* createParticleSystem(String name,String system,bool state = true);

		/** Creates a ParticleSystem
			\param system the name of the particle template
			\param state whether or not it will persist past the end of the current GameState */
		ParticleSystem* createParticleSystem(String system,bool state = true);

		/** Gets the background color of the viewport
			\returns The background colour of the viewport*/
		Colour getBackgroundColour() const;

		/** Sets the background colour of the viewport
			\param colour The colour */
		void setBackgroundColour(const Colour& colour);

		/** Sets the skybox
			\param enabled Whether or not it is enabled at all
			\param name The name of the skybox texture/material
			\param distance The distance between the camera and the skybox */
		void setSkybox(bool enabled,String name,Real distance);

		/** Whether or not the skybox is enabled
			\returns Whether or not it's enabled */
		bool isSkyBoxEnabled();

		/** Gets a pointer to the original (default) camera manager. */
		DefaultCameraManager* getDefaultCameraManager();

		/** Sets the texture filtering mode
			\param filter name of the mode to use (either full names or shortcuts (i.e. "B","T","A2","A16" etc))*/
		void setTextureFiltering(String filter);

		/** Takes a screen capture and saves it to the specified filename
			\param filename The filename to use */
		void takeScreenshot(String filename);

		/** Internal function for when a state ends, it deletes all temporary graphics objects */
		void _endState();

		/** Deletes a node by name */
        void deleteObject(String name);

	private:

		Ogre::Root* mRoot;
		Ogre::Viewport* mViewport;
		Ogre::RenderWindow* mWindow;
		Ogre::Camera* mMainCam;
		Ogre::SceneManager* mSceneManager;

		// Internal boolean used to tell if it has beeninitialized yet
		bool mInitialized;

		// The GUI system
		GUIManager* mGUIManager;

		// The current camera manager
		CameraManager* mCameraManager;

		// The default camera manager
		DefaultCameraManager* mDefaultCameraManager;

		// Nodes that will last until the end of the current state
		std::vector<SceneNode*> mStateNodes;

		// Nodes that will last until the manager is shut down
		std::vector<SceneNode*> mNodes;

	};
}

#endif
