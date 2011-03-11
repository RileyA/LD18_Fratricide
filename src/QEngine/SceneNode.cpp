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
#include "SceneNode.h"
#include "Logger.h"
#include "Ogre.h"

namespace QE
{
	SceneNode::SceneNode(String name)
	{
		mName = name;
		mNode = Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->createSceneNode(name);
	}
	SceneNode::SceneNode(Ogre::SceneNode* node)
	{
		mNode = node;
		mName = node->getName();
	}
	SceneNode::~SceneNode()
	{
		mNode->detachAllObjects();
		mNode->getCreator()->destroySceneNode(mNode);
	}

	void SceneNode::setPosition(Vector3 pos)
	{
		mNode->setPosition(pos);
	}

	void SceneNode::setOrientation(Quaternion ori)
	{
		mNode->setOrientation(ori.w,ori.x,ori.y,ori.z);
	}

	void SceneNode::setScale(Vector3 scale)
	{
		mNode->setScale(scale);
	}

	Vector3 SceneNode::getPosition()
	{
		return Vector3(mNode->getPosition());
	}

	Quaternion SceneNode::getOrientation()
	{
		return Quaternion(mNode->getOrientation().x,mNode->getOrientation().y,mNode->getOrientation().z,mNode->getOrientation().w);
	}

	Vector3 SceneNode::getDerivedPosition()
	{
		return mNode->_getDerivedPosition();
	}

	Quaternion SceneNode::getDerivedOrientation()
	{
		return Quaternion(mNode->_getDerivedOrientation().x,mNode->_getDerivedOrientation().y,mNode->_getDerivedOrientation().z,mNode->_getDerivedOrientation().w);
	}

	Vector3 SceneNode::getScale()
	{
		return mNode->getScale();
	}

	void SceneNode::pitch(Real angle)
	{
		mNode->pitch(Ogre::Radian(Ogre::Degree(angle)));
	}

	void SceneNode::yaw(Real angle)
	{
		mNode->yaw(Ogre::Radian(Ogre::Degree(angle)));
	}

	void SceneNode::roll(Real angle)
	{
		mNode->roll(Ogre::Radian(Ogre::Degree(angle)));
	}

	String SceneNode::getName()
	{
        return mName;
	}

	void SceneNode::attachObject(SceneNode* obj)
	{
		mNode->addChild(obj->getSceneNode());
	}

	void SceneNode::detachAll()
	{
		mNode->detachAllObjects();
	}

	void SceneNode::detachNode(std::string name)
	{
		mNode->removeChild(name);
	}

	void SceneNode::detachNode(SceneNode* node)
	{
		mNode->removeChild(node->getSceneNode());
	}

	void SceneNode::detachFromParent()
	{
		if(mNode->getParent())
		{
			mNode->getParent()->removeChild(mNode);
		}
	}

	void SceneNode::attachToRoot()
	{
		Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->getRootSceneNode()->addChild(mNode);
	}

	Ogre::SceneNode* SceneNode::getSceneNode()
	{
		return mNode;
	}

	void SceneNode::setVisible(bool visible)
	{
		mNode->setVisible(visible);
	}

}
