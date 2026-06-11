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
#include "Lock.h"

#define SAFETY_PERCENT		80

class RingBuffer
{
private:
	char* buffer_;
	int capacity_;

	int writePos_;
	int readPos_;

	Lock lock_;
	int safeSize_;

public:
	RingBuffer(int bufferSize = 100);

	~RingBuffer();

	int capacity() const;

	bool isFull() const;

	void clear();

	int safeSize() const;

	int useSize() const;

	int freeSize() const;

	int enqueue(const char* data, int size);

	int dequeue(char* data, int size);

	int enqueueLocked(const char* data, int size);

	int dequeueLocked(char* data, int size);

	int peek(char* data, int size) const;

	int directEnqueueSize() const;

	int directDequeueSize() const;

	int moveFront(int size);

	int moveRear(int size);

	char* getBufferPtr() const;

	char* getFrontBufferPtr() const;

	char* getRearBufferPtr() const;
};
