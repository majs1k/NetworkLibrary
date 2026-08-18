// -------------------------------------------------
// 
// 직렬화 버퍼에 필요한 가변 인자들 전역 오버로딩 추가
//
// -------------------------------------------------
#include "../Utils/Packet.h"
#include "Item.h"


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


inline Packet& operator<<(Packet& packet, std::list<int>& lst)
{
	packet << static_cast<short>(lst.size());

	for (auto i : lst)
		packet << i;

	return packet;
}

inline Packet& operator>>(Packet& packet, std::list<int>& lst)
{
	short size = 0;

	packet >> size;

	int data;

	for (int i = 0; i < size; i++)
	{
		packet >> data;
		lst.push_back(data);
	}

	return packet;
}


#include "../RPC/RpcServerProxy.cpp"
#include "../RPC/RpcServerStub.cpp"
#include "../RPC/RpcClientProxy.cpp"
#include "../RPC/RpcClientStub.cpp"
