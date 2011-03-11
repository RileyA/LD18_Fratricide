#include "stdafx.h"
#include "QEngine.h"
#include "StaticPhysicsObject.h"
#include "Engine.h"
#include "LogicObject.h"
#include "Logger.h"
#include "QEMath.h"

#include "EnemyShip.h"
#include "PlayerShip.h"
#include "BulletManager.h"

#include "PhysicsMasks.h"
#include "ParticleManager.h"


namespace QE
{
	EnemyShip::EnemyShip(QEngine* engine,String bucket,String name,int typ,Vector3 pos)
		:LogicObject(engine,bucket,name)
	{	

		dead = false;
		weapons = true;
		timer = 0.f;
		stp = typ;
		cooltime = 0;
		if(typ==0)
		{
			pos+=Vector3(randInt(-1.25f,1.25f),0,0);
			weaponCooloff=0.1f;
			health = 1.f;
			mObj = mEngine->getGraphicsManager()->createRenderedObject("Ship01.mesh");
			mObj->attachToRoot();
			mGuns.push_back(gun(Vector3(0.6,0,-1.696),0,25.5f));
			mGuns.push_back(gun(Vector3(-0.6,0,-1.696),0,25.5f));
			mPhys = mEngine->getPhysicsManager()->createConvexHull("Ship01C.mesh",Vector3(0,0,0));
			mPhysCore = mEngine->getPhysicsManager()->createConvexHull("Ship01CC.mesh",Vector3(0,0,0));

			speed_ = randFloat(15.f,20.f);
		}
		else if(typ==1)
		{
			pos+=Vector3(randInt(-1.f,1.f),0,0);
			weaponCooloff=0.2f;
			health = 1.25f;
			mGuns.push_back(gun(Vector3(0.f,0,-2.396),1,25.5f));
			mObj = mEngine->getGraphicsManager()->createRenderedObject("Ship02.mesh");
			mObj->attachToRoot();
			mPhys = mEngine->getPhysicsManager()->createConvexHull("Ship02C.mesh",Vector3(0,0,0));
			mPhysCore = mEngine->getPhysicsManager()->createConvexHull("Ship02CC.mesh",Vector3(0,0,0));
			speed_ = randFloat(10.f,15.f);

		}
		else if(typ==2)
		{
			pos+=Vector3(randInt(-1.1f,1.1f),0,0);
			weaponCooloff=0.15f;
			health = 1.4f;
			mObj = mEngine->getGraphicsManager()->createRenderedObject("Ship03.mesh");
			mObj->attachToRoot();
			mGuns.push_back(gun(Vector3(0.5,0,-1.696),0,25.5f));
			mGuns.push_back(gun(Vector3(0,0,0),1,30.5f));
			mGuns.push_back(gun(Vector3(-0.5,0,-1.696),0,25.5f));
			mPhys = mEngine->getPhysicsManager()->createConvexHull("Ship03C.mesh",Vector3(0,0,0));
			mPhysCore = mEngine->getPhysicsManager()->createConvexHull("Ship03CC.mesh",Vector3(0,0,0));
			speed_ = randFloat(9.75f,12.5f);
		}

		else if(typ==3)
		{
			pos+=Vector3(randInt(-1.4f,1.4f),0,0);
			weaponCooloff=0.085f;
			health = 0.75f;
			mObj = mEngine->getGraphicsManager()->createRenderedObject("Ship04.mesh");
			mObj->attachToRoot();
			mGuns.push_back(gun(Vector3(0.856,0,-0.366),0,29.5f));
			mGuns.push_back(gun(Vector3(-0.856,0,-0.366),0,29.5f));
			mPhys = mEngine->getPhysicsManager()->createConvexHull("Ship04C.mesh",Vector3(0,0,0));
			mPhysCore = mEngine->getPhysicsManager()->createConvexHull("Ship04CC.mesh",Vector3(0,0,0));
			speed_ = randFloat(15.f,25.5f);
		}

		else if(typ==4)
		{
			//pos+=Vector3(randInt(-1.4f,1.4f),0,0);

			weaponCooloff=0.25f;
			health = 3.f;
			mObj = mEngine->getGraphicsManager()->createRenderedObject("Ship05.mesh");
			mObj->attachToRoot();
			mGuns.push_back(gun(Vector3(2.06,0,0),1,27.5f));
			mGuns.push_back(gun(Vector3(-2.06,0,0),1,27.5f));
			mGuns.push_back(gun(Vector3(0,0,-3.3),1,27.5f));
			mPhys = mEngine->getPhysicsManager()->createConvexHull("Ship05C.mesh",Vector3(0,0,0));
			mPhysCore = mEngine->getPhysicsManager()->createConvexHull("Ship05CC.mesh",Vector3(0,0,0));
			speed_ = randFloat(7.f,10.5f);
		}

		mObj->cloneMaterial(0);

		data = SimpleUserData((void*)this,"EnemyShip");
		mPhys->setUserData(&data);


		mPhys->setKinematic(true);
		mPhys->setCollisionGroup(COL_ENEMY);
		mPhys->setCollisionMask(COL_STATICS);

		mPhysCore->setMass(3.f);

		mPhysCore->setCollisionGroup(COL_DYNAMICS);
		mPhysCore->setCollisionMask(COL_PLAYER|COL_DYNAMICS);

		mObj->yaw(180.f);

		mPhys->setOrientation(mObj->getOrientation());
		mPhysCore->setOrientation(mObj->getOrientation());
		mPhys->set2D(true);
		mPhysCore->set2D(true);
		mPhys->wakeup();
		mPhysCore->wakeup();
		speed = 0;
		maxSpeed = 25;
		minSpeed = 15;
		speedGain = 1.f;
		roll = 0;

		mPhys->setPosition(pos);
		mPhysCore->setPosition(pos);
		mObj->setPosition(pos);

		aubducted = false;

		spcdn = false;

		createSignal("fire");
		createSignal("explode");
		createSignal("addScore");
	}

	EnemyShip::~EnemyShip()
	{
		if(mInitialized)
		{
			deinitialize();
		}
	}

	void EnemyShip::update(double delta)
	{

		cooltime+=delta;
		if(!dead)
		{
		float h = health;
		if(h>1)
		{
			h = 1;
		}
		mObj->getMaterial(0).setAmbient(Colour((1-h/1),(h/1),0,1));

			std::vector<void*> data = mEngine->getPhysicsManager()->sphereOverlap(0.5f,mPhys->getPosition(),COL_ENEMY|COL_STATICS,COL_ENEMY|COL_PLAYER);
			bool crashed = false;
			if(data.size()>0)
			{
				for(unsigned int i=0;i<data.size();++i)
				{
					if(data[i])
					{
						SimpleUserData* d = (SimpleUserData*)data[i];
						if(d->type=="EnemyShip")
						{
							EnemyShip* shp = (EnemyShip*)d->data;
							if(shp->getName()!=getName())
							{
								shp->hit(1.5f);
								crashed = true;
							}
						}
						else if(d->type=="PlayerShip")
						{
							PlayerShip* shp = (PlayerShip*)d->data;
							shp->hit(1.f);
							crashed = true;
						}
					}
				}
			}

			if(crashed)
			{
				hit(1.5f);
			}
			else
			{
			//if(mObj->
			//float rollfact = 0.f;

				if(!aubducted)
				{
					mObj->setPosition(mPhysCore->getPosition());
					mObj->setOrientation(mPhysCore->getOrientation());// AABB assert issue here
					mPhys->setOrientation(mPhysCore->getOrientation());// AABB assert issue here
					mPhys->setPosition(mPhysCore->getPosition());

					timer+=delta;
					float rnd = randFloat(0.5f,1.5f);
					if(timer>rnd+weaponCooloff*3*difficulty)
					{
						timer = 0.f;
						shoot();
					}

					Vector3 v = mPhysCore->getVelocity();
					v.normalize();
					float an = v.angleBetween(Vector3(0,0,-1));

					if(mPhysCore->getVelocity().length()<speed_*difficulty)
					{
						mPhysCore->addImpulse(Vector3(0,0,-1*delta*500));
					}

					if(mPhysCore->getPosition().x>24.f)
					{
						mPhysCore->addImpulse(Vector3(-1*delta*250,0,0));
					}
					else if(mPhysCore->getPosition().x<-24.f)
					{
						mPhysCore->addImpulse(Vector3(1*delta*250,0,0));
					}
					else if(an>3.f&&weapons)
					{
						mPhysCore->addImpulse((Vector3(0,0,-1)-v)*delta*500);
					}


					if(mPhysCore->getPosition().z>50||mObj->getDerivedPosition().z<-15||mObj->getDerivedPosition().x>40||mObj->getDerivedPosition().x<-40)
					{
						dead = true;
						mPhys->setPosition(Vector3(0,randFloat(1000.f,20000.f),0));
						mPhysCore->setPosition(Vector3(0,randFloat(1000.f,20000.f),0));
						mPhysCore->setKinematic(true);
						mObj->detachFromParent();
						mObj->attachToRoot();
						mObj->setPosition(Vector3(0,randFloat(1000.f,20000.f),0));
						mObj->setVisible(false);
						aubducted = false;
					}	

				}
				else
				{
					mPhysCore->setPosition(mObj->getDerivedPosition());
					mPhys->setPosition(mObj->getDerivedPosition());
					mPhysCore->setOrientation(mObj->getDerivedOrientation());// AABB assert issue here
					mPhys->setOrientation(mObj->getDerivedOrientation());// AABB assert issue here
				}
			}
		}

	}

	void EnemyShip::deinitialize()
	{

	}

	void EnemyShip::shoot()
	{
		if(weapons&&cooltime>weaponCooloff)
		{
			cooltime = 0.f;
			for(unsigned int i=0;i<mGuns.size();++i)
			{
				bulletDesc dsc = bulletDesc();
				dsc.dir = Vector3(0,0,1);
				dsc.dir = mObj->getDerivedOrientation()*dsc.dir;
				dsc.pos = mObj->getDerivedPosition()-mObj->getDerivedOrientation()*(mGuns[i].relativePos);
				dsc.type = mGuns[i].type;
				dsc.speed = mGuns[i].speed;
				dsc.ori = mObj->getDerivedOrientation();
				getSignal("fire").fire(&MessageAny<bulletDesc>(dsc));
			}
			mEngine->getAudioManager()->play2D("fire01.wav");
		}
	}


	void EnemyShip::pushed(Vector3 oPos,float power)
	{
		Vector3 dir = mPhys->getPosition()-oPos;
		dir.normalize();
		mPhysCore->addImpulse(dir*power);
	}

	void EnemyShip::aubduct(SceneNode* node)
	{
		if(!dead)
		{
			mPhysCore->setKinematic(true);
			mPhysCore->setCollisionGroup(COL_DYNAMICS);
			mPhysCore->setCollisionMask(COL_DYNAMICS);
			mObj->detachFromParent();
			mObj->setPosition(Vector3(0,0,0));
			mObj->setOrientation(Quaternion(0,0,0,1));
			node->attachObject(mObj);
			aubducted = true;
			setWeapons(true);
		}
	}

	void EnemyShip::hit(float damage)
	{
		if(health>0)
		{
			health-=damage;
			if(health<=0)
			{
				dead = true;
				if(mPhys->getPosition().z<33)
				{
					score+=500;
					mEngine->getAudioManager()->play2D("explosion1.wav");
					// aspolsion pfx
					if(getShipType()==4)
					{
						getSignal("explode").fire(&MessageAny<psystemDesc>(psystemDesc(mObj->getDerivedPosition(),1)));
					}
					else
					{
						getSignal("explode").fire(&MessageAny<psystemDesc>(psystemDesc(mObj->getDerivedPosition(),0)));
					}
					getSignal("explode").fire(&MessageAny<psystemDesc>(psystemDesc(mObj->getDerivedPosition(),2)));
				}
				mPhys->setPosition(Vector3(0,10000,0));
				mPhysCore->setPosition(Vector3(0,10000,0));
				mPhysCore->setKinematic(true);
				mObj->detachFromParent();
				mObj->attachToRoot();
				mObj->setPosition(Vector3(0,10000,0));
				mObj->setVisible(false);
				aubducted = false;
			}
		}
	}

	void EnemyShip::release(Vector3 newPos,Quaternion ori,Vector3 veloc)
	{
		if(!dead)
		{
			mPhysCore->setKinematic(false);
			mPhysCore->setCollisionGroup(COL_DYNAMICS);
			mPhysCore->setCollisionMask(COL_DYNAMICS|COL_PLAYER);
			mObj->detachFromParent();
			mObj->attachToRoot();
			mObj->setPosition(newPos);
			mObj->setOrientation(ori);
			mPhys->setPosition(newPos);
			mPhysCore->setPosition(newPos);
			mPhysCore->setVelocity(veloc);
			aubducted = false;
		}
	}

	void EnemyShip::reactivate(Vector3 pos)
	{
		mObj->setPosition(pos);
		mObj->setOrientation(Quaternion(0,0,0,1));
		mObj->yaw(180.f);

		mPhys->setPosition(pos);
		mPhys->setOrientation(mObj->getOrientation());
		mPhysCore->setOrientation(mObj->getOrientation());
		mPhysCore->setPosition(pos);
		mPhysCore->setKinematic(false);

		mPhysCore->setVelocity(Vector3(0,0,0));
		dead = false;
		aubducted = false;
		weapons = true;
		health = 1.f;
		spcdn = false;
		mObj->setVisible(true);
	}

}