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


#ifndef QUANTACCT_H
#define QUANTACCT_H

#include "stdafx.h"
#include "QEngine.h"

#include "LogicObject.h"
#include "QuantaCharacterController.h"

namespace QE
{
	class RenderedObject;

	class QENGINE_API QuantaCCT : public LogicObject
	{
	public:

		QuantaCCT(QEngine* engine,String bucket,String name,Vector3 pos=Vector3(0,0,0));

		virtual ~QuantaCCT();

		virtual void update(double delta);

		virtual void deinitialize();

		Vector3 getPosition();

		Quaternion getOrientation();

		void setQueriesEnabled(bool set);

		void setMoveVector(Vector3 move);

		void setMove(bool move);

		bool jump(float str=1.5f);
		void toggleCrouch(unsigned int sw=0);

		void resetImpact();

		Real getPlayerSpeed();

		void setPlayerSpeed(Real base,Real top, Real acel);

		void setDebugVisualization(bool visible);
		//bool inAir();

	protected:

		QCharacterController* mController;

		RenderedObject* Robjs[5];

	};
}

#endif