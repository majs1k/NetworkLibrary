#include "MyClient.h"
#include "../Utils/Packet.h"

#include "../RPC/RpcClientProxy.h"

MyClient::MyClient()
{
	rpcProxy_ = new RpcClientProxy();
	rpcStub_ = new RpcClientStub();

	rpcProxy_->client_ = this;
	rpcStub_->handler_ = this;
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
	if (!rpcStub_->PacketProc(packet))
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

		strcpy_s(loginSceneStatus, u8"회원가입이 성공하였습니다.");

		break;

	case RESPONSE_CODE::ALREADY_EXISTS:

		strcpy_s(loginSceneStatus, u8"회원가입이 실패하였습니다.");

		break;
	}

	return true;
}

bool MyClient::ResUserLogin(RESPONSE_CODE code, int userId)
{
	switch (code)
	{
	case RESPONSE_CODE::SUCCESS:

		strcpy_s(loginSceneStatus, u8"로그인에 성공하였습니다.");

		userId_ = userId;

		rpcProxy_->ReqPlayerEnterLobby(userId_);

		break;

	case RESPONSE_CODE::LOGIN_FAILED:

		strcpy_s(loginSceneStatus, u8"로그인에 실패하였습니다.");

		break;
	}

	return true;
}


bool MyClient::ResPlayerRegister(RESPONSE_CODE code)
{
	if (code == RESPONSE_CODE::SUCCESS)
	{
		strcpy_s(playerSceneStatus, u8"플레이어 등록이 성공하였습니다.");

		// 메인씬 입장 재시도
		rpcProxy_->ReqPlayerEnterLobby(userId_);

		//screen_ = ClientScreen::MAIN;
	}
	else
	{
		strcpy_s(playerSceneStatus, u8"플레이어 등록이 실패하였습니다.");
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
	else
	{
		//TODO: 이동으로 변경??
		myPlayer_ = player;

		scene_ = SCENE::MAIN;

		return true;
	}
}

bool MyClient::ResPlayerCharacters(std::list<Character>& characters)
{
	for (auto& c : characters)
	{
		myPlayer_.characterList_.push_back(c);
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

bool MyClient::ResLobbyPlayers(std::list<PlayerInfo>& players)
{
	for (auto& info : players)
	{
		playerMap_.insert({ info.playerId_, info });
	}

	return true;
}

bool MyClient::ResPlayerEnterLobby(PlayerInfo& player)
{
	// player 복사가 일어남? 인자가 레퍼런스 였다면 어케됨?
	playerMap_.insert({ player.playerId_, player });

	return true;
}

bool MyClient::ResPlayerLeaveLobby(int playerId)
{
	playerMap_.erase(playerId);

	return true;
}
