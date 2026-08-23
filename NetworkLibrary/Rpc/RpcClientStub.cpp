#include "RpcClientStub.h"


bool RpcClientStub::PacketProc(Packet* packet)
{
    switch (packet->GetHeaderPtr()->type_)
    {
        case 0:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->ReqRegister(loginId, password);
        }
        case 1:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResRegister(code);
        }
        case 2:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->ReqLogin(loginId, password);
        }
        case 3:
        {
			RESPONSE_CODE code;
			int userId;

            *packet >> code >> userId;

            return handler_->ResLogin(code, userId);
        }
        case 10:
        {
			int userId;
			std::string playerName;

            *packet >> userId >> playerName;

            return handler_->ReqCreatePlayer(userId, playerName);
        }
        case 11:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResCreatePlayer(code);
        }
        case 12:
        {
			int userId;

            *packet >> userId;

            return handler_->ReqPlayerProfile(userId);
        }
        case 13:
        {
			RESPONSE_CODE code;
			Player player;

            *packet >> code >> player;

            return handler_->ResPlayerProfile(code, player);
        }
        case 20:
        {

            *packet;

            return handler_->ReqRoomList();
        }
        case 21:
        {

            *packet;

            return handler_->ResRoomList();
        }
        case 22:
        {
			std::string roomName;

            *packet >> roomName;

            return handler_->ReqRoomCreate(roomName);
        }
        case 23:
        {
			RESPONSE_CODE code;
			int roomId;
			std::string roomName;

            *packet >> code >> roomId >> roomName;

            return handler_->ResRoomCreate(code, roomId, roomName);
        }
        case 24:
        {
			int roomId;

            *packet >> roomId;

            return handler_->ResRoomDelete(roomId);
        }
        case 25:
        {
			int roomId;

            *packet >> roomId;

            return handler_->ReqEnterRoom(roomId);
        }
        case 26:
        {
			RESPONSE_CODE code;
			int roomId;
			std::string roomName;
			std::list<int> lst;

            *packet >> code >> roomId >> roomName >> lst;

            return handler_->ResEnterRoom(code, roomId, roomName, lst);
        }
        case 27:
        {

            *packet;

            return handler_->ResEnterOtherUser();
        }
        case 90:
        {

            *packet;

            return handler_->ReqPlayerList();
        }
        case 91:
        {
			std::list<Player> playerList;

            *packet >> playerList;

            return handler_->ResPlayerList(playerList);
        }
        case 100:
        {
			std::string message;

            *packet >> message;

            return handler_->ReqChat(message);
        }
        case 101:
        {
			int playerId;
			std::string message;

            *packet >> playerId >> message;

            return handler_->ResChat(playerId, message);
        }
        case 200:
        {
			std::list<int> lst;

            *packet >> lst;

            return handler_->ReqUseItem(lst);
        }
        case 201:
        {
			std::list<int> lst;

            *packet >> lst;

            return handler_->ResUseItem(lst);
        }
    default:
    {
        // 로그 추가

        return false;
    }
    }
}


bool RpcClientHandler::ReqRegister(std::string& loginId, std::string& password)
{
    return true;
}

bool RpcClientHandler::ResRegister(RESPONSE_CODE code)
{
    return true;
}

bool RpcClientHandler::ReqLogin(std::string& loginId, std::string& password)
{
    return true;
}

bool RpcClientHandler::ResLogin(RESPONSE_CODE code, int userId)
{
    return true;
}

bool RpcClientHandler::ReqCreatePlayer(int userId, std::string& playerName)
{
    return true;
}

bool RpcClientHandler::ResCreatePlayer(RESPONSE_CODE code)
{
    return true;
}

bool RpcClientHandler::ReqPlayerProfile(int userId)
{
    return true;
}

bool RpcClientHandler::ResPlayerProfile(RESPONSE_CODE code, Player player)
{
    return true;
}

bool RpcClientHandler::ReqRoomList()
{
    return true;
}

bool RpcClientHandler::ResRoomList()
{
    return true;
}

bool RpcClientHandler::ReqRoomCreate(std::string& roomName)
{
    return true;
}

bool RpcClientHandler::ResRoomCreate(RESPONSE_CODE code, int roomId, std::string& roomName)
{
    return true;
}

bool RpcClientHandler::ResRoomDelete(int roomId)
{
    return true;
}

bool RpcClientHandler::ReqEnterRoom(int roomId)
{
    return true;
}

bool RpcClientHandler::ResEnterRoom(RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst)
{
    return true;
}

bool RpcClientHandler::ResEnterOtherUser()
{
    return true;
}

bool RpcClientHandler::ReqPlayerList()
{
    return true;
}

bool RpcClientHandler::ResPlayerList(std::list<Player> playerList)
{
    return true;
}

bool RpcClientHandler::ReqChat(std::string& message)
{
    return true;
}

bool RpcClientHandler::ResChat(int playerId, std::string& message)
{
    return true;
}

bool RpcClientHandler::ReqUseItem(std::list<int>& lst)
{
    return true;
}

bool RpcClientHandler::ResUseItem(std::list<int>& lst)
{
    return true;
}
