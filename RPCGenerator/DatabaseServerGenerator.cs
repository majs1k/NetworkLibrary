using PacketGenerator;

class DatabaseServerGenerator
{
    public static void Generate(string readFile)
    {
        GenerateDbServerProxy(readFile);
        GenerateDbServerStub(readFile);
    }


    // ================================================================
    // DatabaseServerProxy
    // ================================================================

    public static void GenerateDbServerProxy(string readFile)
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
                    // ------------------------------------------------

                    funcParam += ", ";
                    funcParam += string.Format(
                        DatabaseServerFormat.funcParam,
                        p.type,
                        p.name);


                    // ------------------------------------------------
                    // Packet << 변수
                    // ------------------------------------------------

                    shiftParam += string.Format(
                        DatabaseServerFormat.shiftLeft,
                        p.name);
                }


                // ====================================================
                // Req / Res에 따른 Queue 결정
                // ====================================================

                string queueName;

                if (parsed.Name.StartsWith("Req"))
                {
                    queueName = "dbQueue_";
                }
                else if (parsed.Name.StartsWith("Res"))
                {
                    queueName = "logicQueue_";
                }
                else
                {
                    throw new Exception(
                        $"DatabaseServer 함수 이름은 Req 또는 Res로 시작해야 합니다. : {parsed.Name}");
                }


                // ====================================================
                // .h
                // ====================================================

                proxyHeaderFunc += string.Format(
                    DatabaseServerFormat.proxyHeaderFunc,
                    parsed.Name + "DB",
                    funcParam);


                // ====================================================
                // .cpp
                // ====================================================

                proxyCppFunc += string.Format(
                    DatabaseServerFormat.proxyCppFunc,
                    parsed.Name + "DB",
                    funcParam,
                    parsed.PacketType,
                    shiftParam,
                    queueName);
            }


            // ========================================================
            // DatabaseServerProxy.h
            // ========================================================

            string proxyHeader = string.Format(
                DatabaseServerFormat.proxyHeader,
                proxyHeaderFunc);

            File.WriteAllText(
                "DatabaseServerProxy.h",
                proxyHeader);


            // ========================================================
            // DatabaseServerProxy.cpp
            // ========================================================

            string proxyCpp =
                DatabaseServerFormat.proxyCppHeader +
                Environment.NewLine +
                proxyCppFunc;

            File.WriteAllText(
                "DatabaseServerProxy.cpp",
                proxyCpp);
        }
    }


    // ================================================================
    // DatabaseServerStub
    // ================================================================

    public static void GenerateDbServerStub(string readFile)
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
                    // =================================================

                    funcParam += ", ";
                    funcParam += string.Format(
                        DatabaseServerFormat.funcParam,
                        p.type,
                        p.name);


                    // =================================================
                    // 함수 호출용
                    // =================================================

                    funcParam1 += ", ";
                    funcParam1 += p.name;


                    // =================================================
                    // Packet 역직렬화용 지역 변수 선언
                    // =================================================

                    string localType = p.type
                        .Replace("&", "")
                        .Trim();

                    funcParam2 += "\t\t\t";

                    funcParam2 += string.Format(
                        DatabaseServerFormat.funcParam,
                        localType,
                        p.name);

                    funcParam2 += ";";
                    funcParam2 += Environment.NewLine;


                    // =================================================
                    // Packet >> 변수
                    // =================================================

                    shiftParam += string.Format(
                        DatabaseServerFormat.shiftRight,
                        p.name);
                }


                // ====================================================
                // .h
                // ====================================================

                stubHeaderFunc += string.Format(
                    DatabaseServerFormat.stubHeaderFunc,
                    parsed.Name + "DB",
                    funcParam);


                // ====================================================
                // PacketProc의 case
                // ====================================================

                stubPacketProc += string.Format(
                    DatabaseServerFormat.stubPacketProcCase,
                    parsed.PacketType,
                    funcParam2,
                    shiftParam,
                    parsed.Name + "DB",
                    funcParam1);


                // ====================================================
                // .cpp의 Database 함수 기본 구현
                // ====================================================

                stubCppFunc += string.Format(
                    DatabaseServerFormat.stubCppFunc,
                    parsed.Name + "DB",
                    funcParam);
            }


            // ========================================================
            // DatabaseServerStub.h
            // ========================================================

            string stubHeader = string.Format(
                DatabaseServerFormat.stubHeader,
                stubHeaderFunc);

            File.WriteAllText(
                "DatabaseServerStub.h",
                stubHeader);


            // ========================================================
            // DatabaseServerStub.cpp
            // ========================================================

            string stubCpp =
                DatabaseServerFormat.stubCppHeader +
                Environment.NewLine +
                string.Format(
                    DatabaseServerFormat.stubCppPacketProc,
                    stubPacketProc) +
                Environment.NewLine +
                stubCppFunc;

            File.WriteAllText(
                "DatabaseServerStub.cpp",
                stubCpp);
        }
    }
}
