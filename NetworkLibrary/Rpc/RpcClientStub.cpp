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
			std::list<Character> characterList;

            *packet >> characterList;

            return handler_->ResPlayerCharacters(characterList);
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
        case 40:
        {

            *packet;

            return handler_->ReqLobbyPlayers();
        }
        case 41:
        {
			std::list<Player> playerList;

            *packet >> playerList;

            return handler_->ResLobbyPlayers(playerList);
        }
        case 42:
        {
			Player player;

            *packet >> player;

            return handler_->ResPlayerEnterLobby(player);
        }
        case 43:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ResPlayerLeaveLobby(playerId);
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

            return handler_->ReqEnterMatch();
        }
        case 61:
        {

            *packet;

            return handler_->ResEnterMatch();
        }
        case 62:
        {

            *packet;

            return handler_->ResStartMatch();
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

bool RpcClientHandler::ResPlayerProfile(Player player)
{
    return true;
}

bool RpcClientHandler::ResPlayerCharacters(std::list<Character> characterList)
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

bool RpcClientHandler::ReqLobbyPlayers()
{
    return true;
}

bool RpcClientHandler::ResLobbyPlayers(std::list<Player> playerList)
{
    return true;
}

bool RpcClientHandler::ResPlayerEnterLobby(Player player)
{
    return true;
}

bool RpcClientHandler::ResPlayerLeaveLobby(int playerId)
{
    return true;
}

bool RpcClientHandler::ReqBuyCharacter()
{
    return true;
}

bool RpcClientHandler::ResBuyCharacter(Character character, int currentMoney)
{
    return true;
}

bool RpcClientHandler::ReqEnterMatch()
{
    return true;
}

bool RpcClientHandler::ResEnterMatch()
{
    return true;
}

bool RpcClientHandler::ResStartMatch()
{
    return true;
}
