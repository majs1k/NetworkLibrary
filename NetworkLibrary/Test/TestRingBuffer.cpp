//#include "../Utils/RingBuffer.h"
//#include "windows.h"
//
//int main()
//{
//	RingBuffer buffer(10);
//	char src[41] = "0123456789012345678901234567890123456789";
//	char dequeueBuf[41];
//	char compBuf[41];
//
//	int maxSize = 5;
//	int enqSize = 0;
//	int deqSize = 0;
//
//	srand(time(nullptr));
//
//	while (1)
//	{
//		buffer.clear();
//
//		int totalSize = 0;
//		int readSize = 0;
//
//		for (int i = 0; i < 5; i++)
//		{
//			int size1 = rand() % maxSize;
//
//			buffer.enqueue(src + totalSize, size1);
//			totalSize += size1;
//
//			int size2 = rand() % maxSize;
//
//			if (size2 > buffer.useSize())
//				continue;
//
//			char buf[10];
//
//			buffer.dequeue(buf, size2);
//
//			for (int i = 0; i < size2; i++)
//			{
//				if (buf[i] - '0' != readSize % 10)
//				{
//					std::cout << "wrong!";
//					DebugBreak();
//				}
//
//				readSize++;
//			}
//			std::cout << "success!" << std::endl;
//		}
//	}
//}
