#include "stdafx.h"
#include "QEngine.h"

#include "LD18PlayState.h"
#include "LD18MenuState.h"
#include "GameOverState.h"
#include "Engine.h"

namespace QE
{
	GameOverState::GameOverState()
	{

	}

	GameOverState::~GameOverState()
	{

	}


	bool GameOverState::update()
	{
		return true;
	}

	void GameOverState::initialize()
	{
	    TimeManager::getSingletonPtr()->setTimeSpeed(1.f);
		mIsStarted = true;
		mEngine->getGraphicsManager()->getGUIManager()->setCursorVisible(false);
		mEngine->getGraphicsManager()->setBackgroundColour(Colour(0.05,0.05,0.05));

		mEngine->getGraphicsManager()->getGUIManager()->createStaticText("GAMEOVER",0.f,0.03f,1.f,0.2,"GAME OVER!");
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText("GAMEOVER").center();
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText("GAMEOVER").setFontName("font_VeraMono.72");
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText("GAMEOVER").setColour(Colour(1,1,1,1));

		mEngine->getGraphicsManager()->getGUIManager()->createStaticText("SCORE",0.54f,0.4f,0.7f,0.3f,"Score: "+StringUtils::toString(score));
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText("SCORE").setFontName("font_VeraMono.72");
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText("SCORE").setColour(Colour(1,1,1,1));

		mMenu = new SimpleMenu(mEngine,"DefaultTemp","GAMEOVERMENU");
		mMenu->addButton("Play Again!");
		mMenu->addFloatOption("Difficulty",difficulty,1.0f,2.f);
		mMenu->addButton("Main Menu");

		createSlot("Play",fastdelegate::MakeDelegate(this,&GameOverState::play));
		createSlot("Quit",fastdelegate::MakeDelegate(this,&GameOverState::quit));

		mEngine->getLogicManager()->connect(getSlot("Play"),mMenu->getSignal("Play Again!"));
		mEngine->getLogicManager()->connect(getSlot("Quit"),mMenu->getSignal("Main Menu"));

		mEngine->getGraphicsManager()->getDefaultCameraManager()->lockMotion();
		mEngine->getGraphicsManager()->getDefaultCameraManager()->lockRotation();
	}

	void GameOverState::processInput()
	{

	}

	void GameOverState::setInputEnabled(bool enable)
	{
		mInputEnabled = enable;
	}

	void GameOverState::deinitialize()
	{
		mIsStarted = false;
	}

	void GameOverState::play(Message* msg)
	{
		difficulty = mMenu->getOption(1).data_f;
		mEngine->addState(new LD18PlayState());
		mEngine->endCurrentState();
	}

	void GameOverState::quit(Message* msg)
	{
		difficulty = mMenu->getOption(1).data_f;
		mEngine->addState(new LD18MenuState());
		mEngine->endCurrentState();
	}

}
