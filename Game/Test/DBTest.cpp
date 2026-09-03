//#include <iostream>
//
//#include "Database.h"
//#include "UserRepository.h"
//
//int main()
//{
//	// DB 楷搬
//	Database database;
//
//	if (database.Connect() == false)
//	{
//		std::cout << "DB Connect Failed" << std::endl;
//		return -1;
//	}
//
//	// DB 立辟 按眉
//	UserRepository userRepository(&database);
//
//	// 皋葛府 包府 按眉
//	UserManager userManager;
//
//
//	std::cout << "start" << std::endl;
//
//	std::cout << (short)userRepository.Register("jamjam1", "12345") << std::endl;
//
//	std::cout << (short)userRepository.Login("jamjam1", "12345") << std::endl;
//
//	return 0;
//}
