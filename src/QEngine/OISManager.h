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


#ifndef OISMANAGER_H
#define OISMANAGER_H
#include "stdafx.h"

#include "MyGUI.h"
#include <OIS.h>
#include "GUIManager.h"
#include "InputManager.h"

/*namespace OIS
{
	class InputManager;
	class Mouse;
	class Keyboard;
	class MouseEvent;
	class MouseButtonID;
	class KeyEvent;
	class MouseListener;
	class KeyListener;
}

namespace MyGUI
{
	class Char;
	class MouseButton;
	class KeyCode;
}*/

namespace QE
{
	// forward declare this here
	class InputManager;

	class OISManager
		: public OIS::MouseListener,
		  public OIS::KeyListener
	{
	public:

		OISManager();
		~OISManager();

		void initialize(size_t _handle,InputManager* inputMgr);

		void deinitialize();

		void update();

		/** Sends a pointer to the GUI system so that input can be injected directly. */
		void activateGUI(GUIManager* guiMgr);

		void createInput(size_t _handle);
		void destroyInput();
		void captureInput();
		void setInputViewSize(int _width, int _height);

		//void injectMouseMove(int _absx, int _absy, int _absz) { }
		//void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id) { }
		//void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id) { }
		//void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text) { }
		//void injectKeyRelease(MyGUI::KeyCode _key) { }

		void setMousePosition(int _x, int _y);
		void updateCursorPosition();

		OIS::Mouse* getMouse();

	protected:

		bool mouseMoved(const OIS::MouseEvent& _arg);
		bool mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
		bool mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
		bool keyPressed(const OIS::KeyEvent& _arg);
		bool keyReleased(const OIS::KeyEvent& _arg);

		void checkPosition();

	private:

		bool mInitialized;

		OIS::InputManager* mInputManager;
		OIS::Keyboard* mKeyboard;
		OIS::Mouse* mMouse;

		int mCursorX;
		int mCursorY;

		GUIManager* mGUIManager;
		InputManager* mInputMgr;

	};
}
#endif