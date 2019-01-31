#include "LobbyScene.h"
#include "PacketManager.h"
#include "JEventBind.h"
#include "JState.h"

bool LobbyScene::Init() noexcept
{
	FirstInit();
	// UI
	LoadUI();

	m_pBackGuard = (AHeroObj*)ObjectManager::Get().TakeObject(L"Paladin", false);
	m_pBackGuard->SetPosition(-40.0f, -31.0f, 35.0f);
	m_pBackGuard->SetRotation(Quaternion::Left * PI * 0.8f);
	m_pBackGuard->SetScale(Vector3::One * 0.625f);
	m_pBackGuard->SetGravityScale(0.0f);

	m_pBackArcher = (AHeroObj*)ObjectManager::Get().TakeObject(L"Archer", false);
	m_pBackArcher->SetPosition(-40.0f, -31.0f, 35.0f);
	m_pBackArcher->SetRotation(Quaternion::Left * PI * 0.8f);
	m_pBackArcher->SetScale(Vector3::One * 0.5f);
	m_pBackArcher->SetGravityScale(0.0f);

	m_pBackMage = (AHeroObj*)ObjectManager::Get().TakeObject(L"Mage", false);
	m_pBackMage->SetPosition(-40.0f, -31.0f, 35.0f);
	m_pBackMage->SetRotation(Quaternion::Left * PI * 0.8f);
	m_pBackMage->SetScale(Vector3::One * 0.5f);
	m_pBackMage->SetGravityScale(0.0f);

	SelectCharacter(true);
	///
	SoundManager::Get().SetBGM("bgm_Lobby_Theme.mp3");
	m_isLoading = false;
	return true;
}


// 프레임
bool LobbyScene::Frame() noexcept
{
	// 채팅
	if (m_toGuestPanel->m_bRender == true)
	{
		if (Input::GetKeyState(VK_BACK) == EKeyState::DOWN &&
			!PacketManager::Get().InputIP.empty())
		{
			// 글자 지우기
			PacketManager::Get().InputIP.erase(--PacketManager::Get().InputIP.end());
			m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().InputIP;
		}
		if (PacketManager::Get().InputIP.size() < 3)
		{
			for (char i = '0'; i <= '9'; i++)
			{
				if (Input::GetKeyState(i) == EKeyState::DOWN)
				{
					PacketManager::Get().InputIP += (char)i;
					m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().InputIP;
				}
			}
			for (char i = VK_NUMPAD0; i <= VK_NUMPAD9; i++)
			{
				if (Input::GetKeyState(i) == EKeyState::DOWN)
				{
					switch (i)
					{
					case VK_NUMPAD0:
						PacketManager::Get().InputIP += '0';
						break;
					case VK_NUMPAD1:
						PacketManager::Get().InputIP += '1';
						break;
					case VK_NUMPAD2:
						PacketManager::Get().InputIP += '2';
						break;
					case VK_NUMPAD3:
						PacketManager::Get().InputIP += '3';
						break;
					case VK_NUMPAD4:
						PacketManager::Get().InputIP += '4';
						break;
					case VK_NUMPAD5:
						PacketManager::Get().InputIP += '5';
						break;
					case VK_NUMPAD6:
						PacketManager::Get().InputIP += '6';
						break;
					case VK_NUMPAD7:
						PacketManager::Get().InputIP += '7';
						break;
					case VK_NUMPAD8:
						PacketManager::Get().InputIP += '8';
						break;
					case VK_NUMPAD9:
						PacketManager::Get().InputIP += '9';
						break;
					}
					m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().InputIP;
				}
			}
		}
	}
	// Click
	if (Input::GetKeyState(EMouseButton::Left) == EKeyState::DOWN)
	{
		SoundManager::Get().Play("SE_Click01.mp3");
	}

	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();
	m_pBackHero->Frame(Timer::SPF, Timer::AccumulateTime);
	// 게임 시작
	if (m_isStart)
	{
		static float frameCount = 0.0f;
		frameCount += Timer::SPF;
		m_pBackHero->Scaling(-Vector3::One * 0.22f * Timer::SPF);
		m_pBackHero->Translate(Vector3::One * 2.1f * Timer::SPF);
		if (frameCount > 2.0f)
		{
			// 시작
			if (PacketManager::Get().isHost)
			{
				m_isStart = false;
				StartupServer();
				SetScene(ESceneName::Main);

				PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				PlayerController::Get().SendReqRespawn(m_selectCharacter);
			}
			else
			{
				m_isStart = false;
				StartupClient();
				SetScene(ESceneName::Main);
				while (PacketManager::Get().pMyInfo->UserSocket == 0);	// 소켓 받을때까지 대기

				PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				PlayerController::Get().SendReqRespawn(m_selectCharacter);
			}
			return true;
		}
	}
	return true;
}

// 랜더
bool LobbyScene::Render() noexcept
{
	DxManager::Get().Render();
	ObjectManager::Get().Render(DxManager::GetDContext());
	SoundManager::Get().Render();

	DxManager::Get().ClearDepthStencilView();
	m_pBackHero->Render(DxManager::GetDContext());
	return true;
}

// 릴리즈
bool LobbyScene::Release() noexcept
{
	ObjectManager::Get().RemoveObject(m_pBackGuard);
	ObjectManager::Get().RemoveObject(m_pBackArcher);
	ObjectManager::Get().RemoveObject(m_pBackMage);
	ObjectManager::Get().Release();
	return true;
}

void LobbyScene::SelectCharacter(const bool& toRight) noexcept
{
	if (toRight)
	{
		m_selectCharacter = (PlayerController::ECharacter)(m_selectCharacter + 1);
		if (m_selectCharacter > 3)
			m_selectCharacter = PlayerController::ECharacter::EGuard;
	}
	else
	{
		m_selectCharacter = (PlayerController::ECharacter)(m_selectCharacter - 1);
		if (m_selectCharacter <= 0)
			m_selectCharacter = PlayerController::ECharacter::EMage;
	}

	switch (m_selectCharacter)
	{
	case PlayerController::EGuard:
		m_pBackHero = m_pBackGuard;
		break;
	case PlayerController::EArcher:
		m_pBackHero = m_pBackArcher;
		break;
	case PlayerController::EMage:
		m_pBackHero = m_pBackMage;
		break;
	default:
		break;
	}
}


bool LobbyScene::FirstInit() noexcept
{
	if (m_isFirstInit)
	{
		return true;
	}
	return false;
}


void LobbyScene::StartToHost()
{
	MainClass::StartToHost();
	switch (m_selectCharacter)
	{
	case PlayerController::ECharacter::EGuard:
	{
		m_pBackHero->SetANIM_Loop(Paladin_RUN);
	}	break;
	case PlayerController::ECharacter::EArcher:
	{
		m_pBackHero->SetANIM_Loop(Archer_DIVE);
	}	break;
	case PlayerController::ECharacter::EMage:
	{
		m_pBackHero->SetANIM_Loop(Mage_ATK_UD);
	}	break;
	}
	m_pStartEffect->EffectPlay();
	//SoundManager::Get().Play("SV_gogogo.mp3");
	PacketManager::Get().isHost = true;
	m_isStart = true;
}

void LobbyScene::StartToGuest()
{
	MainClass::StartToGuest();
	switch (m_selectCharacter)
	{
	case PlayerController::ECharacter::EGuard:
	{
		m_pBackHero->SetANIM_Loop(Paladin_RUN);
	}	break;
	case PlayerController::ECharacter::EArcher:
	{
		m_pBackHero->SetANIM_Loop(Archer_DIVE);
	}	break;
	case PlayerController::ECharacter::EMage:
	{
		m_pBackHero->SetANIM_Loop(Mage_ATK_UD);
	}	break;
	}

	m_pStartEffect->EffectPlay();
	SoundManager::Get().Play("SV_gogogo.mp3");
	PacketManager::Get().isHost = false;
	m_isStart = true;
}

void LobbyScene::LoadUI() noexcept
{
	static auto pToGuest = [](void* pScene) {
		if (PacketManager::Get().InputIP.empty())
		{
			MessageBox(Window::m_hWnd, L"IP 뒷자리를 입력하세염.", L"삐빅-", 0);
			return;
		}
		m_strHostIPv4 = "192.168.0."s + WCharToChar(PacketManager::Get().InputIP.c_str());
		((LobbyScene*)pScene)->StartToGuest();
	};
	static auto pToHost = [](void* pScene) {
		((LobbyScene*)pScene)->StartToHost();
	};
	static auto pToExit = [](void* pScene) {
		//exit(0); 
		Core::isPlaying = false; pScene;
	};

	JState::SetState(DxManager::GetDevice());
	JPanel* pUIRoot = new JPanel(L"UI_IntroRoot");
	pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/Lobby", *pUIRoot);

	// 호스트 입장
	auto pPanel = pUIRoot->find_child(L"D_Host"); // D_Host
	pPanel->EventClick.first = pToHost;
	pPanel->EventClick.second = this;
	// 게스트 입장
	pPanel = (JTextCtrl*)pUIRoot->find_child(L"G_Enter"); // G_Enter
	pPanel->EventClick.first = pToGuest;
	pPanel->EventClick.second = this;
	// Exit
	pPanel = (JButtonCtrl*)pUIRoot->find_child(L"D_Exit");
	pPanel->EventClick.first = pToExit;
	pPanel->EventClick.second = this;
	// IP 창
	m_toGuestIP = (JTextCtrl*)pUIRoot->find_child(L"G_IP"); // G_IP
	m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().InputIP;
	// 패널창
	m_toGuestPanel = m_toGuestIP->m_pParent;
	// 매칭
	auto pMatching2 = [](void* pScene) {
		((LobbyScene*)pScene)->m_pIsMatching->m_bRender = true;
		((LobbyScene*)pScene)->RequestMatch(2);
	};
	auto pMatching3 = [](void* pScene) {
		((LobbyScene*)pScene)->m_pIsMatching->m_bRender = true;
		((LobbyScene*)pScene)->RequestMatch(3);
	};
	auto pMatching4 = [](void* pScene) {
		((LobbyScene*)pScene)->m_pIsMatching->m_bRender = true;
		((LobbyScene*)pScene)->RequestMatch(4);
	};
	JButtonCtrl* pMatchButton = (JButtonCtrl*)pUIRoot->find_child(L"Matching_Two");
	pMatchButton->EventClick.first  = pMatching2;
	pMatchButton->EventClick.second = this;
	pMatchButton = (JButtonCtrl*)pUIRoot->find_child(L"Matching_Three");
	pMatchButton->EventClick.first = pMatching3;
	pMatchButton->EventClick.second = this;
	pMatchButton = (JButtonCtrl*)pUIRoot->find_child(L"Matching_Four");
	pMatchButton->EventClick.first = pMatching4;
	pMatchButton->EventClick.second = this;
	// 시작 이펙
	m_pStartEffect = (JPanel*)pUIRoot->find_child(L"effect_hos"); //1234

	// 캐릭터 선택
	JButtonCtrl* pCharSelect = (JButtonCtrl*)pUIRoot->find_child(L"Char_Left");
	pCharSelect->EventClick.first = [](void* pScene) {
		auto pLobby = (LobbyScene*)pScene;
		pLobby->SelectCharacter(false);
	};
	pCharSelect->EventClick.second = this;
	pCharSelect = (JButtonCtrl*)pUIRoot->find_child(L"Char_Right");
	pCharSelect->EventClick.first = [](void* pScene) {
		auto pLobby = (LobbyScene*)pScene;
		pLobby->SelectCharacter(true);
	};
	pCharSelect->EventClick.second = this;

	// 매칭 대기
	m_pIsMatching = (JPanel*)pUIRoot->find_child(L"isMatching_Panel");

	ObjectManager::Get().PushObject(pUIRoot);
	UI::LobbyEvent(pUIRoot);	
}