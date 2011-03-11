#ifndef LD18MENUSTATE
#define LD18MENUSTATE

#include "stdafx.h"
#include "QEngine.h"

#include "GameState.h"
#include "SimpleMenu.h"

namespace QE
{
	class LD18MenuState : public GameState 
	{
	public:

		LD18MenuState();
		~LD18MenuState();

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