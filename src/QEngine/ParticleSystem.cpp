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

#include "ParticleSystem.h"
#include "OgreParticleSystem.h"
#include "OgreSceneManager.h"
#include "OgreRoot.h"
#include "OgreSceneNode.h"

namespace QE
{
	ParticleSystem::ParticleSystem(String name,String system)
		:SceneNode(name)
	{
		mSystem = Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->createParticleSystem(name+"ParticleSystem",system);
		mNode->attachObject(mSystem);
	}

	ParticleSystem::~ParticleSystem()
	{
		Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->destroyParticleSystem(mSystem);
	}

	Real ParticleSystem::getSpeed()
	{
		return mSystem->getSpeedFactor();
	}

	void ParticleSystem::setSpeed(Real spd)
	{
		mSystem->setSpeedFactor(spd);
	}

	void ParticleSystem::fastForward(Real amnt)
	{
		mSystem->fastForward(amnt);
	}

	void ParticleSystem::setEnabled(bool en)
	{
		mSystem->setEmitting(en);
	}

	bool ParticleSystem::isEnabled()
	{
		return mSystem->getEmitting();
	}

}