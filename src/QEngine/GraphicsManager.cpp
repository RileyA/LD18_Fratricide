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
#include "GraphicsManager.h"
#include "Logger.h"
#include "TimeManager.h"
#include "GUIManager.h"
#include "InputManager.h"

#include "Ogre.h"

namespace QE
{
	GraphicsManager::GraphicsManager()
		: mInitialized(false),
		  mCameraManager(0)
	{

	}

	GraphicsManager::~GraphicsManager()
	{
		if(mInitialized)
		{
			deinitialize();
		}
		if(mGUIManager)
		{
			delete mGUIManager;
			mGUIManager = 0;
		}
		Logger::getSingletonPtr()->logMessage("GraphicsManager Destroyed.");
	}

	void GraphicsManager::initialize()
	{
		mInitialized = true;
		#ifdef _DEBUG
			mRoot = new Ogre::Root("plugins_d.cfg");
		#else
			mRoot = new Ogre::Root("plugins.cfg");
		#endif

		// load up the old config (if possible) then run the dialogue
        mRoot->restoreConfig();
		if(mRoot->showConfigDialog())
		{
			mWindow = mRoot->initialise(true);
		}
		else
		{
			throw QE::Exception(String("EXCEPTION: Config Dialogue Cancelled."));
		}

		// set up the scene manager
		mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC,"mSceneManager");

		// Set up the main camera
		mMainCam = mSceneManager->createCamera("mMainCam");
		mMainCam->setPosition(Ogre::Vector3(0,0,0));
		mMainCam->setDirection(Ogre::Vector3(0,0,-1));
		mMainCam->setFarClipDistance(1000);
		mMainCam->setNearClipDistance(0.01f);
		mMainCam->setFOVy(Ogre::Radian(Ogre::Degree(55)));

		// viewport setup
		mViewport = mWindow->addViewport(mMainCam);
		mMainCam->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

		// resource groups
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("","FileSystem","General");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../media","FileSystem","General");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../media/models","FileSystem","General");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../media/models/Pavel","FileSystem","General");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../media/materials","FileSystem","General");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../media/textures","FileSystem","General");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../media/textures/Misc","FileSystem","General");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../media/MyGUI_Media","FileSystem","General");
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		// camera manager
		mDefaultCameraManager = new DefaultCameraManager();
		setCameraManager(mDefaultCameraManager);

		// gui system
		mGUIManager = new GUIManager;
		mGUIManager->initialize(mWindow,mSceneManager);

		setTextureFiltering("Trilinear");

		//		//iterate through Compositor Managers resources and add name keys to menu
		//Ogre::CompositorManager::ResourceMapIterator resourceIterator =
		//	Ogre::CompositorManager::getSingleton().getResourceIterator();

		//// add all compositor resources to the view container
		//while (resourceIterator.hasMoreElements())
		//{
		//	Ogre::ResourcePtr resource = resourceIterator.getNext();
		//	const Ogre::String& compositorName = resource->getName();
		//	// Don't add base Ogre/Scene compositor to view
		//	if (compositorName == "Ogre/Scene")
		//		continue;
		//	// Don't add the deferred shading compositors, thats a different demo.
		//	if (Ogre::StringUtil::startsWith(compositorName, "DeferredShading", false))
		//		continue;

		//	//mCompositorNames.push_back(compositorName);
		//	int addPosition = -1;
		//	if (compositorName == "HDR")
		//	{
		//		// HDR must be first in the chain
		//		addPosition = 0;
		//	}
		//	try 
		//	{
		//		Ogre::CompositorInstance *instance = Ogre::CompositorManager::getSingleton().addCompositor(mViewport, compositorName, addPosition);
		//		Ogre::CompositorManager::getSingleton().setCompositorEnabled(mViewport, compositorName, false);
		//	} catch (...) {
		//	}
		//}

		//Ogre::CompositorManager::getSingleton().setCompositorEnabled(mViewport, "Glass", true);

		// test scene node


		//Ogre::SceneNode* wNode2 = mSceneManager->createSceneNode("wh01n2");
		//Ogre::Entity* ent2 = mSceneManager->createEntity("wh01e2","MP5SD3.mesh");
		//wNode->attachObject(ent2);
		//mSceneManager->getRootSceneNode()->addChild(wNode2);
	}

	void GraphicsManager::deinitialize()
	{
		if(mInitialized)
		{
			for(unsigned int i = 0; i<mNodes.size();++i)
			{
				delete mNodes[i];
				mNodes[i] = 0;
			}
			for(unsigned int i = 0; i<mStateNodes.size();++i)
			{
				delete mStateNodes[i];
				mStateNodes[i] = 0;
			}
			mNodes.clear();
			mStateNodes.clear();

			if(mDefaultCameraManager)
			{
				delete mDefaultCameraManager;
				mDefaultCameraManager = 0;
			}
			mGUIManager->deinitialize();
			if(mRoot)
			{
				delete mRoot;
				mRoot = 0;
			}
			mViewport = 0;
			mWindow = 0;
			mMainCam = 0;
			mSceneManager = 0;
			mInitialized = false;
			Logger::getSingletonPtr()->logMessage("GraphicsManager Deinitialized.");
		}
	}

	void GraphicsManager::update()
	{
		mGUIManager->update();
		if(mCameraManager)
		{
			mCameraManager->update();
		}
		Ogre::WindowEventUtilities::messagePump();
		mRoot->renderOneFrame();
	}

	Ogre::RenderWindow* GraphicsManager::getWindow()
	{
		return mWindow;
	}

	Ogre::Camera* GraphicsManager::getCamera()
	{
		return mMainCam;
	}

	GUIManager* GraphicsManager::getGUIManager()
	{
		return mGUIManager;
	}

	CameraManager* GraphicsManager::getCameraManager()
	{
		if(mCameraManager)
		{
			return mCameraManager;
		}
		return 0;
	}

	void GraphicsManager::setCameraManager(CameraManager* cmgr)
	{
		if(mCameraManager)
		{
			InputManager* imgr = mCameraManager->getInputManager();
			cmgr->setInputManager(imgr);
			mCameraManager->deinitialize();
		}
		mCameraManager = cmgr;
		mCameraManager->initialize(mSceneManager,mMainCam);
	}

	SceneNode* GraphicsManager::createSceneNode(std::string name,bool state)
	{
		if(state)
		{
			mStateNodes.push_back(new SceneNode(name));
			return mStateNodes[mStateNodes.size()-1];
		}
		else
		{
			mNodes.push_back(new SceneNode(name));
			return mNodes[mNodes.size()-1];
		}
	}

	SceneNode* GraphicsManager::createSceneNode(bool state)
	{
		if(state)
		{
			unsigned int a = 0;
			while(true)
			{
				if(!mSceneManager->hasSceneNode("QEngine_UnnamedSceneNodeNr"+Ogre::StringConverter::toString(a)))
				{
					break;
				}
				++a;
			}
			mStateNodes.push_back(new SceneNode("QEngine_UnnamedSceneNodeNr"+Ogre::StringConverter::toString(a)));
			return mStateNodes[mStateNodes.size()-1];
		}
		else
		{
			unsigned int a = 0;
			while(true)
			{
				if(!mSceneManager->hasSceneNode("QEngine_UnnamedSceneNodeNr"+Ogre::StringConverter::toString(a)))
				{
					break;
				}
				++a;
			}
			mNodes.push_back(new SceneNode("QEngine_UnnamedSceneNodeNr"+Ogre::StringConverter::toString(a)));
			return mNodes[mNodes.size()-1];
		}
	}

	RenderedObject* GraphicsManager::createRenderedObject(std::string name,std::string mesh,bool state)
	{
		if(state)
		{
			mStateNodes.push_back(new RenderedObject(name,mesh));
			return dynamic_cast<RenderedObject*>(mStateNodes[mStateNodes.size()-1]);
		}
		else
		{
			mNodes.push_back(new RenderedObject(name,mesh));
			return dynamic_cast<RenderedObject*>(mNodes[mNodes.size()-1]);
		}
	}

	RenderedObject* GraphicsManager::createRenderedObject(std::string mesh,bool state)
	{
		if(state)
		{
			unsigned int a = 0;
			while(true)
			{
				if(!mSceneManager->hasSceneNode("QEngine_UnnamedRenderedObjectNr"+Ogre::StringConverter::toString(a)))
				{
					break;
				}
				++a;
			}
			mStateNodes.push_back(new RenderedObject("QEngine_UnnamedRenderedObjectNr"+Ogre::StringConverter::toString(a),mesh));
			return dynamic_cast<RenderedObject*>(mStateNodes[mStateNodes.size()-1]);
		}
		else
		{
			unsigned int a = 0;
			while(true)
			{
				if(!mSceneManager->hasSceneNode("QEngine_UnnamedRenderedObjectNr"+Ogre::StringConverter::toString(a)))
				{
					break;
				}
				++a;
			}
			mNodes.push_back(new RenderedObject("QEngine_UnnamedRenderedObjectNr"+Ogre::StringConverter::toString(a),mesh));
			return dynamic_cast<RenderedObject*>(mNodes[mNodes.size()-1]);
		}
	}

	ParticleSystem* GraphicsManager::createParticleSystem(std::string name,std::string system,bool state)
	{
		if(state)
		{
			mStateNodes.push_back(new ParticleSystem(name,system));
			return dynamic_cast<ParticleSystem*>(mStateNodes[mStateNodes.size()-1]);
		}
		else
		{
			mNodes.push_back(new ParticleSystem(name,system));
			return dynamic_cast<ParticleSystem*>(mNodes[mNodes.size()-1]);
		}
	}

	ParticleSystem* GraphicsManager::createParticleSystem(std::string system,bool state)
	{
		if(state)
		{
			unsigned int a = 0;
			while(true)
			{
				if(!mSceneManager->hasSceneNode("QEngine_UnnamedParticleObjectNr"+Ogre::StringConverter::toString(a)))
				{
					break;
				}
				++a;
			}
			mStateNodes.push_back(new ParticleSystem("QEngine_UnnamedParticleObjectNr"+Ogre::StringConverter::toString(a),system));
			return dynamic_cast<ParticleSystem*>(mStateNodes[mStateNodes.size()-1]);
		}
		else
		{
			unsigned int a = 0;
			while(true)
			{
				if(!mSceneManager->hasSceneNode("QEngine_UnnamedParticleObjectNr"+Ogre::StringConverter::toString(a)))
				{
					break;
				}
				++a;
			}
			mNodes.push_back(new ParticleSystem("QEngine_UnnamedParticleObjectNr"+Ogre::StringConverter::toString(a),system));
			return dynamic_cast<ParticleSystem*>(mNodes[mNodes.size()-1]);
		}
	}

	void GraphicsManager::_endState()
	{
		for(unsigned int i = 0; i<mStateNodes.size();++i)
		{
			delete mStateNodes[i];
			mStateNodes[i] = 0;
		}
		mStateNodes.clear();
		mGUIManager->_endState();
	}

	Colour GraphicsManager::getBackgroundColour() const
	{
		return mViewport->getBackgroundColour();
	}

	void GraphicsManager::setBackgroundColour(const Colour& colour)
	{
		mViewport->setBackgroundColour(colour);
	}

	void GraphicsManager::setSkybox(bool enabled,String name,Real distance)
	{
		mSceneManager->setSkyBox(enabled,name,distance);
	}

	bool GraphicsManager::isSkyBoxEnabled()
	{
		return mSceneManager->isSkyBoxEnabled();
	}

	DefaultCameraManager* GraphicsManager::getDefaultCameraManager()
	{
		return (mDefaultCameraManager);
	}

	void GraphicsManager::setTextureFiltering(String filter)
	{
		if(filter=="Bilinear"||filter=="B")
		{
			Ogre::MaterialManager::getSingletonPtr()->setDefaultTextureFiltering(Ogre::TFO_BILINEAR);
		}
		else if(filter=="Trilinear"||filter=="T")
		{
			Ogre::MaterialManager::getSingletonPtr()->setDefaultTextureFiltering(Ogre::TFO_TRILINEAR);
		}
		else if(filter=="Anisotropic"||filter=="A"||filter=="A2"||filter=="Anisotropic x2")
		{
			Ogre::MaterialManager::getSingletonPtr()->setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
			Ogre::MaterialManager::getSingletonPtr()->setDefaultAnisotropy(2);
		}
		else if(filter=="Anisotropic x4"||filter=="A4")
		{
			Ogre::MaterialManager::getSingletonPtr()->setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
			Ogre::MaterialManager::getSingletonPtr()->setDefaultAnisotropy(4);
		}
		else if(filter=="Anisotropic x8"||filter=="A8")
		{
			Ogre::MaterialManager::getSingletonPtr()->setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
			Ogre::MaterialManager::getSingletonPtr()->setDefaultAnisotropy(8);
		}
		else if(filter=="Anisotropic x16"||filter=="A16")
		{
			Ogre::MaterialManager::getSingletonPtr()->setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
			Ogre::MaterialManager::getSingletonPtr()->setDefaultAnisotropy(16);
		}
	}

	void GraphicsManager::takeScreenshot(String filename)
	{
		mWindow->writeContentsToFile(String("../Screenshots/")+filename);
	}

	void GraphicsManager::deleteObject(String name)
	{
        for(unsigned int i=0;i<mStateNodes.size();++i)
        {
            if(mStateNodes[i]->getName()==name)
            {
                std::vector<SceneNode*>::iterator iter = mStateNodes.begin();
                iter+=i;
                delete (*iter);
                mStateNodes.erase(iter);
                return;
            }
        }

        for(unsigned int i=0;i<mNodes.size();++i)
        {
            if(mNodes[i]->getName()==name)
            {
                std::vector<SceneNode*>::iterator iter = mNodes.begin();
                iter+=i;
                delete (*iter);
                mNodes.erase(iter);
                return;
            }
        }
	}


}
