#pragma once

#include "../../Network/LanClient.h"

#include "../ResponseCode.h"
#include "../Player.h"
#include "../Icon.h"

#include "../../RPC/RpcClientStub.h"

#include "../RpcModule.h"

#include <string>
#include <utility>

class DummyClient : public LanClient, public RpcClientStub
{
public:
    enum class STATE
    {
        CONNECTING,
        LOGIN,
        PLAYER_CONNECTION,
        PLAYER_REGISTER,
        LOBBY,
        MATCHING,
        GAME,
        GAME_OVER,
        FINISHED,
        ERR
    };

public:
    explicit DummyClient(int dummyId);
    ~DummyClient() override = default;

public:
    bool Start(const std::wstring& ip, int port);
    void Update();

    bool IsFinished() const;
    bool IsError() const;

    int GetDummyId() const;
    int GetUserId() const;
    STATE GetState() const;

protected:
    // LanClient
    void OnConnect() override;
    void OnRelease() override;
    void OnRecv(Packet* packet) override;

    // RpcClientStub
    bool ResUserRegister(RESPONSE_CODE code) override;
    bool ResUserLogin(RESPONSE_CODE code, int userId) override;

    bool ResPlayerRegister(RESPONSE_CODE code) override;
    bool ResPlayerProfile(Player& player) override;

    bool ResEnterLobby(PlayerInfo& player) override;
    bool ResLeaveLobby(int playerId) override;

    bool ResStartMatch(STONE stone, PlayerInfo& opponent) override;
    bool ResPlaceStone(short row, short col) override;

    bool ResGameResult(
        STONE winner,
        int myRating,
        int opponentRating,
        int myMoney) override;

    bool ResLeaveRoom(int playerId) override;

private:
    void TryMakeMove();

    std::pair<short, short> GetNextMove();

    void ResetGame();

private:
    RpcClientProxy rpcProxy_;

    int dummyId_ = 0;
    int userId_ = 0;

    std::string loginId_;
    std::string password_;
    std::string playerName_;

    STATE state_ = STATE::CONNECTING;

    STONE myStone_ = STONE::NONE;

    bool myTurn_ = false;
    bool gameOver_ = false;

    bool board_[15][15]{};

    int moveCount_ = 0;
    int nextMoveIndex_ = 0;
};