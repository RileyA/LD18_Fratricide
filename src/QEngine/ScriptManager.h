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


#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "stdafx.h"
#include "QEngine.h"

namespace QE
{
	class QEngine;
	class ChaiWrapper;

	/** This class is responsible for the bindings between the engine and the chaiscript scripting system. */
	class ScriptManager
	{
	public:

		ScriptManager(QEngine* engine);
		~ScriptManager();

		/** Start up the ScriptManager, set up the basic bindings etc */
		void initialize();

		/** Shuts it all down */
		void deinitialize();

		/** Runs a .chai script
		@param filename The name of the file to open
		@remarks
			If something goes wrong, this will (eventually) throw an Exception */
		void runScript(String filename);

		/** Runs a line of script
		@param script The scripting code
		@remarks
			If something goes wrong, this will throw an Exception */
		void runString(String script);

		ChaiWrapper* getWrapper();

		// Chaiscript's implementation of this seems to be broken...
		static std::string toString(int val);
		static std::string toString(float val);
		static std::string toString(bool val);
		static std::string toString(std::string val);
		static std::string toString(char val);

	private:

		/** Internal: Register engine types with chai */
		void _registerEngineTypes();

		bool mInitialized;

		QEngine* mEngine;
		ChaiWrapper* mWrapper;

	};
}

#endif
