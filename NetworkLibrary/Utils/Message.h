#pragma once
#include <queue>
#include <mutex>
#include "Packet.h"

class Message
{
private:
	__int64 sessionId_;
	Packet* packet_;

public:
	__int64 sessionId()
	{
		return sessionId_;
	}

	Packet* packet()
	{
		return packet_;
	}

	void setId(__int64 sessionId)
	{
		sessionId_ = sessionId;
	}

	void setPacket(Packet* packet)
	{
		packet_ = packet;
	}
};

class MessageQueue
{
	std::queue<Message*> queue_;
	std::mutex lock_;

public:
	void push(Message* message)
	{
		lock_.lock();

		queue_.push(message);

		lock_.unlock();
	}

	Message* pop()
	{
		lock_.lock();

		if (queue_.empty())
		{
			lock_.unlock();
			return nullptr;
		}

		Message* message = queue_.front();
		queue_.pop();

		lock_.unlock();

		return message;
	}
};
