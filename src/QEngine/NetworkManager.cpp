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
#include "QEngine.h"

#include "NetworkManager.h"

namespace QE
{
	NetworkManager::NetworkManager()
	{

	}

	NetworkManager::~NetworkManager()
	{
		//for(unsigned int i=0;i<mServers.size();++i)
		//{
		//	mServers[i]->shutdown();
		//	delete mServers[i];
		//	mServers[i] = 0;
		//}
		//for(unsigned int i=0;i<mClients.size();++i)
		//{
		//	mClients[i]->shutdown();
		//	delete mClients[i];
		//	mClients[i] = 0;
		//}

		//mServers.clear();
		//mClients.clear();
	}

	void NetworkManager::update()
	{
		//for(unsigned int i=0;i<mServers.size();++i)
		//{
		//	mServers[i]->update();
		//}
		//for(unsigned int i=0;i<mClients.size();++i)
		//{
		//	mClients[i]->update();
		//}
	}

	//void NetworkManager::registerClient(Client* client)
	//{
		//mClients.push_back(client);
	//}

	//void NetworkManager::registerServer(Server* server)
	//{
		//mServers.push_back(server);
	//}
}