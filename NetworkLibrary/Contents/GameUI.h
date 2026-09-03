#pragma once
#include "MyClient.h"
#include "RpcModule.h"
#include "OmokGame.h"






class GameUI
{
private:

	MyClient* client_;

public:

	GameUI(MyClient* client)
		: client_(client)
	{
	}

public:

	void Draw()
	{
		switch (client_->scene_)
		{
		case SCENE::LOGIN:
			DrawLogin();
			break;

		case SCENE::PLAYER_REGISTER:
			DrawPlayerRegister();
			break;

		case SCENE::MAIN:
			DrawMain();
			break;

		case SCENE::GAME:
			DrawGame();
			break;

		}
	}

	void DrawLogin()
	{
		static char loginId[30] = "";
		static char password[30] = "";

		ImGui::Begin("Login");


		ImGui::Text("ID");
		ImGui::InputText("##LoginID", loginId, sizeof(loginId));

		ImGui::Text("Password");
		ImGui::InputText("##Password", password, sizeof(password), ImGuiInputTextFlags_Password);

		ImGui::Spacing();

		/// 로그인 버튼
		if (ImGui::Button("Login"))
		{
			if (loginId[0] == '\0' || password[0] == '\0')
			{
				strcpy_s(client_->loginSceneStatus_, u8"ID와 Password를 입력해주세요.");
			}
			else
			{
				strcpy_s(client_->loginSceneStatus_, u8"");

				std::string id = loginId;
				std::string pw = password;

				g_ClientRpcProxy.ReqUserLogin(id, pw);
			}
		}

		ImGui::SameLine();

		/// 회원가입 버튼
		if (ImGui::Button("Register"))
		{
			if (loginId[0] == '\0' || password[0] == '\0')
			{
				strcpy_s(client_->loginSceneStatus_, u8"ID와 Password를 입력해주세요.");
			}
			else
			{
				strcpy_s(client_->loginSceneStatus_, u8"");

				std::string id = loginId;
				std::string pw = password;

				g_ClientRpcProxy.ReqUserRegister(id, pw);
			}
		}

		ImGui::Spacing();

		/// 상태 메세지 출력
		ImGui::Text("%s", client_->loginSceneStatus_);


		ImGui::End();
	}


	void DrawPlayerRegister()
	{
		static char playerName[30] = "";

		ImGui::Begin("Player Register");

		ImGui::Text("Player Name");
		ImGui::InputText("##PlayerName", playerName, sizeof(playerName));

		ImGui::Spacing();

		/// 확인 버튼
		if (ImGui::Button(u8"확인"))
		{
			if (playerName[0] == '\0')
			{
				strcpy_s(client_->playerSceneStatus_, u8"플레이어 이름을 입력해주세요.");
			}
			else
			{
				strcpy_s(client_->playerSceneStatus_, u8"");

				std::string name = playerName;

				g_ClientRpcProxy.ReqPlayerRegister(client_->userId_, name);
			}
		}

		ImGui::Spacing();

		/// 상태 메시지
		ImGui::Text("%s", client_->playerSceneStatus_);

		ImGui::End();
	}



private:


	void DrawChatting()
	{
		static char chatInput[256];

		ImGui::BeginChild("ChatMessages", ImVec2(0, -40), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

		for (const std::string& message : client_->chatMessages_)
		{
			ImGui::TextUnformatted(message.c_str());
		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		{
			ImGui::SetScrollHereY(1.0f);
		}

		ImGui::EndChild();

		ImGui::PushItemWidth(-1);

		if (ImGui::InputText("##ChatInput", chatInput, sizeof(chatInput), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (chatInput[0] != '\0')
			{
				std::string chat = chatInput;
				g_ClientRpcProxy.ReqChat(chat);
				chatInput[0] = '\0';
				ImGui::SetKeyboardFocusHere(-1);
			}
		}

		ImGui::PopItemWidth();
	}

	void DrawMain()
	{
		static bool showCharacterList = false;
		static bool showShop = false;
		static bool showMatching = false;

		ImVec2 displaySize = ImGui::GetIO().DisplaySize;

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(displaySize);

		ImGui::Begin("MainUI", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::BeginChild("PlayerPanel", ImVec2(600, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::PushFont(titleFont);
		ImGui::Text(u8"내 정보");
		ImGui::PopFont();

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text(u8"이름 : %s", client_->myPlayer_.playerName_.c_str());
		ImGui::Text(u8"레벨 : %d", client_->myPlayer_.level_);
		ImGui::Text(u8"머니 : %d", client_->myPlayer_.money_);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button(u8"캐릭터 목록", ImVec2(180, 50)))
		{
			showCharacterList = true;
			ImGui::SetNextWindowSize(ImVec2(500, 400));
			ImGui::OpenPopup(u8"캐릭터 목록");
		}

		ImGui::SameLine();

		if (ImGui::Button(u8"상점", ImVec2(180, 50)))
		{
			showShop = true;
			ImGui::SetNextWindowSize(ImVec2(500, 300));
			ImGui::OpenPopup(u8"상점");
		}

		if (ImGui::BeginPopupModal(u8"캐릭터 목록", &showCharacterList, ImGuiWindowFlags_NoResize))
		{
			ImGui::BeginChild("CharacterList", ImVec2(0, 0), true);

			for (const Character& character : client_->myPlayer_.characters_)
			{
				// 화면에는 Character ID를 표시
				ImGui::Text(u8"Character ID : %d", character.characterId_);

				ImGui::SameLine();

				// 실제 장착 여부는 Inventory ID로 판단
				if (character.inventoryId_ == client_->myPlayer_.equippedInvenId_)
				{
					ImGui::Text(u8"[장착 중]");
				}
				else
				{
					// Inventory ID를 이용해서 버튼을 유일하게 만듦
					ImGui::PushID(character.inventoryId_);

					if (ImGui::Button(u8"선택", ImVec2(80, 30)))
					{
						g_ClientRpcProxy.ReqChangeEquipment(character.inventoryId_);
					}

					ImGui::PopID();
				}
			}

			ImGui::EndChild();

			ImGui::Separator();

			if (ImGui::Button(u8"닫기", ImVec2(100, 40)))
			{
				ImGui::CloseCurrentPopup();
				showCharacterList = false;
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal(u8"상점", &showShop, ImGuiWindowFlags_NoResize))
		{
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text(u8"일반 캐릭터를 구매하시겠습니까?\n가격: 1,000 골드");

			ImGui::SameLine();

			if (ImGui::Button(u8"구매하기", ImVec2(140, 45)))
			{
				if (client_->myPlayer_.money_ < 1000)
				{
					ImGui::OpenPopup(u8"구매실패");
				}
				else
				{
					g_ClientRpcProxy.ReqBuyCharacter();
					ImGui::OpenPopup(u8"구매성공");
				}
			}

			if (ImGui::BeginPopupModal(u8"구매실패", nullptr, ImGuiWindowFlags_NoResize))
			{
				ImGui::Text(u8"골드가 부족합니다.");

				if (ImGui::Button(u8"확인", ImVec2(100, 40)))
					ImGui::CloseCurrentPopup();

				ImGui::EndPopup();
			}

			if (ImGui::BeginPopupModal(u8"구매성공", nullptr, ImGuiWindowFlags_NoResize))
			{
				ImGui::Text(u8"구매를 성공하였습니다.");

				if (ImGui::Button(u8"확인", ImVec2(100, 40)))
					ImGui::CloseCurrentPopup();

				ImGui::EndPopup();
			}

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::EndPopup();
		}

		if (ImGui::Button(u8"게임 시작", ImVec2(-1, 50)))
		{
			showMatching = true;

			ImGui::SetNextWindowSize(ImVec2(300, 250));
			ImGui::OpenPopup(u8"게임 시작");

			g_ClientRpcProxy.ReqStartMatch();
		}

		if (ImGui::BeginPopupModal(u8"게임 시작", &showMatching, ImGuiWindowFlags_NoResize))
		{
			ImGui::Spacing();
			ImGui::Spacing();

			static int count = 0;
			count++;
			std::string dots((count / 30) % 4, '.');

			ImGui::Text(u8"게임을 찾는 중입니다.");
			ImGui::Text(u8"잠시만 기다려주세요%s", dots);

			ImGui::Spacing();

			if (ImGui::Button(u8"매칭 취소", ImVec2(140, 50)))
			{
				showMatching = false;
				g_ClientRpcProxy.ReqCancelMatch();
			}

			ImGui::EndPopup();
		}

		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("RightPanel", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::PushFont(titleFont);
		ImGui::Text(u8"접속 플레이어 : ");
		ImGui::SameLine();
		ImGui::Text(u8"%d", static_cast<int>(client_->playerMap_.size()));
		ImGui::PopFont();

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::BeginChild("PlayerList", ImVec2(0, 250), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

		for (auto& p : client_->playerMap_)
		{
			std::string playerState;
			if (p.second.state_ == PLAYER_STATE::LOBBY)
				playerState = u8"로비";
			else if (p.second.state_ == PLAYER_STATE::GAMEROOM)
				playerState = u8"게임 중";
			else
				playerState = u8"확인 불가";

			ImGui::Text(u8"%s  Lv.%d %s", p.second.playerName_.c_str(), p.second.level_, playerState);
		}

		ImGui::EndChild();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushFont(titleFont);
		ImGui::Text(u8"채팅");
		ImGui::PopFont();

		ImGui::Separator();
		ImGui::Spacing();

		DrawChatting();

		ImGui::EndChild();

		ImGui::End();
	}





	void DrawGame()
	{
		ImVec2 displaySize = ImGui::GetIO().DisplaySize;

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(displaySize);

		ImGui::Begin("GameUI", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | 
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImVec2 contentSize = ImGui::GetContentRegionAvail();

		const float rightPanelWidth = 450.0f;
		const float spacing = 10.0f;
		const float boardSize = 700.0f;
		const float boardCellSize = boardSize / 14.0f;

		// =========================================================
		// 왼쪽 게임 영역
		// =========================================================

		ImGui::BeginChild("GameBoardArea", ImVec2(contentSize.x - rightPanelWidth - spacing, 0), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		// =========================================================
		// 바둑판
		// =========================================================

		float boardAreaWidth = ImGui::GetContentRegionAvail().x;
		float boardOffsetX = (boardAreaWidth - boardSize) * 0.5f;

		if (boardOffsetX < 0.0f)
			boardOffsetX = 0.0f;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + boardOffsetX);

		ImGui::BeginChild("OmokBoard", ImVec2(boardSize + 80.0f, boardSize + 80.0f), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 boardPos = ImGui::GetCursorScreenPos();

		boardPos.x += 30.0f;
		boardPos.y += 30.0f;

		// ---------------------------------------------------------
		// 바둑판 배경
		// ---------------------------------------------------------

		drawList->AddRectFilled(ImVec2(boardPos.x - 30.0f, boardPos.y - 30.0f), ImVec2(boardPos.x + boardSize + 30.0f, boardPos.y + boardSize + 30.0f), 
			IM_COL32(205, 155, 80, 255), 4.0f);

		// ---------------------------------------------------------
		// 바둑판 선
		// ---------------------------------------------------------

		for (int i = 0; i < OmokGame::BOARD_SIZE; ++i)
		{
			float x = boardPos.x + i * boardCellSize;
			float y = boardPos.y + i * boardCellSize;

			drawList->AddLine(ImVec2(x, boardPos.y), ImVec2(x, boardPos.y + boardSize), IM_COL32(50, 35, 20, 255), 1.0f);
			drawList->AddLine(ImVec2(boardPos.x, y), ImVec2(boardPos.x + boardSize, y), IM_COL32(50, 35, 20, 255), 1.0f);
		}

		// ---------------------------------------------------------
		// 화점
		// ---------------------------------------------------------

		const int starPoints[5][2] = { { 3, 3 }, { 3, 11 }, { 7, 7 }, { 11, 3 }, { 11, 11 } };

		for (const auto& point : starPoints)
		{
			float x = boardPos.x + point[0] * boardCellSize;
			float y = boardPos.y + point[1] * boardCellSize;

			drawList->AddCircleFilled(ImVec2(x, y), 4.0f, IM_COL32(30, 20, 10, 255));
		}

		// ---------------------------------------------------------
		// 돌 그리기
		// ---------------------------------------------------------

		for (int row = 0; row < OmokGame::BOARD_SIZE; row++)
		{
			for (int col = 0; col < OmokGame::BOARD_SIZE; col++)
			{
				STONE stone = client_->omokGame_.GetStone(row, col);

				if (stone == STONE::NONE)
					continue;

				float x = boardPos.x + col * boardCellSize;
				float y = boardPos.y + row * boardCellSize;
				float radius = boardCellSize * 0.43f;

				// 흑
				if (stone == STONE::BLACK)
				{
					drawList->AddCircleFilled(ImVec2(x + 1.5f, y + 2.0f), radius, IM_COL32(0, 0, 0, 180), 32);
					drawList->AddCircleFilled(ImVec2(x, y), radius, IM_COL32(25, 25, 25, 255), 32);
					drawList->AddCircleFilled(ImVec2(x - radius * 0.3f, y - radius * 0.3f), radius * 0.15f, IM_COL32(120, 120, 120, 100), 32);
				}
				// 백
				else if (stone == STONE::WHITE)
				{
					drawList->AddCircleFilled(ImVec2(x + 1.5f, y + 2.0f), radius, IM_COL32(120, 120, 120, 180), 32);
					drawList->AddCircleFilled(ImVec2(x, y), radius, IM_COL32(230, 230, 230, 255), 32);
					drawList->AddCircleFilled(ImVec2(x - radius * 0.3f, y - radius * 0.3f), radius * 0.15f, IM_COL32(255, 255, 255, 220), 32);
				}
			}
		}

		// ---------------------------------------------------------
		// 바둑판 클릭
		// ---------------------------------------------------------

		ImGui::SetCursorScreenPos(boardPos);
		ImGui::InvisibleButton("BoardClickArea", ImVec2(boardSize, boardSize));

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			// TODO: 로직 검증
			if (!client_->omokGame_.IsGameOver() )
				// && client_->myPlayer_.stone_ == g_OmokGame.GetTurn()
			{
				ImVec2 mousePos = ImGui::GetIO().MousePos;
				float localX = mousePos.x - boardPos.x;
				float localY = mousePos.y - boardPos.y;

				int col = static_cast<int>(std::round(localX / boardCellSize));
				int row = static_cast<int>(std::round(localY / boardCellSize));

				if (row >= 0 && row < OmokGame::BOARD_SIZE && col >= 0 && col < OmokGame::BOARD_SIZE)
				{
					//g_OmokGame.PlaceStone(row, col);

					/// 바둑알 놓기 패킷 전송
					g_ClientRpcProxy.ReqPlaceStone(row, col);
				}
			}
		}

		ImGui::EndChild();
		ImGui::EndChild();

		// =========================================================
		// 오른쪽 영역
		// =========================================================

		ImGui::SameLine(0.0f, spacing);

		ImGui::BeginChild("GameRightArea", ImVec2(rightPanelWidth, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		// =========================================================
		// 플레이어 정보
		// =========================================================

		auto DrawPlayerInfo = [&](const char* name, int level, bool isBlack, bool isMyTurn)
			{
				ImVec4 backgroundColor = ImVec4(0.40f, 0.40f, 0.40f, 1.0f);
				ImVec4 borderColor = isMyTurn ? ImVec4(1.0f, 0.75f, 0.1f, 1.0f) : ImVec4(0.70f, 0.70f, 0.70f, 1.0f);

				ImGui::PushStyleColor(ImGuiCol_ChildBg, backgroundColor);
				ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, isMyTurn ? 3.0f : 1.0f);

				ImGui::BeginChild(isBlack ? "BlackPlayer" : "WhitePlayer", ImVec2(0, 105.0f), true);

				ImGui::Spacing();

				// -----------------------------------------------------
				// 닉네임
				// -----------------------------------------------------

				ImVec2 cursor = ImGui::GetCursorScreenPos();
				ImVec2 textPos = ImVec2(cursor.x, cursor.y);

				ImGui::SetCursorScreenPos(textPos);
				ImGui::TextColored(isBlack ? ImVec4(0, 0, 0, 1) : ImVec4(1, 1, 1, 1), u8"레벨 : %d %s", level, name);

				ImGui::EndChild();

				ImGui::PopStyleVar();
				ImGui::PopStyleColor(2);
			};

		// 위쪽 = 흑
		DrawPlayerInfo(u8"흑돌 플레이어", 25, true, client_->omokGame_.GetTurn() == STONE::BLACK);

		ImGui::Spacing();

		// 아래쪽 = 백
		DrawPlayerInfo(u8"백돌 플레이어", 18, false, client_->omokGame_.GetTurn() == STONE::WHITE);

		ImGui::Spacing();


		// =========================================================
		// 턴 타이머
		// =========================================================

		client_->omokGame_.UpdateTurnTimer(ImGui::GetIO().DeltaTime);

		

		// =========================================================
		// 제한 시간 영역
		// =========================================================

		ImGui::BeginChild("TurnTimer", ImVec2(0, 90.0f), true);

		if (!client_->omokGame_.IsGameOver())
		{
			ImGui::Text(u8"남은 시간 : %2d초", client_->omokGame_.GetTurnTime());
		}
		else
		{
			bool playerWon = client_->omokGame_.GetWinner() == client_->myPlayer_.stone_;

			if (playerWon)
				ImGui::TextColored(ImVec4(1.0f, 0.75f, 0.1f, 1.0f), u8"승리!");
			else
				ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f), u8"패배!");

			ImGui::SameLine();

			float buttonWidth = 120.0f;
			float buttonSpacing = 10.0f;

			if (ImGui::Button(u8"재대결", ImVec2(buttonWidth, 45.0f)))
			{
				if (client_->opponentPlayer_.playerId_ != 0)
				{
					//client_->omokGame_.Initialize();

					// TODO: 재대결 신청 패킷 전송
				}
			}

			ImGui::SameLine(0.0f, buttonSpacing);

			if (ImGui::Button(u8"나가기", ImVec2(buttonWidth, 45.0f)))
			{
				/// 게임방 나가기 패킷 전송
				g_ClientRpcProxy.ReqLeaveRoom();
			}
		}

		ImGui::EndChild();


		// =========================================================
		// 채팅
		// =========================================================

		ImGui::Spacing();

		ImGui::BeginChild("GameChatting", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);

		DrawChatting();

		ImGui::EndChild();

		ImGui::EndChild();


		ImGui::End();
	}


};
