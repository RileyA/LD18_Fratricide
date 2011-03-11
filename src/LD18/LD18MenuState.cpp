#include "stdafx.h"
#include "QEngine.h"

#include "LD18PlayState.h"
#include "LD18MenuState.h"
#include "Engine.h"

namespace QE
{
	LD18MenuState::LD18MenuState()
	{

	}

	LD18MenuState::~LD18MenuState()
	{

	}


	bool LD18MenuState::update()
	{
		return true;
	}

	void LD18MenuState::initialize()
	{
	    TimeManager::getSingletonPtr()->setTimeSpeed(1.f);
		mIsStarted = true;
		mEngine->getGraphicsManager()->getGUIManager()->setCursorVisible(false);
		mEngine->getGraphicsManager()->setBackgroundColour(Colour(0.05,0.05,0.05));

		mEngine->getGraphicsManager()->getGUIManager()->createStaticText("Title",0.f,0.03f,1.f,0.2,"Fratricide");
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText("Title").center();
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText("Title").setFontName("font_VeraMono.72");
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText("Title").setColour(Colour(1,1,1,1));

		mMenu = new SimpleMenu(mEngine,"DefaultTemp","LD18MainMenu");
		mMenu->addButton("Play");
		mMenu->addFloatOption("Difficulty",difficulty,1.f,2.f);
		std::vector<String> vals;
		vals.push_back("Arrows");
		vals.push_back("WASD");
		vals.push_back(",AOE");
		mMenu->addStringOption("Movement Controls",vals,moveControl);
		std::vector<String> vals2;
		vals2.push_back("Z and X");
		vals2.push_back("Right Shift and Ctrl");
		vals2.push_back("; and Q");
		mMenu->addStringOption("Action Controls",vals2,actControl);
		mMenu->addButton("Quit");

		createSlot("Play",fastdelegate::MakeDelegate(this,&LD18MenuState::play));
		createSlot("Quit",fastdelegate::MakeDelegate(this,&LD18MenuState::quit));

		mEngine->getLogicManager()->connect(getSlot("Play"),mMenu->getSignal("Play"));
		mEngine->getLogicManager()->connect(getSlot("Quit"),mMenu->getSignal("Quit"));

		mEngine->getGraphicsManager()->getDefaultCameraManager()->lockMotion();
		mEngine->getGraphicsManager()->getDefaultCameraManager()->lockRotation();
	}

	void LD18MenuState::processInput()
	{

	}

	void LD18MenuState::setInputEnabled(bool enable)
	{
		mInputEnabled = enable;
	}

	void LD18MenuState::deinitialize()
	{
		mIsStarted = false;
	}

	void LD18MenuState::play(Message* msg)
	{
		moveControl = mMenu->getOption(2).data_i;
		actControl = mMenu->getOption(3).data_i;
		difficulty = mMenu->getOption(1).data_f;
		mEngine->addState(new LD18PlayState());
		mEngine->endCurrentState();
	}

	void LD18MenuState::quit(Message* msg)
	{
		mEngine->shutdown();
	}

}
