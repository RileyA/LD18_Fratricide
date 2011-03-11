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


#ifndef PHYSICSMASKS_H
#define PHYSICSMASKS_H

#include "stdafx.h"

namespace QE
{

	#define BIT(x) (1<<(x))
	enum CollisionTypes 
	{
		COL_NOTHING = 0, //Collide with nothing
		COL_STATICS = BIT(1), //Collide with default static trimeshes
		COL_DYNAMICS = BIT(2), //Collide with default dynamic stuff
		COL_KINEMATICS = BIT(3), //Collide with default kinematics
		COL_PLAYER = BIT(4), //Collide with player
		COL_ENEMY = BIT(5), //Collide with enemies
		//COL_ENEMY = BIT(5), //Collide with default kinematics
	};
};

#endif