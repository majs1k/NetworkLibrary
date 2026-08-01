// -------------------------------------------------------------------
//
// 링버퍼
//
// 리사이즈 필요시 추후 구현
// 예외처리 최소화. 멀티스레드 환경에서 주의
// 
// TCP의 L4 버퍼 크기는 충분히 크다. (세션마다 할당되어 있음)
// L4 송신버퍼 7만~
// L4 수신버퍼 10만~
//
// -------------------------------------------------------------------
#pragma once
#include <iostream>
#include <mutex>

#define SAFETY_PERCENT		80

class RingBuffer
{
private:
	char* buffer_;
	int capacity_;

	int writePos_;
	int readPos_;

	std::recursive_mutex lock_;
	int safeSize_;

public:
	RingBuffer(int bufferSize)
		:capacity_(bufferSize), writePos_(0), readPos_(0)
	{
		buffer_ = new char[capacity_];

		safeSize_ = capacity_ * SAFETY_PERCENT / 100;
	}

	~RingBuffer()
	{
		delete buffer_;
	}

	void clear()
	{
		writePos_ = 0;
		readPos_ = 0;
	}

	void resize(int size)
	{
		if (capacity_ > size)
			return;

		char* newBuffer = new char[size];

		if (readPos_ <= writePos_)
		{
			memcpy(newBuffer, buffer_ + readPos_, useSize());
		}
		else
		{
			memcpy(newBuffer, buffer_ + readPos_, directDequeueSize());
			memcpy(newBuffer + directDequeueSize(), buffer_, useSize() - directDequeueSize());
		}

		int s = useSize();

		readPos_ = 0;
		writePos_ = s;

		delete buffer_;

		buffer_ = newBuffer;
		capacity_ = size;
		safeSize_ = capacity_ * SAFETY_PERCENT / 100;
	}

	int useSize() const
	{
		//return readPos_ <= writePos_
		//	? writePos_ - readPos_
		//	: writePos_ + capacity_ - readPos_;

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

	int safeSize() const
	{
		return safeSize_;
	}

	bool isFull() const
	{
		return ((writePos_ + 1) % capacity_ == readPos_);
	}

	char* getBufferPtr() const
	{
		return buffer_;
	}

	char* getFrontBufferPtr() const
	{
		return buffer_ + readPos_;
	}

	char* getRearBufferPtr() const
	{
		return buffer_ + writePos_;
	}

	int enqueue(const char* data, int size)
	{
		/// 1회 리사이즈 로직으로 변경 필요
		if (freeSize() < size)
		{
			printf("enqueue over!\n");
			__debugbreak();
			return 0;
		}

		if (capacity_ < writePos_ + size)
		{
			memcpy(buffer_ + writePos_, data, capacity_ - writePos_);
			memcpy(buffer_, data + (capacity_ - writePos_), size - (capacity_ - writePos_));
		}
		else
			memcpy(buffer_ + writePos_, data, size);

		writePos_ = (writePos_ + size) % capacity_;

		return size;
	}

	int dequeue(char* data, int size)
	{
		/// 이후 삭제
		if (useSize() < size)
		{
			printf("dequeue over!\n");
			__debugbreak();
			return 0;
		}

		if (capacity_ < readPos_ + size)
		{
			memcpy(data, buffer_ + readPos_, capacity_ - readPos_);
			memcpy(data + (capacity_ - readPos_), buffer_, size - (capacity_ - readPos_));
		}
		else
			memcpy(data, buffer_ + readPos_, size);

		readPos_ = (readPos_ + size) % capacity_;

		return size;
	}

	int peek(char* data, int size) const
	{
		if (useSize() < size)
			return 0;

		if (capacity_ < readPos_ + size)
		{
			memcpy(data, buffer_ + readPos_, capacity_ - readPos_);
			memcpy(data + capacity_ - readPos_, buffer_, size - (capacity_ - readPos_));
		}
		else
			memcpy(data, buffer_ + readPos_, size);

		return size;
	}

	int directEnqueueSize() const
	{
		if (readPos_ <= writePos_)
		{
			if (readPos_ == 0)
				return capacity_ - writePos_ - 1;

			return capacity_ - writePos_;
		}
		else
			return readPos_ - writePos_ - 1;
	}

	int directDequeueSize() const
	{
		return readPos_ <= writePos_
			? writePos_ - readPos_
			: capacity_ - readPos_;
	}

	int moveFront(int size)
	{
		if (useSize() < size)
			return 0;

		readPos_ = (readPos_ + size) % capacity_;
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
