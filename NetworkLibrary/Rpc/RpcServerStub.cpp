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

            return ReqUserRegister(sessionId, loginId, password);
        }
        case 1:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return ResUserRegister(sessionId, code);
        }
        case 2:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return ReqUserLogin(sessionId, loginId, password);
        }
        case 3:
        {
			RESPONSE_CODE code;
			int userId;

            *packet >> code >> userId;

            return ResUserLogin(sessionId, code, userId);
        }
        case 10:
        {
			int userId;
			std::string playerName;

            *packet >> userId >> playerName;

            return ReqPlayerRegister(sessionId, userId, playerName);
        }
        case 11:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return ResPlayerRegister(sessionId, code);
        }
        case 12:
        {
			int userId;

            *packet >> userId;

            return ReqPlayerConnection(sessionId, userId);
        }
        case 13:
        {
			Player player;

            *packet >> player;

            return ResPlayerProfile(sessionId, player);
        }
        case 14:
        {
			std::vector<Character> characters;

            *packet >> characters;

            return ResPlayerCharacters(sessionId, characters);
        }
        case 15:
        {
			std::vector<PlayerInfo> players;

            *packet >> players;

            return ResLobbyPlayers(sessionId, players);
        }
        case 16:
        {
			PlayerInfo player;

            *packet >> player;

            return ResPlayerEnterLobby(sessionId, player);
        }
        case 17:
        {
			int playerId;

            *packet >> playerId;

            return ResPlayerLeaveLobby(sessionId, playerId);
        }
        case 30:
        {
			std::string message;

            *packet >> message;

            return ReqChat(sessionId, message);
        }
        case 31:
        {
			int playerId;
			std::string message;

            *packet >> playerId >> message;

            return ResChat(sessionId, playerId, message);
        }
        case 50:
        {

            *packet;

            return ReqBuyCharacter(sessionId);
        }
        case 51:
        {
			Character character;
			int curGold;

            *packet >> character >> curGold;

            return ResBuyCharacter(sessionId, character, curGold);
        }
        case 52:
        {
			int inventoryId;

            *packet >> inventoryId;

            return ReqChangeEquipment(sessionId, inventoryId);
        }
        case 53:
        {
			int inventoryId;

            *packet >> inventoryId;

            return ResChangeEquipment(sessionId, inventoryId);
        }
        case 60:
        {

            *packet;

            return ReqStartGame(sessionId);
        }
        case 61:
        {
			PlayerInfo otherPlayer;

            *packet >> otherPlayer;

            return ResStartGame(sessionId, otherPlayer);
        }
        case 62:
        {

            *packet;

            return ReqCancelGame(sessionId);
        }
        case 65:
        {
			int commandType;

            *packet >> commandType;

            return ReqGameCommand(sessionId, commandType);
        }
        case 66:
        {
			Character character;

            *packet >> character;

            return ResGameCommand(sessionId, character);
        }
        case 70:
        {
			int result;
			int currentMoney;

            *packet >> result >> currentMoney;

            return ResEndGame(sessionId, result, currentMoney);
        }
    default:
    {
        // 로그 추가

        return false;
    }
    }
}


bool RpcServerStub::ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool RpcServerStub::ResUserRegister(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool RpcServerStub::ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool RpcServerStub::ResUserLogin(__int64 sessionId, RESPONSE_CODE code, int userId)
{
    return true;
}

bool RpcServerStub::ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName)
{
    return true;
}

bool RpcServerStub::ResPlayerRegister(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool RpcServerStub::ReqPlayerConnection(__int64 sessionId, int userId)
{
    return true;
}

bool RpcServerStub::ResPlayerProfile(__int64 sessionId, Player& player)
{
    return true;
}

bool RpcServerStub::ResPlayerCharacters(__int64 sessionId, std::vector<Character>& characters)
{
    return true;
}

bool RpcServerStub::ResLobbyPlayers(__int64 sessionId, std::vector<PlayerInfo>& players)
{
    return true;
}

bool RpcServerStub::ResPlayerEnterLobby(__int64 sessionId, PlayerInfo& player)
{
    return true;
}

bool RpcServerStub::ResPlayerLeaveLobby(__int64 sessionId, int playerId)
{
    return true;
}

bool RpcServerStub::ReqChat(__int64 sessionId, std::string& message)
{
    return true;
}

bool RpcServerStub::ResChat(__int64 sessionId, int playerId, std::string& message)
{
    return true;
}

bool RpcServerStub::ReqBuyCharacter(__int64 sessionId)
{
    return true;
}

bool RpcServerStub::ResBuyCharacter(__int64 sessionId, Character& character, int curGold)
{
    return true;
}

bool RpcServerStub::ReqChangeEquipment(__int64 sessionId, int inventoryId)
{
    return true;
}

bool RpcServerStub::ResChangeEquipment(__int64 sessionId, int inventoryId)
{
    return true;
}

bool RpcServerStub::ReqStartGame(__int64 sessionId)
{
    return true;
}

bool RpcServerStub::ResStartGame(__int64 sessionId, PlayerInfo& otherPlayer)
{
    return true;
}

bool RpcServerStub::ReqCancelGame(__int64 sessionId)
{
    return true;
}

bool RpcServerStub::ReqGameCommand(__int64 sessionId, int commandType)
{
    return true;
}

bool RpcServerStub::ResGameCommand(__int64 sessionId, Character& character)
{
    return true;
}

bool RpcServerStub::ResEndGame(__int64 sessionId, int result, int currentMoney)
{
    return true;
}
