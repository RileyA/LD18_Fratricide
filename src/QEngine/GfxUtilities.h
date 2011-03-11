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


#ifndef GFXUTILITIES_H
#define GFXUTILITIES_H

#include "stdafx.h"
#include "QEngine.h"

#include "Ogre.h"

namespace QE
{
	void getMeshInformation(const Ogre::MeshPtr mesh,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                Ogre::Vector2* &texcoords,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale,
								unsigned int texcoordSet=1);

}

#endif