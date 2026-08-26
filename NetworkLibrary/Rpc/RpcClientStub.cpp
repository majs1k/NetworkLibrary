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

            return handler_->ReqUserRegister(loginId, password);
        }
        case 1:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResUserRegister(code);
        }
        case 2:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->ReqUserLogin(loginId, password);
        }
        case 3:
        {
			RESPONSE_CODE code;
			int userId;

            *packet >> code >> userId;

            return handler_->ResUserLogin(code, userId);
        }
        case 10:
        {
			int userId;
			std::string playerName;

            *packet >> userId >> playerName;

            return handler_->ReqPlayerRegister(userId, playerName);
        }
        case 11:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResPlayerRegister(code);
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
        case 14:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ReqCharacterList(playerId);
        }
        case 15:
        {
			RESPONSE_CODE code;
			std::list<Character> characterList;

            *packet >> code >> characterList;

            return handler_->ResCharacterList(code, characterList);
        }
        case 20:
        {

            *packet;

            return handler_->ReqPlayerList();
        }
        case 21:
        {
			std::list<Player> playerList;

            *packet >> playerList;

            return handler_->ResPlayerList(playerList);
        }
        case 22:
        {
			Player player;

            *packet >> player;

            return handler_->ResPlayerEnter(player);
        }
        case 23:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ResPlayerLeave(playerId);
        }
        case 24:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ResPlayerDelete(playerId);
        }
        case 30:
        {
			std::string message;

            *packet >> message;

            return handler_->ReqChat(message);
        }
        case 31:
        {
			int playerId;
			std::string message;

            *packet >> playerId >> message;

            return handler_->ResChat(playerId, message);
        }
        case 50:
        {
			int roomId;

            *packet >> roomId;

            return handler_->ReqEnterRoom(roomId);
        }
        case 51:
        {
			RESPONSE_CODE code;
			int roomId;
			std::string roomName;
			std::list<int> lst;

            *packet >> code >> roomId >> roomName >> lst;

            return handler_->ResEnterRoom(code, roomId, roomName, lst);
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


bool RpcClientHandler::ReqUserRegister(std::string& loginId, std::string& password)
{
    return true;
}

bool RpcClientHandler::ResUserRegister(RESPONSE_CODE code)
{
    return true;
}

bool RpcClientHandler::ReqUserLogin(std::string& loginId, std::string& password)
{
    return true;
}

bool RpcClientHandler::ResUserLogin(RESPONSE_CODE code, int userId)
{
    return true;
}

bool RpcClientHandler::ReqPlayerRegister(int userId, std::string& playerName)
{
    return true;
}

bool RpcClientHandler::ResPlayerRegister(RESPONSE_CODE code)
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

bool RpcClientHandler::ReqCharacterList(int playerId)
{
    return true;
}

bool RpcClientHandler::ResCharacterList(RESPONSE_CODE code, std::list<Character> characterList)
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

bool RpcClientHandler::ResPlayerEnter(Player player)
{
    return true;
}

bool RpcClientHandler::ResPlayerLeave(int playerId)
{
    return true;
}

bool RpcClientHandler::ResPlayerDelete(int playerId)
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

bool RpcClientHandler::ReqEnterRoom(int roomId)
{
    return true;
}

bool RpcClientHandler::ResEnterRoom(RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst)
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
