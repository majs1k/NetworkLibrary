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

            *packet;

            return ReqEnterLobby();
        }
        case 16:
        {
			PlayerInfo player;

            *packet >> player;

            return ResEnterLobby(player);
        }
        case 17:
        {
			int playerId;

            *packet >> playerId;

            return ResLeaveLobby(playerId);
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
			int curMoney;

            *packet >> character >> curMoney;

            return ResBuyCharacter(character, curMoney);
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

            return ReqStartMatch();
        }
        case 61:
        {
			STONE stone;
			PlayerInfo opponent;

            *packet >> stone >> opponent;

            return ResStartMatch(stone, opponent);
        }
        case 62:
        {

            *packet;

            return ReqCancelMatch();
        }
        case 65:
        {
			short row;
			short col;

            *packet >> row >> col;

            return ReqPlaceStone(row, col);
        }
        case 66:
        {
			short row;
			short col;

            *packet >> row >> col;

            return ResPlaceStone(row, col);
        }
        case 67:
        {
			STONE stone;
			int level;
			int myMoney;

            *packet >> stone >> level >> myMoney;

            return ResGameResult(stone, level, myMoney);
        }
        case 68:
        {
			int playerId;
			int level;

            *packet >> playerId >> level;

            return ResChangePlayerLevel(playerId, level);
        }
        case 69:
        {
			int playerId;
			PLAYER_STATE state;

            *packet >> playerId >> state;

            return ResChangePlayerState(playerId, state);
        }
        case 70:
        {
			int playerId;

            *packet >> playerId;

            return ResEnterRoom(playerId);
        }
        case 71:
        {

            *packet;

            return ReqLeaveRoom();
        }
        case 72:
        {
			int playerId;

            *packet >> playerId;

            return ResLeaveRoom(playerId);
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

bool RpcClientStub::ReqEnterLobby()
{
    return true;
}

bool RpcClientStub::ResEnterLobby(PlayerInfo& player)
{
    return true;
}

bool RpcClientStub::ResLeaveLobby(int playerId)
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

bool RpcClientStub::ResBuyCharacter(Character& character, int curMoney)
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

bool RpcClientStub::ReqStartMatch()
{
    return true;
}

bool RpcClientStub::ResStartMatch(STONE stone, PlayerInfo& opponent)
{
    return true;
}

bool RpcClientStub::ReqCancelMatch()
{
    return true;
}

bool RpcClientStub::ReqPlaceStone(short row, short col)
{
    return true;
}

bool RpcClientStub::ResPlaceStone(short row, short col)
{
    return true;
}

bool RpcClientStub::ResGameResult(STONE stone, int level, int myMoney)
{
    return true;
}

bool RpcClientStub::ResChangePlayerLevel(int playerId, int level)
{
    return true;
}

bool RpcClientStub::ResChangePlayerState(int playerId, PLAYER_STATE state)
{
    return true;
}

bool RpcClientStub::ResEnterRoom(int playerId)
{
    return true;
}

bool RpcClientStub::ReqLeaveRoom()
{
    return true;
}

bool RpcClientStub::ResLeaveRoom(int playerId)
{
    return true;
}
