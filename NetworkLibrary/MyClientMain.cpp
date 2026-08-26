#include <iostream>
#include <conio.h>
#include "./Contents/MyClient.h"
#include "./Utils/ConfigLoader.h"
#include "./Utils/CrashDump.h"

#include <thread>

#define SERVER_IP			L"127.0.0.1"
#define SERVER_PORT			7777

MyClient client;


void Net()
{
	while (1)
	{
		client.ProcessNetwork();

		Sleep(10);
	}
}

int main()
{
	CrashDump().Initialize();


	client.Connect(SERVER_IP, SERVER_PORT);


	std::thread t1(Net);


	while (1)
	{
		int type;

		std::cout << "message type : ";
		std::cin >> type;

		switch (type)
		{
		case 0:

			client.TestUserRegister();

			break;

		case 1:

			client.TestUserLogin();

			break;

		case 2:

			client.TestPlayerRegister();

			break;

		case 3:

			client.TestPlayerEnterGame();

			break;

		case 4:

			client.TestChat();

			break;

		case 5:

			client.TestLobbyPlayers();

			break;

			
		default:

			break;
		}

		Sleep(500);
	}

	t1.join();

	return 0;
}
