#pragma once
#include <string>

class Session;

class Connector
{
	Session* session_;

public:
	Connector();
	~Connector();

	void connect(std::wstring ip, int port);
	void networkIO();
};
