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
			std::vector<Icon> icons;

            *packet >> icons;

            return ResPlayerIcons(sessionId, icons);
        }
        case 15:
        {

            *packet;

            return ReqEnterLobby(sessionId);
        }
        case 16:
        {
			PlayerInfo player;

            *packet >> player;

            return ResEnterLobby(sessionId, player);
        }
        case 17:
        {
			int playerId;

            *packet >> playerId;

            return ResLeaveLobby(sessionId, playerId);
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

            return ReqBuyIcon(sessionId);
        }
        case 51:
        {
			Icon icon;
			int curMoney;

            *packet >> icon >> curMoney;

            return ResBuyIcon(sessionId, icon, curMoney);
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

            return ReqStartMatch(sessionId);
        }
        case 61:
        {
			STONE stone;
			PlayerInfo opponent;

            *packet >> stone >> opponent;

            return ResStartMatch(sessionId, stone, opponent);
        }
        case 62:
        {

            *packet;

            return ReqCancelMatch(sessionId);
        }
        case 65:
        {
			short row;
			short col;

            *packet >> row >> col;

            return ReqPlaceStone(sessionId, row, col);
        }
        case 66:
        {
			short row;
			short col;

            *packet >> row >> col;

            return ResPlaceStone(sessionId, row, col);
        }
        case 67:
        {
			STONE winner;
			int myRating;
			int opponentRating;
			int myMoney;

            *packet >> winner >> myRating >> opponentRating >> myMoney;

            return ResGameResult(sessionId, winner, myRating, opponentRating, myMoney);
        }
        case 68:
        {
			int playerId;
			int rating;

            *packet >> playerId >> rating;

            return ResChangePlayerRating(sessionId, playerId, rating);
        }
        case 69:
        {
			int playerId;
			PLAYER_STATE state;

            *packet >> playerId >> state;

            return ResChangePlayerState(sessionId, playerId, state);
        }
        case 70:
        {
			int playerId;

            *packet >> playerId;

            return ResEnterRoom(sessionId, playerId);
        }
        case 71:
        {

            *packet;

            return ReqLeaveRoom(sessionId);
        }
        case 72:
        {
			int playerId;

            *packet >> playerId;

            return ResLeaveRoom(sessionId, playerId);
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

bool RpcServerStub::ResPlayerIcons(__int64 sessionId, std::vector<Icon>& icons)
{
    return true;
}

bool RpcServerStub::ReqEnterLobby(__int64 sessionId)
{
    return true;
}

bool RpcServerStub::ResEnterLobby(__int64 sessionId, PlayerInfo& player)
{
    return true;
}

bool RpcServerStub::ResLeaveLobby(__int64 sessionId, int playerId)
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

bool RpcServerStub::ReqBuyIcon(__int64 sessionId)
{
    return true;
}

bool RpcServerStub::ResBuyIcon(__int64 sessionId, Icon& icon, int curMoney)
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

bool RpcServerStub::ReqStartMatch(__int64 sessionId)
{
    return true;
}

bool RpcServerStub::ResStartMatch(__int64 sessionId, STONE stone, PlayerInfo& opponent)
{
    return true;
}

bool RpcServerStub::ReqCancelMatch(__int64 sessionId)
{
    return true;
}

bool RpcServerStub::ReqPlaceStone(__int64 sessionId, short row, short col)
{
    return true;
}

bool RpcServerStub::ResPlaceStone(__int64 sessionId, short row, short col)
{
    return true;
}

bool RpcServerStub::ResGameResult(__int64 sessionId, STONE winner, int myRating, int opponentRating, int myMoney)
{
    return true;
}

bool RpcServerStub::ResChangePlayerRating(__int64 sessionId, int playerId, int rating)
{
    return true;
}

bool RpcServerStub::ResChangePlayerState(__int64 sessionId, int playerId, PLAYER_STATE state)
{
    return true;
}

bool RpcServerStub::ResEnterRoom(__int64 sessionId, int playerId)
{
    return true;
}

bool RpcServerStub::ReqLeaveRoom(__int64 sessionId)
{
    return true;
}

bool RpcServerStub::ResLeaveRoom(__int64 sessionId, int playerId)
{
    return true;
}
