#pragma once

class LanServer;

class ServerProxy
{
public:
	LanServer* server_;

public:
	void cs_create_login(__int64 sessionId, int num);
	void sc_create_login(__int64 sessionId, int num);
};
