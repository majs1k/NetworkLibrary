#include "DbStub.h"


bool DbStub::DbPacketProc(__int64 sessionId, Packet* packet)
{
    switch (packet->GetHeaderPtr()->type_)
    {
        case 0:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return ReqUserRegisterDB(sessionId, loginId, password);
        }
        case 1:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return ResUserRegisterDB(sessionId, code);
        }
        case 2:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return ReqUserLoginDB(sessionId, loginId, password);
        }
        case 3:
        {
			RESPONSE_CODE code;
			int userId;

            *packet >> code >> userId;

            return ResUserLoginDB(sessionId, code, userId);
        }
        case 10:
        {
			int userId;
			std::string playerName;

            *packet >> userId >> playerName;

            return ReqPlayerRegisterDB(sessionId, userId, playerName);
        }
        case 11:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return ResPlayerRegisterDB(sessionId, code);
        }
        case 12:
        {
			int userId;

            *packet >> userId;

            return ReqPlayerConnectionDB(sessionId, userId);
        }
        case 13:
        {
			Player player;

            *packet >> player;

            return ResPlayerProfileDB(sessionId, player);
        }
        case 14:
        {
			std::vector<Character> characters;

            *packet >> characters;

            return ResPlayerCharactersDB(sessionId, characters);
        }
        case 15:
        {
			std::vector<PlayerInfo> players;

            *packet >> players;

            return ResLobbyPlayersDB(sessionId, players);
        }
        case 16:
        {
			PlayerInfo player;

            *packet >> player;

            return ResPlayerEnterLobbyDB(sessionId, player);
        }
        case 17:
        {
			int playerId;

            *packet >> playerId;

            return ResPlayerLeaveLobbyDB(sessionId, playerId);
        }
        case 30:
        {
			std::string message;

            *packet >> message;

            return ReqChatDB(sessionId, message);
        }
        case 31:
        {
			int playerId;
			std::string message;

            *packet >> playerId >> message;

            return ResChatDB(sessionId, playerId, message);
        }
        case 50:
        {
			int playerId;
			int inventoryId;
			int characterId;
			int curGold;

            *packet >> playerId >> inventoryId >> characterId >> curGold;

            return ReqBuyCharacterDB(sessionId, playerId, inventoryId, characterId, curGold);
        }
        case 51:
        {
			Character character;
			int curGold;

            *packet >> character >> curGold;

            return ResBuyCharacterDB(sessionId, character, curGold);
        }
        case 52:
        {
			int playerId;
			int inventoryId;

            *packet >> playerId >> inventoryId;

            return ReqChangeEquipmentDB(sessionId, playerId, inventoryId);
        }
        case 70:
        {
			int result;
			int currentMoney;

            *packet >> result >> currentMoney;

            return ReqEndGameDB(sessionId, result, currentMoney);
        }
    default:
    {
        // 로그 추가

        return false;
    }
    }
}


bool DbStub::ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool DbStub::ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool DbStub::ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool DbStub::ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId)
{
    return true;
}

bool DbStub::ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName)
{
    return true;
}

bool DbStub::ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool DbStub::ReqPlayerConnectionDB(__int64 sessionId, int userId)
{
    return true;
}

bool DbStub::ResPlayerProfileDB(__int64 sessionId, Player& player)
{
    return true;
}

bool DbStub::ResPlayerCharactersDB(__int64 sessionId, std::vector<Character>& characters)
{
    return true;
}

bool DbStub::ResLobbyPlayersDB(__int64 sessionId, std::vector<PlayerInfo>& players)
{
    return true;
}

bool DbStub::ResPlayerEnterLobbyDB(__int64 sessionId, PlayerInfo& player)
{
    return true;
}

bool DbStub::ResPlayerLeaveLobbyDB(__int64 sessionId, int playerId)
{
    return true;
}

bool DbStub::ReqChatDB(__int64 sessionId, std::string& message)
{
    return true;
}

bool DbStub::ResChatDB(__int64 sessionId, int playerId, std::string& message)
{
    return true;
}

bool DbStub::ReqBuyCharacterDB(__int64 sessionId, int playerId, int inventoryId, int characterId, int curGold)
{
    return true;
}

bool DbStub::ResBuyCharacterDB(__int64 sessionId, Character& character, int curGold)
{
    return true;
}

bool DbStub::ReqChangeEquipmentDB(__int64 sessionId, int playerId, int inventoryId)
{
    return true;
}

bool DbStub::ReqEndGameDB(__int64 sessionId, int result, int currentMoney)
{
    return true;
}
