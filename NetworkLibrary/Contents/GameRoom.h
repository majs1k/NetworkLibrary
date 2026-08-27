#pragma once
#include <vector>	

class MyServer;
class Player;

class GameRoom
{
private:
	MyServer* server_;

	std::vector<Player*> players;
	const static int maxPlayers_ = 2;

public:

	// 2명 입장
	bool EnterGameRoom(Player* player)
	{
		if (players.size() > maxPlayers_)
			return false;

		players.push_back(player);

		return true;
	}

	void StartGame()
	{
		// 게임 시작 패킷 보내기?
	}

	void Update()
	{
		// 두명이 공격 명령 패킷을 보냈다면
		// 게임로직처리

		// 다음 턴 시작 패킷 보내기

		// 게임 종료시 EndGame() 호출
	}

	void EndGame()
	{
		// 게임 종료 패킷 보내기
		// 보상 지급 패킷 보내기 및 db 반영

		// 게임룸 삭제?
	}

};

class GameRoomManager
{
private:
	std::vector<GameRoom*> rooms_;

public:

	GameRoom* CreateGameRoom()
	{
		GameRoom* gm = new GameRoom();
		rooms_.push_back(gm);

		return gm;
	}

	void Update()
	{
		for (auto& i : rooms_)
		{
			i->Update();
		}
	}
};
