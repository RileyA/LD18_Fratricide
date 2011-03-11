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
#include "QEngine.h"
#include "LogicManager.h"


namespace QE
{
	LogicManager::LogicManager() : mInitialized(0)
	{
		
	}

	LogicManager::~LogicManager()
	{
		deinitialize();
	}

	void LogicManager::update(double delta)
	{
		std::vector<LogicBucket*>::iterator iter = mBuckets.begin();
		while(iter!=mBuckets.end())
		{
			(*iter)->update(delta);
			++iter;
		}
	}

	void LogicManager::initialize()
	{
		createBucket("Default",false);
		createBucket("DefaultTemp");
		
		mInitialized = true;
	}

	void LogicManager::deinitialize()
	{
		if(mInitialized)
		{
			mBucketMap.clear();
			for(unsigned int i = 0;i<mBuckets.size();++i)
			{
				delete mBuckets[i];
				mBuckets[i] = 0;
				mBuckets.erase(mBuckets.begin()+i);
				--i;
			}
			mInitialized = false;
		}
	}

	void LogicManager::createBucket(String name, bool temporary)
	{
		mBuckets.push_back(new LogicBucket(name,temporary));
		mBucketMap[name] = mBuckets[mBuckets.size()-1];
	}

	LogicBucket* LogicManager::getBucket(String name)
	{
		if(mBucketMap.find(name)!=mBucketMap.end())
		{
			return mBucketMap[name];
		}

		return 0;
	}

	void LogicManager::registerObject(LogicObject* obj,String bucketName)
	{
		if(mBucketMap.find(bucketName)!=mBucketMap.end())
		{
			if(mBucketMap[bucketName])
			{
				mBucketMap[bucketName]->addObject(obj);
			}
		}
		else
		{
			mBucketMap["DefaultTemp"]->addObject(obj);
		}
	}

	LogicObject* LogicManager::getObject(String name)
	{
		for(unsigned int i = 0;i<mBuckets.size();++i)
		{
			LogicObject* obj = mBuckets[i]->getObject(name);
			if(obj)
			{
				return obj;
			}
		}
		return 0;
	}

	bool LogicManager::connect(Slot& slot,Signal& signal)
	{
		signal.addListener(&slot);
		return true;
	}

	bool LogicManager::connect(Signal& signal,Slot& slot)
	{
		signal.addListener(&slot);
		return true;
	}

	bool LogicManager::connect(const String& objA,const String& objASignalName,const String& objB,const String& objBSlotName)
	{
		LogicObject* objectA = getObject(objA);
		LogicObject* objectB = getObject(objB);

		if(objectA&&objectB)
		{
			if(objectA->hasSignal(objASignalName)&&objectB->hasSlot(objBSlotName))
			{
				objectA->getSignal(objASignalName).addListener(&objectB->getSlot(objBSlotName));
				return true;
			}
		}
		return false;
	}

	bool LogicManager::connect(LogicObject* objectA,const String& objASignalName,LogicObject* objectB,const String& objBSlotName)
	{
		if(objectA->hasSignal(objASignalName)&&objectB->hasSlot(objBSlotName))
		{
			objectA->getSignal(objASignalName).addListener(&objectB->getSlot(objBSlotName));
			return true;
		}
		return false;
	}

	void LogicManager::_endState()
	{
		for(unsigned int i = 0;i<mBuckets.size();++i)
		{
			if(mBuckets[i]->isTemporary())
			{
				mBucketMap[mBuckets[i]->getName()] = 0;
				delete mBuckets[i];
				mBuckets[i] = 0;
				mBuckets.erase(mBuckets.begin()+i);
				--i;
			}
		}
		createBucket("DefaultTemp",true);
	}
}