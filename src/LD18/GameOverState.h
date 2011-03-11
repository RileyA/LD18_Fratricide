#ifndef GAMEOVERSTATE
#define GAMEOVERSTATE

#include "stdafx.h"
#include "QEngine.h"

#include "GameState.h"
#include "SimpleMenu.h"

namespace QE
{
	class GameOverState : public GameState 
	{
	public:

		GameOverState();
		~GameOverState();

		virtual bool update();
		virtual void initialize();
		virtual void processInput();
		virtual void setInputEnabled(bool enable);
		void deinitialize();

		void play(Message* msg);
		void quit(Message* msg);

	protected:

		SimpleMenu* mMenu;
		//float timer;
		//StaticText mText;
		//time_t start;

	};
}

#endif