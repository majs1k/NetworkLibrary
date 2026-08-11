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

#define SAFETY_PERCENT		80

class RingBuffer
{
private:
	char* buffer_;
	int capacity_;

	int writePos_;
	int readPos_;

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

	void Clear()
	{
		writePos_ = 0;
		readPos_ = 0;
	}

	void Resize(int size)
	{
		if (capacity_ > size)
			return;

		char* newBuffer = new char[size];

		if (readPos_ <= writePos_)
		{
			memcpy(newBuffer, buffer_ + readPos_, UseSize());
		}
		else
		{
			memcpy(newBuffer, buffer_ + readPos_, DirectDequeueSize());
			memcpy(newBuffer + DirectDequeueSize(), buffer_, UseSize() - DirectDequeueSize());
		}

		int s = UseSize();

		readPos_ = 0;
		writePos_ = s;

		delete buffer_;

		buffer_ = newBuffer;
		capacity_ = size;
		safeSize_ = capacity_ * SAFETY_PERCENT / 100;
	}

	int UseSize() const
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

	int FreeSize() const
	{
		return capacity_ - UseSize() - 1;
	}

	int SafeSize() const
	{
		return safeSize_;
	}

	bool IsFull() const
	{
		return ((writePos_ + 1) % capacity_ == readPos_);
	}

	char* GetBufferPtr() const
	{
		return buffer_;
	}

	char* GetFrontBufferPtr() const
	{
		return buffer_ + readPos_;
	}

	char* GetRearBufferPtr() const
	{
		return buffer_ + writePos_;
	}

	int Enqueue(const char* data, int size)
	{
		if (FreeSize() < size)
		{
			__debugbreak();

			/// TODO: Resize() 1번
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

	int Dequeue(char* data, int size)
	{
		/// 이후 삭제
		if (UseSize() < size)
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

	int Peek(char* data, int size) const
	{
		if (UseSize() < size)
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

	int DirectEnqueueSize() const
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

	int DirectDequeueSize() const
	{
		return readPos_ <= writePos_
			? writePos_ - readPos_
			: capacity_ - readPos_;
	}

	int MoveFront(int size)
	{
		if (UseSize() < size)
			return 0;

		readPos_ = (readPos_ + size) % capacity_;
		return size;
	}

	int MoveRear(int size)
	{
		if (FreeSize() < size)
			return 0;

		writePos_ = (writePos_ + size) % capacity_;
		return size;
	}
};


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

public:
	void Clear()
	{
		writePos_ = 0;
		readPos_ = 0;
	}

	int UseSize() const
	{
		int r = readPos_;
		int w = writePos_;

		return r <= w
			? w - r
			: w + capacity_ - r;
	}

	int FreeSize() const
	{
		return capacity_ - UseSize() - 1;
	}

	void Enqueue(T value)
	{
		if (FreeSize() <= 0)
		{
			__debugbreak();
			return;
		}

		queue_[writePos_] = value;

		writePos_ = (writePos_ + 1) % capacity_;
	}

	void Dequeue(T& value)
	{
		if (UseSize() <= 0)
		{
			__debugbreak();
			return;
		}

		value = queue_[readPos_];

		readPos_ = (readPos_ + 1) % capacity_;
	}

	void Dequeue()
	{
		if (UseSize() <= 0)
		{
			__debugbreak();
			return;
		}

		readPos_ = (readPos_ + 1) % capacity_;
	}

	const T& At(int idx) const
	{
		if (UseSize() <= 0)
		{
			__debugbreak();
			return nullptr;
		}

		if (UseSize() <= idx)
		{
			__debugbreak();
			return nullptr;
		}

		return queue_[(readPos_ + idx) % capacity_];
	}

	int MoveFront(int size)
	{
		if (UseSize() < size)
		{
			__debugbreak();
			return 0;
		}

		readPos_ = (readPos_ + size) % capacity_;
		return size;
	}

	int MoveRear(int size)
	{
		if (FreeSize() < size)
		{
			__debugbreak();
			return 0;
		}

		writePos_ = (writePos_ + size) % capacity_;
		return size;
	}
};
