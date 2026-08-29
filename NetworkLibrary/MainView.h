#pragma once
#include "./Contents/MyClient.h"
#include "./RPC/RpcClientProxy.h"



class MainView
{
private:

	MyClient* client_;
	RpcClientProxy* rpcProxy_;

public:
	MainView(MyClient* client)
		: client_(client), rpcProxy_(client->rpcProxy_)
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

			//case ClientScreen::GAME:
			//	DrawGame();
			//	break;
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

				rpcProxy_->ReqUserLogin(id, pw);
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

				rpcProxy_->ReqUserRegister(id, pw);
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

				rpcProxy_->ReqPlayerRegister(client_->userId_, name);
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
				rpcProxy_->ReqChat(chat);
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
		ImGui::Text(u8"골드 : %d", client_->myPlayer_.gold_);

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
				ImGui::Text(u8"Character ID : %d", character.characterId_);
			}

			ImGui::EndChild();

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal(u8"상점", &showShop, ImGuiWindowFlags_NoResize))
		{
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text(u8"일반 캐릭터를 구매합니다.");

			ImGui::SameLine();

			if (ImGui::Button(u8"구매하기", ImVec2(140, 45)))
			{
				// rpcProxy_->ReqBuyCharacter(...);
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

			// rpcProxy_->ReqEnterMatchQueue();
		}

		if (ImGui::BeginPopupModal(u8"게임 시작", &showMatching, ImGuiWindowFlags_NoResize))
		{
			ImGui::Spacing();
			ImGui::Spacing();

			static int count = 0;
			count++;
			std::string dots((count/30)%4, '.');

			ImGui::Text(u8"게임을 찾는 중입니다.");
			ImGui::Text(u8"잠시만 기다려주세요%s", dots);

			ImGui::Spacing();

			if (ImGui::Button(u8"매칭 취소", ImVec2(140, 50)))
			{
				showMatching = false;
				//rpcProxy_->ReqCancelMatch();
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
			ImGui::Text(u8"%s  Lv.%d", p.second.playerName_.c_str(), p.second.level_);
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

};
