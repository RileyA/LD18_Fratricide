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
#include "AnimationState.h"

#include "OgreAnimationState.h"

namespace QE
{
	AnimationState::AnimationState(Ogre::AnimationState* state):mSpeed(1.0f),mEnabled(0),mWeight(1.0f),mLoop(0)
	{
		mState = state;
		mState->setLoop(false);
		mState->setEnabled(false);
		mState->setWeight(1.0f);
	}

	AnimationState::~AnimationState()
	{
		setEnabled(false);
		mState = 0;
	}

	void AnimationState::setEnabled(bool enabled)
	{
		mEnabled = enabled;
		mState->setEnabled(enabled);
	}

	void AnimationState::setWeight(Real weight)
	{
		mWeight = weight;
		mState->setWeight(weight);
	}

	void AnimationState::setLoop(bool loop)
	{
		mLoop = loop;
		mState->setLoop(loop);
	}	

	void AnimationState::setSpeed(Real speed)
	{
		mSpeed = speed;
	}

	void AnimationState::update(double delta)
	{
		mState->addTime(static_cast<Real>(delta*mSpeed));
	}

	bool AnimationState::hasEnded()
	{
		return mState->hasEnded();
	}

	void AnimationState::setTime(Real time)
	{
		mState->setTimePosition(time);
	}

	float AnimationState::getTime()
	{
		return mState->getTimePosition();
	}

	Real AnimationState::getLength()
	{
		return mState->getLength();
	}

}