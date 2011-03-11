#include "stdafx.h"
#include "QEngine.h"
#include "StaticPhysicsObject.h"
#include "Engine.h"
#include "LogicObject.h"
#include "Logger.h"

#include "BulletManager.h"


namespace QE
{
	BulletManager::BulletManager(QEngine* engine,String bucket,String name)
		:LogicObject(engine,bucket,name)
	{
		createSlot("fire",fastdelegate::MakeDelegate(this,&BulletManager::fireShot));
	}

	BulletManager::~BulletManager()
	{
		
	}

	void BulletManager::update(double delta)
	{
		for(unsigned int i=0;i<mShots.size();++i)
		{
			if(mShots[i]->isDone())
			{
				mShotsCache.push_back(mShots[i]);
				mShots.erase(mShots.begin()+i);
			}
		}
	}

	void BulletManager::deinitialize()
	{

	}

	void BulletManager::fireShot(Message* msg)
	{
		if(MessageAny<bulletDesc>* message = message_cast<bulletDesc>(msg))
		{
			bulletDesc dsc = message_cast<bulletDesc>(msg)->data;
			Shot* sht = 0;
			for(unsigned int i=0;i<mShotsCache.size();++i)
			{
				if(mShotsCache[i]->getBulletType()==dsc.type)
				{
					sht = mShotsCache[i];
					sht->reactivate(dsc.pos,dsc.dir,dsc.speed,dsc.ori);
					mShotsCache.erase(mShotsCache.begin()+i);
					break;
				}
			}
			if(!sht)
			{
				sht = new Shot(mEngine,"Bullets","Bullet"+StringUtils::toString(nrB),dsc.pos,dsc.dir,dsc.ori,dsc.speed,dsc.type);
				++nrB;
			}
			mShots.push_back(sht);
		}
	}


}