#include <iostream>
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

	std::string id;
	std::string pw;

	std::cout << "input id: ";
	std::cin >> id;
	std::cout << "input pw: ";
	std::cin >> pw;

	while (1)
	{
		Sleep(100);
		
		if (GetAsyncKeyState('Z') & 0x0001)
		{
			client.TestRegister(id, pw);
			std::cout << "TestRegister" << std::endl;
		}

		if (GetAsyncKeyState('X') & 0x0001)
		{
			client.TestLogin(id, pw);
			std::cout << "TestLogin" << std::endl;
		}

		client.ProcessNetwork();
	}

	return 0;
}
