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


#ifndef ANIMATIONSTATE_H
#define ANIMATIONSTATE_H

#include "stdafx.h"
#include "QEngine.h"

namespace Ogre
{
	class SceneNode;
	class Entity;
	class AnimationState;
}

namespace QE
{
	class QENGINE_API AnimationState
	{
	public:

		AnimationState(Ogre::AnimationState* state);
		~AnimationState();

		void update(double delta);

		void setEnabled(bool enabled);
		void setWeight(Real weight);
		void setSpeed(Real speed);
		void setLoop(bool loop);
		void setTime(Real time);

		float getTime();
		bool isLooped(){return mLoop;}
		bool isEnabled(){return mEnabled;}
		Real getWeight(){return mWeight;}
		Real getSpeed(){return mSpeed;}
		bool hasEnded();

		Real getLength();

		Ogre::AnimationState* getState(){return mState;}

	protected:

		Real mWeight;
		bool mEnabled;
		bool mLoop;
		Real mSpeed;
		Ogre::AnimationState* mState;
	};
}

#endif