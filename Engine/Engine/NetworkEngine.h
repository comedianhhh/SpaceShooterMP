#pragma once
class NetworkEngine
{
	enum NetworkState 
	{
		INITIALIZE_NETWORK,
		WAITING_FOR_CONNECTION,
		RUNNING,
		DISABLED,
		NETWORK_ERROR,
	};
	DECLARE_SINGLETON(NetworkEngine)
public:
	bool IsClient() { return isClient; }
	bool IsServer() { return isServer; }
	void SendPacket(RakNet::BitStream& bs);
private:
	void Initialize(bool _isServer);
	void InitializeNetwork();
	void LoadSettings();
	void PreUpdate();
	void ReceivePackets();
	void PostUpdate();

private:
	int port;
	bool isServer = false;
	bool isClient = false;
	std::string ipAddress;
	NetworkState state;

	RakNet::RakPeerInterface* rakInterface;
	std::vector<RakNet::RakNetGUID> connections;

	friend class Engine;
};

