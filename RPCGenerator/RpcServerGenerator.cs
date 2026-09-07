using PacketGenerator;

class RpcServerGenerator
{
    public static void Generate(string readFile)
    {
        GenerateRpcServerProxy(readFile);
        GenerateRpcServerStub(readFile);
    }

    // RpcServerProxy
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
                    funcParam += ", ";
                    funcParam += string.Format(RpcServerFormat.funcParam, p.type, p.name);
                    shiftParam += string.Format(RpcServerFormat.shiftLeft, p.name);
                }

                proxyHeaderFunc += string.Format(RpcServerFormat.proxyHeaderFunc, parsed.Name, funcParam);

                proxyCppFunc += string.Format(
                    RpcServerFormat.proxyCppFunc,
                    parsed.Name,
                    funcParam,
                    parsed.PacketType,
                    shiftParam);
            }

            string proxyHeader = string.Format(RpcServerFormat.proxyHeader, proxyHeaderFunc);

            File.WriteAllText("RpcServerProxy.h", proxyHeader);

            string proxyCpp =
                RpcServerFormat.proxyCppHeader +
                Environment.NewLine +
                proxyCppFunc;

            File.WriteAllText("RpcServerProxy.cpp", proxyCpp);
        }
    }

    // RpcServerStub
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
                    funcParam += ", ";
                    funcParam += string.Format(RpcServerFormat.funcParam, p.type, p.name);

                    funcParam1 += ", ";
                    funcParam1 += p.name;

                    string localType = p.type.Replace("&", "").Trim();

                    funcParam2 += "\t\t\t";
                    funcParam2 += string.Format(RpcServerFormat.funcParam, localType, p.name);
                    funcParam2 += ";";
                    funcParam2 += Environment.NewLine;

                    shiftParam += string.Format(RpcServerFormat.shiftRight, p.name);
                }

                stubHeaderFunc += string.Format(
                    RpcServerFormat.stubHeaderFunc,
                    parsed.Name,
                    funcParam);

                stubPacketProc += string.Format(
                    RpcServerFormat.stubPacketProcCase,
                    parsed.PacketType,
                    funcParam2,
                    shiftParam,
                    parsed.Name,
                    funcParam1);

                stubCppFunc += string.Format(
                    RpcServerFormat.stubCppFunc,
                    parsed.Name,
                    funcParam);
            }

            string stubHeader = string.Format(RpcServerFormat.stubHeader, stubHeaderFunc);

            File.WriteAllText("RpcServerStub.h", stubHeader);

            string stubCpp =
                RpcServerFormat.stubCppHeader +
                Environment.NewLine +
                string.Format(RpcServerFormat.stubCppPacketProc, stubPacketProc) +
                Environment.NewLine +
                stubCppFunc;

            File.WriteAllText("RpcServerStub.cpp", stubCpp);
        }
    }
}
