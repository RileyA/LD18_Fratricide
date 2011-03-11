#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "stdafx.h"
#include "QEngine.h"

#include "LogicObject.h"
#include "RenderedObject.h"
#include "PhysicsObject.h"
#include "PhysicsManager.h"
#include "UGLYGLOBALS.h"


namespace QE
{
	class EnemyShip;

	class PlayerShip : public LogicObject
	{
	public:

		PlayerShip(QEngine* engine,String bucket,String name,String mesh);

		virtual ~PlayerShip();

		virtual void update(double delta);

		virtual void deinitialize();

		void hit(float damage);


	protected:

		float health;

		RenderedObject* mObj;
		RenderedObject* push;
		RenderedObject* pull;
		PhysicsObject* mPhys;
		PhysicsObject* mPhys2;

		float maxSpeed;
		float minSpeed;
		float speedGain;
		float speed;

		bool spcdn;

		Vector3 lastMove;

		float roll;

		SimpleUserData data;

		bool zdn;
		bool xdn;

		bool pushing;
		float pushTime;

		bool pulling;
		float pulltime;
		float pullvtime;

		EnemyShip* aubductee;

	};
}

#endif