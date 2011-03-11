#ifndef PARTICLES_H
#define PARTICLES_H

#include "stdafx.h"
#include "QEngine.h"

#include "LogicObject.h"
#include "RenderedObject.h"
#include "PhysicsObject.h"
#include "ParticleSystem.h"

namespace QE
{

	struct psystemDesc
	{
		psystemDesc()
		{
			type = 0;
			pos = Vector3::ZERO;
		}
		psystemDesc(Vector3 v3,int tp)
		{
			type = tp;
			pos = v3;
		}

		Vector3 pos;
		int type;
	};

	struct psystem
	{
		psystem(ParticleSystem* s,int tp)
		{	
			sys = s;
			type = tp;
		}
		ParticleSystem* sys;
		int type;
	};

	class ParticleManager : public LogicObject
	{
	public:

		ParticleManager(QEngine* engine,String bucket,String name);

		virtual ~ParticleManager();

		virtual void update(double delta);

		virtual void deinitialize();

		void addExplosion(Message* msg);

	protected:

		std::vector<psystem> mSystems;
		std::vector<psystem> mSystemsCache;


	};
}

#endif