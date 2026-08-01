#pragma once
#include <iostream>
#include <mutex>

// 템플릿 클래스의 완전 특수화는 기존 템플릿을 하나도 이어받지 않음..
// 그래서 char* 버퍼를 사용하는 기존의 RingBuffer 클래스는 유지
template <typename T, int N>
class TRingBuffer
{
public:
	T queue_[N]{ };
	int capacity_ = N;

	int writePos_ = 0;
	int readPos_ = 0;

	std::mutex lock_;

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

	void enqueue(T value)
	{
		if (freeSize() <= 0)
		{
			__debugbreak();
			return;
		}

		queue_[writePos_] = value;

		writePos_ = (writePos_ + 1) % capacity_;
	}

	void dequeue(T& value)
	{
		if (useSize() <= 0)
		{
			__debugbreak();
			return;
		}

		value = queue_[readPos_];

		readPos_ = (readPos_ + 1) % capacity_;
	}

	void peek(T& value, int idx) const
	{
		if (useSize() <= 0)
		{
			__debugbreak();
			return;
		}

		if (useSize() <= idx)
		{
			__debugbreak();
			return;
		}

		value = queue_[(readPos_ + idx) % capacity_];
	}

	int moveFront(int size)
	{
		if (useSize() < size)
		{
			__debugbreak();
			return 0;
		}

		readPos_ = (readPos_ + size) % capacity_;
		return size;
	}

	int moveRear(int size)
	{
		if (freeSize() < size)
		{
			__debugbreak();
			return 0;
		}

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

