#pragma once
#include <fstream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "Singleton.h"

using namespace rapidjson;

struct ServerConfig
{
    std::string ip;
    int port;
    int sessionMax;
    int concurrentCount;
    int workerCount;

    std::string dbHost;
    std::string dbUser;
    std::string dbPassword;
    std::string dbSchema;
};

extern ServerConfig config;

class ConfigLoader : public Singleton<ConfigLoader>
{
public:
    bool Load(std::wstring path)
    {
        std::ifstream ifs(path);

        if (!ifs.is_open())
        {
            printf("*** error config filename ***\n");
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

        config.ip = doc["IP"].GetString();

        config.port = doc["Port"].GetInt();

        config.sessionMax = doc["SessionMax"].GetInt();

        config.concurrentCount = doc["ConcurrentCount"].GetInt();

        config.workerCount = doc["WorkerCount"].GetInt();



        config.dbHost = doc["DBHost"].GetString();

        config.dbUser = doc["DBUser"].GetString();

        config.dbPassword = doc["DBPassword"].GetString();

        config.dbSchema = doc["DBSchema"].GetString();

        return true;
    }
};

ServerConfig config;
