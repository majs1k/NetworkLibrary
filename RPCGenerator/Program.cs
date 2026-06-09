namespace PacketGenerator
{
    internal class Program
    {
        static void Main(string[] args)
        {
            // TODO: args로 readfile 입력하는 방식으로 변경
            var idlFile = "server.idl";

            RPCGenerator.Generate(idlFile);
        }
    }
}
