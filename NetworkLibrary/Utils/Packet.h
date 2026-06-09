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
#include <iostream>

class Packet
{
protected:
	char* buffer_;

	int	capacity_;

	int writePos_;
	int readPos_;

public:
	// 디폴트 사이즈 이후 재고려
	Packet(int bufferSize = 200)
		:capacity_(bufferSize), writePos_(0), readPos_(0)
	{
		buffer_ = (char*)malloc(capacity_);
	}

	~Packet()
	{
		free(buffer_);
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
		return writePos_;
	}

	//////////////////////////////////////////////////////////////////////////
	// 버퍼 포인터 얻기.
	//
	// Parameters: 없음.
	// Return: (char *)버퍼 포인터.
	//////////////////////////////////////////////////////////////////////////
	char* getBufferPtr()
	{
		return buffer_;
	}

	//////////////////////////////////////////////////////////////////////////
	// 버퍼 Pos 이동. (음수이동은 안됨)
	// GetBufferPtr 함수를 이용하여 외부에서 강제로 버퍼 내용을 수정할 경우 사용
	//
	// Parameters: (int) 이동 사이즈.
	// Return: (int) 이동된 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int	moveWritePos(int size)
	{
		/// TODO: 버퍼 초과시 리사이즈
		writePos_ += size;
		return size;
	}

	int	moveReadPos(int size)
	{
		readPos_ += size;
		return size;
	}

	Packet& operator = (const Packet& packet)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	// 넣기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	Packet& operator << (char value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(char));
		writePos_ += sizeof(char);

		return *this;
	}

	Packet& operator << (unsigned char value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(unsigned char));
		writePos_ += sizeof(unsigned char);

		return *this;
	}

	Packet& operator << (short value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(short));
		writePos_ += sizeof(short);

		return *this;
	}

	Packet& operator << (unsigned short value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(unsigned short));
		writePos_ += sizeof(unsigned short);

		return *this;
	}

	Packet& operator << (int value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(int));
		writePos_ += sizeof(int);

		return *this;
	}

	Packet& operator << (unsigned int value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(unsigned int));
		//*(unsigned int*)(buffer_ + writePos_) = value;
		writePos_ += sizeof(unsigned int);

		return *this;
	}

	Packet& operator << (float value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(float));
		writePos_ += sizeof(float);

		return *this;
	}

	Packet& operator << (__int64 value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(__int64));
		writePos_ += sizeof(__int64);

		return *this;
	}

	Packet& operator << (double value)
	{
		memcpy(buffer_ + writePos_, &value, sizeof(double));
		writePos_ += sizeof(double);

		return *this;
	}


	//////////////////////////////////////////////////////////////////////////
	// 빼기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	Packet& operator >> (char& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(char));
		readPos_ += sizeof(char);

		return *this;
	}

	Packet& operator >> (unsigned char& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(unsigned char));
		readPos_ += sizeof(unsigned char);

		return *this;
	}

	Packet& operator >> (short& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(short));
		readPos_ += sizeof(short);

		return *this;
	}

	Packet& operator >> (unsigned short& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(unsigned short));
		readPos_ += sizeof(unsigned short);

		return *this;
	}

	Packet& operator >> (int& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(int));
		readPos_ += sizeof(int);

		return *this;
	}

	Packet& operator >> (unsigned int& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(unsigned int));
		readPos_ += sizeof(unsigned int);

		return *this;
	}

	Packet& operator >> (float& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(float));
		readPos_ += sizeof(float);

		return *this;
	}

	Packet& operator >> (__int64& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(__int64));
		readPos_ += sizeof(__int64);

		return *this;
	}

	Packet& operator >> (double& value)
	{
		memcpy(&value, buffer_ + readPos_, sizeof(double));
		readPos_ += sizeof(double);

		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// 데이타 얻기.getData
	//
	// Parameters: (char *)Dest 포인터. (int)Size.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int	read(char* chpDest, int size)
	{
		memcpy(chpDest, buffer_ + readPos_, size);
		readPos_ += size;

		return size;
	}

	//////////////////////////////////////////////////////////////////////////
	// 데이타 삽입.putData
	//
	// Parameters: (char *)Src 포인터. (int)SrcSize.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int	write(char* chpSrc, int size)
	{
		/// TODO: 버퍼 초과시 리사이즈
		memcpy(buffer_ + writePos_, chpSrc, size);
		writePos_ += size;

		return size;
	}
};
