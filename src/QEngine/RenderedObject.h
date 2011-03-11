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


#ifndef RENDEREDOBJECT_H
#define RENDEREDOBJECT_H

#include "stdafx.h"
#include "QEngine.h"
#include "SceneNode.h"
#include "Material.h"

#include "AnimationState.h"

namespace Ogre
{
	class SceneNode;
	class Entity;
	class AnimationState;
}

namespace QE
{
	class QENGINE_API RenderedObject : public SceneNode
	{
	public:

		RenderedObject(std::string name,std::string mesh);
		~RenderedObject();

		void updateAnimations(double delta);
		AnimationState* getAnimation(String name);

		Ogre::Entity* getEntity();

		void refreshAnimations();

		void disableAll();

		void setMaterial(String name,unsigned int index=0);

		void setMaterial(const Material& material,unsigned int index=0);

		Material& getMaterial(unsigned int index=0);

		/* Clone this object's material (so that it is unique and may be editted freely */
		void cloneMaterial(unsigned int index);

		bool isVisible();

	protected:

		Ogre::Entity* mEntity;
		std::map<String,AnimationState*> mAnimations;

		std::vector<std::pair<Material,bool> > mMaterials;

	};
}

#endif