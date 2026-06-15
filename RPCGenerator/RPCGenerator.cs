using PacketGenerator;

class RPCGenerator
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
                    funcParam += string.Format(RPCFormat.funcParam, p.type, p.name);
                }

                proxyFunc += string.Format(RPCFormat.proxyHeaderFunc, parsed.Name, funcParam);
            }

            string proxyHeader = string.Format(RPCFormat.proxyHeader, proxyFunc);
            File.WriteAllText("RPCProxy.h", proxyHeader);
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
                    funcParam += string.Format(RPCFormat.funcParam, p.type, p.name);
                    shiftParam += string.Format(RPCFormat.shiftLeft, p.name);
                }

                proxyFunc += string.Format(RPCFormat.proxyCppFunc,
                    parsed.Name, funcParam, parsed.PacketType, shiftParam);
            }

            string proxyCpp = string.Format(RPCFormat.proxyCpp, proxyFunc);
            File.WriteAllText("RPCProxy.cpp", proxyCpp);
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
                    funcParam += string.Format(RPCFormat.funcParam, p.type, p.name);
                }

                stubFunc += string.Format(RPCFormat.stubHeaderFunc, parsed.Name, funcParam);
            }

            string stubHeader = string.Format(RPCFormat.stubHeader, stubFunc);
            File.WriteAllText("RPCStub.h", stubHeader);
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
                    funcParam += string.Format(RPCFormat.funcParam, p.type, p.name);

                    funcParam1 += ", ";
                    funcParam1 += p.name;

                    funcParam2 += "\t\t";
                    funcParam2 += string.Format(RPCFormat.funcParam, p.type, p.name);
                    funcParam2 += ";" + Environment.NewLine;

                    shiftParam += string.Format(RPCFormat.shiftRight, p.name);
                }

                stubFunc1 += string.Format(RPCFormat.stubCppFunc1, parsed.Name, funcParam);
                stubFunc2 += string.Format(RPCFormat.stubCppFunc2,
                    parsed.PacketType, funcParam2, shiftParam, parsed.Name, funcParam1);
            }

            string stubCpp = string.Format(RPCFormat.stubCpp, stubFunc1, stubFunc2);
            File.WriteAllText("RPCStub.cpp", stubCpp);
        }
    }
}
