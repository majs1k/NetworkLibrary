#include <conio.h>
#include "./Contents/UnityClient.h"
#include "./Utils/ConfigLoader.h"
#include "./Utils/CrashDump.h"

#define SERVER_IP			L"127.0.0.1"
#define SERVER_PORT			7777

int main()
{
	CrashDump().Initialize();

	UnityClient client;

	client.Connect(SERVER_IP, SERVER_PORT);

	while (1)
	{
		Sleep(1000);

		client.SendTest();

		client.ProcessNetwork();
	}

	return 0;
}
