using PacketGenerator;

class RpcClientGenerator
{
    public static void Generate(string readFile)
    {
        GenerateRpcClientProxy(readFile);
        GenerateRpcClientStub(readFile);
    }


    // ================================================================
    // RpcClientProxy
    // ================================================================

    public static void GenerateRpcClientProxy(string readFile)
    {
        using (var reader = new StreamReader(readFile))
        {
            string proxyHeaderFunc = "";
            string proxyCppFunc = "";

            string line;

            while ((line = reader.ReadLine()) != null)
            {
                if (string.IsNullOrWhiteSpace(line))
                    continue;

                var parsed = Parser.ParseLine(line);

                string funcParam = "";
                string shiftParam = "";

                foreach (var p in parsed.Parameters)
                {
                    // ------------------------------------------------
                    // 함수 파라미터
                    //
                    // 예:
                    // std::string& s
                    // std::list<int>& lt
                    // ------------------------------------------------

                    if (funcParam.Length > 0)
                        funcParam += ", ";

                    funcParam += string.Format(
                        RpcClientFormat.funcParam,
                        p.type,
                        p.name);


                    // ------------------------------------------------
                    // Packet << 변수
                    // ------------------------------------------------

                    shiftParam += string.Format(
                        RpcClientFormat.shiftLeft,
                        p.name);
                }


                // ====================================================
                // Header
                // ====================================================

                proxyHeaderFunc += string.Format(
                    RpcClientFormat.proxyHeaderFunc,
                    parsed.Name,
                    funcParam);


                // ====================================================
                // CPP
                // ====================================================

                proxyCppFunc += string.Format(
                    RpcClientFormat.proxyCppFunc,
                    parsed.Name,
                    funcParam,
                    parsed.PacketType,
                    shiftParam);
            }


            // ========================================================
            // RpcClientProxy.h
            // ========================================================

            string proxyHeader = string.Format(
                RpcClientFormat.proxyHeader,
                proxyHeaderFunc);

            File.WriteAllText(
                "RpcClientProxy.h",
                proxyHeader);


            // ========================================================
            // RpcClientProxy.cpp
            // ========================================================

            string proxyCpp =
                RpcClientFormat.proxyCppHeader +
                Environment.NewLine +
                proxyCppFunc;

            File.WriteAllText(
                "RpcClientProxy.cpp",
                proxyCpp);
        }
    }


    // ================================================================
    // RpcClientStub
    // ================================================================

    public static void GenerateRpcClientStub(string readFile)
    {
        using (var reader = new StreamReader(readFile))
        {
            string stubHeaderFunc = "";
            string stubPacketProc = "";
            string stubCppFunc = "";

            string line;

            while ((line = reader.ReadLine()) != null)
            {
                if (string.IsNullOrWhiteSpace(line))
                    continue;

                var parsed = Parser.ParseLine(line);

                string funcParam = "";
                string funcParam1 = "";
                string funcParam2 = "";
                string shiftParam = "";

                foreach (var p in parsed.Parameters)
                {
                    // =================================================
                    // 함수 선언용
                    //
                    // IDL:
                    // std::string& s
                    //
                    // 결과:
                    // virtual bool Func(std::string& s);
                    //
                    // &는 유지한다.
                    // =================================================

                    if (funcParam.Length > 0)
                        funcParam += ", ";

                    funcParam += string.Format(
                        RpcClientFormat.funcParam,
                        p.type,
                        p.name);


                    // =================================================
                    // 함수 호출용
                    //
                    // 결과:
                    // this->Func(s);
                    //
                    // 타입은 필요 없다.
                    // =================================================

                    if (funcParam1.Length > 0)
                        funcParam1 += ", ";

                    funcParam1 += p.name;


                    // =================================================
                    // Packet 역직렬화용 지역 변수 선언
                    //
                    // IDL:
                    // std::string& s
                    // std::list<int>& lt
                    //
                    // 결과:
                    // std::string s;
                    // std::list<int> lt;
                    //
                    // 여기서만 &를 제거한다.
                    // =================================================

                    string localType = p.type
                        .Replace("&", "")
                        .Trim();

                    funcParam2 += "\t\t\t";

                    funcParam2 += string.Format(
                        RpcClientFormat.funcParam,
                        localType,
                        p.name);

                    funcParam2 += ";";
                    funcParam2 += Environment.NewLine;


                    // =================================================
                    // Packet >> 변수
                    // =================================================

                    shiftParam += string.Format(
                        RpcClientFormat.shiftRight,
                        p.name);
                }


                // ====================================================
                // Header
                // ====================================================

                stubHeaderFunc += string.Format(
                    RpcClientFormat.stubHeaderFunc,
                    parsed.Name,
                    funcParam);


                // ====================================================
                // PacketProc의 case
                // ====================================================

                stubPacketProc += string.Format(
                    RpcClientFormat.stubPacketProcCase,
                    parsed.PacketType,
                    funcParam2,
                    shiftParam,
                    parsed.Name,
                    funcParam1);


                // ====================================================
                // CPP의 RPC 함수 구현
                // ====================================================

                stubCppFunc += string.Format(
                    RpcClientFormat.stubCppFunc,
                    parsed.Name,
                    funcParam);
            }


            // ========================================================
            // RpcClientStub.h
            // ========================================================

            string stubHeader = string.Format(
                RpcClientFormat.stubHeader,
                stubHeaderFunc);

            File.WriteAllText(
                "RpcClientStub.h",
                stubHeader);


            // ========================================================
            // RpcClientStub.cpp
            // ========================================================

            string stubCpp =
                RpcClientFormat.stubCppHeader +
                Environment.NewLine +
                string.Format(
                    RpcClientFormat.stubCppPacketProc,
                    stubPacketProc) +
                Environment.NewLine +
                stubCppFunc;

            File.WriteAllText(
                "RpcClientStub.cpp",
                stubCpp);
        }
    }
}