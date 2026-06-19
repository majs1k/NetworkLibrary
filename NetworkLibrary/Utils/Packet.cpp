#include <iostream>
#include "Packet.h"

Packet::Packet(int bufferSize)
	:capacity_(bufferSize), writePos_(sizeof(HEADER)), readPos_(sizeof(HEADER))
{
	buffer_ = (char*)malloc(capacity_);
}

Packet::~Packet()
{
	free(buffer_);
}

void Packet::initialize()
{
	writePos_ = sizeof(HEADER);
	readPos_ = sizeof(HEADER);
}

void Packet::setHeader(int size)
{
	HEADER* header = (HEADER*)buffer_;

	header->size_ = size;
}


int	Packet::capacity()
{
	return capacity_;
}

int	Packet::useSize()
{
	return writePos_ - readPos_;
}



int	Packet::moveWritePos(int size)
{
	/// TODO: 버퍼 초과시 리사이즈, 음수 이동 제한?
	writePos_ += size;
	return size;
}

int	Packet::moveReadPos(int size)
{
	readPos_ += size;
	return size;
}

Packet& Packet::operator = (const Packet& packet)
{
	return *this;
}

Packet& Packet::operator << (char value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(char));
	writePos_ += sizeof(char);

	return *this;
}

Packet& Packet::operator << (unsigned char value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(unsigned char));
	writePos_ += sizeof(unsigned char);

	return *this;
}

Packet& Packet::operator << (short value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(short));
	writePos_ += sizeof(short);

	return *this;
}

Packet& Packet::operator << (unsigned short value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(unsigned short));
	writePos_ += sizeof(unsigned short);

	return *this;
}

Packet& Packet::operator << (int value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(int));
	writePos_ += sizeof(int);

	return *this;
}

Packet& Packet::operator << (unsigned int value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(unsigned int));
	//*(unsigned int*)(buffer_ + writePos_) = value;
	writePos_ += sizeof(unsigned int);

	return *this;
}

Packet& Packet::operator << (float value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(float));
	writePos_ += sizeof(float);

	return *this;
}

Packet& Packet::operator << (__int64 value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(__int64));
	writePos_ += sizeof(__int64);

	return *this;
}

Packet& Packet::operator << (double value)
{
	memcpy(buffer_ + writePos_, &value, sizeof(double));
	writePos_ += sizeof(double);

	return *this;
}

Packet& Packet::operator >> (char& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(char));
	readPos_ += sizeof(char);

	return *this;
}

Packet& Packet::operator >> (unsigned char& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(unsigned char));
	readPos_ += sizeof(unsigned char);

	return *this;
}

Packet& Packet::operator >> (short& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(short));
	readPos_ += sizeof(short);

	return *this;
}

Packet& Packet::operator >> (unsigned short& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(unsigned short));
	readPos_ += sizeof(unsigned short);

	return *this;
}

Packet& Packet::operator >> (int& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(int));
	readPos_ += sizeof(int);

	return *this;
}

Packet& Packet::operator >> (unsigned int& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(unsigned int));
	readPos_ += sizeof(unsigned int);

	return *this;
}

Packet& Packet::operator >> (float& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(float));
	readPos_ += sizeof(float);

	return *this;
}

Packet& Packet::operator >> (__int64& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(__int64));
	readPos_ += sizeof(__int64);

	return *this;
}

Packet& Packet::operator >> (double& value)
{
	memcpy(&value, buffer_ + readPos_, sizeof(double));
	readPos_ += sizeof(double);

	return *this;
}

int	Packet::read(char* dest, int size)
{
	memcpy(dest, buffer_ + readPos_, size);
	readPos_ += size;

	return size;
}

int	Packet::write(char* src, int size)
{
	/// TODO: 버퍼 초과시 리사이즈
	memcpy(buffer_ + writePos_, src, size);
	writePos_ += size;

	return size;
}
