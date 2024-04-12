#include "EngineCore.h"
#include "NetworkEngine.h"
#include "SceneManager.h"
void NetworkEngine::Initialize(bool _isServer)
{
	isServer=_isServer;
	isClient = !_isServer;

	LoadSettings();

	rakInterface=RakNet::RakPeerInterface::GetInstance();
	state = NetworkState::INITIALIZE_NETWORK;
}
void NetworkEngine::LoadSettings()
{
	std::ifstream inputStream("../Assets/NetworkSettings.json");
	std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	json::JSON document = json::JSON::Load(str);

	THROW_RUNTIME_ERROR(document.hasKey("ipaddress") == false, "Unable to determine ip address");
	ipAddress = document["ipaddress"].ToString();

	THROW_RUNTIME_ERROR(document.hasKey("port") == false, "Unable to determine ip address");
	port = document["port"].ToInt();
}

void NetworkEngine::InitializeNetwork()
{
	if (isClient)
	{
		RakNet::SocketDescriptor sd(0, NULL);
		if (rakInterface->Startup(1, &sd, 1) != RakNet::RAKNET_STARTED)
		{
			std::cerr << "*** Failed to start Network" << port << std::endl;
			exit(1);
		}
		if (rakInterface->Connect(ipAddress.c_str(), port, NULL, 0) != RakNet::CONNECTION_ATTEMPT_STARTED)
		{
			std::cerr << "*** Failed to connect to server. Going to try later" << std::endl;
		}
	}
	else 
	{
		RakNet::SocketDescriptor sd(port, NULL);
		if (rakInterface->Startup(8, &sd, 1) != RakNet::RAKNET_STARTED)
		{
			std::cerr<<"*** Failed to start server on port"<<port<<std::endl;
			exit(1);
		}
		rakInterface->SetMaximumIncomingConnections(8);
	}
	state = NetworkState::RUNNING;
}

void NetworkEngine::PreUpdate()
{
	if (!isClient && !isServer) return;

	switch (state)
	{
	case NetworkState::DISABLED:
		break;
	case NetworkState::INITIALIZE_NETWORK:
		if (rakInterface->IsActive())
		{
			rakInterface->Shutdown(0);
		}
		InitializeNetwork();
		break;
	case NetworkState::RUNNING:
		ReceivePackets();
		break;
	default:
		break;
	}

}

void NetworkEngine::ReceivePackets()
{
	unsigned char packetId;

	RakNet::Packet* packet = rakInterface->Receive();
	while (packet != nullptr)
	{
		RakNet::BitStream bs(packet->data, packet->length, false);
		bs.Read(packetId);
		switch (packetId)
		{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				if (isClient)
				{
					std::cout << "\nConnected to" << packet->systemAddress.ToString(true) << std::endl;
					connections.push_back(packet->guid);
					state = NetworkState::RUNNING;
				}
				break;
			case MSG_SCENE_MANAGER:
				SceneManager::Instance().ProcessPacket(bs);
				break;

			case ID_NEW_INCOMING_CONNECTION:
				std::cout << "Got connection from" << packet->systemAddress.ToString(true) << std::endl;
				connections.push_back(packet->guid);
				SceneManager::Instance().SerializeSnapshot();
				if (connections.size() > 1)
				{
					RakNet::BitStream bs;
					bs.Write((unsigned char)NetworkPacketIds::MSG_SCENE_MANAGER);
					bs.Write((unsigned char)NetworkPacketIds::MSG_NEW_GAME);
					SendPacket(bs);
				}
				break;
			case ID_CONNECTION_LOST:
			case ID_DISCONNECTION_NOTIFICATION:
				std::cout<<"Disconnected from"<<packet->systemAddress.ToString(true)<<std::endl;
				connections.erase(std::find(connections.begin(), connections.end(), packet->guid));
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				std::cerr << "*** Connection attempt failed" << std::endl;
				state = NetworkState::INITIALIZE_NETWORK;
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				std::cerr << "*** Server is full" << std::endl;
				state = NetworkState::INITIALIZE_NETWORK;
				break;

			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				std::cerr << "*** Incompatible protocol version" << std::endl;
				state = NetworkState::INITIALIZE_NETWORK;
				break;
		default:
			std::cerr << "*** Unknown connection response" << std::endl;
			state = NetworkState::INITIALIZE_NETWORK;
			break;
		}
		rakInterface->DeallocatePacket(packet);
		packet = rakInterface->Receive();
	}

}
void NetworkEngine::PostUpdate()
{
	if (!isClient && !isServer) return;

	SceneManager::Instance().NetworkUpdate();
}

void NetworkEngine::SendPacket(RakNet::BitStream& bs)
{
	for (int i=0;i<connections.size();i++)
	{
		rakInterface->Send(&bs, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, connections[i], false);
	}
}