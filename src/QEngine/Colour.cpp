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
#include "Colour.h"

#include "OgreColourValue.h"

namespace QE
{
	const Colour Colour::ZERO = Colour(0.f,0.f,0.f,0.f);
	const Colour Colour::Red = Colour(1.f,0.f,0.f);
	const Colour Colour::Green = Colour(0.f,1.f,0.f);
	const Colour Colour::Blue = Colour(0.f,0.f,1.f);
	const Colour Colour::Black = Colour(0.f,0.f,0.f);
	const Colour Colour::White = Colour(1.f,1.f,1.f);

	Colour::Colour(const Ogre::ColourValue& col)
		:r(col.r),g(col.g),b(col.b),a(col.a)
	{}

	Colour::operator Ogre::ColourValue() const
	{
		return Ogre::ColourValue(r,g,b,a);
	}

    bool Colour::operator==(const Colour& col) const
    {
        return (r == col.r && g == col.g &&
            b == col.b && a == col.a);
    }

    bool Colour::operator!=(const Colour& col) const
    {
        return !(*this == col);
    }

}