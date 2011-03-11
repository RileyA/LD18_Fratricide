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

#include "Material.h"
#include "OgreMaterialManager.h"
#include "OgreMaterial.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "StringUtils.h"

namespace QE
{
	Material::Material(String name)
	{
		Ogre::MaterialPtr ptr = Ogre::MaterialManager::getSingletonPtr()->getByName(name);
		mMaterial = ptr.getPointer();
	}

	void Material::setDiffuse(const Colour& colour,unsigned int technique,unsigned int pass)
	{
		if(technique)
		{
			if(pass)
			{
				mMaterial->getTechnique(technique)->getPass(pass)->setDiffuse(colour);
			}
			else
			{
				mMaterial->getTechnique(technique)->setDiffuse(colour);
			}
		}
		else
		{
			mMaterial->setDiffuse(colour);
		}
	}

	Colour Material::getDiffuse(unsigned int technique,unsigned int pass)
	{
		return mMaterial->getTechnique(technique)->getPass(pass)->getDiffuse();
	}

	void Material::setSpecular(const Colour& colour,unsigned int technique,unsigned int pass)
	{
		if(technique)
		{
			if(pass)
			{
				mMaterial->getTechnique(technique)->getPass(pass)->setSpecular(colour);
			}
			else
			{
				mMaterial->getTechnique(technique)->setSpecular(colour);
			}
		}
		else
		{
			mMaterial->setSpecular(colour);
		}
	}

	Colour Material::getSpecular(unsigned int technique,unsigned int pass)
	{
		return mMaterial->getTechnique(technique)->getPass(pass)->getSpecular();
	}

	void Material::setAmbient(const Colour& colour,unsigned int technique,unsigned int pass)
	{
		if(technique)
		{
			if(pass)
			{
				mMaterial->getTechnique(technique)->getPass(pass)->setAmbient(colour);
			}
			else
			{
				mMaterial->getTechnique(technique)->setAmbient(colour);
			}
		}
		else
		{
			mMaterial->setAmbient(colour);
		}
	}

	Colour Material::getAmbient(unsigned int technique,unsigned int pass)
	{
		return mMaterial->getTechnique(technique)->getPass(pass)->getAmbient();
	}

	String Material::getName()
	{
		return mMaterial->getName();
	}

	Material Material::cloneMe()
	{
		String name = mMaterial->getName();

		unsigned int i = 0;
		Ogre::MaterialPtr ptr = Ogre::MaterialManager::getSingletonPtr()->getByName(name+StringUtils::toString(i));
		while(ptr.getPointer())
		{
			++i;// keep going until we get a unique name
			ptr = Ogre::MaterialManager::getSingletonPtr()->getByName(name+StringUtils::toString(i));
		}

		Ogre::MaterialPtr newptr = mMaterial->clone(name+StringUtils::toString(i));

		return Material(newptr.getPointer());
	}
}	