#ifndef BULLETMAN_H
#define BULLETMAN_H

#include "stdafx.h"
#include "QEngine.h"

#include "LogicObject.h"
#include "RenderedObject.h"
#include "PhysicsObject.h"
#include "Shot.h"
#include "StringUtils.h"

#include "UGLYGLOBALS.h"

namespace QE
{
	struct bulletDesc
	{
		bulletDesc()
		{
			speed = 5.f;
			type = 0;
			pos = Vector3(0,0,0);
			dir = Vector3(0,0,1);
			ori = Quaternion(0,0,0,1);
		};
		float speed;
		int type;
		Vector3 pos;
		Vector3 dir;
		Quaternion ori;
	};

	class BulletManager : public LogicObject
	{
	public:

		BulletManager(QEngine* engine,String bucket,String name);

		virtual ~BulletManager();

		virtual void update(double delta);

		virtual void deinitialize();

		virtual void fireShot(Message* msg);


	protected:

		std::vector<Shot*> mShots;
		std::vector<Shot*> mShotsCache;

		unsigned int nrB;
		//PhysicsObject* mPhys;

		//float maxSpeed;
		//float minSpeed;
		//float speedGain;
		//float speed;

		//Vector3 lastMove;

	};
}

#endif