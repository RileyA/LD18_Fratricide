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


#ifndef MATERIAL_H
#define MATERIAL_H

#include "stdafx.h"
#include "QEngine.h"

namespace Ogre
{
	class Material;
}

namespace QE
{
	class QENGINE_API Material
	{
	public:

		Material()
		{

		}

		Material(Ogre::Material* mat)
		{
			mMaterial = mat;
		}

		Material(String name);

		~Material()
		{
			
		}

		void setDiffuse(const Colour& colour,unsigned int technique=0,unsigned int pass=0);
		Colour getDiffuse(unsigned int technique=0,unsigned int pass=0);

		void setSpecular(const Colour& colour,unsigned int technique=0,unsigned int pass=0);
		Colour getSpecular(unsigned int technique=0,unsigned int pass=0);

		void setAmbient(const Colour& colour,unsigned int technique=0,unsigned int pass=0);
		Colour getAmbient(unsigned int technique=0,unsigned int pass=0);

		String getName();

		Material cloneMe();

	protected:

		Ogre::Material* mMaterial;

	};
}

#endif