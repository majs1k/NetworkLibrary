using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
    class RPCFormat
    {
        public static string proxyHeader =
@"#pragma once

class RPCProxy
{{
public:{0}
}};
";

        public static string proxyHeaderFunc =
@"
	static void {0}(__int64 sessionId{1});";

        public static string funcParam =
@"{0} {1}";

        public static string proxyCpp =
@"#include ""RPCProxy.h""
#include ""../Network/LanServer.h""
{0}";

        public static string proxyCppFunc =
@"
void RPCProxy::{0}(__int64 sessionId{1})
{{
	Packet packet;
	int size = 0{2};

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char){3};
	packet{4};

	server->sendPacket(sessionId, packet);
}}
";

        public static string sizeOf =
@" + sizeof({0})";

        public static string shiftLeft =
@" << {0}";



        public static string stubHeader =
@"#pragma once
#include ""../Utils/Singleton.h""

class Session;
class Packet;

class IPacketHandler
{{
public:
	virtual ~IPacketHandler() = 0;
{0}
}};

class RPCStub : public Singleton<RPCStub>
{{
private:
	IPacketHandler* handle_ = nullptr;

public:
	void initialize(IPacketHandler* handle);
	bool packetProc(__int64 sessionId, Packet& packet, int type);
}};
";

        public static string stubHeaderFunc =
@"
	virtual bool {0}(__int64 sessionId{1});";





        public static string stubCpp =
@"#include ""RPCStub.h""

IPacketHandler::~IPacketHandler()
{{

}}
{0}
void RPCStub::initialize(IPacketHandler* handle)
{{
	handle_ = handle;
}}

bool RPCStub::packetProc(__int64 sessionId, Packet& packet, int type)
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
