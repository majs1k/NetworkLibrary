using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
    class RPCFormat
    {
// -------------------------------------------------------------------
        public static string proxyHeader =
@"#pragma once

class LanServer;

class ServerProxy
{{
public:
	LanServer* server_;

public:{0}
}};
";

        public static string proxyHeaderFunc =
@"
	void {0}(__int64 sessionId{1});";

        public static string funcParam =
@"{0} {1}";

// -------------------------------------------------------------------

        public static string proxyCpp =
@"#include ""ServerProxy.h""
#include ""../Network/LanServer.h""
#include ""../Utils/Packet.h""
{0}";

        public static string proxyCppFunc =
@"
void ServerProxy::{0}(__int64 sessionId{1})
{{
	SPacket* packet = new SPacket();
	packet->Initialize();

	packet->GetHeaderPtr()->type_ = {2};
	*packet{3};

	server_->SendPacket(sessionId, packet);
}}
";

        public static string shiftLeft =
@" << {0}";

// -------------------------------------------------------------------

        public static string stubHeader =
@"#pragma once

class LanServer;
class Packet;

class ServerStub
{{
public:
	bool PacketProc(__int64 sessionId, Packet* packet);
{0}
}};
";

        public static string stubHeaderFunc =
@"
	virtual bool {0}(__int64 sessionId{1});";

// -------------------------------------------------------------------

        public static string stubCpp =
@"#include ""ServerStub.h""
#include ""../Utils/Packet.h""

bool ServerStub::PacketProc(__int64 sessionId, Packet* packet)
{{
	FIGHTER_HEADER header;
	header.type_ = packet->GetHeaderPtr()->type_;

	switch (header.type_)
	{{{0}
	default:
	{{
		// 로그 추가

		return false;
	}}
	}}
}}
{1}
";

        public static string stubCppFunc1 =
@"
bool ServerStub::{0}(__int64 sessionId{1})
{{
	return true;
}}
";

        public static string stubCppFunc2 =
@"
	case {0}: 
	{{
{1}
		*packet{2};

		return this->{3}(sessionId{4});
		break;
	}}
";

        public static string shiftRight =
@" >> {0}";

    }

}
