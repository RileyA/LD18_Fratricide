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


#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "QEngine.h"
#include "stdafx.h"

#include "ActiveSound.h"
#include "BufferedSound.h"
#include "SoundPtr.h"

namespace QE
{
	class QENGINE_API AudioManager
	{
	public:

		AudioManager();
		~AudioManager();

		void initialize();
		void deinitialize();

		SoundPtr play2D(String filename);

		bool bufferSound(String filename);

		void update();

		bool loadSound(String filename);

		ActiveSound* getActiveSound(unsigned int index);

	protected:

		bool mInitialized;
		// store as a map, for easy access by name
		std::map<std::string,BufferedSound*> mBuffers;
		std::vector<ActiveSound*> mSounds;
		unsigned int mMaxSources;
	};
}

#endif
