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

class Packet
{
protected:
	char* buffer_;

	int	capacity_;

	int writePos_;
	int readPos_;

public:
	// 디폴트 사이즈?
	Packet(int bufferSize = 200);
	~Packet();

	void initialize();

	int	capacity();
	int	useSize();

	char* getBufferPtr();

	//getBufferPtr()로 버퍼 내용 수정할 경우 사용
	int	moveWritePos(int size);
	int	moveReadPos(int size);

	Packet& operator = (const Packet& packet);

	Packet& operator << (char value);
	Packet& operator << (unsigned char value);
	Packet& operator << (short value);
	Packet& operator << (unsigned short value);
	Packet& operator << (int value);
	Packet& operator << (unsigned int value);
	Packet& operator << (float value);
	Packet& operator << (__int64 value);
	Packet& operator << (double value);

	Packet& operator >> (char& value);
	Packet& operator >> (unsigned char& value);
	Packet& operator >> (short& value);
	Packet& operator >> (unsigned short& value);
	Packet& operator >> (int& value);
	Packet& operator >> (unsigned int& value);
	Packet& operator >> (float& value);
	Packet& operator >> (__int64& value);
	Packet& operator >> (double& value);

	int	read(char* chpDest, int size);
	int	write(char* chpSrc, int size);
};
