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


#ifndef MESSAGEANY_H
#define MESSAGEANY_H

#include "Message.h"

namespace QE
{
	template<class T>
	class MessageAny : public Message
	{
	public:

		MessageAny(T _data)
		{
			data = _data;
		}

		T data;

	};

	template<class T>
	MessageAny<T>* message_cast(Message* msg)
	{
		if(MessageAny<T>* message = dynamic_cast<MessageAny<T>* >(msg))
		{
			return message;
		}
		else
		{
			return 0;
		}
	}
}

#endif