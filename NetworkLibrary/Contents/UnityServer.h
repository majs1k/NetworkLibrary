#pragma once
#include <list>

#include "../Network/LanServer.h"
#include "../Utils/PacketQueue.h"

#include "ResponseCode.h"
#include "../Database/Database.h"

#include "../RPC/RpcServerProxy.h"
#include "../RPC/RpcServerStub.h"

#include "../Database/DatabaseServerProxy.h"
#include "../Database/DatabaseServerStub.h"



#include <vector>
#include <string>
#include <queue>
#include <unordered_map>

#include "Repository/UserRepository.h"


struct User
{
	int userId;
	std::string loginId;
	std::string password;
};

class UnityServer : public LanServer, public RpcServerHandler, public DatabaseServerHandler
{
private:

	HANDLE hLogicThread_;
	bool shutdown_ = false;

	PacketQueue packetQueue_;

	RpcServerProxy rpc_;
	RpcServerStub stub_;

public:
	UnityServer();

	bool OnConnectionRequest(const std::wstring& ip, int port) override;
	void OnAccept(__int64 sessionId) override;
	void OnRelease(__int64 sessionId) override;
	void OnRecv(__int64 sessionId, Packet* packet) override;

	static unsigned int __stdcall LogicThread(void* param);
	void Update();

	void ProcessNetPacket();

	// ----------------------------------------------------- //

	bool ReqRegister(__int64 sessionId, std::string& loginId, std::string& password);
	bool ResRegister(__int64 sessionId, RESPONSE_CODE code);
	bool ReqLogin(__int64 sessionId, std::string& loginId, std::string& password);
	bool ResLogin(__int64 sessionId, RESPONSE_CODE code);

	// ----------------------------------------------------- //


private:
	PacketQueue DatabaseReqQueue_;
	PacketQueue DatabaseResQueue_;

	HANDLE hDatabaseThread_;
	static unsigned int __stdcall DatabaseThread(void* param);

	UserRepository userRepository;

	DatabaseServerProxy dbProxy_;
	DatabaseServerStub dbStub_;

public:

	void InitDatabase(Database* db)
	{
		userRepository.Init(db);
	}


	bool DbReqRegister(__int64 sessionId, std::string& loginId, std::string& password);

	bool DbReqLogin(__int64 sessionId, std::string& loginId, std::string& password);


	// ----------------------------------------------------- //

private:
	std::unordered_map<int, User*> users_;

public:
	void CreateUser(const User& user)
	{
		users_[user.userId] = new User(user);
	}

	void DeleteUser(int userId)
	{
		auto iter = users_.find(userId);

		if (iter != users_.end())
		{
			delete iter->second;
			users_.erase(iter);
		}
	}

	User* FindUser(int userId)
	{
		auto iter = users_.find(userId);

		if (iter == users_.end())
			return nullptr;

		return iter->second;
	}

	// ----------------------------------------------------- //

};
