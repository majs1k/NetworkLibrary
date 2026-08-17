#include <conio.h>
#include "./Contents/UnityServer.h"
#include "./Utils/ConfigLoader.h"
#include "./Utils/CrashDump.h"
#include "./Utils/Profiler.h"

#define CONFIG_FILENAME			L"unityconfig.json"

int main()
{
	CrashDump().Initialize();

	ConfigLoader::Instance().Load(CONFIG_FILENAME);

	UnityServer server;

	server.Start(config.ip, config.port, config.sessionMax, config.concurrentCount, config.workerCount);

	while (1)
	{
		Sleep(1000);

		if (GetAsyncKeyState('X') & 0x0001)
			break;

		else if (GetAsyncKeyState(VK_SPACE) & 0x0001)
			ProfilerManager::Instance().Save();

		else if (GetAsyncKeyState('C') & 0x0001)
			ProfilerManager::Instance().Clear();

		server.Monitoring();
	}

	server.Stop();

	return 0;
}
