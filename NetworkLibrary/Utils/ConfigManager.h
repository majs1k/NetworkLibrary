#pragma once
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
using namespace rapidjson;

struct NetworkConfig
{
	std::string ip;
	int port;
	int backlog;
};

struct ThreadConfig
{
	int workerCount;
};

struct DBConfig
{
	std::string host;
	std::string user;
	std::string password;
};

struct ConfigManager : public Singleton<ConfigManager>
{
public:
	void load(const WCHAR* path)
	{
		std::ifstream ifs(path);

		if (!ifs.is_open())
		{
			wprintf(L"config filename error\n");

			return;
		}

		IStreamWrapper isw(ifs);

		Document doc;
		doc.ParseStream(isw);

		if (doc.HasParseError())
		{
			wprintf(L"config parse error\n");
		}

		const Value& network = doc["Network"];

		network_.ip = network["IP"].GetString();
		network_.port = network["Port"].GetInt();
		network_.backlog = network["Backlog"].GetInt();


		const Value& thread = doc["Thread"];

		thread_.workerCount = thread["WorkerCount"].GetInt();


		const Value& db = doc["DB"];

		db_.host = db["Host"].GetString();
		db_.user = db["User"].GetString();
		db_.password = db["Password"].GetString();

		wprintf(L"### Config OK ###\n");
	}

public:
	NetworkConfig network_;
	ThreadConfig thread_;
	DBConfig db_;
};
