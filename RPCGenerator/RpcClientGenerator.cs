using PacketGenerator;

class RpcClientGenerator
{
    public static void Generate(string readFile)
    {
        GenerateRpcClientProxy(readFile);
        GenerateRpcClientStub(readFile);
    }

    // RpcClientProxy
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
                    if (funcParam.Length > 0)
                        funcParam += ", ";

                    funcParam += string.Format(
                        RpcClientFormat.funcParam,
                        p.type,
                        p.name);

                    shiftParam += string.Format(
                        RpcClientFormat.shiftLeft,
                        p.name);
                }

                proxyHeaderFunc += string.Format(
                    RpcClientFormat.proxyHeaderFunc,
                    parsed.Name,
                    funcParam);

                proxyCppFunc += string.Format(
                    RpcClientFormat.proxyCppFunc,
                    parsed.Name,
                    funcParam,
                    parsed.PacketType,
                    shiftParam);
            }

            string proxyHeader = string.Format(
                RpcClientFormat.proxyHeader,
                proxyHeaderFunc);

            File.WriteAllText(
                "RpcClientProxy.h",
                proxyHeader);

            string proxyCpp =
                RpcClientFormat.proxyCppHeader +
                Environment.NewLine +
                proxyCppFunc;

            File.WriteAllText(
                "RpcClientProxy.cpp",
                proxyCpp);
        }
    }

    // RpcClientStub
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
                    if (funcParam.Length > 0)
                        funcParam += ", ";

                    funcParam += string.Format(
                        RpcClientFormat.funcParam,
                        p.type,
                        p.name);

                    if (funcParam1.Length > 0)
                        funcParam1 += ", ";

                    funcParam1 += p.name;

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

                    shiftParam += string.Format(
                        RpcClientFormat.shiftRight,
                        p.name);
                }

                stubHeaderFunc += string.Format(
                    RpcClientFormat.stubHeaderFunc,
                    parsed.Name,
                    funcParam);

                stubPacketProc += string.Format(
                    RpcClientFormat.stubPacketProcCase,
                    parsed.PacketType,
                    funcParam2,
                    shiftParam,
                    parsed.Name,
                    funcParam1);

                stubCppFunc += string.Format(
                    RpcClientFormat.stubCppFunc,
                    parsed.Name,
                    funcParam);
            }

            string stubHeader = string.Format(
                RpcClientFormat.stubHeader,
                stubHeaderFunc);

            File.WriteAllText(
                "RpcClientStub.h",
                stubHeader);

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
