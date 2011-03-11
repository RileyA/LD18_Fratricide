#ifndef SIMPLEMENU_H
#define SIMPLEMENU_H

#include "stdafx.h"
#include "QEngine.h"
#include "GUIManager.h"
#include "LogicObject.h"

namespace QE
{
	enum QENGINE_API SimpleMenuOptionType
	{
		SMOT_FLOAT,
		SMOT_INT,
		SMOT_STRING,
		SMOT_BUTTON
	};

	struct QENGINE_API SimpleMenuOption
	{
		String name;
		// these are the only possible types and i don't feel 
		// like doing some fancy template-y stuff for something this simple...
		float data_f;
		float max_f;
		float min_f;

		int data_i;
		int max_i;
		int min_i;

		String data_s;
		std::vector<String> list_s;
		SimpleMenuOptionType type;

		StaticText text;
	};

	class QENGINE_API SimpleMenu : public LogicObject
	{
	public:

		SimpleMenu(QEngine* engine,String bucket,String name);

		virtual ~SimpleMenu();

		virtual void update(double delta);

		virtual void deinitialize();

		virtual void addFloatOption(String name,float val,float min,float max);
		virtual void addIntOption(String name,int val,int min,int max);
		virtual void addStringOption(String name,std::vector<String> list,int index=0);
		virtual void addButton(String name);

		SimpleMenuOption getOption(int index);
		int getSelection(int index);

	protected:

		std::vector<SimpleMenuOption> mOptions;

		bool mLeftDown;
		bool mRightDown;
		bool mUpDown;
		bool mDownDown;
		bool mEnterDown;

		float mStartX;
		float mIncrX;

		float mStartY;
		float mIncrY;

		float mWidth;
		float mHeight;

		unsigned int mSelection;

		float timer;

	};
}

#endif