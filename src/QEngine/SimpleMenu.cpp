#include "stdafx.h"
#include "QEngine.h"

#include "Engine.h"
#include "SimpleMenu.h"
#include "StringUtils.h"

namespace QE
{
	SimpleMenu::SimpleMenu(QEngine* engine,String bucket,String name)
		:LogicObject(engine,bucket,name),
		mLeftDown(0),
		mRightDown(0),
		mUpDown(0),
		mDownDown(0),
		mEnterDown(0),
		mStartX(0.1f),
		mIncrX(0.f),
		mStartY(0.3f),
		mIncrY(0.1f),
		mWidth(1.f),
		mHeight(0.09f),
		mSelection(0),
		timer(0.f)
	{
		mInitialized = true;
	}

	SimpleMenu::~SimpleMenu()
	{
		deinitialize();
	}

	void SimpleMenu::update(double delta)
	{
		timer+=delta;
		// Left arrow

		if(mEngine->getInputManager()->isKeyDown("KC_LEFT")&&!mLeftDown)
		{

			mLeftDown = true;

			if(mOptions[mSelection].type==SMOT_INT)
			{
				mOptions[mSelection].data_i-=1;
				if(mOptions[mSelection].data_i<mOptions[mSelection].min_i)
				{
					mOptions[mSelection].data_i = mOptions[mSelection].min_i;
				}
				mOptions[mSelection].text.setCaption(mOptions[mSelection].name+": "+StringUtils::toString(mOptions[mSelection].data_i));

			}
			else if(mOptions[mSelection].type==SMOT_FLOAT)
			{
				mOptions[mSelection].data_f-=0.1f;
				if(mOptions[mSelection].data_f<mOptions[mSelection].min_f)
				{
					mOptions[mSelection].data_f = mOptions[mSelection].min_f;
				}
				mOptions[mSelection].text.setCaption(mOptions[mSelection].name+": "+StringUtils::toString(mOptions[mSelection].data_f));
			}
			else if(mOptions[mSelection].type==SMOT_STRING)
			{
				mOptions[mSelection].data_i--;
				if(mOptions[mSelection].data_i<0)
				{	
					mOptions[mSelection].data_i = mOptions[mSelection].list_s.size()-1;
				}
				mOptions[mSelection].data_s = mOptions[mSelection].list_s[mOptions[mSelection].data_i];
				mOptions[mSelection].text.setCaption(mOptions[mSelection].name+": "+mOptions[mSelection].data_s);
			}
			// do stuffs
		}
		else if(!mEngine->getInputManager()->isKeyDown("KC_LEFT")&&mLeftDown)
		{
			mLeftDown = false;
		}

		// Right Arrow
		if(mEngine->getInputManager()->isKeyDown("KC_RIGHT")&&!mRightDown)
		{
			mRightDown = true;

			if(mOptions[mSelection].type==SMOT_INT)
			{
				mOptions[mSelection].data_i+=1;
				if(mOptions[mSelection].data_i>mOptions[mSelection].max_i)
				{
					mOptions[mSelection].data_i = mOptions[mSelection].max_i;
				}
				mOptions[mSelection].text.setCaption(mOptions[mSelection].name+": "+StringUtils::toString(mOptions[mSelection].data_i));

			}
			else if(mOptions[mSelection].type==SMOT_FLOAT)
			{
				mOptions[mSelection].data_f+=0.1f;
				if(mOptions[mSelection].data_f>mOptions[mSelection].max_f)
				{
					mOptions[mSelection].data_f = mOptions[mSelection].max_f;
				}
				mOptions[mSelection].text.setCaption(mOptions[mSelection].name+": "+StringUtils::toString(mOptions[mSelection].data_f));
			}
			else if(mOptions[mSelection].type==SMOT_STRING)
			{
				mOptions[mSelection].data_i++;
				if(mOptions[mSelection].data_i>=mOptions[mSelection].list_s.size())
				{	
					mOptions[mSelection].data_i = 0;
				}
				mOptions[mSelection].data_s = mOptions[mSelection].list_s[mOptions[mSelection].data_i];
				mOptions[mSelection].text.setCaption(mOptions[mSelection].name+": "+mOptions[mSelection].data_s);
			}
			// do stuffs
		}
		else if(!mEngine->getInputManager()->isKeyDown("KC_RIGHT")&&mRightDown)
		{
			mRightDown = false;
		}

		// Up Arrow
		if(mEngine->getInputManager()->isKeyDown("KC_UP")&&!mUpDown)
		{
			mUpDown = true;

			mOptions[mSelection].text.setColour(Colour(1,1,1,1));


			if(mSelection==0)
			{
				mSelection = mOptions.size()-1;
			}
			else
			{
				mSelection--;
			}

			mOptions[mSelection].text.setColour(Colour(1,0.8f,0.08f));
		}
		else if(!mEngine->getInputManager()->isKeyDown("KC_UP")&&mUpDown)
		{
			mUpDown = false;
		}

		// Down arrow
		if(mEngine->getInputManager()->isKeyDown("KC_DOWN")&&!mDownDown)
		{
			mDownDown = true;

			mOptions[mSelection].text.setColour(Colour(1,1,1,1));

			mSelection++;

			if(mSelection>=mOptions.size())
			{
				mSelection = 0;
			}
			mOptions[mSelection].text.setColour(Colour(1,0.8f,0.08f));
		}
		else if(!mEngine->getInputManager()->isKeyDown("KC_DOWN")&&mDownDown)
		{
			mDownDown = false;
		}

		if(timer>0.5f)
		{
			// Enter Key
			if(mEngine->getInputManager()->isKeyDown("KC_RETURN")&&!mEnterDown)
			{
				mEnterDown = true;
				if(mOptions[mSelection].type==SMOT_BUTTON)
				{
					getSignal(mOptions[mSelection].name).fire(&MessageAny<bool>(true));
				}
			}
			else if(!mEngine->getInputManager()->isKeyDown("KC_RETURN")&&mEnterDown)
			{
				mEnterDown = false;
			}
		}
	}

	void SimpleMenu::deinitialize()
	{
		if(mInitialized)
		{
			mInitialized = false;
		}
	}

	void SimpleMenu::addFloatOption(String name,float val,float min,float max)
	{
		mOptions.push_back(SimpleMenuOption());
		mOptions.back().name = name;
		mOptions.back().data_f = val;
		mOptions.back().max_f = max;
		mOptions.back().min_f = min;
		mOptions.back().type = SMOT_FLOAT;
		mOptions.back().text = mEngine->getGraphicsManager()->getGUIManager()->createStaticText(getName()+"_"+name,
			mStartX+(mOptions.size()-1)*mIncrX,mStartY+(mOptions.size()-1)*mIncrY,mWidth,mHeight,
			name+": "+StringUtils::toString(val));
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText(getName()+"_"+name).setFontName("font_VeraMono.72");
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText(getName()+"_"+name).setColour(Colour(1,1,1,1));
		createSignal(name);

		if(mOptions.size()==1)
		{
			mOptions[0].text.setColour(Colour(1.f,0.8f,0.08f));
		}
	}

	void SimpleMenu::addIntOption(String name,int val,int min,int max)
	{
		mOptions.push_back(SimpleMenuOption());
		mOptions.back().name = name;
		mOptions.back().data_i = val;
		mOptions.back().max_i = max;
		mOptions.back().min_i = min;
		mOptions.back().type = SMOT_INT;
		mOptions.back().text = mEngine->getGraphicsManager()->getGUIManager()->createStaticText(getName()+"_"+name,
			mStartX+(mOptions.size()-1)*mIncrX,mStartY+(mOptions.size()-1)*mIncrY,mWidth,mHeight,
			name+": "+StringUtils::toString(val));
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText(getName()+"_"+name).setFontName("font_VeraMono.72");
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText(getName()+"_"+name).setColour(Colour(1,1,1,1));
		createSignal(name);

		if(mOptions.size()==1)
		{
			mOptions[0].text.setColour(Colour(1.f,0.8f,0.08f));
		}
	}

	void SimpleMenu::addStringOption(String name,std::vector<String> list,int index)
	{
		mOptions.push_back(SimpleMenuOption());
		mOptions.back().name = name;
		mOptions.back().list_s = list;	
		mOptions.back().data_s = list[index];	
		mOptions.back().data_i = index;	
		mOptions.back().type = SMOT_STRING;	
		mOptions.back().text = mEngine->getGraphicsManager()->getGUIManager()->createStaticText(getName()+"_"+name,
			mStartX+(mOptions.size()-1)*mIncrX,mStartY+(mOptions.size()-1)*mIncrY,mWidth,mHeight,
			name+": "+list[index]);
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText(getName()+"_"+name).setFontName("font_VeraMono.72");
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText(getName()+"_"+name).setColour(Colour(1,1,1,1));
		createSignal(name);

		if(mOptions.size()==1)
		{
			mOptions[0].text.setColour(Colour(1.f,0.8f,0.08f));
		}
	}

	void SimpleMenu::addButton(String name)
	{
		mOptions.push_back(SimpleMenuOption());
		mOptions.back().name = name;
		mOptions.back().type = SMOT_BUTTON;	
		mOptions.back().text = mEngine->getGraphicsManager()->getGUIManager()->createStaticText(getName()+"_"+name,
			mStartX+(mOptions.size()-1)*mIncrX,mStartY+(mOptions.size()-1)*mIncrY,mWidth,mHeight,
			name);
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText(getName()+"_"+name).setFontName("font_VeraMono.72");
		mEngine->getGraphicsManager()->getGUIManager()->getStaticText(getName()+"_"+name).setColour(Colour(1,1,1,1));
		createSignal(name);

		if(mOptions.size()==1)
		{
			mOptions[0].text.setColour(Colour(1.f,0.8f,0.08f));
		}
	}

	SimpleMenuOption SimpleMenu::getOption(int index)
	{
		return mOptions[index];
	}

	int SimpleMenu::getSelection(int index)
	{
		return mSelection;
	}


}