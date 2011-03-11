#ifndef ENEMYMAN_H
#define ENEMYMAN_H

#include "stdafx.h"
#include "QEngine.h"

#include "LogicObject.h"
#include "RenderedObject.h"
#include "PhysicsObject.h"
#include "Shot.h"
#include "StringUtils.h"
#include "EnemyShip.h"
#include "UGLYGLOBALS.h"


namespace QE
{

	struct lane
	{
		lane(int nra,float timera)
		{
			nr = nra;
			timer = timera;
		}
		int nr;
		float timer;
	};
	class EnemyGenerator : public LogicObject
	{
	public:

		EnemyGenerator(QEngine* engine,String bucket,String name);

		virtual ~EnemyGenerator();

		virtual void update(double delta);

		virtual void deinitialize();

		void createShip(int lane);

	protected:

		std::vector<EnemyShip*> mShips;
		std::vector<EnemyShip*> mShipsCache;
		std::vector<lane> lanes;

		float timer;

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