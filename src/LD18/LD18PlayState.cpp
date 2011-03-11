#include "stdafx.h"
#include "QEngine.h"

#include "LD18PlayState.h"
#include "LD18MenuState.h"
#include "Engine.h"
#include "EnemyGenerator.h"
#include "ParticleManager.h"

#include "StaticPhysicsObject.h"
#include "LogicRigidBody.h"
#include "PlayerShip.h"
#include "EnemyShip.h"
#include "QEMath.h"
#include "StringUtils.h"

namespace QE
{
	LD18PlayState::LD18PlayState()
	{
		srand(time(0));
	}

	LD18PlayState::~LD18PlayState()
	{

	}


	bool LD18PlayState::update()
	{
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText("SCORE").setCaption("Score: "+StringUtils::toString(score));

		//mEngine->getGraphicsManager()->getDefaultCameraManager()->setPosition(mEngine->getGraphicsManager()->getDefaultCameraManager()->getPosition()+Vector3(0,mEngine->getTimeManager()->getDeltaTime(),0));
		return true;
	}

	void LD18PlayState::initialize()
	{
		score =0;
		mIsStarted = true;
		mEngine->getTimeManager()->setTimeSpeed(1.f);
		mEngine->getGraphicsManager()->getGUIManager()->setCursorVisible(false);
		mEngine->getGraphicsManager()->setBackgroundColour(Colour(0,0,0));
		mEngine->getPhysicsManager()->setupPhysX();
		mEngine->getPhysicsManager()->setGravity(Vector3(0,0.f,0));

		mEngine->getLogicManager()->createBucket("Ships");
		mEngine->getLogicManager()->createBucket("Bullets");
		mEngine->getLogicManager()->createBucket("Generators");

		mPlayer = new PlayerShip(mEngine,"Ships","PLSHIP","Ship01.mesh");

		mBG = new BulletManager(mEngine,"Generators","BullGen");
		new EnemyGenerator(mEngine,"Generators","EnGen");
		new ParticleManager(mEngine,"Generators","PartGen");

		mEngine->getGraphicsManager()->createRenderedObject("Stars.mesh")->attachToRoot();

		mInputEnabled = true;

		mEngine->getGraphicsManager()->getDefaultCameraManager()->lockMotion();
		mEngine->getGraphicsManager()->getDefaultCameraManager()->lockRotation();
		mEngine->getGraphicsManager()->getDefaultCameraManager()->setPosition(Vector3(0,40.f,0));

		mEngine->getGraphicsManager()->getGUIManager()->createStaticText("SCORE",0.1f,0.1f,0.7f,0.3f,"Score: 0");
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText("SCORE").setFontName("font_VeraMono.72");
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText("SCORE").setColour(Colour(1,1,1,1));

		//createSlot("addScore",fasdelegate::MakeDelegate(this,&LD18PlayState::addScore));
	}

	void LD18PlayState::processInput()
	{

	}

	void LD18PlayState::setInputEnabled(bool enable)
	{
		mInputEnabled = enable;
	}

	void LD18PlayState::deinitialize()
	{
		mEngine->getPhysicsManager()->ReleasePhysX();
		mIsStarted = false;
	}

	void LD18PlayState::scoreAdd(Message* msg)
	{
		if(MessageAny<int>* message = message_cast<int>(msg))
		{
			score+=message_cast<int>(msg)->data;
		}
	}


}