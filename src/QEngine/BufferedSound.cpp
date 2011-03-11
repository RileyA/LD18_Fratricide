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

#include "BufferedSound.h"
#include "Logger.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

namespace QE
{
	BufferedSound::BufferedSound(const String& filename,bool isOgg,bool delayLoad)
	{
		mFilename = filename;
		mOgg = isOgg;
		mLoaded = false;
		if(!delayLoad)
		{
			load();
		}
	}

	BufferedSound::~BufferedSound()
	{
		unload();
	}

	bool BufferedSound::load()
	{
		if(!mLoaded&&!mOgg)
		{
			alGetError();
			mBuffer = alutCreateBufferFromFile(String("../media/audio/"+mFilename).c_str());
			// Check for errors
			if(alGetError() == AL_NO_ERROR)
			{
				Logger::getSingletonPtr()->logMessage("Sound Loaded: "+mFilename);
				mLoaded = true;
			}
			else
			{
				mLoaded = false;
			}
		}
		return mLoaded;
	}

	void BufferedSound::unload()
	{
		if(mLoaded)
		{
			alDeleteBuffers(1,&mBuffer);
			mLoaded = false;
			mBuffer = 0;
		}
	}

	bool BufferedSound::isLoaded()
	{
		return mLoaded;
	}

	bool BufferedSound::isOgg()
	{
		return mOgg;
	}

	unsigned int BufferedSound::getBuffer()
	{
		return mBuffer;
	}

	String BufferedSound::getFilename()
	{
		return mFilename;
	}

}
