#ifndef LDPLAY_H
#define LDPLAY_H

#include "stdafx.h"
#include "QEngine.h"

#include "GameState.h"
#include "SimpleMenu.h"
#include "PlayerShip.h"

#include "QuantaCCT.h"
#include "BulletManager.h"

namespace QE
{
	//float difficulty = 1.f;

	class LD18PlayState : public GameState 
	{
	public:

		LD18PlayState();
		~LD18PlayState();

		virtual bool update();
		virtual void initialize();
		virtual void processInput();
		virtual void setInputEnabled(bool enable);
		void deinitialize();

		void scoreAdd(Message* msg);

	protected:

		PlayerShip* mPlayer;
		BulletManager* mBG;
		//QuantaCCT* CCTobj;
		//float timer;



		//bool win;
		//bool lose;
		//bool winning;
		//bool losing;

		//unsigned int nrBW;

	};
}

#endif