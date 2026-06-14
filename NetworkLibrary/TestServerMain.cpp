//#include <conio.h>
//
//#include "./Network/LanServer.h"
//#include "./Network/TestServer.h"
//
//#include "./Contents/PacketHandler.h"
//#include "./Contents/Player.h"
//
//#include "./Utils/ConfigLoader.h"
//#include "./Utils/CrashDump.h"
//
//#define CONFIG_FILENAME			L"testconfig.json"
//
//int main()
//{
//	CrashDump().Initialize();
//
//	ConfigLoader::getInstance().load(CONFIG_FILENAME);
//
//	RPCStub::getInstance().initialize(new PacketHandler());
//
//	server = new TestServer();
//
//	server->start(config.ip, config.port, config.sessionMax, config.concurrentCount, config.workerCount);
//
//	while (1)
//	{
//		int c = _getch();
//
//		if (c == 'x')
//			break;
//
//		if (c == 'p')
//			server->printTps();
//	}
//	
//	server->stop();
//
//	return 0;
//}
