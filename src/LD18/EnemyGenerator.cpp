#include "stdafx.h"
#include "QEngine.h"
#include "StaticPhysicsObject.h"
#include "Engine.h"
#include "LogicObject.h"
#include "Logger.h"
#include "QEMath.h"

#include "EnemyGenerator.h"


namespace QE
{
	EnemyGenerator::EnemyGenerator(QEngine* engine,String bucket,String name)
		:LogicObject(engine,bucket,name)
	{
		timer = 0.f;

		for(unsigned int i=0;i<8;++i)
		{
			lanes.push_back(lane(i,randInt(0.f,5.f)));
		}
	}

	EnemyGenerator::~EnemyGenerator()
	{
		
	}

	void EnemyGenerator::update(double delta)
	{
		for(unsigned int i=0;i<mShips.size();++i)
		{
			if(mShips[i]->isDead())
			{
				mShipsCache.push_back(mShips[i]);
				mShips.erase(mShips.begin()+i);
			}
		}

		for(unsigned int i=0;i<8;++i)
		{
			lanes[i].timer-=delta+delta*((difficulty-1)*0.75f);
			if(lanes[i].timer<0)
			{
				lanes[i].timer = 0;
				createShip(i);
			}
		}

		//timer+=delta;
		//if(timer>0.95f)
		//{
		//	createShip();
		//	timer = 0;
		//}
	}

	void EnemyGenerator::deinitialize()
	{

	}

	void EnemyGenerator::createShip(int lane)
	{
		int typee = randInt(0,4);

		bool reuse = false;

		for(unsigned int i=0;i<mShipsCache.size();++i)
		{
			if(mShipsCache[i]->getShipType()==typee)
			{
				reuse = true;
				mShips.push_back(mShipsCache[i]);
				mShipsCache.erase(mShipsCache.begin()+i);
				mShips.back()->reactivate(Vector3(lane*6-21,0,35));
				break;
			}
		}

		if(!reuse)
		{
			mShips.push_back(new EnemyShip(mEngine,"Ships","EnemyShip_"+StringUtils::toString(nrB),typee,Vector3(Vector3(lane*6-21,0,35))));
			mEngine->getLogicManager()->connect("EnemyShip_"+StringUtils::toString(nrB),"fire","BullGen","fire");
			mEngine->getLogicManager()->connect("EnemyShip_"+StringUtils::toString(nrB),"explode","PartGen","addExplosion");
		}

		++nrB;

		lanes[lane].timer = 5.f-(mShips.back()->speed_-10.f)/2.5f;
	}


}