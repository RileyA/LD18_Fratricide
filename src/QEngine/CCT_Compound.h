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


#ifndef CCTCOMPOUND_H
#define CCTCOMPOUND_H

#include "stdafx.h"
#include "QEngine.h"
#include "CCT_Primitive.h"
#include "Ogre.h"

// NOTE! This is some of the ugliest code ever to exist , it was originally written and rewritten about 30 times on a tight deadline for a game with PhysX like 2 years ago,
// then really horribly adapted to Bullet. It works pretty darn well as a character controller that can rotate dynamically with gravity, but don't expect the code to make any sense.


// A compound of primitive spheres, to form a fully featured CCT rig with a fully arbitrary up vector (I hope)

namespace QE
{
	class CCTCompound
	{
	public:

		// constructor. choose the number of primitives and the height/radius
		// the offset value is generated based off this (the spheres are placed evenly along the compound capsule shape
		CCTCompound(unsigned int n, float height_, float radius_,btDynamicsWorld* scene,const btVector3& posi);

		~CCTCompound();

		// move the CCT in a given direction/magnitude, it will slide and all that if such features are enabled (see various etters and gatters later in this header.
		void move(const btVector3& dir,bool slidingE,double deltaT,bool zeroG=false);

		float getRadius()
		{
			return radius;
		}

		btVector3 getPosition()
		{
			return position;
		}

		btVector3 getEyePosition()
		{
			return cctp[numberPrimitives-1]->getPosition();
		}

		float getHeight()
		{
			return height;
		}

		void disableQs();
		void enableQs();

		// sets the position (returns false if the overlap fails and the move isn't posible...)
		bool setPosition(const btVector3& pos, bool set,const btVector3& UP,bool report);
		bool setPosition(const btVector3& pos, bool set,const btVector3& UP);

		// utilities for setting up actor flags
		void setCollision0(unsigned int except,bool collision);

		void setCollision(bool collision);

		// simple linear capsule sweep
		bool sweepCapsule(const btVector3& pos,const btVector3& dir,const btVector3& UP);

		// returns the up vector
		btVector3 getUpVector()
		{
			return upVector;
		}

		//tries to set the up vector, returns the real upvect after this...
		// resembles a movement func with sliding and such
		btVector3 setUpVector(const btVector3& newUp,double delta);

		SweepClosestCall sweepCapsuleDetail(const btVector3& pos,const btVector3& dir,const btVector3& UP);

	//(un)protected:

		// various private bits of data
		btVector3 position;
		float height;
		float radius;
		int numberPrimitives;
		float increment;
		float skinWidth;

		//NxVec3 pos1;
		//NxVec3 pos2;

		btDynamicsWorld* Scene;

		// nodes to store the orientation data, and to help interpolate between up vectors
		Ogre::SceneNode* targetNode;
		Ogre::SceneNode* lastNode;
		Ogre::SceneNode* currentNode;

		// the distance between the centers of each cct primitive
		float offset;

		// capsules for sweeping and for overlap checking, no actual actor here... collsions are done with the constituent primitives, these really aren't truly necessary.
		NxCapsule sweepCap;
		NxCapsule overCap;

		std::vector<CCTPrimitive*> cctp;

		bool move_;

		// visualization junk
		Ogre::BillboardSet* st;// move individual particles
		Ogre::SceneNode* node;// aligned with the bottommost primitive as with all other position metrics (I think...)


		//gravity swapping stuff
		NxVec3 upVector;

		float mUpInterp;

	};
}

#endif
