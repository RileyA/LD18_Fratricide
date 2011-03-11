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


#ifndef GUICOMPONENT_H
#define GUICOMPONENT_H

#include "stdafx.h"
#include "QEngine.h"

namespace MyGUI
{
	class Edit;
	class Widget;
	typedef std::vector<Widget*> VectorWidgetPtr;
	typedef Widget *WidgetPtr;
}

namespace QE
{
	class GUIComponent
	{
	public:

		GUIComponent(String layoutFile,bool visible = true);

		~GUIComponent();

		void setVisible(bool visible);

		bool getVisible();

		void setTemporary(bool temporary);

		bool isTemporary();

	protected:

		MyGUI::VectorWidgetPtr mRoot;
		MyGUI::WidgetPtr mParent;
		bool mVisible;
		bool mTemporary;

	};
}

#endif