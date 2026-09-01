using System;

namespace PacketGenerator
{
    class DbFormat
    {
        // ============================================================
        // Proxy
        // ============================================================

        public static string proxyHeader =
        @"#pragma once

#include ""../Utils/PacketQueue.h""
#include ""../Utils/Packet.h""

class DbProxy
{{
public:
    PacketQueue* dbReqQueue_;
    PacketQueue* dbResQueue_;

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
void DbProxy::{0}(__int64 sessionId{1})
{{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = {2};
    *packet{3};

    {4}->Push(packet);
}}
";

        public static string proxyCppHeader =
@"#include ""DbProxy.h""
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

class DbStub
{{
public:

    bool DbPacketProc(__int64 sessionId, Packet* packet);
    {0}
}};
";

        // Header에는 함수 선언만
        public static string stubHeaderFunc =
@"
    virtual bool {0}(__int64 sessionId{1});";

        // PacketProc 구현
        public static string stubCppPacketProc =
@"
bool DbStub::DbPacketProc(__int64 sessionId, Packet* packet)
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
bool DbStub::{0}(__int64 sessionId{1})
{{
    return true;
}}
";

        public static string stubCppHeader =
@"#include ""DbStub.h""
";

        public static string stubPacketProcCase =
@"
        case {0}:
        {{
{1}
            *packet{2};

            return {3}(sessionId{4});
        }}";

        public static string shiftRight =
@" >> {0}";
    }
}
