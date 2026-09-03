#include "MyClient.h"
#include "../Utils/Packet.h"

#include "../RPC/RpcClientProxy.h"
#include "RpcModule.h"


void MyClient::AttachProxy(RpcClientProxy* rpcProxy)
{
	rpcProxy->client_ = this;
}

void MyClient::OnConnect()
{

}

void MyClient::OnRelease()
{

}

void MyClient::OnRecv(Packet* packet)
{
	__int64 sessionId = 0;

	// 함수의 인자 자료형 주의
	if (!PacketProc(packet))
	{
		//disconnect(sessionId);

		return;
	}

	delete packet;
}


//------------------------------------------------------------------------------------------------------//

bool MyClient::ResUserRegister(RESPONSE_CODE code)
{
	switch (code)
	{
	case RESPONSE_CODE::SUCCESS:

		strcpy_s(loginSceneStatus_, u8"회원가입이 성공하였습니다.");

		break;

	case RESPONSE_CODE::ALREADY_EXISTS:

		strcpy_s(loginSceneStatus_, u8"회원가입이 실패하였습니다.");

		break;
	}

	return true;
}

bool MyClient::ResUserLogin(RESPONSE_CODE code, int userId)
{
	switch (code)
	{
	case RESPONSE_CODE::SUCCESS:

		strcpy_s(loginSceneStatus_, u8"로그인에 성공하였습니다.");

		userId_ = userId;

		g_ClientRpcProxy.ReqPlayerConnection(userId_);

		break;

	case RESPONSE_CODE::LOGIN_FAILED:

		strcpy_s(loginSceneStatus_, u8"로그인에 실패하였습니다.");

		break;
	}

	return true;
}


bool MyClient::ResPlayerRegister(RESPONSE_CODE code)
{
	if (code == RESPONSE_CODE::SUCCESS)
	{
		strcpy_s(playerSceneStatus_, u8"플레이어 등록이 성공하였습니다.");

		// 메인씬 입장 재시도
		g_ClientRpcProxy.ReqPlayerConnection(userId_);

		//screen_ = ClientScreen::MAIN;
	}
	else
	{
		strcpy_s(playerSceneStatus_, u8"플레이어 등록이 실패하였습니다.");
	}

	return true;
}

bool MyClient::ResPlayerProfile(Player& player)
{
	if (player.playerId_ == 0)
	{
		scene_ = SCENE::PLAYER_REGISTER;

		return true;
	}
	else if (player.playerId_ == -1)
	{
		scene_ = SCENE::LOGIN;

		return true;
	}
	else
	{
		//TODO: 이동으로 변경??
		myPlayer_ = player;

		// 초기화
		playerMap_.clear();
		chatMessages_.clear();

		scene_ = SCENE::MAIN;

		g_ClientRpcProxy.ReqEnterLobby();

		return true;
	}
}

bool MyClient::ResPlayerCharacters(std::vector<Character>& characters)
{
	for (auto& c : characters)
	{
		myPlayer_.characters_.push_back(c);
	}

	return true;
}

bool MyClient::ResChat(int playerId, std::string& message)
{
	if (playerMap_.count(playerId) == 0)
		return true;

	// 플레이어 이름 + 메세지
	chatMessages_.push_back(playerMap_[playerId].playerName_ + " : " + message);

	return true;
}

bool MyClient::ResEnterLobby(PlayerInfo& player)
{
	// 다시 로비에 들어왔다면 정보 업데이트
	if (playerMap_.count(player.playerId_))
	{
		playerMap_[player.playerId_].level_ = player.level_;
		playerMap_[player.playerId_].state_ = player.state_;

		return true;
	}

	// TODO: player 복사가 일어남? 인자가 레퍼런스 였다면 어케됨?
	playerMap_.insert({ player.playerId_, player });

	return true;
}

bool MyClient::ResLeaveLobby(int playerId)
{
	playerMap_.erase(playerId);

	return true;
}

bool MyClient::ResBuyCharacter(Character& character, int curMoney)
{
	myPlayer_.money_ = curMoney;

	myPlayer_.characters_.push_back(character);

	return true;
}

bool MyClient::ResChangeEquipment(int inventoryId)
{
	myPlayer_.equippedInvenId_ = inventoryId;

	return true;
}

bool MyClient::ResStartMatch(STONE stone, PlayerInfo& opponent)
{
	chatMessages_.clear();

	opponentPlayer_ = opponent;

	myPlayer_.stone_ = stone;

	omokGame_.Initialize();

	scene_ = SCENE::GAME;

	return true;
}

bool MyClient::ResPlaceStone(short row, short col)
{
	omokGame_.PlaceStone(row, col);

	return true;
}

bool MyClient::ResGameResult(STONE stone, int level, int myMoney)
{
	// 클라이언트에서도 승리자를 계산하지만 한번 더 덮어씀.
	omokGame_.SetWinner(stone);

	myPlayer_.money_ = myMoney;
	myPlayer_.level_ = level;

	return true;
}

bool MyClient::ResChangePlayerLevel(int playerId, int level)
{
	if (myPlayer_.playerId_ == playerId)
		myPlayer_.level_ = level;

	playerMap_[playerId].level_ = level;

	return true;
}

bool MyClient::ResChangePlayerState(int playerId, PLAYER_STATE state)
{
	if (myPlayer_.playerId_ == playerId)
		myPlayer_.state_ = state;

	playerMap_[playerId].state_ = state;

	return true;
}

bool MyClient::ResLeaveRoom(int playerId)
{
	if (myPlayer_.playerId_ == playerId)
	{
		// 초기화
		playerMap_.clear();
		chatMessages_.clear();

		g_ClientRpcProxy.ReqEnterLobby();

		scene_ = SCENE::MAIN;
	}
	else if(opponentPlayer_.playerId_ == playerId)
	{
		opponentPlayer_.level_ = 0;
		opponentPlayer_.playerId_ = 0;
		opponentPlayer_.playerName_ = "";
	}

	return true;
}
