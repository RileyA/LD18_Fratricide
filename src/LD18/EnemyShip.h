#ifndef ENEMYSHIP_H
#define ENEMYSHIP_H

#include "stdafx.h"
#include "QEngine.h"

#include "LogicObject.h"
#include "RenderedObject.h"
#include "PhysicsObject.h"

#include "PhysicsManager.h"
#include "UGLYGLOBALS.h"


namespace QE
{
	struct gun
	{
		gun(Vector3 pos,int tp,float spd)
		{
			relativePos = pos;
			type = tp;
			speed = spd;
		}
		Vector3 relativePos;
		int type;
		float speed;
	};

	class EnemyShip : public LogicObject
	{
	public:

		EnemyShip(QEngine* engine,String bucket,String name,int type,Vector3 pos);

		virtual ~EnemyShip();

		virtual void update(double delta);

		virtual void deinitialize();

		virtual void shoot();

		virtual void pushed(Vector3 oPos,float power);

		virtual void aubduct(SceneNode* node);
		virtual void release(Vector3 newPos,Quaternion ori,Vector3 veloc);

		virtual void hit(float damage);

		bool isDead(){return dead;};

		bool getWeapons(){return weapons;};
		void setWeapons(bool w){weapons = w;};

		void reactivate(Vector3 pos);

		int getShipType(){return stp;}

		float speed_;

		bool aubducted;

	protected:


		RenderedObject* mObj;
		PhysicsObject* mPhys;
		PhysicsObject* mPhysCore;

		float maxSpeed;
		float minSpeed;
		float speedGain;
		float speed;

		int stp;

		float timer;

		bool spcdn;

		Vector3 lastMove;

		float roll;

		std::vector<gun> mGuns;
		float weaponCooloff;
		float cooltime;

		SimpleUserData data;


		float health;
		bool dead;

		bool weapons;

	};
}

#endif