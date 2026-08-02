#pragma once
#include <queue>
#include <mutex>
#include "Packet.h"

struct Message
{
	__int64 sessionId_;
	Packet* packet_;

	void Initialize(__int64 sessionId, Packet* packet)
	{
		sessionId_ = sessionId;
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
