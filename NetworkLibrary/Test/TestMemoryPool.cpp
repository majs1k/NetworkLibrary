//#include "../Utils/MemoryPool.h"
//#include <iostream>
//using namespace std;
//
//class A
//{
//	int a;
//
//public:
//	A()
//	{
//		a = 1;
//		std::cout << "ctor A!\n";
//	}
//	~A()
//	{
//		std::cout << "dtor A!\n";
//	}
//};
//
//// free 코드를 주석처리하면, 할당해제를 놓친 메모리를 자동 출력
//int main()
//{
//	MemoryPool<A> pool(5);
//
//	cout << "Make MemoryPool!\n";
//
//	A* ptr[10];
//
//	// -----------------------------------------------------------------//
//	for (int i = 0; i < 5; i++)
//	{
//		ptr[i] = pool.alloc();
//		//cout << ptr[i] << " ";
//	}
//	cout << endl;
//	cout << pool.size() << " " << pool.capacity() << endl;
//
//	//for (int i = 0; i < 5; i++)
//	//{
//	//	pool.free(ptr[i]);
//	//}
//
//	cout << pool.size() << " " << pool.capacity() << endl;
//
//	// -----------------------------------------------------------------//
//
//	for (int i = 0; i < 10; i++)
//	{
//		ptr[i] = pool.alloc();
//		//cout << ptr[i] << " ";
//	}
//	cout << endl;
//	cout << pool.size() << " " << pool.capacity() << endl;
//
//	for (int i = 0; i < 10; i++)
//	{
//		pool.free(ptr[i]);
//	}
//
//	cout << pool.size() << " " << pool.capacity() << endl;
//}
