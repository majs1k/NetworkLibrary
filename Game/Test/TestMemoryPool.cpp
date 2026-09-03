//#include <iostream>
//#include <thread>
//
//#include "Profiler.h"
//#include "MemoryPool.h"
//
//using namespace std;
//
//class A
//{
//	int a;
//	int b;
//	int c;
//};
//
//class B
//{
//	char a;
//	int b;
//	double c;
//};
//
//
//int main()
//{
//	while (1)
//	{
//		PRO_BEGIN(L"new");
//
//		for (int i = 0; i < 100000; i++)
//		{
//			A* ptr = new A();
//			B* ptr2 = new B();
//
//			delete ptr;
//			delete ptr2;
//		}
//
//		TMemoryPool<A> pool1(100);
//		TMemoryPool<B> pool2(100);
//
//		PRO_END(L"new");
//
//		PRO_BEGIN(L"pool");
//
//		for (int i = 0; i < 100000; i++)
//		{
//			A* ptr = pool1.Alloc();
//			B* ptr2 = pool2.Alloc();
//
//			pool1.Free(ptr);
//			pool2.Free(ptr2);
//		}
//
//		PRO_END(L"pool");
//	}
//
//}
