//#include "Client.h"
//
//Connector::Connector()
//{
//	WSADATA wsa;
//	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//		error(L"WSAStartup() error!");
//}
//
//Connector::~Connector()
//{
//	closesocket(session_->socket());
//
//	delete session_;
//
//	WSACleanup();
//}
//
//void Connector::connect(const WCHAR ip[], int port)
//{
//	SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
//	if (sock == INVALID_SOCKET)
//		error(L"socket() error!");
//
//	LINGER optval;
//	optval.l_onoff = 1;
//	optval.l_linger = 0;
//	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
//		error(L"setsockopt() error!");
//
//	SOCKADDR_IN serverAddr;
//	ZeroMemory(&serverAddr, sizeof(serverAddr));
//
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_port = htons(port);
//	InetPton(AF_INET, ip, &serverAddr.sin_addr);
//
//	if (::connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//		error(L"connect() error!");
//
//	// connect 이후 논블로킹 소켓으로 전환
//	u_long on = 1;
//	if (ioctlsocket(sock, FIONBIO, &on) == SOCKET_ERROR)
//		error(L"iocltsocket error!");
//
//	session_ = new Session();
//	//session_->initialize(sock, serverAddr, 0);
//}
