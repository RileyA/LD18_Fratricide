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
#include "OISManager.h"
#include "InputManager.h"

namespace QE
{
	#if WIN32
	MyGUI::Char translateWin32Text(MyGUI::KeyCode kc)
	{
		static WCHAR deadKey = 0;

		BYTE keyState[256];
		HKL  layout = GetKeyboardLayout(0);
		if ( GetKeyboardState(keyState) == 0 )
			return 0;

		int code = *((int*)&kc);
		unsigned int vk = MapVirtualKeyEx((UINT)code, 3, layout);
		if ( vk == 0 )
			return 0;

		WCHAR buff[3] = { 0, 0, 0 };
		int ascii = ToUnicodeEx(vk, (UINT)code, keyState, buff, 3, 0, layout);
		if (ascii == 1 && deadKey != '\0' )
		{
			// A dead key is stored and we have just converted a character key
			// Combine the two into a single character
			WCHAR wcBuff[3] = { buff[0], deadKey, '\0' };
			WCHAR out[3];

			deadKey = '\0';
			if(FoldStringW(MAP_PRECOMPOSED, (LPWSTR)wcBuff, 3, (LPWSTR)out, 3))
				return out[0];
		}
		else if (ascii == 1)
		{
			// We have a single character
			deadKey = '\0';
			return buff[0];
		}
		else if(ascii == 2)
		{
			// Convert a non-combining diacritical mark into a combining diacritical mark
			// Combining versions range from 0x300 to 0x36F; only 5 (for French) have been mapped below
			// http://www.fileformat.info/info/unicode/block/combining_diacritical_marks/images.htm
			switch(buff[0])	{
			case 0x5E: // Circumflex accent: â
				deadKey = 0x302; break;
			case 0x60: // Grave accent: à
				deadKey = 0x300; break;
			case 0xA8: // Diaeresis: ü
				deadKey = 0x308; break;
			case 0xB4: // Acute accent: é
				deadKey = 0x301; break;
			case 0xB8: // Cedilla: ç
				deadKey = 0x327; break;
			default:
				deadKey = buff[0]; break;
			}
		}

		return 0;
	}
	#endif


	OISManager::OISManager()
		:mInputManager(0),
		 mInputMgr(0),
		 mKeyboard(0),
		 mMouse(0),
		 mCursorX(0),
		 mCursorY(0),
		 mGUIManager(0)
	{

	}

	OISManager::~OISManager()
	{
		deinitialize();
	}


	void OISManager::initialize(size_t _handle,InputManager* inputMgr)
	{
		createInput(_handle);
		mInputMgr = inputMgr;
		mInitialized = true;
	}

	void OISManager::deinitialize()
	{
		if(mInitialized)
		{
			destroyInput();
			mInitialized = false;
		}
	}

	void OISManager::update()
	{
		captureInput();
	}

	void OISManager::activateGUI(GUIManager* guiMgr)
	{
		mGUIManager = guiMgr;
	}

	void OISManager::createInput(size_t _handle)
	{
		std::ostringstream windowHndStr;
		windowHndStr << _handle;

		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		//#ifdef OIS_WIN32_PLATFORM
		///pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		///pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
		///pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
		///pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
		//#elif defined OIS_LINUX_PLATFORM
		//paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
		//paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
		//paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
		//paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
		//#endif

		mInputManager = OIS::InputManager::createInputSystem(pl);

		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
		mKeyboard->setEventCallback(this);

		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
		mMouse->setEventCallback(this);
	}

	void OISManager::destroyInput()
	{
		if (mInputManager)
		{
			if (mMouse)
			{
				mInputManager->destroyInputObject( mMouse );
				mMouse = nullptr;
			}
			if (mKeyboard)
			{
				mInputManager->destroyInputObject( mKeyboard );
				mKeyboard = nullptr;
			}
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = nullptr;
		}
	}

	bool OISManager::mouseMoved(const OIS::MouseEvent& _arg)
	{
		mCursorX += _arg.state.X.rel;
		mCursorY += _arg.state.Y.rel;

		checkPosition();

		// we send the absolute pos to the rest of the engine...
		if(mInputMgr)
		{
			mInputMgr->injectMouseMove(_arg.state.X.rel, _arg.state.Y.rel, _arg.state.Z.rel);
		}

		if(mGUIManager)
		{
			mGUIManager->injectMouseMove(mCursorX, mCursorY, _arg.state.Z.abs);
		}
		return true;
	}

	bool OISManager::mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id)
	{
		if(mInputMgr)
		{
			mInputMgr->injectMousePress(mCursorX, mCursorY, MyGUI::MouseButton::Enum(_id));
		}

		if(mGUIManager)
		{
			mGUIManager->injectMousePress(mCursorX, mCursorY, MyGUI::MouseButton::Enum(_id));
		}
		return true;
	}

	bool OISManager::mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id)
	{
		if(mInputMgr)
		{
			mInputMgr->injectMouseRelease(mCursorX, mCursorY, MyGUI::MouseButton::Enum(_id));
		}

		if(mGUIManager)
		{
			mGUIManager->injectMouseRelease(mCursorX, mCursorY, MyGUI::MouseButton::Enum(_id));
		}
		return true;
	}

	bool OISManager::keyPressed(const OIS::KeyEvent& _arg)
	{
		MyGUI::Char text = (MyGUI::Char)_arg.text;
		MyGUI::KeyCode key = MyGUI::KeyCode::Enum(_arg.key);
		int scan_code = key.toValue();

		if (scan_code > 70 && scan_code < 84)
		{
			static MyGUI::Char nums[13] = { 55, 56, 57, 45, 52, 53, 54, 43, 49, 50, 51, 48, 46 };
			text = nums[scan_code-71];
		}
		else if (key == MyGUI::KeyCode::Divide)
		{
			text = '/';
		}
		else
		{
		#if WIN32
			text = translateWin32Text(key);
		#endif
		}

		if(mInputMgr)
		{
			mInputMgr->injectKeyPress(_arg.key, text);
		}

		// get rid of tilda's...
		if(mGUIManager&&_arg.key!=OIS::KC_GRAVE)
		{
			mGUIManager->injectKeyPress(key, text);
		}

		return true;
	}

	bool OISManager::keyReleased(const OIS::KeyEvent& _arg)
	{
		if(mInputMgr)
		{
			mInputMgr->injectKeyRelease(_arg.key);
		}
		if(mGUIManager&&_arg.key!=OIS::KC_GRAVE)
		{
			mGUIManager->injectKeyRelease(MyGUI::KeyCode::Enum(_arg.key));
		}
		return true;
	}

	void OISManager::captureInput()
	{
		if (mMouse) mMouse->capture();
		mKeyboard->capture();
	}

	void OISManager::setInputViewSize(int _width, int _height)
	{
		if (mMouse)
		{
			const OIS::MouseState &ms = mMouse->getMouseState();
			ms.width = _width;
			ms.height = _height;

			checkPosition();
		}
	}

	void OISManager::setMousePosition(int _x, int _y)
	{
		//const OIS::MouseState &ms = mMouse->getMouseState();
		mCursorX = _x;
		mCursorY = _y;

		checkPosition();
	}

	void OISManager::checkPosition()
	{
		const OIS::MouseState &ms = mMouse->getMouseState();

		if (mCursorX < 0)
			mCursorX = 0;
		else if (mCursorX >= ms.width)
			mCursorX = ms.width - 1;

		if (mCursorY < 0)
			mCursorY = 0;
		else if (mCursorY >= ms.height)
			mCursorY = ms.height - 1;
	}

	void OISManager::updateCursorPosition()
	{
		const OIS::MouseState &ms = mMouse->getMouseState();

		if(mGUIManager)
		{
			mGUIManager->injectMouseMove(mCursorX, mCursorY, ms.Z.abs);
		}
	}

	OIS::Mouse* OISManager::getMouse()
	{
		return mMouse;
	}

}
