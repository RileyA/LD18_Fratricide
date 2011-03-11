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

#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "stdafx.h"
#include "QEngine.h"

#include "SceneNode.h"

namespace Ogre
{
	class ParticleSystem;
}

namespace QE
{
	class QENGINE_API ParticleSystem : public SceneNode
	{
	public:

		ParticleSystem(String name,String system);
		~ParticleSystem();

		Real getSpeed();
		void setSpeed(Real spd);
		void fastForward(Real amnt);
		void setEnabled(bool en);
		bool isEnabled();

	protected:

		// The Ogre particle system
		Ogre::ParticleSystem* mSystem;

	};
}

#endif