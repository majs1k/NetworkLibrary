#include "SessionManager.h"
#include "Session.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"

SessionManager::SessionManager()
	:idSeed_(0), sessionMax_(MAX_SESSION), sessionSize_(0)
{
	sessionMap_.reserve(MAX_SESSION);
}

SessionManager::~SessionManager()
{
	for (auto& session : sessionMap_)
	{
		delete session.second;
	}
}

int SessionManager::size() const
{
	return sessionSize_;
}

bool SessionManager::isFull() const
{
	return sessionSize_ >= sessionMax_;
}

__int64 SessionManager::requireId()
{
	return ++idSeed_;
}

void SessionManager::addSession(Session* session)
{
	//lock_.lock();

	sessionMap_.insert({ session->id(), session });

	sessionSize_++;

	//lock_.unlock();

	//LOG_INFO(L"[NETWORK] session create count=%d", sessionSize_);
}

void SessionManager::removeSession(Session* session)
{
	//lock_.lock();

	sessionMap_.erase(session->id());

	sessionSize_--;

	//lock_.unlock();

	//LOG_INFO(L"[NETWORK] session delete count=%d", sessionSize_);

	// erase에서 세션 삭제???
	closesocket(session->socket());

	delete session;
}

Session* SessionManager::find(__int64 id)
{
	lock_.lock();

	auto it = sessionMap_.find(id);
	auto end = sessionMap_.end();
	
	lock_.unlock();

	if (it != end)
	{
		return (*it).second;
	}
	else
		return nullptr;
}

void SessionManager::sendPacket(__int64 id, Packet& packet)
{
	Session* session = this->find(id);

	session->sendPacket(packet);
}

//// 지연삭제
//void SessionManager::lazyDeletion()
//{
//	for (auto it = sessionList_.begin(); it != sessionList_.end(); )
//	{
//		SelectSession* session = *it;
//
//		if (!session->isActive())
//		{
//			delete session;
//
//			it = sessionList_.erase(it);
//		}
//		else
//			++it;
//	}
//}
