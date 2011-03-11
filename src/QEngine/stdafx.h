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


//The PCH, pretty much just going to be standard library stuff here
#ifndef STDAFX_H
#define STDAFX_H

#ifdef WIN32
    #pragma warning( disable: 4251 )
    #pragma warning( disable: 4275 )
    #pragma warning( disable: 4181 )
    #pragma warning( disable: 4146 )

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

/** The PCH includes */

// Standard stuff
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <stdexcept>

#endif