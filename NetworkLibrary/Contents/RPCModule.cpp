// -------------------------------------------------
// 
// 직렬화 버퍼에 필요한 가변 인자들 전역 오버로딩 추가
//
// -------------------------------------------------
#include "../Utils/Packet.h"

#include <list>	
#include <string>

#include "ResponseCode.h"


inline Packet& operator<<(Packet& packet, std::string str)
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


inline Packet& operator<<(Packet& packet, std::list<int> lst)
{
	packet << static_cast<short>(lst.size());

	for (auto l : lst)
		packet << l;

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


#include "../RPC/RpcServerProxy.cpp"
#include "../RPC/RpcServerStub.cpp"
#include "../RPC/RpcClientProxy.cpp"
#include "../RPC/RpcClientStub.cpp"

#include "../Database/DatabaseServerProxy.cpp"
#include "../Database/DatabaseServerStub.cpp"
