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


#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "stdafx.h"

#include "GUIComponent.h"

// forward declarations??
namespace MyGUI
{
	struct KeyCode;
	struct MouseButton;
	class Gui;
	class OgrePlatform;
	class StaticText;
}

namespace Ogre
{
	class RenderWindow;
	class SceneManager;
}

namespace QE
{

	class QENGINE_API StaticText
	{
	public:

		StaticText(){};
		StaticText(MyGUI::StaticText* txt)
		{
			mText = txt;
		}

		void setSize(Real w,Real h);
		void setPos(Real x,Real y);
		void setCaption(String cap);
		void setColour(Colour col);
		void setFontName(String font);
		void setFontHeight(Real h);
		void setVisible(bool visible);
		void center();

	//protected:

		MyGUI::StaticText* mText;

	};

	/** Manages the GUI system */
	class QENGINE_API GUIManager
	{
	public:

		GUIManager();

		~GUIManager();

		void initialize(Ogre::RenderWindow* window,Ogre::SceneManager* sceneManager);

		void deinitialize();

		void update();

		void injectMouseMove(int _absx, int _absy, int _absz);
		void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
		void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
		void injectKeyPress(MyGUI::KeyCode _key, unsigned int _text);
		void injectKeyRelease(MyGUI::KeyCode _key);

		void setCursorVisible(bool value);
		void setCursorName(const std::string& name);

		//std::pair<Real,Real> getMouseCoords();

		bool isInputEnabled();
		void setInputEnabled(bool enabled);

		void registerComponent(GUIComponent* component,bool temporary);

		void _endState();

		void setInputAllowed(bool allowed);

		bool isInputAllowed();

		StaticText createStaticText(String name,Real x,Real y,Real w,Real h,String caption);

		StaticText getStaticText(String name);

	private:

		bool mInitialized;
		bool mInput;

		MyGUI::Gui* mGUI;
		MyGUI::OgrePlatform* mPlatform;

		std::vector<GUIComponent*> mComponents;

		std::map<String,StaticText> mTexts;

	};
}

#endif
