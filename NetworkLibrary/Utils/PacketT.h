#pragma once
#include <iostream>
#include <queue>
#include <Windows.h>
#include "Lock.h"

#define SAFETY_PERCENT		80

template <typename T>
class PacketT
{
private:
	std::queue<T> queue_;
	int refCount_;

	Lock lock_;

public:
	void clear()
	{

	}

	int useSize() const
	{
		return queue_.size();
	}

	int freeSize() const
	{
		
	}

	void increase()
	{
		refCount_++;
	}

	void decrease()
	{
		refCount_--;
	}

	void enqueue(T value)
	{
		queue_.push(value);
	}

	void dequeue(T* value)
	{
		*value = queue_.front();
		queue_.pop();
	}

	int peek(char* data, int size) const
	{
		return size;
	}

	int moveFront(int size)
	{
		return size;
	}

	int moveRear(int size)
	{
		return size;
	}

	void lock()
	{
		lock_.lock();
	}

	void unlock()
	{
		lock_.unlock();
	}
};
