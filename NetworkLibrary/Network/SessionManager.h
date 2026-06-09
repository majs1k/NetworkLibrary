#pragma once
#include "../Utils/Singleton.h"
#include "../Utils/Lock.h"
#include <unordered_map>

class Session;
class Packet;

#define MAX_SESSION			200

class SessionManager : public Singleton<SessionManager>
{
	//friend class Server;

private:
	__int64 idSeed_;
	int sessionMax_;

	// rehash 일어나면(갑자기 삽입 했을때 등..) 이터레이터 무효화 주의
	std::unordered_map<__int64, Session*> sessionMap_;

	int sessionSize_;

	Lock lock_;

public:
	SessionManager();
	~SessionManager();

	int size() const;
	bool isFull() const;

	__int64 requireId();

	void addSession(Session* session);
	void removeSession(Session* session);
	// lock 사용하면서 const 제거
	Session* find(__int64 id);

	void sendPacket(__int64 id, Packet& packet);
};
