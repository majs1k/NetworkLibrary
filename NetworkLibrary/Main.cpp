#include <conio.h>

#include "./Network/LanServer.h"
#include "./Network/LanTestServer.h"

#include "./Contents/PacketHandler.h"
#include "./Contents/Player.h"

#include "./Utils/ConfigManager.h"
#include "./Utils/CrashDump.h"

#define CONFIG_FILENAME			L"config.json"

int main()
{
	CrashDump().Initialize();

	ConfigManager::getInstance().load(CONFIG_FILENAME);

	RPCStub::getInstance().initialize(new PacketHandler());

	server = new LanTestServer();

	server->start();

	while (1)
	{
		int c = _getch();

		if (c == 'x')
			break;

		if (c == 'p')
			server->printTps();
	}
	
	server->stop();

	return 0;
}
