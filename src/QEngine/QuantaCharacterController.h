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


#ifndef QCHARACTERCONTROLLER_H
#define QCHARACTERCONTROLLER_H

#include "stdafx.h"
#include "QEngine.h"
#include "Quaternion.h"
#include "Vector3.h"

#include "CharacterController.h"

namespace QE
{
	class CCTCompound;
	class PhysicsManager;

	class QCharacterController : public CharacterController
	{
	public:

		QCharacterController(PhysicsManager* pmgr,Vector3 startPos);
		virtual ~QCharacterController();

		virtual void update(float delta);

		virtual void initialize();

		virtual void deinitialize();

		void setMove(bool setting);

		void setMoveVector(Vector3 move);

		virtual Vector3 getPosition();

		Vector3 getSegmentPosition(int segment);

		Vector3 moveV;

		Quaternion getOrientation();

		void setQueriesEnabled(bool set);

		void setPlayerSpeed(float start,float top,float accel);

		void setPlayerGravitySpeed(float base,float terminal,float accel);

		bool jump(float factor);

		void crouchToggle(unsigned int sw=0);

		bool inAir();

		void resetImpactLevel();

		Real getSpeed();

	protected:

		// the controller itself...
		CCTCompound* mController;

		float mAccumulation;
		float mInterpolation;
		float mTimeStep;

		Quaternion q_1;
		Quaternion q_2;

		Vector3 p_1;
		Vector3 p_2;

		float mPlayerSpeedStart;
		float mPlayerSpeedTop;
		float mPlayerSpeedAccel;

		float mPlayerGravityBase;
		float mPlayerGravityTerminal;
		float mPlayerGravityAccel;

		float mPlayerSpeedInterpolation;
		float mPlayerGravityInterpolation;

		float mPlayerSpeedLast;
		float mPlayerGravityLast;

		unsigned int mGracePeriod;

		bool mCrouching;
		bool mCrouched;
		Real mCrouchLevel;
		bool mInMidair;
		Real mMidairTime;
		Real mMidairTimeBase;
		Real mJumpFactor;
		bool mInAirLast;
		bool mInAirLast2;
		bool mIsJumping;
		Real mOffset;

		bool mHittingGround;
		float mGroundImpact;
		float mGroundImpactSpd;
		Real mImpactLevel;

		// head bob stuff
		bool mBobSwap;
		float mBobPeriod;
		bool mBobLR;
		float mBobLRPeriod;

	};
}

#endif
