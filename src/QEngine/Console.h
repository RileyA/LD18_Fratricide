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


#ifndef GUICONSOLE_H
#define GUICONSOLE_H

#include "stdafx.h"
#include "GUIComponent.h"

namespace QE
{
	class QEngine;

	class Console : public GUIComponent
	{
	public:

		Console(String layout,QEngine* engine,bool visible=true);
		~Console();
		
		void enterCode();

		void focus();

		//void setMaxLines(const unsigned int);

		//unsigned int getMaxLines();

	protected:

		MyGUI::Edit* mEdit;
		QEngine* mEngine;

		bool mVisible;
		//std::vector<String> mLines;
		//unsigned int mMaxLines;

	};
}
#endif