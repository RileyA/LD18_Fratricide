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
#include "GUIManager.h"
#include "InputManager.h"
#include "OISManager.h"

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"

namespace QE
{
	void StaticText::setSize(Real w,Real h)
	{
		mText->setRealSize(w,h);
	}

	void StaticText::setPos(Real x,Real y)
	{
		mText->setRealPosition(x,y);
	}

	void StaticText::setCaption(String cap)
	{
		mText->setCaption(cap);
	}

	void StaticText::setColour(Colour col)
	{
		mText->setTextColour(MyGUI::Colour(col.r,col.g,col.b,col.a));
	}

	void StaticText::setFontName(String font)
	{
		mText->setFontName(font);
	}

	void StaticText::setFontHeight(Real h)
	{
		mText->setFontHeight(h);
	}

	void StaticText::setVisible(bool visible)
	{
		mText->setVisible(visible);
	}

	void StaticText::center()
	{
		mText->setTextAlign(MyGUI::Align::Center);
	}

	GUIManager::GUIManager()
		:mGUI(0),
		 mPlatform(0),
		 mInitialized(false)
	{
		
	}

	GUIManager::~GUIManager()
	{

	}

	void GUIManager::initialize(Ogre::RenderWindow* window,Ogre::SceneManager* sceneManager)
	{
		mPlatform = new MyGUI::OgrePlatform();
		mPlatform->initialise(window, sceneManager);
		mGUI = new MyGUI::Gui();
		mGUI->initialise("core.xml");
		mInitialized = true;


		//mGUI->

		//MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("console.layout");
		//dynamic_cast<MyGUI::Edit*>(root.at(0)->findWidget("list_History"))->addText("\n This is a test...");
	}

	void GUIManager::deinitialize()
	{
		if(mInitialized)
		{
			std::vector<GUIComponent*>::iterator iter = mComponents.begin();
			while(iter!=mComponents.end())
			{
				delete (*iter);
				++iter;
			}
			mComponents.clear();

			if (mGUI)
			{
				mGUI->shutdown();
				delete mGUI;
				mGUI = 0;
			}

			if (mPlatform)
			{
				mPlatform->shutdown();
				delete mPlatform;
				mPlatform = 0;
			}

			mInitialized = false;
		}
	}

	void GUIManager::update()
	{

	}

	void GUIManager::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if(!mGUI)
		{
			return;
		}
		if(mInput)
		{
			mGUI->injectMouseMove(_absx, _absy, _absz);
		}
	}

	void GUIManager::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if(!mGUI)
		{
			return;
		}	
		if(mInput)
		{
			mGUI->injectMousePress(_absx, _absy, _id);
		}
	}

	void GUIManager::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if(!mGUI)
		{
			return;
		}
		if(mInput)
		{
			mGUI->injectMouseRelease(_absx, _absy, _id);
		}
	}

	void GUIManager::injectKeyPress(MyGUI::KeyCode _key, unsigned int _text)
	{
		if(!mGUI)
		{
			return;
		}
		if(mInput)
		{
			mGUI->injectKeyPress(_key, _text);
		}
	}

	void GUIManager::injectKeyRelease(MyGUI::KeyCode _key)
	{
		if(!mGUI)
		{
			return;
		}
		if(mInput)
		{
			mGUI->injectKeyRelease(_key);
		}
	}

	void GUIManager::setCursorVisible(bool value)
	{
		MyGUI::PointerManager::getInstance().setVisible(value);
	}

	void GUIManager::setCursorName(const std::string& name)
	{
		MyGUI::PointerManager::getInstance().setPointer(name);
	}

	//std::pair<Real,Real> GUIManager::getMouseCoords()
	//{
		//return std::pair<Real,Real>(5,5);
		
		//return std::pair<Real,Real>(mGUI->get
	//}

	void GUIManager::_endState()
	{
		std::vector<GUIComponent*>::iterator iter = mComponents.begin();
		while(iter!=mComponents.end())
		{
			if((*iter)->isTemporary())
			{
				delete (*iter);
				mComponents.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		std::map<String,StaticText>::iterator iter2 = mTexts.begin();
		while(iter2!=mTexts.end())
		{
			mGUI->destroyWidget((*iter2).second.mText);
			++iter2;
			//mTexts.erase(iter2);
		}
		mTexts.clear();
	}

	void GUIManager::registerComponent(GUIComponent* component,bool temporary)
	{
		component->setTemporary(temporary);
		mComponents.push_back(component);
	}

	void GUIManager::setInputAllowed(bool allowed)
	{
		mInput = allowed;
	}

	bool GUIManager::isInputAllowed()
	{
		return mInput;
	}

	
	StaticText GUIManager::createStaticText(String name,Real x,Real y,Real w,Real h,String caption)
	{
		mTexts[name] = StaticText(mGUI->createWidget<MyGUI::StaticText>("StaticText",MyGUI::IntCoord(0,0,400,400),MyGUI::Align::Relative,"Back",name));
		mTexts[name].setPos(x,y);
		mTexts[name].setSize(w,h);
		mTexts[name].setCaption(caption);
		return mTexts[name];
		//mTexts[name] = 
	}

	StaticText GUIManager::getStaticText(String name)
	{
		return mTexts[name];
	}

}