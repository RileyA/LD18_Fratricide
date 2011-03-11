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


#ifndef QENGINE_H
#define QENGINE_H

// Unless we're using windows, this does nothing.
#ifdef WIN32
    #ifdef QENGINE_EXPORTS
		#define QENGINE_API __declspec(dllexport)
    #else
		#define QENGINE_API __declspec(dllimport)
    #endif
#else
	#define QENGINE_API
#endif

#define QENGINE_VERSION 0.3.1

namespace Ogre
{
	class Vector3;
	class Quaternion;
}

namespace QE
{
	typedef std::string String;
	typedef float Real;
};

// Same basic types
#include "Exception.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Colour.h"

#endif
