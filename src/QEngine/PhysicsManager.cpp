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


#include"stdafx.h"
#include "QEngine.h"
#include "PhysicsManager.h"

#include "TimeManager.h"
#include "Logger.h"
#include "PhysicsMasks.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"

#include "OgreStringConverter.h"

#include "Ogre.h"
#include "CCT_Primitive.h"

namespace QE
{
	PhysicsManager::PhysicsManager() : started(0)
	{
		mDynamicsWorld = 0;
		mAccumulation = 0;
		mInterpolation = 0;
		mTimeStep = 1.0f/100.0f;
	}

	PhysicsManager::~PhysicsManager()
	{
		QE::Logger::getSingletonPtr()->logMessage("Physics Manager Shutting Down...");
		ReleasePhysX();
		QE::Logger::getSingletonPtr()->logMessage("Physics Manager Shut Down Successfully.");
	}

	void PhysicsManager::setupPhysX()
	{
		mDynamicsWorld = NULL;

		mCollisionConfiguration = new btDefaultCollisionConfiguration();

		mDispatcher = new	btCollisionDispatcher(mCollisionConfiguration);
		mBroadphase = new btDbvtBroadphase();

		btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
		mSolver = sol;

		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher,mBroadphase,mSolver,mCollisionConfiguration);

		mDynamicsWorld->setGravity(btVector3(0,-10,0));
		//setGravity(Vector3(0,0,0));

		started = true;
		mAccumulation = 0;
		mInterpolation = 0;
		mTimeStep = 1.0f/100.0f;
	}

	void PhysicsManager::ReleasePhysX()
	{
		if(started)
		{
			int i;
			for (i=mDynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
			{
				btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[i];
				btRigidBody* body = btRigidBody::upcast(obj);
				if (body && body->getMotionState())
				{
					delete body->getMotionState();
				}
				mDynamicsWorld->removeCollisionObject( obj );
				delete obj;
			}

			for(unsigned int i=0;i<mObjects.size();++i)
			{
				delete mObjects[i];
				mObjects[i] = 0;
			}
			mObjects.clear();

			std::map<String,btCollisionShape*>::iterator iter = mShapes.begin();
			while(iter!=mShapes.end())
			{
				delete iter->second;
				++iter;
			}
			mShapes.clear();

			delete mDynamicsWorld;
			delete mSolver;
			delete mBroadphase;
			delete mDispatcher;
			delete mCollisionConfiguration;

			started = false;
		}
	};

	void PhysicsManager::GetPhysXResults()
	{

	};

	void PhysicsManager::ResetPhysX()
	{

	};


	void PhysicsManager::StartPhysX(float delta)
	{
		mDynamicsWorld->stepSimulation(mTimeStep,0);
	};

	bool PhysicsManager::isEnabled()
	{
		return started;
	}

	btDynamicsWorld* PhysicsManager::getScene()
	{
		return mDynamicsWorld;
	}

	void PhysicsManager::update(float delta)
	{
		mAccumulation += delta;

		while(mAccumulation>=mTimeStep)
		{
			StartPhysX(mTimeStep);

			for(unsigned int i=0;i<mObjects.size();++i)
			{
			    if(!mObjects[i]->readyForDelete())
			    {
                    mObjects[i]->update(true,0,mGlobalGravity);
			    }
			    else
			    {
                    delete mObjects[i];
                    mObjects[i] = 0;
                    mObjects.erase(mObjects.begin()+i);
                    --i;
			    }
			}
			mAccumulation -= mTimeStep;
		}

		mInterpolation = mAccumulation/mTimeStep;
		for(unsigned int i=0;i<mObjects.size();++i)
		{
			mObjects[i]->update(false,mInterpolation,mGlobalGravity);
		}
	}

	float PhysicsManager::getInterpolation()
	{
		return mInterpolation;
	}

	PhysicsObject* PhysicsManager::createStaticTrimesh(String meshname,Vector3 pos)
	{
		size_t vertex_count;
		btVector3* vertices;
		size_t numTris;
		size_t index_count;
		unsigned* indices;
		float vArray[9];
		vertex_count = index_count = 0;
		bool added_shared = false;
		size_t current_offset = vertex_count;
		size_t shared_offset = vertex_count;
		size_t next_offset = vertex_count;
		size_t index_offset = index_count;
		size_t prev_vert = vertex_count;
		size_t prev_ind = index_count;

		bool newShape = false;

		if(mShapes.find(meshname)==mShapes.end())
		{
			newShape = true;
			btTriangleMesh *mTriMesh = new btTriangleMesh();

			Ogre::MeshPtr mesh = Ogre::MeshManager::getSingletonPtr()->load(meshname,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);//Ogre::MeshPtr(mesh_);

			for(int i = 0;i < mesh->getNumSubMeshes();i++)
			{
				Ogre::SubMesh* submesh = mesh->getSubMesh(i);

				if(submesh->useSharedVertices)
				{
					if(!added_shared)
					{
						Ogre::VertexData* vertex_data = mesh->sharedVertexData;
						vertex_count += vertex_data->vertexCount;
						added_shared = true;
					}
				}
				else
				{
					Ogre::VertexData* vertex_data = submesh->vertexData;
					vertex_count += vertex_data->vertexCount;
				}

				Ogre::IndexData* index_data = submesh->indexData;
				index_count += index_data->indexCount;
			}

			int a = vertex_count;
			vertices = new btVector3[vertex_count];
			indices = new unsigned[index_count];

			added_shared = false;

			int rVT = 0;
			int rIT = 0;

			for(int i = 0;i < mesh->getNumSubMeshes();i++)
			{
				Ogre::SubMesh* submesh = mesh->getSubMesh(i);

				Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
				if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
				{
					if(submesh->useSharedVertices)
					{
						added_shared = true;
						shared_offset = current_offset;
					}

					const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
					Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
					unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
					Ogre::Real* pReal;

					for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
					{
						posElem->baseVertexPointerToElement(vertex, &pReal);

						Ogre::Vector3 pt;

						pt.x = (*pReal++);
						pt.y = (*pReal++);
						pt.z = (*pReal++);

						//pt = quat*pt;// + loc; //compensate for scale/transform/rotation (uneeded for static stuff)
						vertices[rVT + current_offset + j].setX(pt.x);
						vertices[rVT + current_offset + j].setY(pt.y);
						vertices[rVT + current_offset + j].setZ(pt.z);
					}

					vbuf->unlock();
					next_offset += vertex_data->vertexCount;
				}

				Ogre::IndexData* index_data = submesh->indexData;

				numTris = index_data->indexCount / 3;
				unsigned short* pShort;
				unsigned int* pInt;
				Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
				bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
				if (use32bitindexes) pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
				else pShort = static_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

				for(size_t k = 0; k < numTris; ++k)
				{
					size_t offset = (submesh->useSharedVertices)?shared_offset:current_offset;

					unsigned int vindex = use32bitindexes? *pInt++ : *pShort++;
					indices[rIT + index_offset + 0] = vindex + offset;
					vArray[0] =  vertices[indices[index_offset + 0]].x();
					vArray[1] =  vertices[indices[index_offset + 0]].y();
					vArray[2] =  vertices[indices[index_offset + 0]].z();
					vindex = use32bitindexes? *pInt++ : *pShort++;
					indices[rIT + index_offset + 1] = vindex + offset;
					vArray[3] =  vertices[indices[index_offset + 1]].x();
					vArray[4] =  vertices[indices[index_offset + 1]].y();
					vArray[5] =  vertices[indices[index_offset + 1]].z();
					vindex = use32bitindexes? *pInt++ : *pShort++;
					indices[rIT + index_offset + 2] = vindex + offset;
					vArray[6] =  vertices[indices[index_offset + 2]].x();
					vArray[7] =  vertices[indices[index_offset + 2]].y();
					vArray[8] =  vertices[indices[index_offset + 2]].z();

					mTriMesh->addTriangle(btVector3(vArray[0],vArray[1],vArray[2]),
						btVector3(vArray[3],vArray[4],vArray[5]),
						btVector3(vArray[6],vArray[7],vArray[8]));

					index_offset += 3;
				}
				ibuf->unlock();
				current_offset = next_offset;
			}

			btCollisionShape *triMeshShape = new btBvhTriangleMeshShape(mTriMesh,true);
			mShapes[meshname] = triMeshShape;

		}

		btVector3 localInertia(0,0,0);
		btCollisionObject* actor = new btCollisionObject();
		actor->setCollisionShape(mShapes[meshname]);
		actor->setWorldTransform(btTransform(btQuaternion::getIdentity(),btVector3(pos.x,pos.y,pos.z)));
		actor->setRestitution(0.1f);
		actor->setFriction(1.5f);
		//actor->setAnisotropicFriction(btVector3(0.8f,0.8f,0.8f));
		mDynamicsWorld->addCollisionObject(actor,COL_STATICS);
		mObjects.push_back(new PhysicsObject(actor,mDynamicsWorld));

		if(newShape)
		{
			delete[] vertices;
			delete[] indices;
		}

		return mObjects[mObjects.size()-1];
		//return NULL;
	}

	PhysicsObject* PhysicsManager::createStaticTrimesh(RenderedObject* object,Vector3 pos)
	{
		return createStaticTrimesh(object->getEntity()->getMesh()->getName(),pos);
	}

	PhysicsObject* PhysicsManager::createCube(Vector3 scale,Vector3 pos)
	{
		if(mShapes.find("BOX"+Ogre::StringConverter::toString(scale))==mShapes.end())
		{
			btBoxShape *boxShape = new btBoxShape(btVector3(scale.x,scale.y,scale.z));
			mShapes["BOX"+Ogre::StringConverter::toString(scale)] = (boxShape);
		}

		btVector3 localInertia(0,0,0);
		mShapes["BOX"+Ogre::StringConverter::toString(scale)]->calculateLocalInertia(18.f,localInertia);

		btRigidBody* actor = new btRigidBody(18.f,0,mShapes["BOX"+Ogre::StringConverter::toString(scale)],localInertia);	
		actor->setRestitution(0.3f);
		actor->setFriction(0.8f);
		actor->setAnisotropicFriction(btVector3(0.9f,0.9f,0.9f));
		actor->setWorldTransform(btTransform(btQuaternion::getIdentity(),btVector3(pos.x,pos.y,pos.z)));

		dynamic_cast<btDiscreteDynamicsWorld*>(mDynamicsWorld)->addRigidBody(actor,COL_DYNAMICS,COL_DYNAMICS|COL_KINEMATICS|COL_STATICS);
		mObjects.push_back(new PhysicsObject(actor,mDynamicsWorld));

		return mObjects[mObjects.size()-1];
	}

	PhysicsObject* PhysicsManager::createSphere(float radius,Vector3 pos)
	{
		if(mShapes.find("SPHERE"+Ogre::StringConverter::toString(radius))==mShapes.end())
		{
			btSphereShape *sphereShape = new btSphereShape(radius);
			mShapes["SPHERE"+Ogre::StringConverter::toString(radius)] = (sphereShape);
		}

		btVector3 localInertia(0,0,0);
		mShapes["SPHERE"+Ogre::StringConverter::toString(radius)]->calculateLocalInertia(18.f,localInertia);

		btRigidBody* actor = new btRigidBody(18.f,0,mShapes["SPHERE"+Ogre::StringConverter::toString(radius)],localInertia);	
		actor->setRestitution(0.3f);
		actor->setFriction(0.8f);
		actor->setAnisotropicFriction(btVector3(0.9f,0.9f,0.9f));
		actor->setWorldTransform(btTransform(btQuaternion::getIdentity(),btVector3(pos.x,pos.y,pos.z)));

		//dynamic_cast<btDiscreteDynamicsWorld*>(mDynamicsWorld)->addRigidBody(actor,COL_DYNAMICS,COL_DYNAMICS|COL_KINEMATICS|COL_STATICS);
		dynamic_cast<btDiscreteDynamicsWorld*>(mDynamicsWorld)->addRigidBody(actor,COL_KINEMATICS,COL_DYNAMICS|COL_KINEMATICS|COL_STATICS); //if I need collision filtering..

		mObjects.push_back(new PhysicsObject(actor,mDynamicsWorld));

		return mObjects[mObjects.size()-1];
	}

	PhysicsObject* PhysicsManager::createConvexHull(String meshname,Vector3 pos,Vector3 scale)
	{
		bool newshape = false;
		size_t vertex_count;
		float* vertices;
		size_t index_count;
		vertex_count = index_count = 0;

		bool added_shared = false;
		size_t current_offset = vertex_count;
		size_t shared_offset = vertex_count;
		size_t next_offset = vertex_count;
		size_t index_offset = index_count;
		size_t prev_vert = vertex_count;
		size_t prev_ind = index_count;
		std::vector<Vector3> vertVect;
		btConvexShape *convexShape;
		btShapeHull* hull;

		if(meshname=="CUBE"||meshname=="BOX")
		{
			return createCube(scale,pos);
		}
		if(mShapes.find(meshname+"convex")==mShapes.end())
		{
			newshape = true;

			Ogre::MeshPtr mesh = Ogre::MeshManager::getSingletonPtr()->load(meshname,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

			// Calculate how many vertices and indices we're going to need
			for(int i = 0;i < mesh->getNumSubMeshes();i++)
			{
				Ogre::SubMesh* submesh = mesh->getSubMesh(i);

				// We only need to add the shared vertices once
				if(submesh->useSharedVertices)
				{
					if(!added_shared)
					{
						Ogre::VertexData* vertex_data = mesh->sharedVertexData;
						vertex_count += vertex_data->vertexCount;
						added_shared = true;
					}
				}
				else
				{
					Ogre::VertexData* vertex_data = submesh->vertexData;
					vertex_count += vertex_data->vertexCount;
				}

				// Add the indices
				Ogre::IndexData* index_data = submesh->indexData;
				index_count += index_data->indexCount;
			}

			// Allocate space for the vertices and indices
			int a = vertex_count;

			vertices = new float[vertex_count*3];

			added_shared = false;

			// Run through the submeshes again, adding the data into the arrays
			for(int i = 0;i < mesh->getNumSubMeshes();i++)
			{
				Ogre::SubMesh* submesh = mesh->getSubMesh(i);

				Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
				if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
				{
					if(submesh->useSharedVertices)
					{
						added_shared = true;
						shared_offset = current_offset;
					}

					const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
					Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
					unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
					Ogre::Real* pReal;

					for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
					{
						posElem->baseVertexPointerToElement(vertex, &pReal);

						Ogre::Vector3 pt;

						pt.x = (*pReal++);
						pt.y = (*pReal++);
						pt.z = (*pReal++);

						bool skip = false;

						// ignore duped verts
						for(unsigned int i=0;i<vertVect.size();++i)
						{
							if(pt.squaredDistance(vertVect[i])==0||vertVect[i]==pt)
							{
								skip = true;
								//std::cout<<"IGNORED!\n";
							}
						}

						skip = false;

						if(!skip)
						{
							vertices[current_offset + (j*3)] = pt.x*0.92f;
							vertices[current_offset + (j*3) + 1] = pt.y*0.92f;
							vertices[current_offset + (j*3) + 2] = pt.z*0.92f;
							vertVect.push_back(pt);
						}
					}
					vbuf->unlock();
					next_offset += vertex_data->vertexCount;
				}
			}


			convexShape = new btConvexHullShape(static_cast<btScalar*>(vertices),vertVect.size(),3*sizeof(btScalar));
			//create a hull approximation
			btConvexShape *finalShape = 0;
			std::cout<<"size: "<<vertVect.size()<<"\n";
			if(vertVect.size()>75)
			{
				hull = new btShapeHull(convexShape);
				btScalar margin = convexShape->getMargin();
				hull->buildHull(margin);
				//btConvexHullShape* simplifiedConvexShape = new btConvexHullShape(hull->getVertexPointer(),hull->numVertices());
				
				btConvexHullShape* simplifiedConvexShape = new btConvexHullShape();
				for (int i=0;i<hull->numVertices();i++)
				{
					btVector3 vect = hull->getVertexPointer()[i]*0.9;
					std::cout<<"Vert: "<<vect.x()<<" "<<vect.y()<<" "<<vect.z()<<"\n";
					simplifiedConvexShape->addPoint(vect);
				}
				mShapes[meshname+"convex"] = (simplifiedConvexShape);
				finalShape = simplifiedConvexShape;
			}
			else
			{
				mShapes[meshname+"convex"] = (convexShape);
				finalShape = convexShape;
			}
		}
		//finalShape->setMargin(0.f);

		btVector3 localInertia(0,0,0);
		mShapes[meshname+"convex"]->calculateLocalInertia(10.f,localInertia);

		btRigidBody* actor = new btRigidBody(10.f,0,mShapes[meshname+"convex"],localInertia);	
		actor->setWorldTransform(btTransform(btQuaternion::getIdentity(),btVector3(pos.x,pos.y,pos.z)));
		actor->setRestitution(0.01f);
		actor->setFriction(0.9f);
		//actor->setAnisotropicFriction(btVector3(0.3f,0.3f,0.3f));
		actor->setDamping(0.f,0.5f);

		//mDynamicsWorld->addCollisionObject(actor,COL_STATICS);

		dynamic_cast<btDiscreteDynamicsWorld*>(mDynamicsWorld)->addRigidBody(actor,COL_DYNAMICS,COL_DYNAMICS|COL_KINEMATICS|COL_STATICS);
		//mDynamicsWorld->addRigidBody(actor);
		mObjects.push_back(new PhysicsObject(actor,mDynamicsWorld));

		if(newshape)
		{
			delete[] vertices;
			if(vertVect.size()>75)
			{
				delete hull;
				delete convexShape;
			}
		}

		return mObjects[mObjects.size()-1];
	}

	PhysicsObject* PhysicsManager::createConvexHull(RenderedObject* object,Vector3 pos)
	{
		return createConvexHull(object->getEntity()->getMesh()->getName(),pos);
	}

	Vector3 PhysicsManager::getLocalGravity(Vector3 pos)
	{
		return mGlobalGravity;
	}

	Vector3 PhysicsManager::getGravity()
	{
		return mGlobalGravity;
	}

	void PhysicsManager::setGravity(Vector3 grav)
	{
		mGlobalGravity = grav;
	}

	std::vector<void*> PhysicsManager::sphereOverlap(float radius,Vector3 pos,short int group,short int mask)
	{
		OverlapResultCallback4 overlapCall = OverlapResultCallback4();
		overlapCall.m_collisionFilterGroup = group;
		overlapCall.m_collisionFilterMask = mask;
		btSphereShape sphsh = btSphereShape(radius);
		btCollisionObject* bcobj = new btCollisionObject();
		bcobj->setUserPointer(NULL);
		btVector3 boxPosition = btVector3(pos.x,pos.y,pos.z);
		bcobj->setWorldTransform(btTransform(btQuaternion::getIdentity(),boxPosition));
		bcobj->setCollisionShape(&sphsh);
		getScene()->contactTest(bcobj,overlapCall);
		delete bcobj;

		return overlapCall.mObjectData;
	}

	std::vector<void*> PhysicsManager::actorOverlap(PhysicsObject* actor,short int group,short int mask)
	{
		OverlapResultCallback4 overlapCall = OverlapResultCallback4();
		overlapCall.m_collisionFilterGroup = group;
		overlapCall.m_collisionFilterMask = mask;
		btCollisionObject* bcobj = new btCollisionObject();
		bcobj->setUserPointer(NULL);
		Vector3 pos = actor->getPosition();
		btVector3 boxPosition = btVector3(pos.x,pos.y,pos.z);
		bcobj->setWorldTransform(btTransform(btQuaternion::getIdentity(),boxPosition));
		bcobj->setCollisionShape(actor->getActor()->getCollisionShape());
		getScene()->contactTest(bcobj,overlapCall);
		delete bcobj;

		return overlapCall.mObjectData;
		//OverlapResultCallback4 overlapCall = OverlapResultCallback4();
		//overlapCall.m_collisionFilterGroup = group;
		//overlapCall.m_collisionFilterMask = mask;
		//getScene()->contactTest(actor->getActor(),overlapCall);
		//return overlapCall.mObjectData;
	}

	RaycastReport PhysicsManager::raycastSimple(Vector3 pos,Vector3 dir,short group,short mask,Real dist)
	{
		//NxRaycastHit h;
		//dir*=dist;
		dir.normalize();
		dir*=dist;

		btCollisionWorld::ClosestRayResultCallback cb(btVector3(pos.x,pos.y,pos.z), btVector3(pos.x+dir.x,pos.y+dir.y,pos.z+dir.z));
		cb.m_collisionFilterGroup = group;
		cb.m_collisionFilterMask = mask;
		mDynamicsWorld->rayTest(btVector3(pos.x,pos.y,pos.z),btVector3(pos.x+dir.x,pos.y+dir.y,pos.z+dir.z),cb);
		
		//NxShape* closeShape = mScene->raycastClosestShape(NxRay(NxVec3(pos.x,pos.y,pos.z),NxVec3(dir.x,dir.y,dir.z)),NX_ALL_SHAPES,h,0|1|2|3|4|5,150.f);
		if(cb.hasHit())
		{
			//cb.m_collisionObject->getU
			return RaycastReport(
				Vector3(cb.m_hitNormalWorld.x(),cb.m_hitNormalWorld.y(),cb.m_hitNormalWorld.z()),
				Vector3(cb.m_hitPointWorld.x(),cb.m_hitPointWorld.y(),cb.m_hitPointWorld.z()),
				(SimpleUserData*)cb.m_collisionObject->getUserPointer()
				);
		}
		else
		{
			return RaycastReport();
		}
	}




};
