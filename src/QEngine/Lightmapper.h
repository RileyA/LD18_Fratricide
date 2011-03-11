#ifndef LOGICLIGHTMAPPER_H
#define LOGICLIGHTMAPPER_H

#include "stdafx.h"
#include "QEngine.h"

#include "LogicObject.h"
#include "RenderedObject.h"
#include "PhysicsObject.h"
#include "StaticPhysicsObject.h"

namespace QE
{
	class ScriptManager;

	class Lightmapper : public LogicObject
	{
	public:

		Lightmapper(QEngine* engine,String bucket,String name);

		virtual ~Lightmapper();

		virtual void update(double delta);

		virtual void deinitialize();

		virtual void setObject(StaticPhysicsObject* obj);

		virtual void calculateTexture();

		static void registerClass(ScriptManager* scriptMgr);

		static Lightmapper* toLightmapper(LogicObject* obj);

		static Lightmapper* createLightmapper(QEngine* engine,String bucket,String name);

		// Utilities, these should eventually be stuck into the gfx manager itself??

	protected:

		StaticPhysicsObject* mObject;

	};
}

#endif
