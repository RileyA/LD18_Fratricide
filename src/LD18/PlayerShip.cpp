#include "stdafx.h"
#include "QEngine.h"
#include "StaticPhysicsObject.h"
#include "Engine.h"
#include "LogicObject.h"
#include "Logger.h"

#include "PlayerShip.h"
#include "EnemyShip.h"
#include "BulletManager.h"
#include "GameOverState.h"


#include "PhysicsMasks.h"


namespace QE
{
	PlayerShip::PlayerShip(QEngine* engine,String bucket,String name,String mesh)
		:LogicObject(engine,bucket,name)
	{
		health = 3.f;
		mObj = mEngine->getGraphicsManager()->createRenderedObject("PlayerShip.mesh");
		push = mEngine->getGraphicsManager()->createRenderedObject("Push.mesh");
		//ParticleSystem* sys = mEngine->getGraphicsManager()->createParticleSystem("Examples/Smoke");
		//mObj->attachObject(sys);
		//sys->setPosition(Vector3(0,0,-4.65f));
		push->setPosition(Vector3(0,-0.235,-0.9f));
		push->setVisible(false);
		mObj->attachToRoot();
		//push->attachToRoot();
		mObj->attachObject(push);
		pull = mEngine->getGraphicsManager()->createRenderedObject("Pull2.mesh");
		mObj->attachObject(pull);
		pull->setVisible(false);

		//mPhys = mEngine->getPhysicsManager()->createSphere(0.5f,Vector3(0,0,0));
		mPhys = mEngine->getPhysicsManager()->createConvexHull("PlayerShipC.mesh",Vector3(0,0,0));
		mPhys->set2D(true);
		mPhys->setAngularFactor(Vector3(0,0,0));
		mPhys->wakeup();


		mPhys2 = mEngine->getPhysicsManager()->createConvexHull("PlayerShipCC.mesh",Vector3(0,0,0));
		mPhys2->setAngularFactor(Vector3(0,0,0));
		mPhys2->wakeup();
		mPhys2->set2D(true);
		mPhys2->setKinematic(true);
		mPhys2->setCollisionGroup(COL_PLAYER);
		mPhys2->setCollisionMask(COL_STATICS);

		speed = 0;
		maxSpeed = 27.5;
		minSpeed = 15.0;
		speedGain = 25.f;
		roll = 0;
		pushing = 0;
		pushTime = 0.f;
		mPhys->setMass(4.f);
		mPhys->setCollisionGroup(COL_DYNAMICS);
		mPhys->setCollisionMask(COL_DYNAMICS);

		data = SimpleUserData((void*)this,"PlayerShip");
		mPhys->setUserData(&data);
		mPhys2->setUserData(&data);

		 pulling = false;
		 pulltime = 0;
		 pullvtime = 0;
		aubductee  =0;

		spcdn = false;
		zdn = 0;
		xdn = 0;

		createSignal("fire");
	}

	PlayerShip::~PlayerShip()
	{
		if(mInitialized)
		{
			deinitialize();
		}
	}

	void PlayerShip::update(double delta)
	{

		float rollfact = 0.f;
		if(aubductee)
		{
			if(aubductee->isDead())
			{
				aubductee = 0;
			}
		}

		health+=delta/6;
		if(health>3.f)
		{
			health = 3.f;
		}

		mObj->getMaterial(0).setAmbient(Colour((3.f-health)/3.f,0,health/3.f));

		mObj->setPosition(mPhys->getPosition());
		mObj->setOrientation(mPhys->getOrientation());// AABB assert issue here

		mPhys2->setPosition(mPhys->getPosition());
		mPhys2->setOrientation(mPhys->getOrientation());

		Vector3 moveV = Vector3::ZERO;

		
		///////////////////////////////////////////////////////////////////////////////
		////
		//// MOTION
		////
		///////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////
		////
		//// ARROW KEYS - MOVE
		////
		////////////////////////////////////////////////////////
		if(moveControl==0)
		{
			if(mEngine->getInputManager()->isKeyDown("KC_UP"))
			{
				moveV+=Vector3(0,0,1);
			}
			if(mEngine->getInputManager()->isKeyDown("KC_DOWN"))
			{
				moveV+=Vector3(0,0,-1);
			}
			if(mEngine->getInputManager()->isKeyDown("KC_LEFT"))
			{
				moveV+=Vector3(1,0,0);
				rollfact += -delta*45;
				if(roll>0)
				{
					rollfact+=-delta*45;
				}
			}
			if(mEngine->getInputManager()->isKeyDown("KC_RIGHT"))
			{
				moveV+=Vector3(-1,0,0);
				rollfact += delta*45;
				if(roll<0)
				{
					rollfact+=-delta*45;
				}
			}

			moveV.normalize();
			moveV*=20.f;
			mPhys->setVelocity(moveV);
		}
		/////////////////////////////////////////////////////////
		////
		//// MOUSE - MOVE
		////
		////////////////////////////////////////////////////////
		else if(moveControl==10)
		{
			float xMove = mEngine->getInputManager()->getMouseX();
			float yMove = mEngine->getInputManager()->getMouseY();

			std::cout<<xMove<<" "<<yMove<<"\n";

			moveV = Vector3(-xMove*3,0,-yMove*3);
			//if(moveV.length()>25.f)
			//{
			//	moveV.normalize();
			//	moveV*=25.f;
			//}
		}
		/////////////////////////////////////////////////////////
		////
		//// WASD - MOVE
		////
		////////////////////////////////////////////////////////
		else if(moveControl==1)
		{
			if(mEngine->getInputManager()->isKeyDown("KC_W"))
			{
				moveV+=Vector3(0,0,1);
			}
			if(mEngine->getInputManager()->isKeyDown("KC_S"))
			{
				moveV+=Vector3(0,0,-1);
			}
			if(mEngine->getInputManager()->isKeyDown("KC_A"))
			{
				moveV+=Vector3(1,0,0);
				rollfact += -delta*45;
				if(roll>0)
				{
					rollfact+=-delta*45;
				}
			}
			if(mEngine->getInputManager()->isKeyDown("KC_D"))
			{
				moveV+=Vector3(-1,0,0);
				rollfact += delta*45;
				if(roll<0)
				{
					rollfact+=-delta*45;
				}
			}

			moveV.normalize();
			moveV*=20.f;
		}
		/////////////////////////////////////////////////////////
		////
		//// ,AOE - MOVE (DVORAK WASD)
		////
		////////////////////////////////////////////////////////
		else if(moveControl==2)
		{
			if(mEngine->getInputManager()->isKeyDown("KC_COMMA"))
			{
				moveV+=Vector3(0,0,1);
			}
			if(mEngine->getInputManager()->isKeyDown("KC_O"))
			{
				moveV+=Vector3(0,0,-1);
			}
			if(mEngine->getInputManager()->isKeyDown("KC_A"))
			{
				moveV+=Vector3(1,0,0);
				rollfact += -delta*45;
				if(roll>0)
				{
					rollfact+=-delta*45;
				}
			}
			if(mEngine->getInputManager()->isKeyDown("KC_E"))
			{
				moveV+=Vector3(-1,0,0);
				rollfact += delta*45;
				if(roll<0)
				{
					rollfact+=-delta*45;
				}
			}

			moveV.normalize();
			moveV*=20.f;
		}

		if(moveV.length()!=0.f)
		{
			if(mPhys->getPosition().x>25&&moveV.x>0)
			{
				moveV.x = 0;
			}
			else if(mPhys->getPosition().x<-25&&moveV.x<0)
			{
				moveV.x = 0;
			}

			if(mPhys->getPosition().z>30&&moveV.z>0)
			{
				moveV.z = 0;
			}
			else if(mPhys->getPosition().z<-10&&moveV.z<0)
			{
				moveV.z = 0;
			}
		}

		if(rollfact==0.f)
		{
			if(roll<0)
			{
				rollfact = delta*60;
				if(roll+rollfact>0)
				{
					rollfact = -roll;
				}
			}
			else if(roll>0)
			{
				rollfact = -delta*60;
				if(roll+rollfact<0)
				{
					rollfact = -roll;
				}
			}
		}


		if(abs(roll+rollfact)<=20.f)
		{
			roll+=rollfact;
			mObj->roll(roll*1.5f);
			//mObj->yaw(-roll);
		}
		else if(abs(roll+rollfact)>20.f)
		{
			if(rollfact>0.f)
			{
				roll = 20.f;
			}
			else
			{
				roll = -20.f;
			}
			mObj->roll(roll*1.5f);
			//mObj->yaw(-roll);
		}


		float mod = 1.f;
		if(aubductee)
		{
			mod = aubductee->speed_/20.f;
		}
		mPhys->setVelocity(moveV*mod);

		///////////////////////////////////////////////////////////////////////////////
		////
		//// END MOTION
		////
		///////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////
		////
		//// ACTION
		////
		///////////////////////////////////////////////////////////////////////////////

		bool mouseControl = false;
		String pushName = "KC_Z";
		String pullName = "KC_X";

		/////////////////////////////////////////////////////////
		////
		//// Z - Pull/Shoot
		//// X - Force Push
		////
		////////////////////////////////////////////////////////
		if(actControl==0)
		{
			mouseControl = false;
			pushName = "KC_Z";
			pullName = "KC_X";
		}
		/////////////////////////////////////////////////////////
		////
		//// LMB - Pull/Shoot
		//// RMB - Force Push
		////
		////////////////////////////////////////////////////////
		else if(actControl==10)
		{
			mouseControl = true;
			pushName = "MB_Left";
			pullName = "MB_Right";
		}
		/////////////////////////////////////////////////////////
		////
		//// RSHIFT - Pull/Shoot
		//// RCTRL - Force Push
		////
		////////////////////////////////////////////////////////
		else if(actControl==1)
		{
			mouseControl = false;
			pushName = "KC_RSHIFT";
			pullName = "KC_RCONTROL";
		}
		/////////////////////////////////////////////////////////
		////
		//// ; - Pull/Shoot
		//// Q - Force Push
		////
		////////////////////////////////////////////////////////
		else if(actControl==2)
		{
			mouseControl = false;
			pushName = "KC_SEMICOLON";
			pullName = "KC_Q";
		}

		////////////////////////////////////////////////////////////////
		////
		//// Do the actual stuff
		////
		///////////////////////////////////////////////////////////////
	
		// Space Always Shoots
		if(mEngine->getInputManager()->isKeyDown("KC_SPACE")&&!spcdn)
		{
			spcdn = true;
			if(aubductee)
			{
				aubductee->shoot();
			}
		}
		else if(!mEngine->getInputManager()->isKeyDown("KC_SPACE"))
		{
			spcdn = false;
		}

		// Poll the input
		bool pullD = false;
		bool pushD = false;
		if(mouseControl)
		{
			pullD = mEngine->getInputManager()->isMouseButtonDown(pullName);
			pushD = mEngine->getInputManager()->isMouseButtonDown(pushName);
		}
		else
		{
			pullD = mEngine->getInputManager()->isKeyDown(pullName);
			pushD = mEngine->getInputManager()->isKeyDown(pushName);
		}

		// Pull
		if(pullD&&!zdn)
		{
			zdn = true;

			if(!aubductee)
			{
				pushing = true;
				pushTime = 0.25f;
				mEngine->getAudioManager()->play2D("push.wav");
			}
			else
			{
				aubductee->release(mObj->getPosition()+mObj->getOrientation()*Vector3(0,0,5.f),mObj->getOrientation(),mObj->getOrientation()*Vector3(0,0,30.f));
				aubductee->setWeapons(false);
				aubductee = 0;
			}
		}
		else if(!pullD)
		{
			zdn = false;
		}

		// Push
		if(pushD&&!xdn)
		{
			xdn = true;

			if(!aubductee)
			{
				pullvtime = 0.2f;
				pull->setVisible(true);
				pulling = true;
				pulltime = 0.2f;
			}

		}
		else if(!pushD)
		{
			xdn = false;
		}

		///////////////////////////////////////////////////////////////////////////////
		////
		//// END ACTION
		////
		///////////////////////////////////////////////////////////////////////////////


		pullvtime-=delta;
		if(pullvtime<0.f)
		{
			pullvtime = 0.f;
			pull->setVisible(false);
		}

		if(pulling&&pulltime>0.f)
		{
			pulltime-=delta;
			std::vector<void*> userD = mEngine->getPhysicsManager()->sphereOverlap(1.5,mObj->getPosition()+mObj->getOrientation()*Vector3(0,0,0.9f),COL_STATICS,COL_ENEMY);
			if(userD.size()>0)
			{
				if(((SimpleUserData*)userD[0])->type=="EnemyShip")
				{
					aubductee = (EnemyShip*)((SimpleUserData*)userD[0])->data;
					aubductee->aubduct(mObj);
					pulling = false;
					pulltime = 0.f;
				}
			}
		}

		if(xdn&&aubductee)
		{
			aubductee->shoot();
		}

		//if(rollfact==0.f)
		//{
		//	if(roll<0)
		//	{
		//		rollfact = delta*60;
		//		if(roll+rollfact>0)
		//		{
		//			rollfact = -roll;
		//		}
		//	}
		//	else if(roll>0)
		//	{
		//		rollfact = -delta*60;
		//		if(roll+rollfact<0)
		//		{
		//			rollfact = -roll;
		//		}
		//	}
		//}


		//if(abs(roll+rollfact)<=20.f)
		//{
		//	roll+=rollfact;
		//	mObj->roll(roll*1.5f);
		//	//mObj->yaw(-roll);
		//}
		//else if(abs(roll+rollfact)>20.f)
		//{
		//	if(rollfact>0.f)
		//	{
		//		roll = 20.f;
		//	}
		//	else
		//	{
		//		roll = -20.f;
		//	}
		//	mObj->roll(roll*1.5f);
		//	//mObj->yaw(-roll);
		//}

		//	if(speed<minSpeed)
		//	{
		//		speed = minSpeed;
		//	}
		//	//moveV.normalize();
		//	//if(moveV!=lastMove)
		//	//{
		//	//	speed*=1-(moveV-lastMove).length();
		//	//}
		//	//speed+=delta*speedGain;
		//	//if(speed>maxSpeed)
		//	//{
		//	//	speed = maxSpeed;
		//	//}

		//	mPhys->setVelocity(moveV);//*speed);
		//	lastMove = moveV;
		//}
		//else
		//{
		//	speed-=delta*60;
		//	if(speed<0)
		//	{
		//		speed =0.f;
		//	}
		//	//mPhys->setVelocity(lastMove*speed);

		//}

		if(pushing&&pushTime>0.f)
		{
			push->setVisible(true);
			float rad = (0.26f-pushTime)*4*8;
			push->setScale(Vector3(rad,rad,rad));
			pushTime-=delta;
			std::vector<void*> userD = mEngine->getPhysicsManager()->sphereOverlap(rad,mObj->getPosition(),COL_STATICS,COL_ENEMY);
			for(unsigned int i=0;i<userD.size();++i)
			{
				if(userD[i])
				{
					SimpleUserData* d = (SimpleUserData*)userD[i];
					if(d->type=="EnemyShip")
					{
						EnemyShip* shp = (EnemyShip*)d->data;
						shp->pushed(mObj->getPosition()-mObj->getOrientation()*Vector3(0,0,0.9f),delta*15000.f*pushTime);
					}
				}
			}
		}
		else
		{
			push->setVisible(false);
		}

	}

	void PlayerShip::deinitialize()
	{

	}

	void PlayerShip::hit(float damage)
	{
		health-=damage*difficulty;
		if(health<0)
		{
			// game over screen
			mEngine->addState(new GameOverState());
			mEngine->endCurrentState();
			//mEngine->shutdown();
		}
	}



}