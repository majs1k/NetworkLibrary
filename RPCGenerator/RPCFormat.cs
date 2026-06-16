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

class RPCProxy
{{
public:
	LanServer* server;

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
@"#include ""RPCProxy.h""
#include ""../Network/LanServer.h""
#include ""../Utils/Packet.h""
{0}";

        public static string proxyCppFunc =
@"
void RPCProxy::{0}(__int64 sessionId{1})
{{
	Packet packet;

	packet << static_cast<unsigned char>({2});
	packet{3};

	server->sendPacket(sessionId, packet);
}}
";

        public static string shiftLeft =
@" << {0}";

// -------------------------------------------------------------------

        public static string stubHeader =
@"#pragma once
#include ""../Utils/Singleton.h""

class Packet;

class IPacketHandler
{{
public:
	virtual ~IPacketHandler() = 0;
{0}
}};

class RPCStub
{{
private:
	IPacketHandler* handle_ = nullptr;

public:
	void initialize(IPacketHandler* handle);
	bool packetProc(__int64 sessionId, Packet& packet, unsigned char type);
}};
";

        public static string stubHeaderFunc =
@"
	virtual bool {0}(__int64 sessionId{1});";

// -------------------------------------------------------------------

        public static string stubCpp =
@"#include ""RPCStub.h""
#include ""../Utils/Packet.h""

IPacketHandler::~IPacketHandler()
{{

}}
{0}
void RPCStub::initialize(IPacketHandler* handle)
{{
	handle_ = handle;
}}

bool RPCStub::packetProc(__int64 sessionId, Packet& packet, unsigned char type)
{{
	switch (type)
	{{{1}
	default:
	{{
		// 로그 추가

		return false;
	}}
	}}
}}
";

        public static string stubCppFunc1 =
@"
bool IPacketHandler::{0}(__int64 sessionId{1})
{{
	return true;
}}
";

        public static string stubCppFunc2 =
@"
	case {0}: 
	{{
{1}
		packet{2};

		return handle_->{3}(sessionId{4});
		break;
	}}
";

        public static string shiftRight =
@" >> {0}";

    }

}
