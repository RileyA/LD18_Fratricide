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


#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "stdafx.h"
#include "QEngine.h"

namespace OIS
{
	class KeyEvent;
}

namespace QE
{
	// forward declaration
	class OISManager;
	class GUIManager;

	class QENGINE_API InputManager
	{
	public:

		InputManager();

		~InputManager();

		void update();

		void initialize(size_t _handle);

		void deinitialize();

		/** Sends a pointer to the GUI manager to the OIS system, so that input can be directly injected */
		void activateGUI(GUIManager* guiMgr);

		bool isKeyDown(const unsigned int& code);
		bool isKeyDown(const std::string& code);

		bool isMouseButtonDown(unsigned int code);
		bool isMouseButtonDown(std::string code);

		void assignKeyConversions();

		void injectMouseMove(int absx, int absy, int absz);
		void injectMousePress(int absx, int absy, unsigned int id);
		void injectMouseRelease(int absx, int absy, unsigned int id);
		void injectKeyPress(unsigned int key, unsigned int text);
		void injectKeyRelease(unsigned int key);

		void setWindowSize(unsigned int w,unsigned int h);

		int getMouseX();
		int getMouseXAbs();
		int getMouseY();
		int getMouseYAbs();
		int getMouseZ();

		OISManager* getOISManager(){return mOISManager;};

	protected:


	private:

		std::map<unsigned int,std::string> mKeyConversions;
		std::vector<std::string> mMouseConversions;
		std::map<unsigned int,bool> mKeyStates;
		std::vector<bool> mMouseStates;
		std::map<std::string, unsigned int> mKeyNames;
		std::map<std::string, unsigned int> mMouseNames;

		OISManager* mOISManager;

		bool mInitialized;

		// RELATIVE!
		int mMouseX;
		// RELATIVE!
		int mMouseY;
		// ABSOLUTE!
		int mMouseZ;

	};
}

#endif