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


#ifndef CHAIWRAPPER_H
#define CHAIWRAPPER_H

#include "stdafx.h"
#include "QEngine.h"

#pragma warning( disable: 4146 )
#pragma warning( disable: 4181 )

#include <chaiscript/chaiscript.hpp>

namespace QE
{
	/** Since typdefs won't let me forward declare this in the ScriptManager and I don't want to force the host application to have ChaiScript
	stuff included,  I had to do this hack-y little workaraound... */
	class ChaiWrapper
	{
	public:
		ChaiWrapper()
			:mChai()
		{

		}

		~ChaiWrapper()
		{

		}

		chaiscript::ChaiScript* getChai()
		{
			return &mChai;
		}

	protected:

		chaiscript::ChaiScript mChai;

	};
}

#endif
