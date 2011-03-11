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
#include "Vector3.h"

#include "OgreVector3.h"

namespace QE
{
	const Vector3 Vector3::ZERO( 0, 0, 0 );
    const Vector3 Vector3::UNIT_X( 1, 0, 0 );
    const Vector3 Vector3::UNIT_Y( 0, 1, 0 );
    const Vector3 Vector3::UNIT_Z( 0, 0, 1 );
    const Vector3 Vector3::NEGATIVE_UNIT_X( -1,  0,  0 );
    const Vector3 Vector3::NEGATIVE_UNIT_Y(  0, -1,  0 );
    const Vector3 Vector3::NEGATIVE_UNIT_Z(  0,  0, -1 );
    const Vector3 Vector3::UNIT_SCALE(1, 1, 1);

	Vector3::operator Ogre::Vector3() const
	{
		return Ogre::Vector3(x,y,z);
	}

	Vector3::Vector3(const Ogre::Vector3 vect)
		:x(vect.x),y(vect.y),z(vect.z)
	{
	}

	//Vector3 Vector3::crossProduct(Ogre::Vector3 vect)
	//{
	//	return Vector3( y * vect.z - z * vect.y,
	//					z * vect.x - x * vect.z,
	//					x * vect.y - y * vect.x);
	//}

	//float Vector3::dotProduct(const Ogre::Vector3 vect)
	//{
	//	return x*vect.x + y*vect.y + z*vect.z;
	//}	

	///** Operators wih Ogre Vectors */


	//bool Vector3::operator==(const Ogre::Vector3 vect)
	//{
	//	return(x==vect.x&&y==vect.y&&z==vect.z);
	//}

	//Vector3& Vector3::operator=(const Ogre::Vector3 vect)
	//{
	//	x = vect.x;
	//	y = vect.y;
	//	z = vect.z;
	//	return *this;
	//}

	//bool Vector3::operator!=(const Ogre::Vector3 vect)
	//{
	//	return(x!=vect.x||y!=vect.y||z!=vect.z);
	//}

	//Vector3 Vector3::operator+(const Ogre::Vector3 vect)
	//{
	//	return Vector3(x+vect.x,y+vect.y,z+vect.z);
	//}

	//Vector3 Vector3::operator-(const Ogre::Vector3 vect)
	//{
	//	return Vector3(x-vect.x,y-vect.y,z-vect.z);
	//}

	//Vector3 Vector3::operator*(const Ogre::Vector3 vect)
	//{
	//	return Vector3(x*vect.x,y*vect.y,z*vect.z);
	//}

	//Vector3 Vector3::operator/(const Ogre::Vector3 vect)
	//{
	//	return Vector3(x/vect.x,y/vect.y,z/vect.z);
	//}

	//Vector3& Vector3::operator+=(const Ogre::Vector3 vect)
	//{
	//	x+=vect.x;
	//	y+=vect.y;
	//	z+=vect.z;
	//	return *this;
	//}

	//Vector3& Vector3::operator-=(const Ogre::Vector3 vect)
	//{
	//	x-=vect.x;
	//	y-=vect.y;
	//	z-=vect.z;
	//	return *this;
	//}

	//Vector3& Vector3::operator*=(const Ogre::Vector3 vect)
	//{
	//	x*=vect.x;
	//	y*=vect.y;
	//	z*=vect.z;
	//	return *this;
	//}

	//Vector3& Vector3::operator/=(const Ogre::Vector3 vect)
	//{
	//	x/=vect.x;
	//	y/=vect.y;
	//	z/=vect.z;
	//	return *this;
	//}
}