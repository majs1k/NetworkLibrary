#pragma once
#include <iostream>
#include "Lock.h"
#include <Windows.h>

template <typename T, int N>
class BufferT
{
public:
	T circularQueue_[N]{ };
	int capacity_ = N;

	int refCount_ = 0;

	int writePos_ = 0;
	int readPos_ = 0;

	Lock lock_;
	int cnt;

public:
	void clear()
	{
		writePos_ = 0;
		readPos_ = 0;
	}

	int useSize() const
	{
		int r = readPos_;
		int w = writePos_;

		return r <= w
			? w - r
			: w + capacity_ - r;
	}

	int freeSize() const
	{
		return capacity_ - useSize() - 1;
	}

	char* getBufferPtr()
	{
		return nullptr;
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
		int s = freeSize();
		if (s <= 0)
		{
			DebugBreak();
			return;
		}

		circularQueue_[writePos_] = value;

		writePos_ = (writePos_ + 1) % capacity_;

		cnt++;
	}

	void dequeue(T& value)
	{
		if (useSize() <= 0)
			DebugBreak();

		value = circularQueue_[readPos_];

		readPos_ = (readPos_ + 1) % capacity_;
	}

	int moveFront(int size)
	{
		if (useSize() < size)
			return 0;

		readPos_ = (readPos_ + size) % capacity_;
		return size;
	}

	int moveFrontReverse(int size)
	{
		if (size > freeSize())
			return 0;

		readPos_ = (readPos_ - size + capacity_) % capacity_;
		return size;
	}

	int moveRear(int size)
	{
		if (freeSize() < size)
			return 0;

		writePos_ = (writePos_ + size) % capacity_;
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

//template<>
//char* PacketT<Buffer*>::getBufferPtr()
//{
//	//return circularQueue_front()->getBufferPtr();
//}
