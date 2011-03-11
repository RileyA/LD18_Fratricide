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


#ifndef SCRIPTCONSOLE_H
#define SCRIPTCONSOLE_H

#include "stdafx.h"
#include "QEngine.h"

#include "Console.h"

#include "LogicObject.h"
#include "Engine.h"

namespace QE
{
	class ScriptManager;

	class LogicConsole : public LogicObject
	{
	public:

		LogicConsole(QEngine* engine,String bucket,String name);

		virtual ~LogicConsole();

		virtual void update(double delta);

		virtual void deinitialize();

		void enterScript();

		void setVisible(bool visible);

		bool isVisible();

		static void registerClass(ScriptManager* scriptMgr);

	protected:

		Console* mConsole;
		bool mVisible;
		bool mTildaDown;

	};
}

#endif