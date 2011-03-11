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


#ifndef SIMPLEOVERLAY_H
#define SIMPLEOVERLAY_H

#include "stdafx.h"
#include "QEngine.h"

#include "LogicObject.h"
#include "RenderedObject.h"
#include "PhysicsObject.h"

namespace Ogre
{
	class Overlay;
}

namespace QE
{

	class QENGINE_API SimpleOverlay : public LogicObject
	{
	public:

		SimpleOverlay(QEngine* engine,String bucket,String name);

		virtual ~SimpleOverlay();

		virtual void update(double delta);

		virtual void deinitialize();

		void setThreshold(Real x,Real y);


	protected:

		Ogre::Overlay* mOverlay;
		Real tx,ty;


	};
}

#endif