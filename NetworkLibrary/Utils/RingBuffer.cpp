#include <iostream>
#include "RingBuffer.h"

RingBuffer::RingBuffer(int bufferSize)
	:capacity_(bufferSize), writePos_(0), readPos_(0)
{
	buffer_ = (char*)malloc(capacity_);

	safeSize_ = capacity_ * SAFETY_PERCENT / 100;
}

RingBuffer::~RingBuffer()
{
	free(buffer_);
}

void RingBuffer::clear()
{
	writePos_ = 0;
	readPos_ = 0;
}

int RingBuffer::useSize() const
{
	return readPos_ <= writePos_
		? writePos_ - readPos_
		: writePos_ + capacity_ - readPos_;
}

int RingBuffer::freeSize() const
{
	return capacity_ - useSize() - 1;
}

int RingBuffer::safeSize() const
{
	return safeSize_;
}

bool RingBuffer::isFull() const
{
	return ((writePos_ + 1) % capacity_ == readPos_);
}

char* RingBuffer::getBufferPtr() const
{
	return buffer_;
}

char* RingBuffer::getFrontBufferPtr() const
{
	return buffer_ + readPos_;
}

char* RingBuffer::getRearBufferPtr() const
{
	return buffer_ + writePos_;
}

int RingBuffer::enqueue(const char* data, int size)
{
	/// 1회 리사이즈 로직으로 변경 필요
	if (freeSize() < size)
	{
		printf("enqueue over!\n");
		DebugBreak();
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

int RingBuffer::dequeue(char* data, int size)
{
	/// 이후 삭제
	if (useSize() < size)
	{
		printf("dequeue over!\n");
		DebugBreak();
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

int RingBuffer::peek(char* data, int size) const
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

int RingBuffer::directEnqueueSize() const
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

int RingBuffer::directDequeueSize() const
{
	return readPos_ <= writePos_
		? writePos_ - readPos_
		: capacity_ - readPos_;
}

int RingBuffer::moveFront(int size)
{
	if (useSize() < size)
		return 0;

	readPos_ = (readPos_ + size + capacity_) % capacity_;
	return size;
}

int RingBuffer::moveRear(int size)
{
	if (freeSize() < size)
		return 0;

	writePos_ = (writePos_ + size + capacity_) % capacity_;
	return size;
}

void RingBuffer::lock()
{
	lock_.lock();
}

void RingBuffer::unlock()
{
	lock_.unlock();
}
