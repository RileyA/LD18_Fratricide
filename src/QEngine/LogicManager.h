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


#ifndef LOGICMANAGER_H
#define LOGICMANAGER_H

#include "stdafx.h"
#include "QEngine.h"
#include "LogicBucket.h"

namespace QE
{
	/** This is in charge of pretty much EVERYTHING that acts/reacts, it will use a bucket system to divide out game object updates */
	class QENGINE_API LogicManager
	{
	public:

		LogicManager();
		~LogicManager();

		/** Updates the manager and all buckets (thus all objects)
			@param delta The current delta time (time since last frame) */
		void update(double delta);

		/** Starts the manager up */
		void initialize();

		/** Shuts the LogicManager down (this is called by the destructor) */
		void deinitialize();

		/** Creates a Bucket
			@param name The name of the Bucket (this is important since the host application will not be dealing with Buckets directly
			@param temporary Whether or not the bucket will be deleted at the end of the current state */
		void createBucket(String name,bool temporary=true);

		/** Gets a named bucket
			@param name The name of the object */
		LogicBucket* getBucket(String name);

		/** Registers an object with the logic system (this should be done automatically by the base LogicObject class
			@param obj The object to add 
			@param bucketName The name of the bucket to place it in */
		void registerObject(LogicObject* obj,String bucketName);

		/** Gets an object pointer based on a name (this searches all active buckets for named object) 
			@param name The name of the object */
		LogicObject* getObject(String name);

		/** Connects a LogicObject's Signal with another's Slot (Delegate) 
			@param Signal The Signal to emit events/messages
			@param Slot The Slot to recieve the event(s)
			@returns Whether or not the connection succeeded */
		bool connect(Signal& signal,Slot& slot);

		/** Connects a LogicObject's Signal with another's Slot (Delegate) 
			@param Signal The Signal to emit events/messages
			@param Slot The Slot to recieve the event(s)
			@returns Whether or not the connection succeeded */
		bool connect(Slot& slot,Signal& signal);

		/** Connects a LogicObject's Signal with another's Slot (Delegate) 
			@param objA The name of the Object whose signal will be connected
			@param objASignalName The name of the Signal
			@param objB The name of the Object whose slot will be connected
			@param objBSlotName The name of the slot
			@returns Whether or not the connection succeeded */
		bool connect(const String& objA,const String& objASignalName,const String& objB,const String& objBSlotName);

		/** Connects a LogicObject's Signal with another's Slot (Delegate) 
			@param objA The LogicObject whose signal will be connected
			@param objASignalName The name of the Signal
			@param objB The LogicObject whose slot will be connected
			@param objBSlotName The name of the slot
			@returns Whether or not the connection succeeded */
		bool connect(LogicObject* objA,const String& objASignalName,LogicObject* objB,const String& objBSlotName);

		/** Internal function for ending the state (it deletes temporary buckets). */
		void _endState();

	protected:

		std::vector<LogicBucket*> mBuckets;
		std::map<String,LogicBucket*> mBucketMap;
		bool mInitialized;

	};
}

#endif