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
//	CrashDump().Initialize();
//
//	ConfigLoader::Instance().Load(CONFIG_FILENAME);
//
//	FighterServer server;
//
//	server.Start(config.ip, config.port, config.sessionMax, config.concurrentCount, config.workerCount);
//
//	while (1)
//	{
//		int c = _getch();
//
//		if (c == 'x')
//			break;
//
//		else if (c == ' ')
//			ProfilerManager::Instance().Save();
//
//		else if (c == 'c')
//			ProfilerManager::Instance().Clear();
//	}
//
//	server.Stop();
//
//	return 0;
//}
