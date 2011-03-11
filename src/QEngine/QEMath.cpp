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

#include "QEMath.h"

namespace QE
{
	float randFloat(float low, float high)
	{
		float temp;

		if (low > high)
		{
			temp = low;
			low = high;
			high = temp;
		}

		temp = (rand() / (static_cast<float>(RAND_MAX) + 1.0))
		* (high - low) + low;
		return temp;
	};

	int randInt(int low, int high)
	{
		float temp;

		if (low > high)
		{
			temp = low;
			low = high;
			high = temp;
		}

		temp = low + rand()%(high+1-low);//(rand() / (static_cast<float>(RAND_MAX) + 1.0))
		//* (high - low) + low;
		return temp;
	};

	// Shamelessly stolen from Ogre, should be relocated to a string utils thing somewhere...
	void splitBaseFilename(const String& fullName,String& outBasename,String& outExtention)
	{
		size_t i = fullName.find_last_of(".");
		if (i == String::npos)
		{
			outExtention.clear();
			outBasename = fullName;
		}
		else
		{
			outExtention = fullName.substr(i+1);
			outBasename = fullName.substr(0, i);
		}
	}
};