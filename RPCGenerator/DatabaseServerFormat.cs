using System;

namespace PacketGenerator
{
    class DatabaseServerFormat
    {
        // ============================================================
        // Proxy
        // ============================================================

        public static string proxyHeader =
@"#pragma once

#include ""../Utils/PacketQueue.h""
#include ""../Utils/Packet.h""

class DatabaseServerProxy
{{
public:
    PacketQueue* queue_;

public:{0}
}};
";

        // Header에는 함수 선언만
        public static string proxyHeaderFunc =
@"
    void {0}(__int64 sessionId{1});";

        // CPP에 들어갈 실제 구현
        public static string proxyCppFunc =
@"
void DatabaseServerProxy::{0}(__int64 sessionId{1})
{{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = {2};
    *packet{3};

    queue_->Push(packet);
}}
";

        public static string proxyCppHeader =
@"#include ""DatabaseServerProxy.h""
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

class DatabaseServerHandler
{{
public:{0}
}};

class DatabaseServerStub
{{
public:
    DatabaseServerHandler* handler_;

public:
    bool DbPacketProc(__int64 sessionId, Packet* packet);
}};
";

        // Header에는 함수 선언만
        public static string stubHeaderFunc =
@"
    virtual bool {0}(__int64 sessionId{1});";

        // PacketProc 구현
        public static string stubCppPacketProc =
@"
bool DatabaseServerStub::DbPacketProc(__int64 sessionId, Packet* packet)
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

        // 각 Database 함수의 기본 구현
        public static string stubCppFunc =
@"
bool DatabaseServerHandler::{0}(__int64 sessionId{1})
{{
    return true;
}}
";

        public static string stubCppHeader =
@"#include ""DatabaseServerStub.h""
";

        public static string stubPacketProcCase =
@"
        case {0}:
        {{
{1}
            *packet{2};

            return handler_->{3}(sessionId{4});
        }}";

        public static string shiftRight =
@" >> {0}";
    }
}