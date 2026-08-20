#pragma once
#include <queue>
#include <mutex>
#include "Packet.h"

class PacketQueue
{
	std::queue<Packet*> queue_;
	std::mutex lock_;

public:
	void Push(Packet* packet)
	{
		lock_.lock();

		queue_.push(packet);

		lock_.unlock();
	}

	Packet* Pop()
	{
		lock_.lock();

		if (queue_.empty())
		{
			lock_.unlock();

			return nullptr;
		}

		Packet* packet = queue_.front();
		queue_.pop();

		lock_.unlock();

		return packet;
	}
};
