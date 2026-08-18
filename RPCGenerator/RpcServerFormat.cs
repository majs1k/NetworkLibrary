using System;

namespace PacketGenerator
{
    class RpcServerFormat
    {
        public static string proxyHeader =
@"#pragma once

#include ""../Network/LanServer.h""
#include ""../Utils/Packet.h""

class RpcServerProxy
{{
public:
	LanServer* server_;

public:{0}
}};
";

        public static string proxyHeaderFunc =
@"
	void {0}(__int64 sessionId{1})
	{{
		Packet* packet = new Packet();
		packet->Initialize();

		packet->GetHeaderPtr()->type_ = {2};
		*packet{3};

		server_->SendPacket(sessionId, packet);
	}}";

        public static string funcParam =
@"{0} {1}";


        public static string shiftLeft =
@" << {0}";

        // -------------------------------------------------------------------

        public static string stubHeader =
@"#pragma once

#include ""../Utils/Packet.h""

class RpcServerStub
{{
public:
	bool PacketProc(__int64 sessionId, Packet* packet)
	{{
		switch (packet->GetHeaderPtr()->type_)
		{{{0}
		default:
		{{
			// 로그 추가

			return false;
		}}
		}}
	}}
{1}
}};
";

        public static string stubHeaderFunc =
@"
	virtual bool {0}(__int64 sessionId{1})
	{{
		return true;
	}}";

        public static string stubPacketProcCase =
@"
		case {0}: 
		{{
{1}
			*packet{2};

			return this->{3}(sessionId{4});
			break;
		}}";

        public static string shiftRight =
@" >> {0}";
    }
}