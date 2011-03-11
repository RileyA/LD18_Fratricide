#include "stdafx.h"
#include "QEngine.h"
#include "StaticPhysicsObject.h"
#include "Engine.h"
#include "LogicObject.h"
#include "Logger.h"

#include "ParticleManager.h"

namespace QE
{
	ParticleManager::ParticleManager(QEngine* engine,String bucket,String name)
		:LogicObject(engine,bucket,name)
	{
		createSlot("addExplosion",fastdelegate::MakeDelegate(this,&ParticleManager::addExplosion));
	}

	ParticleManager::~ParticleManager()
	{
		if(mInitialized)
		{
			deinitialize();
		}
	}

	void ParticleManager::update(double delta)
	{
		for(unsigned int i=0;i<mSystems.size();++i)
		{
			if(!mSystems[i].sys->isEnabled())
			{
				mSystemsCache.push_back(mSystems[i]);
				mSystems[i].sys->setVisible(false);
				mSystems.erase(mSystems.begin()+i);
			}	
			else
			{
				mSystems[i].sys->setSpeed(TimeManager::getSingletonPtr()->getTimeSpeed());
			}

		}
	}

	void ParticleManager::deinitialize()
	{

	}

	void ParticleManager::addExplosion(Message* msg)
	{
		if(MessageAny<psystemDesc>* message = message_cast<psystemDesc>(msg))
		{
			psystemDesc dsc = message_cast<psystemDesc>(msg)->data;
			String typ;
			if(dsc.type==0)
			{
				typ = "Explosion1";
			}
			else if(dsc.type==1)
			{
				typ = "Explosion2";
			}
			else if(dsc.type==2)
			{
				typ = "Debris1";
			}
			bool reuse = false;
			for(unsigned int i=0;i<mSystemsCache.size();++i)
			{
				if(mSystemsCache[i].type==dsc.type)
				{
					reuse = true;
					mSystems.push_back(mSystemsCache[i]);
					mSystemsCache.erase(mSystemsCache.begin()+i);
					mSystems.back().sys->setEnabled(true);
					mSystems.back().sys->setVisible(true);
					break;
				}
			}
			if(!reuse)
			{
				mSystems.push_back(psystem(mEngine->getGraphicsManager()->createParticleSystem(typ),dsc.type));
				mSystems.back().sys->setPosition(dsc.pos);
				mSystems.back().sys->setEnabled(true);
				mSystems.back().sys->attachToRoot();
			}
		}
	}

}