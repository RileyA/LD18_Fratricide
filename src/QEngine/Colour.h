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


#ifndef COLOUR_H
#define COLOUR_H

#include "stdafx.h"
#include "QEngine.h"

namespace Ogre
{
	class ColourValue;
}

namespace QE
{
	class QENGINE_API Colour
	{
	public:

		// Some preset constant values
		static const Colour ZERO;
        static const Colour Red;
        static const Colour Green;
        static const Colour Blue;
		static const Colour Black;
        static const Colour White;
		
	    explicit Colour( float red = 1.0f,float green = 1.0f,float blue = 1.0f,float alpha = 1.0f ) 
			: r(red), g(green), b(blue), a(alpha)
        {}

		Colour(const Ogre::ColourValue& col);

		operator Ogre::ColourValue() const;

		bool operator==(const Colour& col) const;
	    bool operator!=(const Colour& col) const;

		void saturate()
		{
			if(r>1)
				r=1.f;
			else if(r<0)
				r=0.f;

			if(g>1)
				g=1.f;
			else if(g<0)
				g=0.f;

			if(b>1)
				b=1.f;
			else if(b<0)
				b=0.f;

			if(a>1)
				a=1.f;
			else if(a<0)
				a=0.f;
		}

		Colour saturateCopy() const
        {
            Colour copy = *this;
            copy.saturate();

            return copy;
        }

		Colour operator + (const Colour& col) const
		{
			Colour out;

			out.r = r + col.r;
			out.g = g + col.g;
			out.b = b + col.b;
			out.a = a + col.a;

			return out;
		}
		
		Colour& operator += (const Colour& col)
		{
			r += col.r;
			g += col.g;
			b += col.b;
			a += col.a;

			return *this;
		}

		Colour operator - (const Colour& col) const
		{
			Colour out;

			out.r = r - col.r;
			out.g = g - col.g;
			out.b = b - col.b;
			out.a = a - col.a;

			return out;
		}

		Colour& operator -= (const Colour& col)
		{
			r -= col.r;
			g -= col.g;
			b -= col.b;
			a -= col.a;

			return *this;
		}

		Colour operator * (const float scalar) const
		{
			Colour out;

			out.r = r * scalar;
			out.g = g * scalar;
			out.b = b * scalar;
			out.a = a * scalar;

			return out;
		}

		Colour& operator *= (const float scalar)
		{
			r *= scalar;
			g *= scalar;
			b *= scalar;
			a *= scalar;

			return *this;
		}

		Colour operator / (const float scalar) const
		{
			Colour out;

			out.r = r / scalar;
			out.g = g / scalar;
			out.b = b / scalar;
			out.a = a / scalar;

			return out;
		}

		Colour& operator /= (const float scalar)
		{
			r /= scalar;
			g /= scalar;
			b /= scalar;
			a /= scalar;

			return *this;
		}

		Real r;
		Real g;
		Real b;
		Real a;
	};
}

#endif