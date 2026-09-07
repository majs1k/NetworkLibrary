using PacketGenerator;

class DbGenerator
{
    public static void Generate(string readFile)
    {
        GenerateDbServerProxy(readFile);
        GenerateDbServerStub(readFile);
    }

    // DatabaseServerProxy
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
                    funcParam += ", ";
                    funcParam += string.Format(
                        DbFormat.funcParam,
                        p.type,
                        p.name);

                    shiftParam += string.Format(
                        DbFormat.shiftLeft,
                        p.name);
                }

                string queueName;

                if (parsed.Name.StartsWith("Req"))
                {
                    queueName = "dbReqQueue_";
                }
                else if (parsed.Name.StartsWith("Res"))
                {
                    queueName = "dbResQueue_";
                }
                else
                {
                    throw new Exception(
                        $"DatabaseServer 함수 이름은 Req 또는 Res로 시작해야 합니다. : {parsed.Name}");
                }

                proxyHeaderFunc += string.Format(
                    DbFormat.proxyHeaderFunc,
                    parsed.Name + "DB",
                    funcParam);

                proxyCppFunc += string.Format(
                    DbFormat.proxyCppFunc,
                    parsed.Name + "DB",
                    funcParam,
                    parsed.PacketType,
                    shiftParam,
                    queueName);
            }

            string proxyHeader = string.Format(
                DbFormat.proxyHeader,
                proxyHeaderFunc);

            File.WriteAllText(
                "DbProxy.h",
                proxyHeader);

            string proxyCpp =
                DbFormat.proxyCppHeader +
                Environment.NewLine +
                proxyCppFunc;

            File.WriteAllText(
                "DbProxy.cpp",
                proxyCpp);
        }
    }

    // DatabaseServerStub
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
                    funcParam += ", ";
                    funcParam += string.Format(
                        DbFormat.funcParam,
                        p.type,
                        p.name);

                    funcParam1 += ", ";
                    funcParam1 += p.name;

                    string localType = p.type
                        .Replace("&", "")
                        .Trim();

                    funcParam2 += "\t\t\t";

                    funcParam2 += string.Format(
                        DbFormat.funcParam,
                        localType,
                        p.name);

                    funcParam2 += ";";
                    funcParam2 += Environment.NewLine;

                    shiftParam += string.Format(
                        DbFormat.shiftRight,
                        p.name);
                }

                stubHeaderFunc += string.Format(
                    DbFormat.stubHeaderFunc,
                    parsed.Name + "DB",
                    funcParam);

                stubPacketProc += string.Format(
                    DbFormat.stubPacketProcCase,
                    parsed.PacketType,
                    funcParam2,
                    shiftParam,
                    parsed.Name + "DB",
                    funcParam1);

                stubCppFunc += string.Format(
                    DbFormat.stubCppFunc,
                    parsed.Name + "DB",
                    funcParam);
            }

            string stubHeader = string.Format(
                DbFormat.stubHeader,
                stubHeaderFunc);

            File.WriteAllText(
                "DbStub.h",
                stubHeader);

            string stubCpp =
                DbFormat.stubCppHeader +
                Environment.NewLine +
                string.Format(
                    DbFormat.stubCppPacketProc,
                    stubPacketProc) +
                Environment.NewLine +
                stubCppFunc;

            File.WriteAllText(
                "DbStub.cpp",
                stubCpp);
        }
    }
}
