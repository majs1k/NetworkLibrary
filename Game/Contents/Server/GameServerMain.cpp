#include <conio.h>
#include "GameServer.h"
#include "../RpcModule.h"
#include "../../Database/Database.h"
#include "../../Utils/ConfigLoader.h"
#include "../../Utils/CrashDump.h"
#include "../../Utils/Profiler.h"

#define CONFIG_FILENAME			L"game_config.json"

int main()
{
	CrashDump().Initialize();

	ConfigLoader::Instance().Load(CONFIG_FILENAME);

	Database db;

	db.Connect(config.dbHost, config.dbUser, config.dbPassword, config.dbSchema);

	GameServer server;

	server.AttachProxy(&g_RpcProxy);
	server.AttachDb(&db);

	server.Start(config.ip, config.port, config.sessionMax, config.concurrentCount, config.workerCount);


	while (1)
	{
		//if (GetAsyncKeyState('X') & 0x0001)
		//	break;

		//else if (GetAsyncKeyState(VK_SPACE) & 0x0001)
		//	ProfilerManager::Instance().Save();

		//else if (GetAsyncKeyState('C') & 0x0001)
		//	ProfilerManager::Instance().Clear();

		//server.Monitoring();

		Sleep(INFINITE);
	}

	server.Stop();

	return 0;
}
