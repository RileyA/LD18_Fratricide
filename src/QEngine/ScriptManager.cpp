// Copyright Riley Adams 2010

// This file is part of QEngine.

// QEngine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// QEngine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with QEngine.  If not, see <http://www.gnu.org/licenses/>.


#include "stdafx.h"
#include "QEngine.h"
#include "Engine.h"
#include "ScriptManager.h"
#include "InputManager.h"

#include "StaticPhysicsObject.h"
#include "LogicRigidBody.h"
#include "Lightmapper.h"

#include "ChaiWrapper.h"

namespace QE
{
	ScriptManager::ScriptManager(QEngine* engine)
		:mInitialized(false)
	{
		mEngine = engine;
	}

	ScriptManager::~ScriptManager()
	{
		if(mInitialized)
		{
			deinitialize();
		}
	}

	void ScriptManager::initialize()
	{
		mWrapper = new ChaiWrapper();
		_registerEngineTypes();
		mInitialized = true;
	}

	void ScriptManager::deinitialize()
	{
		if(mInitialized)
		{
			delete mWrapper;
			mWrapper = 0;
			mInitialized = false;
		}
	}

	void ScriptManager::runScript(String filename)
	{
		try
		{
			mWrapper->getChai()->eval_file(filename);
		}
		catch(chaiscript::Eval_Error e)
		{
			std::cout<<e.what()<<"\n";
		}
	}

	void ScriptManager::runString(std::string script)
	{
		try
		{
			mWrapper->getChai()->eval(script);
		}
		catch(chaiscript::Eval_Error e)
		{
			std::cout<<e.what()<<"\n";
		}
	}

	void ScriptManager::_registerEngineTypes()
	{

		#pragma warning( disable: 4244 )


		mWrapper->getChai()->add(chaiscript::fun(static_cast<std::string (*)(int)>(&ScriptManager::toString)), "toString");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<std::string (*)(float)>(&ScriptManager::toString)), "toString");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<std::string (*)(bool)>(&ScriptManager::toString)), "toString");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<std::string (*)(std::string)>(&ScriptManager::toString)), "toString");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<std::string (*)(char)>(&ScriptManager::toString)), "toString");

		mWrapper->getChai()->add(chaiscript::user_type<GraphicsManager>(), "GraphicsManager");
		mWrapper->getChai()->add(chaiscript::bootstrap::copy_constructor<GraphicsManager>("GraphicsManager"));
		mWrapper->getChai()->add(chaiscript::fun(&GraphicsManager::setTextureFiltering), "setTextureFiltering");
		mWrapper->getChai()->add(chaiscript::fun(&GraphicsManager::takeScreenshot), "takeScreenshot");

		mWrapper->getChai()->add(chaiscript::user_type<PhysicsManager>(), "PhysicsManager");
		mWrapper->getChai()->add(chaiscript::bootstrap::copy_constructor<PhysicsManager>("PhysicsManager"));

		mWrapper->getChai()->add(chaiscript::user_type<LogicObject>(), "LogicObject");
		mWrapper->getChai()->add(chaiscript::fun(&LogicObject::getName), "getName");
		mWrapper->getChai()->add(chaiscript::fun(&LogicObject::getType), "getType");
		mWrapper->getChai()->add(chaiscript::fun(&LogicObject::_kill), "_kill");

		mWrapper->getChai()->add(chaiscript::user_type<LogicManager>(), "LogicManager");
		mWrapper->getChai()->add(chaiscript::bootstrap::copy_constructor<LogicManager>("LogicManager"));
		mWrapper->getChai()->add(chaiscript::fun(&LogicManager::getObject), "getObject");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<bool (LogicManager::*)(const std::string&,const std::string&,const std::string&,const std::string&)>(&LogicManager::connect)), "connect");

		mWrapper->getChai()->add(chaiscript::user_type<InputManager>(), "InputManager");
		mWrapper->getChai()->add(chaiscript::bootstrap::copy_constructor<InputManager>("InputManager"));

		mWrapper->getChai()->add(chaiscript::fun(static_cast<bool (InputManager::*)(const std::string&)>(&InputManager::isKeyDown)), "isKeyDown");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<bool (InputManager::*)(const unsigned int&)>(&InputManager::isKeyDown)), "isKeyDown");

		mWrapper->getChai()->add(chaiscript::user_type<QEngine>(), "QEngine");
		mWrapper->getChai()->add(chaiscript::bootstrap::copy_constructor<QEngine>("QEngine"));
		mWrapper->getChai()->add(chaiscript::fun(&QEngine::getGraphicsManager), "getGraphicsManager");
		mWrapper->getChai()->add(chaiscript::fun(&QEngine::getPhysicsManager), "getPhysicsManager");
		mWrapper->getChai()->add(chaiscript::fun(&QEngine::getInputManager), "getInputManager");
		mWrapper->getChai()->add(chaiscript::fun(&QEngine::getLogicManager), "getLogicManager");
		mWrapper->getChai()->add(chaiscript::fun(&QEngine::endCurrentState), "endCurrentState");

		mWrapper->getChai()->add(chaiscript::fun(&UglyHack::getEngine), "getEngine");

		// Vector3's
		mWrapper->getChai()->add(chaiscript::user_type<Vector3>(), "Vector3");
		// Bind the (relevant) constructors
		mWrapper->getChai()->add(chaiscript::bootstrap::copy_constructor<Vector3>("Vector3"));
		mWrapper->getChai()->add(chaiscript::constructor<Vector3 (const Real,const Real,const Real)>(), "Vector3");
		mWrapper->getChai()->add(chaiscript::constructor<Vector3 (const int,const int,const int)>(), "Vector3");
		//mWrapper->getChai()->add(chaiscript::constructor<Vector3 (const Vector3)>(), "Vector3");
		mWrapper->getChai()->add(chaiscript::constructor<Vector3 ()>(), "Vector3");
		// Bind various functions
		mWrapper->getChai()->add(chaiscript::fun(&Vector3::length), "length");
		mWrapper->getChai()->add(chaiscript::fun(&Vector3::squaredLength), "squaredLength");
		mWrapper->getChai()->add(chaiscript::fun(&Vector3::normalize), "normalize");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3 (Vector3::*)(const Vector3)>(&Vector3::crossProduct)), "crossProduct");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Real (Vector3::*)(const Vector3)>(&Vector3::dotProduct)), "dotProduct");
		// Bind some operators...
		mWrapper->getChai()->add(chaiscript::fun(static_cast<bool (Vector3::*)(const Vector3)>(&Vector3::operator==)), "==");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<bool (Vector3::*)(const Vector3)>(&Vector3::operator!=)), "!=");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3 (Vector3::*)(const Vector3)>(&Vector3::operator+)), "+");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3 (Vector3::*)(const Vector3)>(&Vector3::operator-)), "-");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3 (Vector3::*)(const Vector3)>(&Vector3::operator*)), "*");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3 (Vector3::*)(const Vector3)>(&Vector3::operator/)), "/");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3& (Vector3::*)(const Vector3)>(&Vector3::operator=)), "=");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3& (Vector3::*)(const Vector3)>(&Vector3::operator+=)), "+=");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3& (Vector3::*)(const Vector3)>(&Vector3::operator-=)), "-=");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3& (Vector3::*)(const Vector3)>(&Vector3::operator*=)), "*=");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3& (Vector3::*)(const Vector3)>(&Vector3::operator/=)), "/=");
		// Bind operators with scalars...
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3 (Vector3::*)(const Real)>(&Vector3::operator*)), "*");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3 (Vector3::*)(const Real)>(&Vector3::operator/)), "/");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3& (Vector3::*)(const Real)>(&Vector3::operator*=)), "*=");
		mWrapper->getChai()->add(chaiscript::fun(static_cast<Vector3& (Vector3::*)(const Real)>(&Vector3::operator/=)), "/=");
		// Public members
		mWrapper->getChai()->add(chaiscript::fun(&Vector3::x), "x");
		mWrapper->getChai()->add(chaiscript::fun(&Vector3::y), "y");
		mWrapper->getChai()->add(chaiscript::fun(&Vector3::z), "z");

		// register built-in logic types
		StaticPhysicsObject::registerClass(this);
		RigidBody::registerClass(this);
		Lightmapper::registerClass(this);

		// Some simple helper functions (automagically cast object types etc)
		runScript("helpers.chai");
	}

	ChaiWrapper* ScriptManager::getWrapper()
	{
		return mWrapper;
	}

	std::string ScriptManager::toString(int val)
	{
		std::stringstream stream;
        stream.width(0);
        stream.fill(' ');
        stream << val;
        return stream.str();
	}

	std::string ScriptManager::toString(float val)
	{
		std::stringstream stream;
        stream.precision(6);
        stream.width(0);
        stream.fill(' ');
        stream << val;
        return stream.str();
	}

	std::string ScriptManager::toString(bool val)
	{
		std::stringstream stream;
        stream.width(0);
        stream.fill(' ');
        stream << val;
        return stream.str();
	}

	std::string ScriptManager::toString(std::string val)
	{
		return val;
	}

	std::string ScriptManager::toString(char val)
	{
		std::string str("");
		str+=val;
		return str;
	}

}
