#pragma once

#include "../Utils/Packet.h"

class RpcClientStub
{
public:

    bool PacketProc(Packet* packet);
    
    virtual bool ReqUserRegister(std::string& loginId, std::string& password);
    virtual bool ResUserRegister(RESPONSE_CODE code);
    virtual bool ReqUserLogin(std::string& loginId, std::string& password);
    virtual bool ResUserLogin(RESPONSE_CODE code, int userId);
    virtual bool ReqPlayerRegister(int userId, std::string& playerName);
    virtual bool ResPlayerRegister(RESPONSE_CODE code);
    virtual bool ReqPlayerConnection(int userId);
    virtual bool ResPlayerProfile(Player& player);
    virtual bool ResPlayerCharacters(std::vector<Character>& characters);
    virtual bool ReqEnterLobby();
    virtual bool ResEnterLobby(PlayerInfo& player);
    virtual bool ResLeaveLobby(int playerId);
    virtual bool ReqChat(std::string& message);
    virtual bool ResChat(int playerId, std::string& message);
    virtual bool ReqBuyCharacter();
    virtual bool ResBuyCharacter(Character& character, int curMoney);
    virtual bool ReqChangeEquipment(int inventoryId);
    virtual bool ResChangeEquipment(int inventoryId);
    virtual bool ReqStartMatch();
    virtual bool ResStartMatch(STONE stone, PlayerInfo& opponent);
    virtual bool ReqCancelMatch();
    virtual bool ReqPlaceStone(short row, short col);
    virtual bool ResPlaceStone(short row, short col);
    virtual bool ResGameResult(STONE stone, int level, int myMoney);
    virtual bool ResChangePlayerLevel(int playerId, int level);
    virtual bool ResChangePlayerState(int playerId, PLAYER_STATE state);
    virtual bool ResEnterRoom(int playerId);
    virtual bool ReqLeaveRoom();
    virtual bool ResLeaveRoom(int playerId);
};
