#pragma once
#include <string>

class Connector
{
public:
	Connector();
	~Connector();

	void connect(std::wstring ip, int port);
};
