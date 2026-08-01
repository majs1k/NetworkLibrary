#include <iostream>
#include "RecvPacket.h"

int	RecvPacket::moveWritePos(int size)
{
	/// TODO: 버퍼 초과시 리사이즈, 음수 이동 제한?
	writePos_ += size;
	return size;
}

int	RecvPacket::moveReadPos(int size)
{
	readPos_ += size;
	return size;
}

RecvPacket& RecvPacket::operator << (char value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(char));
	writePos_ += sizeof(char);

	return *this;
}

RecvPacket& RecvPacket::operator << (unsigned char value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(unsigned char));
	writePos_ += sizeof(unsigned char);

	return *this;
}

RecvPacket& RecvPacket::operator << (short value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(short));
	writePos_ += sizeof(short);

	return *this;
}

RecvPacket& RecvPacket::operator << (unsigned short value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(unsigned short));
	writePos_ += sizeof(unsigned short);

	return *this;
}

RecvPacket& RecvPacket::operator << (int value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(int));
	writePos_ += sizeof(int);

	return *this;
}

RecvPacket& RecvPacket::operator << (unsigned int value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(unsigned int));
	//*(unsigned int*)(bufferPtr_ + writePos_) = value;
	writePos_ += sizeof(unsigned int);

	return *this;
}

RecvPacket& RecvPacket::operator << (float value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(float));
	writePos_ += sizeof(float);

	return *this;
}

RecvPacket& RecvPacket::operator << (__int64 value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(__int64));
	writePos_ += sizeof(__int64);

	return *this;
}

RecvPacket& RecvPacket::operator << (double value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(double));
	writePos_ += sizeof(double);

	return *this;
}

RecvPacket& RecvPacket::operator >> (char& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(char));
	readPos_ += sizeof(char);

	return *this;
}

RecvPacket& RecvPacket::operator >> (unsigned char& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(unsigned char));
	readPos_ += sizeof(unsigned char);

	return *this;
}

RecvPacket& RecvPacket::operator >> (short& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(short));
	readPos_ += sizeof(short);

	return *this;
}

RecvPacket& RecvPacket::operator >> (unsigned short& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(unsigned short));
	readPos_ += sizeof(unsigned short);

	return *this;
}

RecvPacket& RecvPacket::operator >> (int& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(int));
	readPos_ += sizeof(int);

	return *this;
}

RecvPacket& RecvPacket::operator >> (unsigned int& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(unsigned int));
	readPos_ += sizeof(unsigned int);

	return *this;
}

RecvPacket& RecvPacket::operator >> (float& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(float));
	readPos_ += sizeof(float);

	return *this;
}

RecvPacket& RecvPacket::operator >> (__int64& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(__int64));
	readPos_ += sizeof(__int64);

	return *this;
}

RecvPacket& RecvPacket::operator >> (double& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(double));
	readPos_ += sizeof(double);

	return *this;
}

int	RecvPacket::read(char* chpDest, int size)
{
	memcpy(chpDest, buffer_ + readPos_, size);
	readPos_ += size;

	return size;
}

int	RecvPacket::write(char* chpSrc, int size)
{
	/// TODO: 버퍼 초과시 리사이즈
	memcpy(buffer_ + writePos_, chpSrc, size);
	writePos_ += size;

	return size;
}
