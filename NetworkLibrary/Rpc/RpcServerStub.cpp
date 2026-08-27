#include "RpcServerStub.h"


bool RpcServerStub::PacketProc(__int64 sessionId, Packet* packet)
{
    switch (packet->GetHeaderPtr()->type_)
    {
        case 0:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->ReqUserRegister(sessionId, loginId, password);
        }
        case 1:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResUserRegister(sessionId, code);
        }
        case 2:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->ReqUserLogin(sessionId, loginId, password);
        }
        case 3:
        {
			RESPONSE_CODE code;
			int userId;

            *packet >> code >> userId;

            return handler_->ResUserLogin(sessionId, code, userId);
        }
        case 10:
        {
			int userId;
			std::string playerName;

            *packet >> userId >> playerName;

            return handler_->ReqPlayerRegister(sessionId, userId, playerName);
        }
        case 11:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResPlayerRegister(sessionId, code);
        }
        case 12:
        {
			int userId;

            *packet >> userId;

            return handler_->ReqPlayerEnterLobby(sessionId, userId);
        }
        case 13:
        {
			Player player;

            *packet >> player;

            return handler_->ResPlayerProfile(sessionId, player);
        }
        case 14:
        {
			std::list<Character> characterList;

            *packet >> characterList;

            return handler_->ResPlayerCharacters(sessionId, characterList);
        }
        case 30:
        {
			std::string message;

            *packet >> message;

            return handler_->ReqChat(sessionId, message);
        }
        case 31:
        {
			int playerId;
			std::string message;

            *packet >> playerId >> message;

            return handler_->ResChat(sessionId, playerId, message);
        }
        case 40:
        {

            *packet;

            return handler_->ReqLobbyPlayers(sessionId);
        }
        case 41:
        {
			std::list<Player> playerList;

            *packet >> playerList;

            return handler_->ResLobbyPlayers(sessionId, playerList);
        }
        case 42:
        {
			Player player;

            *packet >> player;

            return handler_->ResPlayerEnterLobby(sessionId, player);
        }
        case 43:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ResPlayerLeaveLobby(sessionId, playerId);
        }
        case 50:
        {

            *packet;

            return handler_->ReqBuyCharacter(sessionId);
        }
        case 51:
        {
			Character character;
			int currentMoney;

            *packet >> character >> currentMoney;

            return handler_->ResBuyCharacter(sessionId, character, currentMoney);
        }
        case 60:
        {

            *packet;

            return handler_->ReqEnterMatchQueue(sessionId);
        }
        case 61:
        {
			Player otherPlayer;

            *packet >> otherPlayer;

            return handler_->ResEnterMatchQueue(sessionId, otherPlayer);
        }
        case 62:
        {
			int result;
			int currentMoney;

            *packet >> result >> currentMoney;

            return handler_->ResEndMatch(sessionId, result, currentMoney);
        }
    default:
    {
        // 로그 추가

        return false;
    }
    }
}


bool RpcServerHandler::ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool RpcServerHandler::ResUserRegister(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool RpcServerHandler::ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool RpcServerHandler::ResUserLogin(__int64 sessionId, RESPONSE_CODE code, int userId)
{
    return true;
}

bool RpcServerHandler::ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName)
{
    return true;
}

bool RpcServerHandler::ResPlayerRegister(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool RpcServerHandler::ReqPlayerEnterLobby(__int64 sessionId, int userId)
{
    return true;
}

bool RpcServerHandler::ResPlayerProfile(__int64 sessionId, Player player)
{
    return true;
}

bool RpcServerHandler::ResPlayerCharacters(__int64 sessionId, std::list<Character> characterList)
{
    return true;
}

bool RpcServerHandler::ReqChat(__int64 sessionId, std::string& message)
{
    return true;
}

bool RpcServerHandler::ResChat(__int64 sessionId, int playerId, std::string& message)
{
    return true;
}

bool RpcServerHandler::ReqLobbyPlayers(__int64 sessionId)
{
    return true;
}

bool RpcServerHandler::ResLobbyPlayers(__int64 sessionId, std::list<Player> playerList)
{
    return true;
}

bool RpcServerHandler::ResPlayerEnterLobby(__int64 sessionId, Player player)
{
    return true;
}

bool RpcServerHandler::ResPlayerLeaveLobby(__int64 sessionId, int playerId)
{
    return true;
}

bool RpcServerHandler::ReqBuyCharacter(__int64 sessionId)
{
    return true;
}

bool RpcServerHandler::ResBuyCharacter(__int64 sessionId, Character character, int currentMoney)
{
    return true;
}

bool RpcServerHandler::ReqEnterMatchQueue(__int64 sessionId)
{
    return true;
}

bool RpcServerHandler::ResEnterMatchQueue(__int64 sessionId, Player otherPlayer)
{
    return true;
}

bool RpcServerHandler::ResEndMatch(__int64 sessionId, int result, int currentMoney)
{
    return true;
}
