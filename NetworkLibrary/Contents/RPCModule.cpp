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
	unsigned short len_;
	char message_[128];
};

inline Packet& operator<<(Packet& packet, ChatMsg* chatMsg)
{
	packet << chatMsg->len_;
	packet.Write(chatMsg->message_, chatMsg->len_);
}

inline Packet& operator>>(Packet& packet, ChatMsg* chatMsg)
{
	packet >> chatMsg->len_;
	packet.Read(chatMsg->message_, chatMsg->len_);
}

#include "../RPC/ServerProxy.cpp"
#include "../RPC/ServerStub.cpp"
