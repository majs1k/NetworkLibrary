namespace PacketGenerator
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var rpcFile = "myrpc_idl.txt";
            var dbFile = "mydb_idl.txt";

            RpcServerGenerator.Generate(rpcFile);
            RpcClientGenerator.Generate(rpcFile);
            DbGenerator.Generate(dbFile);
        }
    }
}
