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
#include "RenderedObject.h"
#include "Ogre.h"
#include "Logger.h"

namespace QE
{
	RenderedObject::RenderedObject(std::string name,std::string mesh)
		:SceneNode(name)
	{
		mEntity = Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->createEntity(name,mesh);
		mNode->attachObject(mEntity);

		for(unsigned int i = 0; i < mEntity->getNumSubEntities(); ++i)
		{
			mMaterials.push_back(std::pair<Material,bool>(Material(mEntity->getSubEntity(i)->getMaterial().getPointer()),false));
		}

		// check that there are indeed a skeleton and animations...
		if(mEntity->getSkeleton() && mEntity->getSkeleton()->getNumAnimations() > 0)
		{
			Ogre::AnimationStateSet *set = mEntity->getAllAnimationStates();
			Ogre::AnimationStateIterator it = set->getAnimationStateIterator();
			while(it.hasMoreElements())
			{
				Ogre::AnimationState* state = it.getNext();
				mAnimations[state->getAnimationName()] = new AnimationState(state);
			}
		}
	}

	RenderedObject::~RenderedObject()
	{
		std::map<String,AnimationState*>::iterator iter = mAnimations.begin();
		while(iter!=mAnimations.end())
		{
			delete iter->second;
			++iter;
		}
		mAnimations.clear();
		mEntity->_getManager()->destroyEntity(mEntity);
	}
	
	Ogre::Entity* RenderedObject::getEntity()
	{
		return mEntity;
	}

	void RenderedObject::updateAnimations(double delta)
	{
		std::map<String,AnimationState*>::iterator iter = mAnimations.begin();
		while(iter!=mAnimations.end())
		{
			if(iter->second->isEnabled())
			{
				iter->second->update(delta*static_cast<double>(iter->second->getSpeed()));
			}
			++iter;
		}
	}

	AnimationState* RenderedObject::getAnimation(String name)
	{
		if(mAnimations.find(name)!=mAnimations.end())
		{
			return mAnimations[name];
		}
		return 0;
	}

	void RenderedObject::refreshAnimations()
	{
		Ogre::AnimationStateSet *set = mEntity->getAllAnimationStates();
		Ogre::AnimationStateIterator it = set->getAnimationStateIterator();
		while(it.hasMoreElements())
		{
			Ogre::AnimationState* state = it.getNext();
			if(mAnimations.find(state->getAnimationName())==mAnimations.end())
			{
				mAnimations[state->getAnimationName()] = new AnimationState(state);
			}
		}
	}

	void RenderedObject::disableAll()
	{
		std::map<std::string,AnimationState*>::iterator iter = mAnimations.begin();
		while(iter!=mAnimations.end())
		{
			(*iter).second->setEnabled(false);
			++iter;
		}
	}

	void RenderedObject::setMaterial(String name,unsigned int index)
	{
		mMaterials[index].first = Material(name);
		mEntity->getSubEntity(index)->setMaterialName(name);
		mMaterials[index].second = false;
	}

	void RenderedObject::setMaterial(const Material& material,unsigned int index)
	{
		mMaterials[index].first = material;
		mEntity->getSubEntity(index)->setMaterialName(mMaterials[index].first.getName());
		mMaterials[index].second = false;
	}

	Material& RenderedObject::getMaterial(unsigned int index)
	{
		return mMaterials[index].first;
	}

	void RenderedObject::cloneMaterial(unsigned int index)
	{
		if(!mMaterials[index].second)
		{
			mMaterials[index].first = mMaterials[index].first.cloneMe();
			mEntity->getSubEntity(index)->setMaterialName(mMaterials[index].first.getName());
			mMaterials[index].second = true;
		}
	}

}