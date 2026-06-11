#include <conio.h>
#include "./Network/Server.h"
#include "./Network/Session.h"
#include "./Contents/PacketHandler.h"
#include "./Contents/Player.h"
#include "./Utils/ConfigManager.h"
#include "./Utils/TickController.h"
#include "./Utils/Logger.h"
#include "./Utils/CrashDump.h"

#define CONFIG_FILENAME			L"config.json"

bool _shutdown;

int main()
{
	CrashDump().Initialize();

	ConfigManager::getInstance().load(CONFIG_FILENAME);

	RPCStub::getInstance().initialize(new PacketHandler());

	Server::getInstance().initialize();

	while (1)
	{
		int c = _getch();

		// 추가할만한 명령어?
		if (c == 'x')
			break;

		if (c == 'a')
		{
			printf("cnt : %d\n", cnt);
		}
	}

	//while (!_shutdown)
	//{
	//	PlayerManager::getInstance().update();
	//	TickController::getInstance().update();
	//}
	
	Server::getInstance().serverExit();

	return 0;
}
