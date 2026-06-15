//#include <conio.h>
//
//#include "./Network/LanServer.h"
//#include "./Network/FighterServer.h"
//
//#include "./Contents/PacketHandler.h"
//#include "./Contents/Player.h"
//#include "./Contents/LogicThread.h"
//
//#include "./Utils/ConfigLoader.h"
//#include "./Utils/CrashDump.h"
//
//#define CONFIG_FILENAME			L"fighterconfig.json"
//
//FighterServer server;
//
//int main()
//{
//	CrashDump().Initialize();
//
//	ConfigLoader::getInstance().load(CONFIG_FILENAME);
//
//	RPCStub::getInstance().initialize(new PacketHandler());
//
//	server.start(config.ip, config.port, config.sessionMax, config.concurrentCount, config.workerCount);
//
//	logicThread.start();
//
//	while (1)
//	{
//		int c = _getch();
//
//		if (c == 'x')
//			break;
//
//		if (c == 'p')
//			server.printTps();
//	}
//
//	server.stop();
//
//	logicThread.stop();
//
//	return 0;
//}
