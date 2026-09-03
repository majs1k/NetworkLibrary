// -------------------------------------------------
// 
// 직렬화 버퍼에 필요한 가변 인자들 전역 오버로딩
// RPC 클래스 컴파일
//
// -------------------------------------------------
#include "RpcModule.h"
#include "../Utils/Packet.h"

#include <vector>
#include <string>

RpcServerProxy g_RpcProxy;
RpcClientProxy g_ClientRpcProxy;

DbProxy g_DbProxy;


inline Packet& operator<<(Packet& packet, std::string& str)
{
	packet << static_cast<short>(str.size());

	packet.Write(str.data(), static_cast<short>(str.size()));

	return packet;
}

inline Packet& operator>>(Packet& packet, std::string& str)
{
	short size = 0;

	packet >> size;
	str.resize(size);

	packet.Read(const_cast<char*>(str.data()), size);

	return packet;
}

template<typename T>
inline Packet& operator<<(Packet& packet, std::vector<T>& vec)
{
	packet << static_cast<short>(vec.size());

	for (auto& v : vec)
		packet << v;

	return packet;
}

template<typename T>
inline Packet& operator>>(Packet& packet, std::vector<T>& vec)
{
	short size = 0;

	packet >> size;

	T data;

	for (int i = 0; i < size; i++)
	{
		packet >> data;
		vec.push_back(data);
	}

	return packet;
}

inline Packet& operator<<(Packet& packet, RESPONSE_CODE code)
{
	packet << static_cast<short>(code);

	return packet;
}

inline Packet& operator>>(Packet& packet, RESPONSE_CODE& code)
{
	short c;
	packet >> c;

	code = static_cast<RESPONSE_CODE>(c);

	return packet;
}


inline Packet& operator<<(Packet& packet, PLAYER_STATE state)
{
	packet << static_cast<short>(state);

	return packet;
}

inline Packet& operator>>(Packet& packet, PLAYER_STATE& state)
{
	short s;
	packet >> s;

	state = static_cast<PLAYER_STATE>(s);

	return packet;
}


inline Packet& operator<<(Packet& packet, Player player)
{
	packet << player.playerId_;
	packet << player.playerName_;
	packet << player.win_;
	packet << player.lose_;
	packet << player.rating_;
	packet << player.money_;
	packet << player.equippedInvenId_;
	packet << player.state_;

	return packet;
}

inline Packet& operator>>(Packet& packet, Player& player)
{
	packet >> player.playerId_;
	packet >> player.playerName_;
	packet >> player.win_;
	packet >> player.lose_;
	packet >> player.rating_;
	packet >> player.money_;
	packet >> player.equippedInvenId_;
	packet >> player.state_;

	return packet;
}

inline Packet& operator<<(Packet& packet, PlayerInfo info)
{
	packet << info.playerId_;
	packet << info.playerName_;
	packet << info.rating_;
	packet << info.state_;

	return packet;
}

inline Packet& operator>>(Packet& packet, PlayerInfo& info)
{
	packet >> info.playerId_;
	packet >> info.playerName_;
	packet >> info.rating_;
	packet >> info.state_;

	return packet;
}

inline Packet& operator<<(Packet& packet, Icon icon)
{
	packet << icon.inventoryId_;
	packet << icon.iconId_;

	return packet;
}

inline Packet& operator>>(Packet& packet, Icon& icon)
{
	packet >> icon.inventoryId_;
	packet >> icon.iconId_;

	return packet;
}

inline Packet& operator<<(Packet& packet, STONE stone)
{
	packet << static_cast<short>(stone);

	return packet;
}

inline Packet& operator>>(Packet& packet, STONE& stone)
{
	short s;
	packet >> s;

	stone = static_cast<STONE>(s);

	return packet;
}


#include "../RPC/RpcServerProxy.cpp"
#include "../RPC/RpcServerStub.cpp"
#include "../RPC/RpcClientProxy.cpp"
#include "../RPC/RpcClientStub.cpp"

#include "../Database/DbProxy.cpp"
#include "../Database/DbStub.cpp"
