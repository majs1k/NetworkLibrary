#include <conio.h>
#include "./Contents/TestServer.h"
#include "./Utils/ConfigLoader.h"
#include "./Utils/CrashDump.h"
#include "./Utils/Profiler.h"

#define CONFIG_FILENAME			L"testconfig.json"

int main()
{
	CrashDump().Initialize();

	ConfigLoader::Instance().Load(CONFIG_FILENAME);

	TestServer server;

	server.Start(config.ip, config.port, config.sessionMax, config.concurrentCount, config.workerCount);

	while (1)
	{
		int c = _getch();

		if (c == 'x')
			break;

		else if (c == ' ')
			Profiler::Instance().Save();

		else if (c == 'c')
			Profiler::Instance().Clear();
	}

	server.Stop();

	return 0;
}
