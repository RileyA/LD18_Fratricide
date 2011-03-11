#ifndef SHOT_H
#define SHOT_H

#include "stdafx.h"
#include "QEngine.h"

#include "LogicObject.h"
#include "RenderedObject.h"
#include "PhysicsObject.h"
#include "UGLYGLOBALS.h"


namespace QE
{

	class Shot : public LogicObject
	{
	public:

		Shot(QEngine* engine,String bucket,String name,Vector3 pos,Vector3 dir,Quaternion ori,float speed,int type);

		virtual ~Shot();

		virtual void update(double delta);

		virtual void deinitialize();

		bool isDone(){return done;};

		int getBulletType(){return stype;}

		void reactivate(Vector3 pos,Vector3 dir,float spd,Quaternion q)
		{
			pos.y/=3;
			mObj->setPosition(pos);
			direction = dir;
			direction.y = 0;
			direction.normalize();
			speed = spd*difficulty;
			mObj->setOrientation(q);
			mObj->setVisible(true);done = false;timer=0.f;
		}


	protected:

		RenderedObject* mObj;
		Vector3 direction;
		float speed;
		int stype;
		bool done;
		float timer;
		float raytimer;
		float damage;
		//PhysicsObject* mPhys;

		//float maxSpeed;
		//float minSpeed;
		//float speedGain;
		//float speed;

		//Vector3 lastMove;

	};
}

#endif