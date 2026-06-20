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
#include "Lock.h"
#include <Windows.h>

// ------------------------------------------------------- //

struct HEADER
{
	unsigned short size_;
};

// ------------------------------------------------------- //

class SendPacket
{
protected:
	char* buffer_;
	int	capacity_;

	int writePos_;
	int readPos_;

	LONG refCount_;

public:
	// 디폴트 사이즈?
	SendPacket(int bufferSize = 200);
	~SendPacket();
	void initialize();

	void setHeader(int size);

	int	capacity();
	int	useSize();

	//char* getBufferPtr()
	//{
	//	return buffer_;
	//}

	char* getSendPacketPtr()
	{
		return buffer_ + sizeof(HEADER);
	}

	char* getCompletePtr()
	{
		return buffer_;
	}

	int completeSize()
	{
		return writePos_;
	}

	void incrementRef()
	{
		InterlockedIncrement(&refCount_);
	}

	int decrementRef()
	{
		return InterlockedDecrement(&refCount_);
	}

	//getBufferPtr()로 버퍼 내용 수정할 경우 사용
	int	moveWritePos(int size);
	int	moveReadPos(int size);

	SendPacket& operator = (const SendPacket& SendPacket);

	SendPacket& operator << (char value);
	SendPacket& operator << (unsigned char value);
	SendPacket& operator << (short value);
	SendPacket& operator << (unsigned short value);
	SendPacket& operator << (int value);
	SendPacket& operator << (unsigned int value);
	SendPacket& operator << (float value);
	SendPacket& operator << (__int64 value);
	SendPacket& operator << (double value);

	SendPacket& operator >> (char& value);
	SendPacket& operator >> (unsigned char& value);
	SendPacket& operator >> (short& value);
	SendPacket& operator >> (unsigned short& value);
	SendPacket& operator >> (int& value);
	SendPacket& operator >> (unsigned int& value);
	SendPacket& operator >> (float& value);
	SendPacket& operator >> (__int64& value);
	SendPacket& operator >> (double& value);

	int	read(char* dest, int size);
	int	write(char* src, int size);
};
