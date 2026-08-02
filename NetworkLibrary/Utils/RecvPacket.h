#pragma once
#include <Windows.h>

class PacketBuffer
{
	friend class RecvPacket;

	char* buffer_;
	int capacity_;

	LONG refCount_;

public:
	PacketBuffer(int bufferSize = 5000)
		:capacity_(bufferSize)
	{
		buffer_ = (char*)malloc(capacity_);

		refCount_ = 1;
	}

	~PacketBuffer()
	{
		free(buffer_);
	}

	void Initialize()
	{
		refCount_ = 1;
	}
};

class RecvPacket
{
private:
	PacketBuffer* packetBuffer_;

	char* buffer_;
	int	capacity_;

	int writePos_= 0;
	int readPos_ = 0;

	LONG refCount_;

public:
	//~RecvPacket()
	//{
	//	free(buffer_);
	//}

	int	Capacity()
	{
		return capacity_;
	}

	int	UseSize()
	{
		return writePos_ - readPos_;
	}

	int FreeSize()
	{
		return capacity_ - writePos_;
	}

	bool IsFull()
	{
		return writePos_ == capacity_;
	}

	char* GetBufferPtr()
	{
		return buffer_;
	}

	char* GetWriteBufferPtr()
	{
		return buffer_ + writePos_;
	}

	char* GetReadBufferPtr()
	{
		return buffer_ + readPos_;
	}

	void Initialize()
	{
		refCount_ = 1;
	}

	void Initialize(PacketBuffer* buf)
	{
		packetBuffer_ = buf;

		buffer_ = buf->buffer_;
		capacity_ = buf->capacity_;

		writePos_ = 0;
		readPos_ = 0;

		refCount_ = 1;
	}

	RecvPacket& operator = (const RecvPacket& ref)
	{
		packetBuffer_ = ref.packetBuffer_;
		InterlockedIncrement(&packetBuffer_->refCount_);

		buffer_ = ref.buffer_;
		capacity_ = ref.capacity_;

		writePos_ = ref.writePos_;
		readPos_ = ref.readPos_;

		return *this;
	}

	int Peek(char* data, int size)
	{
		if (UseSize() < size)
			return 0;

		memcpy(data, buffer_ + readPos_, size);

		return size;
	}

	void IncrementRef()
	{
		InterlockedIncrement(&packetBuffer_->refCount_);
		InterlockedIncrement(&refCount_);
	}

	int DecrementRef()
	{
		if (InterlockedDecrement(&packetBuffer_->refCount_) == 0)
		{
			delete packetBuffer_;
		}

		return InterlockedDecrement(&refCount_);
	}

	//getBufferPtr()로 버퍼 내용 수정할 경우 사용
	int	MoveWritePos(int size);
	int	MoveReadPos(int size);

	RecvPacket& operator << (char value);
	RecvPacket& operator << (unsigned char value);
	RecvPacket& operator << (short value);
	RecvPacket& operator << (unsigned short value);
	RecvPacket& operator << (int value);
	RecvPacket& operator << (unsigned int value);
	RecvPacket& operator << (float value);
	RecvPacket& operator << (__int64 value);
	RecvPacket& operator << (double value);

	RecvPacket& operator >> (char& value);
	RecvPacket& operator >> (unsigned char& value);
	RecvPacket& operator >> (short& value);
	RecvPacket& operator >> (unsigned short& value);
	RecvPacket& operator >> (int& value);
	RecvPacket& operator >> (unsigned int& value);
	RecvPacket& operator >> (float& value);
	RecvPacket& operator >> (__int64& value);
	RecvPacket& operator >> (double& value);

	int	Read(char* chpDest, int size);
	int	Write(char* chpSrc, int size);
};
