using PacketGenerator;

class RpcServerGenerator
{
    public static void Generate(string readFile)
    {
        GenerateRpcServerProxy(readFile);
        GenerateRpcServerStub(readFile);
    }

    public static void GenerateRpcServerProxy(string readFile)
    {
        using (var reader = new StreamReader(readFile))
        {
            string proxyFunc = "";
            string line;

            while ((line = reader.ReadLine()) != null)
            {
                if (string.IsNullOrWhiteSpace(line))
                    continue;

                var parsed = IDLParser.ParseLine(line);

                string funcParam = "";
                string shiftParam = "";

                foreach (var p in parsed.Parameters)
                {
                    funcParam += ", ";
                    funcParam += string.Format(RpcServerFormat.funcParam, p.type, p.name);

                    shiftParam += string.Format(RpcServerFormat.shiftLeft, p.name);
                }

                proxyFunc += string.Format(
                    RpcServerFormat.proxyHeaderFunc,
                    parsed.Name,
                    funcParam,
                    parsed.PacketType,
                    shiftParam);
            }

            string proxyHeader = string.Format(
                RpcServerFormat.proxyHeader,
                proxyFunc);

            File.WriteAllText("RpcServerProxy.h", proxyHeader);
        }
    }

    public static void GenerateRpcServerStub(string readFile)
    {
        using (var reader = new StreamReader(readFile))
        {
            string stubFunc = "";
            string stubPacketProc = "";

            string line;

            while ((line = reader.ReadLine()) != null)
            {
                if (string.IsNullOrWhiteSpace(line))
                    continue;

                var parsed = IDLParser.ParseLine(line);

                string funcParam = "";
                string funcParam1 = "";
                string funcParam2 = "";
                string shiftParam = "";

                foreach (var p in parsed.Parameters)
                {
                    funcParam += ", ";
                    funcParam += string.Format(
                        RpcServerFormat.funcParam,
                        p.type,
                        p.name);

                    funcParam1 += ", ";
                    funcParam1 += p.name;

                    funcParam2 += "\t\t\t";
                    funcParam2 += string.Format(
                        RpcServerFormat.funcParam,
                        p.type,
                        p.name);

                    funcParam2 += ";";
                    funcParam2 += Environment.NewLine;

                    shiftParam += string.Format(
                        RpcServerFormat.shiftRight,
                        p.name);
                }

                stubFunc += string.Format(
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
            }

            string stubHeader = string.Format(
                RpcServerFormat.stubHeader,
                stubPacketProc,
                stubFunc);

            File.WriteAllText("RpcServerStub.h", stubHeader);
        }
    }
}
