#include <conio.h>
#include "./Contents/TestClient.h"
#include "./Utils/ConfigLoader.h"
#include "./Utils/CrashDump.h"

#define SERVER_IP			L"127.0.0.1"
#define SERVER_PORT			6000

int main()
{
	CrashDump().Initialize();

	TestClient client;

	client.Connect(SERVER_IP, SERVER_PORT);

	while (1)
	{
		Sleep(1000);

		client.SendTestNum();

		client.ProcessNetwork();
	}

	return 0;
}
