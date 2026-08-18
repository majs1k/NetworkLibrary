using PacketGenerator;

class RpcGenerator
{
    public static void Generate(string readFile)
    {
        GenerateProxyHeader(readFile);
        GenerateProxyCpp(readFile);
        GenerateStubHeader(readFile);
        GenerateStubCpp(readFile);
    }

    public static void GenerateProxyHeader(string readFile)
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

                foreach (var p in parsed.Parameters)
                {
                    funcParam += ", ";
                    funcParam += string.Format(RpcFormat.funcParam, p.type, p.name);
                }

                proxyFunc += string.Format(RpcFormat.proxyHeaderFunc, parsed.Name, funcParam);
            }

            string proxyHeader = string.Format(RpcFormat.proxyHeader, proxyFunc);
            File.WriteAllText("ServerProxy.h", proxyHeader);
        }
    }

    public static void GenerateProxyCpp(string readFile)
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
                    funcParam += string.Format(RpcFormat.funcParam, p.type, p.name);
                    shiftParam += string.Format(RpcFormat.shiftLeft, p.name);
                }

                proxyFunc += string.Format(RpcFormat.proxyCppFunc,
                    parsed.Name, funcParam, parsed.PacketType, shiftParam);
            }

            string proxyCpp = string.Format(RpcFormat.proxyCpp, proxyFunc);
            File.WriteAllText("ServerProxy.cpp", proxyCpp);
        }
    }

    public static void GenerateStubHeader(string readFile)
    {
        using (var reader = new StreamReader(readFile))
        {
            string stubFunc = "";
            string line;

            while ((line = reader.ReadLine()) != null)
            {
                if (string.IsNullOrWhiteSpace(line))
                    continue;

                var parsed = IDLParser.ParseLine(line);

                string funcParam = "";

                foreach (var p in parsed.Parameters)
                {
                    funcParam += ", ";
                    funcParam += string.Format(RpcFormat.funcParam, p.type, p.name);
                }

                stubFunc += string.Format(RpcFormat.stubHeaderFunc, parsed.Name, funcParam);
            }

            string stubHeader = string.Format(RpcFormat.stubHeader, stubFunc);
            File.WriteAllText("IRpcStub.h", stubHeader);
        }
    }

    public static void GenerateStubCpp(string readFile)
    {
        using (var reader = new StreamReader(readFile))
        {
            string stubFunc1 = "";
            string stubFunc2 = "";
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
                string proxySize = "";

                foreach (var p in parsed.Parameters)
                {
                    funcParam += ", ";
                    funcParam += string.Format(RpcFormat.funcParam, p.type, p.name);

                    funcParam1 += ", ";
                    funcParam1 += p.name;

                    funcParam2 += "\t\t";
                    funcParam2 += string.Format(RpcFormat.funcParam, p.type, p.name);
                    funcParam2 += ";" + Environment.NewLine;

                    shiftParam += string.Format(RpcFormat.shiftRight, p.name);
                }

                stubFunc1 += string.Format(RpcFormat.stubCppFunc1, parsed.Name, funcParam);
                stubFunc2 += string.Format(RpcFormat.stubCppFunc2,
                    parsed.PacketType, funcParam2, shiftParam, parsed.Name, funcParam1);
            }

            string stubCpp = string.Format(RpcFormat.stubCpp, stubFunc2, stubFunc1);
            File.WriteAllText("IRpcStub.cpp", stubCpp);
        }
    }
}
