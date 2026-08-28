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

            return handler_->ReqPlayerEnterLobby(userId);
        }
        case 13:
        {
			Player player;

            *packet >> player;

            return handler_->ResPlayerProfile(player);
        }
        case 14:
        {
			std::list<Character> characters;

            *packet >> characters;

            return handler_->ResPlayerCharacters(characters);
        }
        case 15:
        {
			std::list<PlayerInfo> players;

            *packet >> players;

            return handler_->ResLobbyPlayers(players);
        }
        case 16:
        {
			PlayerInfo player;

            *packet >> player;

            return handler_->ResPlayerEnterLobby(player);
        }
        case 17:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ResPlayerLeaveLobby(playerId);
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

            *packet;

            return handler_->ReqBuyCharacter();
        }
        case 51:
        {
			Character character;
			int currentMoney;

            *packet >> character >> currentMoney;

            return handler_->ResBuyCharacter(character, currentMoney);
        }
        case 60:
        {

            *packet;

            return handler_->ReqEnterMatchQueue();
        }
        case 61:
        {
			PlayerInfo otherPlayer;

            *packet >> otherPlayer;

            return handler_->ResEnterMatchQueue(otherPlayer);
        }
        case 62:
        {
			int result;
			int currentMoney;

            *packet >> result >> currentMoney;

            return handler_->ResEndMatch(result, currentMoney);
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

bool RpcClientHandler::ReqPlayerEnterLobby(int userId)
{
    return true;
}

bool RpcClientHandler::ResPlayerProfile(Player& player)
{
    return true;
}

bool RpcClientHandler::ResPlayerCharacters(std::list<Character>& characters)
{
    return true;
}

bool RpcClientHandler::ResLobbyPlayers(std::list<PlayerInfo>& players)
{
    return true;
}

bool RpcClientHandler::ResPlayerEnterLobby(PlayerInfo& player)
{
    return true;
}

bool RpcClientHandler::ResPlayerLeaveLobby(int playerId)
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

bool RpcClientHandler::ReqBuyCharacter()
{
    return true;
}

bool RpcClientHandler::ResBuyCharacter(Character& character, int currentMoney)
{
    return true;
}

bool RpcClientHandler::ReqEnterMatchQueue()
{
    return true;
}

bool RpcClientHandler::ResEnterMatchQueue(PlayerInfo& otherPlayer)
{
    return true;
}

bool RpcClientHandler::ResEndMatch(int result, int currentMoney)
{
    return true;
}
