using System;

namespace PacketGenerator
{
    class RpcClientFormat
    {
        // ============================================================
        // Proxy
        // ============================================================

        public static string proxyHeader =
@"#pragma once

#include ""../Network/LanClient.h""
#include ""../Utils/Packet.h""

class RpcClientProxy
{{
public:
    LanClient* client_;

public:{0}
}};
";

        // Header에는 함수 선언만
        public static string proxyHeaderFunc =
@"
    void {0}({1});";

        // CPP에는 함수 구현
        public static string proxyCppFunc =
@"
void RpcClientProxy::{0}({1})
{{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = {2};
    *packet{3};

    client_->SendPacket(packet);
}}
";

        public static string proxyCppHeader =
@"#include ""RpcClientProxy.h""
";

        public static string funcParam =
@"{0} {1}";

        public static string shiftLeft =
@" << {0}";


        // ============================================================
        // Stub
        // ============================================================

        public static string stubHeader =
@"#pragma once

#include ""../Utils/Packet.h""

class RpcClientStub
{{
public:

    bool PacketProc(Packet* packet);
    {0}
}};
";

        // Header에는 함수 선언만
        public static string stubHeaderFunc =
@"
    virtual bool {0}({1});";

        // CPP의 PacketProc 구현
        public static string stubCppPacketProc =
@"
bool RpcClientStub::PacketProc(Packet* packet)
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
";

        // CPP의 RPC 함수 구현
        public static string stubCppFunc =
@"
bool RpcClientStub::{0}({1})
{{
    return true;
}}
";

        public static string stubCppHeader =
@"#include ""RpcClientStub.h""
";

        public static string stubPacketProcCase =
@"
        case {0}:
        {{
{1}
            *packet{2};

            return {3}({4});
        }}";

        public static string shiftRight =
@" >> {0}";
    }
}
