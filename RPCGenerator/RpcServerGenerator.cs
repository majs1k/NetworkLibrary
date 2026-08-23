using PacketGenerator;

class RpcServerGenerator
{
    public static void Generate(string readFile)
    {
        GenerateRpcServerProxy(readFile);
        GenerateRpcServerStub(readFile);
    }


    // ================================================================
    // RpcServerProxy
    // ================================================================

    public static void GenerateRpcServerProxy(string readFile)
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

                    funcParam += ", ";
                    funcParam += string.Format(
                        RpcServerFormat.funcParam,
                        p.type,
                        p.name);


                    // ------------------------------------------------
                    // Packet << 변수
                    // ------------------------------------------------

                    shiftParam += string.Format(
                        RpcServerFormat.shiftLeft,
                        p.name);
                }


                // ====================================================
                // .h
                // ====================================================

                proxyHeaderFunc += string.Format(
                    RpcServerFormat.proxyHeaderFunc,
                    parsed.Name,
                    funcParam);


                // ====================================================
                // .cpp
                // ====================================================

                proxyCppFunc += string.Format(
                    RpcServerFormat.proxyCppFunc,
                    parsed.Name,
                    funcParam,
                    parsed.PacketType,
                    shiftParam);
            }


            // ========================================================
            // RpcServerProxy.h
            // ========================================================

            string proxyHeader = string.Format(
                RpcServerFormat.proxyHeader,
                proxyHeaderFunc);

            File.WriteAllText(
                "RpcServerProxy.h",
                proxyHeader);


            // ========================================================
            // RpcServerProxy.cpp
            // ========================================================

            string proxyCpp =
                RpcServerFormat.proxyCppHeader +
                Environment.NewLine +
                proxyCppFunc;

            File.WriteAllText(
                "RpcServerProxy.cpp",
                proxyCpp);
        }
    }


    // ================================================================
    // RpcServerStub
    // ================================================================

    public static void GenerateRpcServerStub(string readFile)
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
                    // virtual bool Func(..., std::string& s);
                    // =================================================

                    funcParam += ", ";
                    funcParam += string.Format(
                        RpcServerFormat.funcParam,
                        p.type,
                        p.name);


                    // =================================================
                    // 함수 호출용
                    //
                    // 결과:
                    // this->Func(..., s);
                    // =================================================

                    funcParam1 += ", ";
                    funcParam1 += p.name;


                    // =================================================
                    // Packet 역직렬화용 지역 변수 선언
                    //
                    // 핵심:
                    //
                    // std::string&  -> std::string
                    // std::list<int>& -> std::list<int>
                    //
                    // 여기서만 &를 제거한다.
                    // =================================================

                    string localType = p.type
                        .Replace("&", "")
                        .Trim();

                    funcParam2 += "\t\t\t";

                    funcParam2 += string.Format(
                        RpcServerFormat.funcParam,
                        localType,
                        p.name);

                    funcParam2 += ";";
                    funcParam2 += Environment.NewLine;


                    // =================================================
                    // Packet >> 변수
                    // =================================================

                    shiftParam += string.Format(
                        RpcServerFormat.shiftRight,
                        p.name);
                }


                // ====================================================
                // .h
                // ====================================================

                stubHeaderFunc += string.Format(
                    RpcServerFormat.stubHeaderFunc,
                    parsed.Name,
                    funcParam);


                // ====================================================
                // PacketProc의 case
                // ====================================================

                stubPacketProc += string.Format(
                    RpcServerFormat.stubPacketProcCase,
                    parsed.PacketType,
                    funcParam2,
                    shiftParam,
                    parsed.Name,
                    funcParam1);


                // ====================================================
                // .cpp의 RPC 함수 기본 구현
                // ====================================================

                stubCppFunc += string.Format(
                    RpcServerFormat.stubCppFunc,
                    parsed.Name,
                    funcParam);
            }


            // ========================================================
            // RpcServerStub.h
            // ========================================================

            string stubHeader = string.Format(
                RpcServerFormat.stubHeader,
                stubHeaderFunc);

            File.WriteAllText(
                "RpcServerStub.h",
                stubHeader);


            // ========================================================
            // RpcServerStub.cpp
            // ========================================================

            string stubCpp =
                RpcServerFormat.stubCppHeader +
                Environment.NewLine +
                string.Format(
                    RpcServerFormat.stubCppPacketProc,
                    stubPacketProc) +
                Environment.NewLine +
                stubCppFunc;

            File.WriteAllText(
                "RpcServerStub.cpp",
                stubCpp);
        }
    }
}
