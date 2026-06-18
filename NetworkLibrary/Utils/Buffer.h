#pragma once
#include <iostream>
#include <Windows.h>
#include "Lock.h"

// -------------------------------------------------------------------
//
// 직렬화 버퍼
//
// 템플릿은 기본자료형 말고도 다 받아서 적용 x
// TODO: 디버그 모드에서 패킷 사이즈 체크 후 리사이즈?
// 
// vs2022 release mode -> 내장함수 최적화
// memcpy 호출 인라인화 -> 포인터 방식과 성능 동일
//
// -------------------------------------------------------------------
#pragma once

class Buffer
{
protected:
	char* buffer_;

	int	capacity_;

	int writePos_;
	int readPos_;

	LONG refCount_;
	Lock lock_;

public:
	Buffer(int bufferSize = 100)
		:capacity_(bufferSize), writePos_(0), readPos_(0)
	{
		buffer_ = (char*)malloc(capacity_);
	}

	~Buffer()
	{
		free(buffer_);
	}

	void increase(int count)
	{
		InterlockedExchange(&refCount_, count);
	}

	int decrease()
	{
		return InterlockedDecrement(&refCount_);
	}

	void lock()
	{
		lock_.lock();
	}

	void unlock()
	{
		lock_.unlock();
	}

	void initialize()
	{
		writePos_ = 0;
		readPos_ = 0;
	}

	void clear()
	{
		writePos_ = 0;
		readPos_ = 0;
	}

	int	capacity()
	{
		return capacity_;
	}

	int	useSize()
	{
		return writePos_ - readPos_;
	}

	char* getBufferPtr()
	{
		return buffer_;
	}

	int	moveWritePos(int size)
	{
		/// TODO: 버퍼 초과시 리사이즈, 음수 이동 제한?
		writePos_ += size;
		return size;
	}

	int	moveReadPos(int size)
	{
		readPos_ += size;
		return size;
	}

	Buffer& operator = (const Buffer& Buffer)
	{
		return *this;
	}

	Buffer& operator << (char value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(char));
		writePos_ += sizeof(char);

		return *this;
	}

	Buffer& operator << (unsigned char value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(unsigned char));
		writePos_ += sizeof(unsigned char);

		return *this;
	}

	Buffer& operator << (short value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(short));
		writePos_ += sizeof(short);

		return *this;
	}

	Buffer& operator << (unsigned short value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(unsigned short));
		writePos_ += sizeof(unsigned short);

		return *this;
	}

	Buffer& operator << (int value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(int));
		writePos_ += sizeof(int);

		return *this;
	}

	Buffer& operator << (unsigned int value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(unsigned int));
		//*(unsigned int*)(buffer_ + writePos_) = value;
		writePos_ += sizeof(unsigned int);

		return *this;
	}

	Buffer& operator << (float value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(float));
		writePos_ += sizeof(float);

		return *this;
	}

	Buffer& operator << (__int64 value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(__int64));
		writePos_ += sizeof(__int64);

		return *this;
	}

	Buffer& operator << (double value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(double));
		writePos_ += sizeof(double);

		return *this;
	}

	Buffer& operator >> (char& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(char));
		readPos_ += sizeof(char);

		return *this;
	}

	Buffer& operator >> (unsigned char& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(unsigned char));
		readPos_ += sizeof(unsigned char);

		return *this;
	}

	Buffer& operator >> (short& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(short));
		readPos_ += sizeof(short);

		return *this;
	}

	Buffer& operator >> (unsigned short& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(unsigned short));
		readPos_ += sizeof(unsigned short);

		return *this;
	}

	Buffer& operator >> (int& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(int));
		readPos_ += sizeof(int);

		return *this;
	}

	Buffer& operator >> (unsigned int& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(unsigned int));
		readPos_ += sizeof(unsigned int);

		return *this;
	}

	Buffer& operator >> (float& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(float));
		readPos_ += sizeof(float);

		return *this;
	}

	Buffer& operator >> (__int64& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(__int64));
		readPos_ += sizeof(__int64);

		return *this;
	}

	Buffer& operator >> (double& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(double));
		readPos_ += sizeof(double);

		return *this;
	}

	int	read(char* dest, int size)
	{
		memcpy(dest, buffer_ + readPos_, size);
		readPos_ += size;

		return size;
	}

	int	write(char* src, int size)
	{
		/// TODO: 버퍼 초과시 리사이즈
		memcpy(buffer_ + writePos_, src, size);
		writePos_ += size;

		return size;
	}
};
