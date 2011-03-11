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


#ifndef SCENENODE_H
#define SCENENODE_H
#include "stdafx.h"
#include "QEngine.h"

namespace Ogre
{
	class SceneNode;
	class Entity;
}

namespace QE
{
	/** A scene node, this represents a node in 3d space. It is a wrapper of Ogre's scene
	node class, and can be extended to a variety of types (for lights, cameras, entities etc).*/
	class QENGINE_API SceneNode
	{
	public:

		/** Constructor */
		SceneNode(String name);
		/** Constructor */
		SceneNode(Ogre::SceneNode* node);
		/** Destructor */
		virtual ~SceneNode();

		/** Sets the node's position
			\param pos The desired position in QEngine's Vector3 format */
		void setPosition(Vector3 pos);

		/** Sets the node's orientation (rotation)
			\param ori The desired orientation in QEngine's Quaternion format */
		void setOrientation(Quaternion ori);

		/** Sets the node's scale
			\param scale the desired scale */
		void setScale(Vector3 scale);

		/** Gets the position of the node
			\returns the position in Vector3 format */
		Vector3 getPosition();

		Vector3 getDerivedPosition();
		Quaternion getDerivedOrientation();

		Quaternion getOrientation();

		/** Gets the node's scale
			\returns the node's scale */
		Vector3 getScale();

		void pitch(Real angle);
		void yaw(Real angle);
		void roll(Real angle);

		/** Gets the name of the node
            \returns The name of the node */
		String getName();

		/** Attach another object (anything derived from SceneNode) to this node
			\param obj The object to be attached */
		void attachObject(SceneNode* obj);

		/** Sets the node to be visible or invisible
			\param visible Whether or not it will be visible */
		void setVisible(bool visible);

		/** Detaches all attached objects */
		void detachAll();

		/** Detaches an object from this node by name */
		void detachNode(std::string name);

		/** Detaches an object from this node by name */
		void detachNode(SceneNode* node);

		/** Detaches this node from its parent */
		void detachFromParent();

		/** Attaches this node to the root SceneNode */
		void attachToRoot();

		/** Gets the Ogre node
			\returns The Ogre scene node*/
		Ogre::SceneNode* getSceneNode();

	protected:

		// The attached objects
		std::vector<SceneNode*> mAttached;

		// The Ogre scene node
		Ogre::SceneNode* mNode;

		// The name of this node
		String mName;
	};
}

#endif
