namespace PacketGenerator
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var rpcFile = "game_rpc_idl.txt";
            var dbFile = "game_db_idl.txt";

            RpcServerGenerator.Generate(rpcFile);
            RpcClientGenerator.Generate(rpcFile);
            DbGenerator.Generate(dbFile);
        }
    }
}
