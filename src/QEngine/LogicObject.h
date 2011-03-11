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


#ifndef LOGICOBJECT_H
#define LOGICOBJECT_H

#include "stdafx.h"
#include "QEngine.h"

#include "SlotHandler.h"
#include "SignalHandler.h"

namespace QE
{
	class QEngine;
	class ScriptManager;

	class QENGINE_API LogicObject : public SlotHandler,public SignalHandler
	{
	public:

		LogicObject(QEngine* engine,String bucket,String name);

		virtual ~LogicObject(){};

		virtual void update(double delta) = 0;

		virtual void deinitialize() = 0;

		bool isReadyForDeletion(){return mReadyForDelete;}

		void _kill(){mReadyForDelete=true;}

		String getType(){return mType;}
		String getName(){return mName;}

	protected:

		void selfDestruct(){mReadyForDelete=true;}

		bool mInitialized;
		bool mReadyForDelete;

		QEngine* mEngine;

		String mName;
		String mType;

	};
}

#endif