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


#ifndef SOUNDPTR_H
#define SOUNDPTR_H

#include "stdafx.h"
#include "QEngine.h"

#include "ActiveSound.h"
#include "BufferedSound.h"

namespace QE
{
	class AudioManager;

	class QENGINE_API SoundPtr
	{
	public:

		SoundPtr(unsigned int index,time_t start,AudioManager* amgr);
		~SoundPtr();

		bool isValid();

		SoundData* getData();

	protected:

		time_t mStart;
		unsigned int mSoundIndex;
		bool mValid;
		AudioManager* mAudioMgr;

	};
}

#endif