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


#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "stdafx.h"
#include "QEngine.h"

//#include "Client.h"
//#include "Server.h"

namespace QE
{
	class NetworkManager
	{
	public:

		NetworkManager();
		~NetworkManager();

		void update();

		//void registerClient(Client* client);
		//void registerServer(Server* server);

	protected:

		// these are pretty much identical with how RakNet works
		// but made em separate for the sake of clarity when deriving...
		//std::vector<Server*> mServers;
		//std::vector<Client*> mClients;

	};
}

#endif