//#include <conio.h>
//#include "./Contents/FighterServer.h"
//#include "./Utils/ConfigLoader.h"
//#include "./Utils/CrashDump.h"
//#include "./Utils/Profiler.h"
//
//#define CONFIG_FILENAME			L"fighterconfig.json"
//
//int main()
//{
//	CrashDump().initialize();
//
//	ConfigLoader::getInstance().load(CONFIG_FILENAME);
//
//	FighterServer server;
//
//	server.start(config.ip, config.port, config.sessionMax, config.concurrentCount, config.workerCount);
//
//	while (1)
//	{
//		int c = _getch();
//
//		if (c == 'x')
//			break;
//
//		else if (c == 'p')
//			server.printTps();
//
//		else if (c == ' ')
//			Profiler::getInstance().save();
//
//		else if (c == 'c')
//			Profiler::getInstance().clear();
//	}
//
//	server.stop();
//
//	return 0;
//}
