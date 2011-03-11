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
#include "LogicBucket.h"

namespace QE
{
	LogicBucket::LogicBucket(String name,bool temporary)
		:mInitialized(1)
		,mTemporary(temporary)
		,mName(name)
	{
	}

	LogicBucket::~LogicBucket()
	{
		deinitialize();
	}

	void LogicBucket::update(double delta)
	{
		std::vector<LogicObject*>::iterator iter = mObjects.begin();
		while(iter!=mObjects.end())
		{
		    // possible double update after deleting something??
		    if(!(*iter)->isReadyForDeletion())
		    {
                (*iter)->update(delta);
                ++iter;
		    }
		    else
		    {
                delete (*iter);
                (*iter) = 0;
                mObjects.erase(iter);
                --iter;
		    }

        }
	}

	void LogicBucket::deinitialize()
	{
		if(mInitialized)
		{
			while(!mObjects.empty())
			{
				delete mObjects[0];
				mObjects[0] = 0;
				mObjects.erase(mObjects.begin());
			}
			mInitialized = false;
		}
	}

	void LogicBucket::addObject(LogicObject* obj)
	{
		mObjects.push_back(obj);
	}

	LogicObject* LogicBucket::getObject(String name)
	{
		for(unsigned int i = 0;i<mObjects.size();++i)
		{
			if(mObjects[i]->getName()==name)
			{
				return mObjects[i];
			}
		}
		return 0;
	}


}
