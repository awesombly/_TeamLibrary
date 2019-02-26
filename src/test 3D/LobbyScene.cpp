#include "LobbyScene.h"
#include "PacketManager.h"
#include "JEventBind.h"
#include "JState.h"

bool LobbyScene::Init() noexcept
{
	FirstInit();
	// UI
	LoadUI();

	ObjectManager::Get().Lights.front()->SetPosition(0.0f, -30.0f, 36.0f);
	ObjectManager::Get().Lights.front()->SetRotation(Quaternion::Left * PI * 0.5f);

	m_pBackGuard = (AHeroObj*)ObjectManager::Get().TakeObject(L"Paladin", false);
	m_pBackGuard->SetPosition(-40.0f, -31.0f, 35.0f);
	m_pBackGuard->SetRotation(Quaternion::Left * PI * 0.8f);
	m_pBackGuard->SetScale(Vector3::One * 0.625f);
	m_pBackGuard->SetGravityScale(0.0f);
	((Renderer*)m_pBackGuard->GetComponent(EComponent::Renderer))->SetLightRate(0.5f);

	m_pBackArcher = (AHeroObj*)ObjectManager::Get().TakeObject(L"Archer", false);
	m_pBackArcher->SetPosition(-40.0f, -31.0f, 35.0f);
	m_pBackArcher->SetRotation(Quaternion::Left * PI * 0.8f);
	m_pBackArcher->SetScale(Vector3::One * 0.5f);
	m_pBackArcher->SetGravityScale(0.0f);
	((Renderer*)m_pBackArcher->GetComponent(EComponent::Renderer))->SetLightRate(0.5f);

	m_pBackMage = (AHeroObj*)ObjectManager::Get().TakeObject(L"Mage", false);
	m_pBackMage->SetPosition(-40.0f, -31.0f, 35.0f);
	m_pBackMage->SetRotation(Quaternion::Left * PI * 0.8f);
	m_pBackMage->SetScale(Vector3::One * 0.5f);
	m_pBackMage->SetGravityScale(0.0f);
	((Renderer*)m_pBackMage->GetComponent(EComponent::Renderer))->SetLightRate(0.5f);

	SelectCharacter(true);
	///
	SoundManager::Get().Stop("bgm_Intro_Theme.mp3");
	SoundManager::Get().Play("bgm_Lobby_Theme.mp3");
	m_isLoading = false;
	return true;
}


// ������
bool LobbyScene::Frame() noexcept
{
	// ������ �Է�
	if (m_toGuestPanel->m_bRender == true)
	{
		if (Input::GetKeyState(VK_BACK) == EKeyState::DOWN &&
			!PacketManager::Get().InputIP.empty())
		{
			// ���� �����
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
	if (Input::GetKeyState(VK_LBUTTON) == EKeyState::DOWN)
	{
		SoundManager::Get().Play("SE_Click01.mp3");
	}

	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();
	// ���� ����
	if (m_isStart)
	{
		static float frameCount = 0.0f;
		frameCount += Timer::SPF;
		//m_pBackHero->Scaling(-m_pBackHero->GetScale() * Timer::SPF);
		m_pBackHero->Translate(Timer::SPF * 0.01f, Timer::SPF * 0.005f, Timer::SPF * 0.05f);
		if (frameCount > 2.0f)
		{
			// ����
			if (PacketManager::Get().isHost)
			{
				m_isStart = false;
				SetScene(ESceneName::Main);
				StartupServer();

				PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			}
			else
			{
				m_isStart = false;
				SetScene(ESceneName::Main);
				StartupClient();
				while (PacketManager::Get().pMyInfo->UserSocket == 0);	// ���� ���������� ���

				PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
			}
			return true;
		}
	}
	m_pBackHero->Frame(Timer::SPF, Timer::AccumulateTime);
	return true;
}

// ����
bool LobbyScene::Render() noexcept
{
	DxManager::Get().Render();
	ObjectManager::Get().Render(DxManager::GetDContext());
	SoundManager::Get().Render();

	DxManager::Get().ClearDepthStencilView();
	m_pBackHero->Render(DxManager::GetDContext());
	return true;
}

// ������
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
		PlayerController::Get().m_selectCharacter = (PlayerController::ECharacter)(PlayerController::Get().m_selectCharacter + 1);
		if (PlayerController::Get().m_selectCharacter > 3)
			PlayerController::Get().m_selectCharacter = PlayerController::ECharacter::EGuard;
	}
	else
	{
		PlayerController::Get().m_selectCharacter = (PlayerController::ECharacter)(PlayerController::Get().m_selectCharacter - 1);
		if (PlayerController::Get().m_selectCharacter <= 0)
			PlayerController::Get().m_selectCharacter = PlayerController::ECharacter::EMage;
	}

	switch (PlayerController::Get().m_selectCharacter)
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
	switch (PlayerController::Get().m_selectCharacter)
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
	PacketManager::Get().isHost = true;
	m_isStart = true;
}

void LobbyScene::StartToGuest()
{
	MainClass::StartToGuest();
	switch (PlayerController::Get().m_selectCharacter)
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
	PacketManager::Get().isHost = false;
	m_isStart = true;
}

void LobbyScene::LoadUI() noexcept
{
	JState::SetState(DxManager::GetDevice());
	JPanel* pUIRoot = new JPanel(L"UI_IntroRoot");
	pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/Lobby", *pUIRoot);

	// ȣ��Ʈ ����
	auto pPanel = pUIRoot->find_child(L"D_Host"); // D_Host
	pPanel->EventClick.first = [](void* pScene) {
		((LobbyScene*)pScene)->StartToHost();
	};
	pPanel->EventClick.second = this;
	// �Խ�Ʈ ����
	pPanel = (JTextCtrl*)pUIRoot->find_child(L"G_Enter"); // G_Enter
	pPanel->EventClick.first = [](void* pScene) {
		if (PacketManager::Get().InputIP.empty())
		{
			MessageBox(Window::m_hWnd, L"IP ���ڸ��� �Է��ϼ���.", L"�ߺ�-", 0);
			return;
		}
		m_strHostIPv4 = "192.168.0."s + WCharToChar(PacketManager::Get().InputIP.c_str());
		((LobbyScene*)pScene)->StartToGuest();
	};
	pPanel->EventClick.second = this;
	// Exit
	pPanel = (JButtonCtrl*)pUIRoot->find_child(L"D_Exit");
	pPanel->EventClick.first = [](void*) {
		Core::isPlaying = false;
	};
	//pPanel->EventClick.second = this;
	// IP â
	m_toGuestIP = (JTextCtrl*)pUIRoot->find_child(L"G_IP"); // G_IP
	m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().InputIP;
	// �г�â
	m_toGuestPanel = m_toGuestIP->m_pParent;
	// ��Ī
	JButtonCtrl* pMatchButton = (JButtonCtrl*)pUIRoot->find_child(L"Matching_Two");
	pMatchButton->EventClick.first  = [](void* pScene) {
		((LobbyScene*)pScene)->m_pIsMatching->m_bRender = true;
		((LobbyScene*)pScene)->RequestMatch(2);
	};
	pMatchButton->EventClick.second = this;
	pMatchButton = (JButtonCtrl*)pUIRoot->find_child(L"Matching_Three");
	pMatchButton->EventClick.first = [](void* pScene) {
		((LobbyScene*)pScene)->m_pIsMatching->m_bRender = true;
		((LobbyScene*)pScene)->RequestMatch(3);
	};
	pMatchButton->EventClick.second = this;
	pMatchButton = (JButtonCtrl*)pUIRoot->find_child(L"Matching_Four");
	pMatchButton->EventClick.first = [](void* pScene) {
		((LobbyScene*)pScene)->m_pIsMatching->m_bRender = true;
		((LobbyScene*)pScene)->RequestMatch(4);
	};
	pMatchButton->EventClick.second = this;
	// ���� ����
	m_pStartEffect = (JPanel*)pUIRoot->find_child(L"effect_hos"); //1234

	// ĳ���� ����
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

	// ��Ī ���
	m_pIsMatching = (JPanel*)pUIRoot->find_child(L"isMatching_Panel");

	ObjectManager::Get().PushObject(pUIRoot);
	UI::LobbyEvent(pUIRoot);	
	UI::Loading(pUIRoot);
}