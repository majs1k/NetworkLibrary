// -------------------------------------------------------------------
//
// 직렬화 버퍼
//
// 템플릿은 기본자료형 말고도 다 받아서 적용 x
// TODO: 디버그 모드에서 패킷 사이즈 체크 후 리사이즈?
// 
// -------------------------------------------------------------------
#pragma once
#include <iostream>
#include "RingBuffer.h"

// ------------------------------------------------------- //

struct TEST_HEADER
{
	short size_;
};

// ------------------------------------------------------- //

#define PACKET_CODE				0x89

struct FIGHTER_HEADER
{
	unsigned char code_;
	char size_;
	char type_;
};

// ------------------------------------------------------- //

struct UNITY_HEADER
{
	short size_;
	short type_;
};

// ------------------------------------------------------- //

///헤더 변경시 수정//////////////////////////////////////////////////////////////////////////////////////
//#define HEADER_SIZE		sizeof(TEST_HEADER)
//#define HEADER_SIZE		sizeof(FIGHTER_HEADER)
#define HEADER_SIZE		sizeof(UNITY_HEADER)

class Packet
{
protected:
	char* buffer_{ nullptr };

	int	capacity_{ 200 };

	int writePos_{ 0 };
	int readPos_{ 0 };

	__int64 sessionId_;

public:
	Packet()
	{
		buffer_ = new char[capacity_];
	}

	~Packet()
	{
		delete buffer_;
	}

	void Initialize()
	{
		writePos_ = HEADER_SIZE;
		readPos_ = HEADER_SIZE;
	}

	void SetId(__int64 sessionId)
	{
		sessionId_ = sessionId;
	}

	__int64 GetId()
	{
		return sessionId_;
	}

	///헤더 변경시 수정//////////////////////////////////////////////////////////////////////////////////////
	UNITY_HEADER* GetHeaderPtr()
	{
		return reinterpret_cast<UNITY_HEADER*>(buffer_);
	}

	char* GetBodyPtr()
	{
		return buffer_ + HEADER_SIZE;
	}

	int TotalUseSize()
	{
		return this->UseSize() + HEADER_SIZE;
	}

	int	Capacity()
	{
		return capacity_;
	}

	// 헤더 크기 제외
	int	UseSize()
	{
		return writePos_ - readPos_;
	}

	char* GetBufferPtr()
	{
		return buffer_;
	}

	int	MoveWritePos(int size)
	{
		/// TODO: 버퍼 초과시 리사이즈, 음수 이동 제한?
		writePos_ += size;
		return size;
	}

	int	MoveReadPos(int size)
	{
		readPos_ += size;
		return size;
	}

	Packet& operator = (const Packet& packet)
	{

	}

	Packet& operator << (char value)
	{
		*(char*)(buffer_ + writePos_) = value;
		writePos_ += sizeof(char);

		return *this;
	}

	Packet& operator << (unsigned char value)
	{
		*(unsigned char*)(buffer_ + writePos_) = value;
		writePos_ += sizeof(unsigned char);

		return *this;
	}

	Packet& operator << (short value)
	{
		*(short*)(buffer_ + writePos_) = value;
		writePos_ += sizeof(short);

		return *this;
	}

	Packet& operator << (unsigned short value)
	{
		*(unsigned short*)(buffer_ + writePos_) = value;
		writePos_ += sizeof(unsigned short);

		return *this;
	}

	Packet& operator << (int value)
	{
		*(int*)(buffer_ + writePos_) = value;
		writePos_ += sizeof(int);

		return *this;
	}

	Packet& operator << (unsigned int value)
	{
		*(unsigned int*)(buffer_ + writePos_) = value;
		writePos_ += sizeof(unsigned int);

		return *this;
	}

	Packet& operator << (float value)
	{
		*(float*)(buffer_ + writePos_) = value;
		writePos_ += sizeof(float);

		return *this;
	}

	Packet& operator << (__int64 value)
	{
		*(__int64*)(buffer_ + writePos_) = value;
		writePos_ += sizeof(__int64);

		return *this;
	}

	Packet& operator << (double value)
	{
		*(double*)(buffer_ + writePos_) = value;
		writePos_ += sizeof(double);

		return *this;
	}

	Packet& operator >> (char& value)
	{
		value = *(char*)(buffer_ + readPos_);
		readPos_ += sizeof(char);

		return *this;
	}

	Packet& operator >> (unsigned char& value)
	{
		value = *(unsigned char*)(buffer_ + readPos_);
		readPos_ += sizeof(unsigned char);

		return *this;
	}

	Packet& operator >> (short& value)
	{
		value = *(short*)(buffer_ + readPos_);
		readPos_ += sizeof(short);

		return *this;
	}

	Packet& operator >> (unsigned short& value)
	{
		value = *(unsigned short*)(buffer_ + readPos_);
		readPos_ += sizeof(unsigned short);

		return *this;
	}

	Packet& operator >> (int& value)
	{
		value = *(int*)(buffer_ + readPos_);
		readPos_ += sizeof(int);

		return *this;
	}

	Packet& operator >> (unsigned int& value)
	{
		value = *(unsigned int*)(buffer_ + readPos_);
		readPos_ += sizeof(unsigned int);

		return *this;
	}

	Packet& operator >> (float& value)
	{
		value = *(float*)(buffer_ + readPos_);
		readPos_ += sizeof(float);

		return *this;
	}

	Packet& operator >> (__int64& value)
	{
		value = *(__int64*)(buffer_ + readPos_);
		readPos_ += sizeof(__int64);

		return *this;
	}

	Packet& operator >> (double& value)
	{
		value = *(double*)(buffer_ + readPos_);
		readPos_ += sizeof(double);

		return *this;
	}

	int	Read(char* dest, int size)
	{
		memcpy(dest, buffer_ + readPos_, size);
		readPos_ += size;

		return size;
	}

	int	Write(char* src, int size)
	{
		/// TODO: 버퍼 초과시 리사이즈
		memcpy(buffer_ + writePos_, src, size);
		writePos_ += size;

		return size;
	}
};

/// TODO: 별도의 클래스 생성
//class RPacket : public Packet
//{
//	std::shared_ptr<RingBuffer> refQueue_;
//
//public:
//	void Initialize(std::shared_ptr<RingBuffer> recvQueue)
//	{
//		writePos_ = 0;
//		readPos_ = 0;
//
//		refQueue_ = recvQueue;
//		buffer_ = recvQueue->GetFrontBufferPtr();
//	}
//};
