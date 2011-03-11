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

#include "ActiveSound.h"
#include "TimeManager.h"
#include "Logger.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#include "OgreStringConverter.h"

#include "OggStream.h"

namespace QE
{
	ActiveSound::ActiveSound()
	{
		alGenSources(1,&mSource);
		mActive = false;
		mStart = clock();
		mOgg = new OggStream();
	}

	ActiveSound::~ActiveSound()
	{
		delete mOgg;
		clear();
		alDeleteSources(1,&mSource);
	}

	void ActiveSound::update()
	{
		if(mActive)
		{
			ALint state;

			if(mData.mBuffer->isOgg())
			{
				if(!mOgg->update())
				{
					mActive = false;
					clear();
					return;
				}

				if(!mOgg->playing()&&mActive)
				{
					alSourcePlay(mOgg->getSource());
				}
			}
			else
			{
				alGetSourcei(mSource,AL_SOURCE_STATE,&state);

				if(state!=AL_PLAYING&&state!=AL_INITIAL)
				{
					clear();
					return;
				}
			}

			alSourcef(mSource,AL_PITCH,mData.mPitch*TimeManager::getSingletonPtr()->getTimeSpeed());

		}
	}

	void ActiveSound::clear()
	{
		if(mActive)
		{
			if(!mData.mBuffer->isOgg())
			{
				ALint state;
				alGetSourcei(mSource,AL_SOURCE_STATE,&state);
				alSourceStop(mSource);
				alSourcei(mSource,AL_BUFFER,AL_NONE);
				mActive = false;
			}
			else
			{
				mActive = false;
				mOgg->release();
			}

			if (alGetError() != AL_NO_ERROR)
			{
				//add log message here
			}
		}
	}

	bool ActiveSound::isActive()
	{
		return mActive;
	}

	void ActiveSound::playSound(SoundData data)
	{
		mData = data;
		mStart = clock();
		if(!mData.mBuffer->isOgg())
		{
			float pos[] = {0,0,0};
			alSourcei(mSource,AL_SOURCE_RELATIVE,true);
			alSourcefv(mSource,AL_POSITION,pos);

			alGetError();

			alSourcei(mSource,AL_BUFFER,mData.mBuffer->getBuffer());
			alSourcei(mSource,AL_LOOPING,mData.mLooping);
			alSourcef(mSource,AL_MIN_GAIN,0.f);
			alSourcef(mSource,AL_MAX_GAIN,1.f);
			alSourcef(mSource,AL_GAIN,1.f);
			//alSourcef(mSource,AL_ROLLOFF_FACTOR,0.8f);

			alGetError();

			alSourcePlay(mSource);
			//alSourcePlay(mSource);

			if(alGetError() == AL_NO_ERROR)
			{
				mActive = true;
			}
			else
			{
				Logger::getSingletonPtr()->logMessage("Audio Playback Failed:" + mData.mBuffer->getFilename());
				mActive = false;
			}
		}
		else
		{
			mOgg->open("../media/audio/"+mData.mBuffer->getFilename(),mSource);
			mOgg->playback();
			mActive = true;
		}
	}

	time_t ActiveSound::getStartTime()
	{
		return mStart;
	}

}
