#include <iostream>
#include "SendPacket.h"

SendPacket::SendPacket(int bufferSize)
	:capacity_(bufferSize), writePos_(sizeof(HEADER)), readPos_(sizeof(HEADER))
{
	buffer_ = (char*)malloc(capacity_);
}

SendPacket::~SendPacket()
{
	free(buffer_);
}

void SendPacket::initialize()
{
	writePos_ = sizeof(HEADER);
	readPos_ = sizeof(HEADER);
}

void SendPacket::setHeader(int size)
{
	HEADER* header = (HEADER*)buffer_;

	header->size_ = size;
}

int	SendPacket::capacity()
{
	return capacity_;
}

int	SendPacket::useSize()
{
	return writePos_ - readPos_;
}

int	SendPacket::moveWritePos(int size)
{
	/// TODO: 버퍼 초과시 리사이즈, 음수 이동 제한?
	writePos_ += size;
	return size;
}

int	SendPacket::moveReadPos(int size)
{
	readPos_ += size;
	return size;
}

SendPacket& SendPacket::operator = (const SendPacket& SendPacket)
{
	return *this;
}

SendPacket& SendPacket::operator << (char value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(char));
	writePos_ += sizeof(char);

	return *this;
}

SendPacket& SendPacket::operator << (unsigned char value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(unsigned char));
	writePos_ += sizeof(unsigned char);

	return *this;
}

SendPacket& SendPacket::operator << (short value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(short));
	writePos_ += sizeof(short);

	return *this;
}

SendPacket& SendPacket::operator << (unsigned short value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(unsigned short));
	writePos_ += sizeof(unsigned short);

	return *this;
}

SendPacket& SendPacket::operator << (int value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(int));
	writePos_ += sizeof(int);

	return *this;
}

SendPacket& SendPacket::operator << (unsigned int value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(unsigned int));
	//*(unsigned int*)(buffer_ + writePos_) = value;
	writePos_ += sizeof(unsigned int);

	return *this;
}

SendPacket& SendPacket::operator << (float value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(float));
	writePos_ += sizeof(float);

	return *this;
}

SendPacket& SendPacket::operator << (__int64 value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(__int64));
	writePos_ += sizeof(__int64);

	return *this;
}

SendPacket& SendPacket::operator << (double value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(double));
	writePos_ += sizeof(double);

	return *this;
}

SendPacket& SendPacket::operator >> (char& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(char));
	readPos_ += sizeof(char);

	return *this;
}

SendPacket& SendPacket::operator >> (unsigned char& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(unsigned char));
	readPos_ += sizeof(unsigned char);

	return *this;
}

SendPacket& SendPacket::operator >> (short& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(short));
	readPos_ += sizeof(short);

	return *this;
}

SendPacket& SendPacket::operator >> (unsigned short& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(unsigned short));
	readPos_ += sizeof(unsigned short);

	return *this;
}

SendPacket& SendPacket::operator >> (int& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(int));
	readPos_ += sizeof(int);

	return *this;
}

SendPacket& SendPacket::operator >> (unsigned int& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(unsigned int));
	readPos_ += sizeof(unsigned int);

	return *this;
}

SendPacket& SendPacket::operator >> (float& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(float));
	readPos_ += sizeof(float);

	return *this;
}

SendPacket& SendPacket::operator >> (__int64& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(__int64));
	readPos_ += sizeof(__int64);

	return *this;
}

SendPacket& SendPacket::operator >> (double& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(double));
	readPos_ += sizeof(double);

	return *this;
}

int	SendPacket::read(char* dest, int size)
{
	memcpy(dest, buffer_ + readPos_, size);
	readPos_ += size;

	return size;
}

int	SendPacket::write(char* src, int size)
{
	/// TODO: 버퍼 초과시 리사이즈
	memcpy(buffer_ + writePos_, src, size);
	writePos_ += size;

	return size;
}
