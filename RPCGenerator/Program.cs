namespace PacketGenerator
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var idlFile = "my_idl.txt";

            RpcServerGenerator.Generate(idlFile);
            RpcClientGenerator.Generate(idlFile);
            DatabaseServerGenerator.Generate(idlFile);
        }
    }
}
