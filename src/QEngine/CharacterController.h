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


#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include "stdafx.h"
#include "QEngine.h"
#include "Vector3.h"

namespace QE
{
	class PhysicsManager;

	class CharacterController
	{
	public:

		CharacterController(PhysicsManager* pmgr,Vector3 startPos=Vector3(0,0,0)){mPhysicsManager = pmgr;mStartPos = startPos;}
		virtual ~CharacterController(){};

		virtual void update(float delta) = 0;

		virtual void initialize() = 0;

		virtual void deinitialize() = 0;

		virtual Vector3 getPosition() = 0;

	protected:

		PhysicsManager* mPhysicsManager;
		Vector3 mStartPos;

	};
}

#endif