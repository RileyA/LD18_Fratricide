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


#ifndef QUATERNION_H 
#define QUATERNION_H 

#include "stdafx.h"
#include "QEngine.h"

namespace Ogre
{
	class Quaternion;
}

namespace QE
{
	/** A quaternion, for representing rotations! */
	class QENGINE_API Quaternion
	{
	public:

		Quaternion()
			:x(0),y(0),z(0),w(0)
		{
		}

		Quaternion(Real x_,Real y_,Real z_,Real w_)
			:x(x_),y(y_),z(z_),w(w_)
		{
		}

		Vector3 operator*(Vector3 vect);

		Quaternion(Ogre::Quaternion q);

		Ogre::Quaternion toOgre();
	
	//protected:

		Real x;
		Real y;
		Real z;
		Real w;


	};
}

#endif