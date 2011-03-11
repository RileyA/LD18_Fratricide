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


#ifndef HALTON_H
#define HALTON_H

#include "stdafx.h"
#include "QEngine.h"

namespace QE
{

	/** Class for generating a halton sequence */
	class Halton
	{
		float _invBases[2];
		float _prev[2];

		float halton(float baseRec,float prev) const
		{
			float r = 1 -prev - 1e-10;
			if(baseRec<r) 
				return prev+baseRec;
			float h=baseRec;
			float hh;
			do
			{
				hh=h;
				h*=baseRec;
			} 
			while(h>=r);

			return prev+hh+h-1;
		}

	public:

		void Reset()
		{
			_prev[0]=_prev[1]=0;
		}

		Halton()
		{
			_invBases[0]=1./2;
			_invBases[1]=1./3;
			Reset();
		}

		void GetNext(float out[2])
		{
			out[0]=halton(_invBases[0],_prev[0]);
			out[1]=halton(_invBases[1],_prev[1]);
			_prev[0]=out[0];
			_prev[1]=out[1];
		}
	};
};

#endif