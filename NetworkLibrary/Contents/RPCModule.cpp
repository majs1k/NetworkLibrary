// -------------------------------------------------
// 
// RPC 프록시 스텁 코드는 서버 라이브러리 x
// 컨텐츠단에서 컴파일
// 직렬화 버퍼에 필요한 가변 인자들 전역 오버로딩 추가
//
// -------------------------------------------------
#include "../Utils/Packet.h"

struct ChatMsg
{
	unsigned short len;
	char message[128];
};

inline Packet& operator<<(Packet& packet, ChatMsg* chatMsg)
{
	packet << chatMsg->len;
	packet.write(chatMsg->message, chatMsg->len);
}

inline Packet& operator>>(Packet& packet, ChatMsg* chatMsg)
{
	packet >> chatMsg->len;
	packet.read(chatMsg->message, chatMsg->len);
}

#include "../RPC/RPCProxy.cpp"
#include "../RPC/RPCStub.cpp"
