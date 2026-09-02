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

            return ReqUserRegister(loginId, password);
        }
        case 1:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return ResUserRegister(code);
        }
        case 2:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return ReqUserLogin(loginId, password);
        }
        case 3:
        {
			RESPONSE_CODE code;
			int userId;

            *packet >> code >> userId;

            return ResUserLogin(code, userId);
        }
        case 10:
        {
			int userId;
			std::string playerName;

            *packet >> userId >> playerName;

            return ReqPlayerRegister(userId, playerName);
        }
        case 11:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return ResPlayerRegister(code);
        }
        case 12:
        {
			int userId;

            *packet >> userId;

            return ReqPlayerConnection(userId);
        }
        case 13:
        {
			Player player;

            *packet >> player;

            return ResPlayerProfile(player);
        }
        case 14:
        {
			std::vector<Character> characters;

            *packet >> characters;

            return ResPlayerCharacters(characters);
        }
        case 15:
        {
			std::vector<PlayerInfo> players;

            *packet >> players;

            return ResLobbyPlayers(players);
        }
        case 16:
        {
			PlayerInfo player;

            *packet >> player;

            return ResPlayerEnterLobby(player);
        }
        case 17:
        {
			int playerId;

            *packet >> playerId;

            return ResPlayerLeaveLobby(playerId);
        }
        case 30:
        {
			std::string message;

            *packet >> message;

            return ReqChat(message);
        }
        case 31:
        {
			int playerId;
			std::string message;

            *packet >> playerId >> message;

            return ResChat(playerId, message);
        }
        case 50:
        {

            *packet;

            return ReqBuyCharacter();
        }
        case 51:
        {
			Character character;
			int curGold;

            *packet >> character >> curGold;

            return ResBuyCharacter(character, curGold);
        }
        case 52:
        {
			int inventoryId;

            *packet >> inventoryId;

            return ReqChangeEquipment(inventoryId);
        }
        case 53:
        {
			int inventoryId;

            *packet >> inventoryId;

            return ResChangeEquipment(inventoryId);
        }
        case 60:
        {

            *packet;

            return ReqStartGame();
        }
        case 61:
        {
			PlayerInfo otherPlayer;

            *packet >> otherPlayer;

            return ResStartGame(otherPlayer);
        }
        case 62:
        {

            *packet;

            return ReqCancelGame();
        }
        case 65:
        {
			int commandType;

            *packet >> commandType;

            return ReqGameCommand(commandType);
        }
        case 66:
        {
			Character character;

            *packet >> character;

            return ResGameCommand(character);
        }
        case 70:
        {
			int result;
			int currentMoney;

            *packet >> result >> currentMoney;

            return ResEndGame(result, currentMoney);
        }
    default:
    {
        // 로그 추가

        return false;
    }
    }
}


bool RpcClientStub::ReqUserRegister(std::string& loginId, std::string& password)
{
    return true;
}

bool RpcClientStub::ResUserRegister(RESPONSE_CODE code)
{
    return true;
}

bool RpcClientStub::ReqUserLogin(std::string& loginId, std::string& password)
{
    return true;
}

bool RpcClientStub::ResUserLogin(RESPONSE_CODE code, int userId)
{
    return true;
}

bool RpcClientStub::ReqPlayerRegister(int userId, std::string& playerName)
{
    return true;
}

bool RpcClientStub::ResPlayerRegister(RESPONSE_CODE code)
{
    return true;
}

bool RpcClientStub::ReqPlayerConnection(int userId)
{
    return true;
}

bool RpcClientStub::ResPlayerProfile(Player& player)
{
    return true;
}

bool RpcClientStub::ResPlayerCharacters(std::vector<Character>& characters)
{
    return true;
}

bool RpcClientStub::ResLobbyPlayers(std::vector<PlayerInfo>& players)
{
    return true;
}

bool RpcClientStub::ResPlayerEnterLobby(PlayerInfo& player)
{
    return true;
}

bool RpcClientStub::ResPlayerLeaveLobby(int playerId)
{
    return true;
}

bool RpcClientStub::ReqChat(std::string& message)
{
    return true;
}

bool RpcClientStub::ResChat(int playerId, std::string& message)
{
    return true;
}

bool RpcClientStub::ReqBuyCharacter()
{
    return true;
}

bool RpcClientStub::ResBuyCharacter(Character& character, int curGold)
{
    return true;
}

bool RpcClientStub::ReqChangeEquipment(int inventoryId)
{
    return true;
}

bool RpcClientStub::ResChangeEquipment(int inventoryId)
{
    return true;
}

bool RpcClientStub::ReqStartGame()
{
    return true;
}

bool RpcClientStub::ResStartGame(PlayerInfo& otherPlayer)
{
    return true;
}

bool RpcClientStub::ReqCancelGame()
{
    return true;
}

bool RpcClientStub::ReqGameCommand(int commandType)
{
    return true;
}

bool RpcClientStub::ResGameCommand(Character& character)
{
    return true;
}

bool RpcClientStub::ResEndGame(int result, int currentMoney)
{
    return true;
}
