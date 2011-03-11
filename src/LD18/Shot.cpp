#include "stdafx.h"
#include "QEngine.h"
#include "StaticPhysicsObject.h"
#include "Engine.h"
#include "LogicObject.h"
#include "Logger.h"
#include "PhysicsManager.h"
#include "PhysicsMasks.h"

#include "Shot.h"
#include "EnemyShip.h"
#include "PlayerShip.h"

namespace QE
{
	Shot::Shot(QEngine* engine,String bucket,String name,Vector3 pos,Vector3 dir,Quaternion ori,float spd,int type)
		:LogicObject(engine,bucket,name)
	{
		if(type==0)
		{
			mObj = mEngine->getGraphicsManager()->createRenderedObject("Shot.mesh");
			damage = 0.26f;
			mObj->setScale(Vector3(1.9f,1.9f,2.f));

		}
		else if(type==1)
		{
			mObj = mEngine->getGraphicsManager()->createRenderedObject("Shot.mesh");
			mObj->setMaterial("Shot02");
			mObj->setScale(Vector3(2.2f,2.2f,3.f));
			damage = 0.8f;
		}
		pos.y/=3;
		mObj->setPosition(pos);
		mObj->attachToRoot();
		mObj->setOrientation(ori);
		direction = dir;
		direction.y = 0;
		direction.normalize();
		speed = spd*difficulty;
		stype = type;
		timer = 0.f;
		raytimer = 0.25f;
		done = false;
	}

	Shot::~Shot()
	{
		
	}

	void Shot::update(double delta)
	{
		timer+=delta;
		raytimer+=delta;
		if(raytimer>0.02f&&!done)
		{
			// raycast?
			RaycastReport rp = mEngine->getPhysicsManager()->raycastSimple(mObj->getPosition(),direction,COL_ENEMY|COL_STATICS,COL_ENEMY|COL_PLAYER,0.05f*speed);
			if(rp.hit)
			{
				if(rp.data->type=="EnemyShip")
				{
					done = true;
					timer = 10.f;
					mEngine->getAudioManager()->play2D("hit01.wav");
					EnemyShip* shp = (EnemyShip*)rp.data->data;
					if(shp->aubducted)
					{
						shp->hit(damage/1.5);
					}
					else
					{
						shp->hit(damage/2);
					}
				}
				else if(rp.data->type=="PlayerShip")
				{
					done = true;
					timer = 10.f;
					PlayerShip* shp = (PlayerShip*)rp.data->data;
					shp->hit(damage/2.f);
					mEngine->getAudioManager()->play2D("hit01.wav");
				}
			}
			else
			{
				//mObj->setPosition(mObj->getPosition()+direction*0.2f*speed);
			}

			raytimer = 0.f;

		}

		if(timer>=5.f)
		{
			done = true;
			mObj->setVisible(false);
		}

		if(!done)
		{
			// move
			mObj->setPosition(mObj->getPosition()+direction*speed*delta);

			// hit check

		}

	}

	void Shot::deinitialize()
	{

	}

}