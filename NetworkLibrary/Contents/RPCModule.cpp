// -------------------------------------------------
// 
// 직렬화 버퍼에 필요한 가변 인자들 전역 오버로딩 추가
//
// -------------------------------------------------
#include "RpcModule.h"
#include "../Utils/Packet.h"

#include <list>	
#include <string>

RpcServerProxy g_RpcProxy;
RpcClientProxy g_ClientRpcProxy;


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
inline Packet& operator<<(Packet& packet, std::list<T>& lst)
{
	packet << static_cast<short>(lst.size());

	for (auto& l : lst)
		packet << l;

	return packet;
}

template<typename T>
inline Packet& operator>>(Packet& packet, std::list<T>& lst)
{
	short size = 0;

	packet >> size;

	T data;

	for (int i = 0; i < size; i++)
	{
		packet >> data;
		lst.push_back(data);
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



inline Packet& operator<<(Packet& packet, Player player)
{
	packet << player.playerId_;
	packet << player.playerName_;
	packet << player.level_;
	packet << player.gold_;
	packet << player.equippedInvenId_;

	return packet;
}

inline Packet& operator>>(Packet& packet, Player& player)
{
	packet >> player.playerId_;
	packet >> player.playerName_;
	packet >> player.level_;
	packet >> player.gold_;
	packet >> player.equippedInvenId_;

	return packet;
}

inline Packet& operator<<(Packet& packet, PlayerInfo info)
{
	packet << info.playerId_;
	packet << info.playerName_;
	packet << info.level_;

	return packet;
}

inline Packet& operator>>(Packet& packet, PlayerInfo& info)
{
	packet >> info.playerId_;
	packet >> info.playerName_;
	packet >> info.level_;

	return packet;
}

inline Packet& operator<<(Packet& packet, Character character)
{
	packet << character.inventoryId_;
	packet << character.characterId_;
	packet << character.level_;
	packet << character.attack_;
	packet << character.hp_;

	return packet;
}

inline Packet& operator>>(Packet& packet, Character& character)
{
	packet >> character.inventoryId_;
	packet >> character.characterId_;
	packet >> character.level_;
	packet >> character.attack_;
	packet >> character.hp_;

	return packet;
}


#include "../RPC/RpcServerProxy.cpp"
#include "../RPC/RpcServerStub.cpp"
#include "../RPC/RpcClientProxy.cpp"
#include "../RPC/RpcClientStub.cpp"

#include "../Database/DbProxy.cpp"
#include "../Database/DbStub.cpp"
