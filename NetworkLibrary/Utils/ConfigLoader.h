#pragma once
#include <fstream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "Singleton.h"

using namespace rapidjson;

struct ServerConfig
{
    std::wstring ip;
    int port;
    int sessionMax;
    int concurrentCount;
    int workerCount;

    std::wstring DBHost;
    std::wstring DBUser;
    std::wstring DBPassword;
};

extern ServerConfig config;

class ConfigLoader : public Singleton<ConfigLoader>
{
public:
    bool load(std::wstring path)
    {
        std::ifstream ifs(path);

        if (!ifs.is_open())
        {
            printf("check config filename\n");
            return false;
        }

        IStreamWrapper isw(ifs);

        Document doc;
        doc.ParseStream(isw);

        if (doc.HasParseError())
        {
            printf("config parse error\n");
            return false;
        }

        std::string ip = doc["IP"].GetString();
        config.ip.assign(ip.begin(), ip.end());

        config.port = doc["Port"].GetInt();

        config.sessionMax = doc["SessionMax"].GetInt();

        config.concurrentCount = doc["ConcurrentCount"].GetInt();

        config.workerCount = doc["WorkerCount"].GetInt();

        std::string dbHost = doc["DBHost"].GetString();
        config.DBHost.assign(dbHost.begin(), dbHost.end());

        std::string dbUser = doc["DBUser"].GetString();
        config.DBUser.assign(dbUser.begin(), dbUser.end());

        std::string dbPassword = doc["DBPassword"].GetString();
        config.DBPassword.assign(dbPassword.begin(), dbPassword.end());

        return true;
    }
};

ServerConfig config;
