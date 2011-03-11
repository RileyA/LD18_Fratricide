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


#ifndef LOGICBUCKET_H
#define LOGICBUCKET_H

#include "stdafx.h"
#include "QEngine.h"
#include "LogicObject.h"

namespace QE
{
	class LogicBucket
	{
	public:

		LogicBucket(String name,bool temporary=true);
		virtual ~LogicBucket();

		/** This updates all of the contained objects
			@param delta The time since last frame*/
		virtual void update(double delta);

		/** This destroys all objects cointained in this bucket and shuts it down */
		virtual void deinitialize();

		/** Returns whether or not the bucket is temporary (will be deleted at the end of the curret state) */
		bool isTemporary(){return mTemporary;}

		/** Returns the name of the object */
		String getName(){return mName;}
		
		/** Adds an object to this bucket */
		void addObject(LogicObject* obj);

		/** Returns a pointer to the named object (or NULL if it doesn't exist) */
		LogicObject* getObject(String name);

	protected:

		bool mTemporary;
		bool mInitialized;

		String mName;

		std::vector<LogicObject*> mObjects;

	};
}

#endif