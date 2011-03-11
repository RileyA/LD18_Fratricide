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
#include "Quaternion.h"
#include "Ogre.h"

#include "Vector3.h"

namespace QE
{
	Ogre::Quaternion Quaternion::toOgre()
	{
		return Ogre::Quaternion(w,x,y,z);
	}
	Quaternion::Quaternion(Ogre::Quaternion q)
		:x(q.x),y(q.y),z(q.z),w(q.w)
	{
	}

	Vector3 Quaternion::operator* (Vector3 v)
    {
		Vector3 uv, uuv;
		Vector3 qvec(x, y, z);
		uv = qvec.crossProduct(v);
		uuv = qvec.crossProduct(uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;
		return v + uv + uuv;
    }

}