#include "LobbyScene.h"
#include "PacketManager.h"
#include "JEventBind.h"
#include "JState.h"


bool LobbyScene::Init() noexcept
{
	FirstInit();
	// UI
	LoadUI();

	m_pBackHero = (AHeroObj*)ObjectManager::Get().TakeObject(L"Guard", false);
	m_pBackHero->SetPosition(-40.0f, -28.0f, 30.0f);
	m_pBackHero->SetRotation(Quaternion::Left * PI * 0.8f);
	m_pBackHero->SetGravityScale(0.0f);
	///
	SoundManager::Get().SetBGM("bgm_Title01.mp3");
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

	if (m_isStart)
	{
		static float frameCount = 0.0f;
		frameCount += Timer::SPF;
		if (frameCount > 2.3f)
		{
			// 시작
			if (PacketManager::Get().isHost)
			{
				m_isStart = false;
				StartupServer();
				SetScene(ESceneName::Main);
			}
			else
			{
				m_isStart = false;
				StartupClient();
				SetScene(ESceneName::Main);

				PacketManager::Get().SendPacket('\0', 0, PACKET_ReqAddPlayer);
				PacketManager::Get().SendPacket('\0', 0, PACKET_ReqSyncSpawns);
			}
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
	ObjectManager::Get().RemoveObject(m_pBackHero);
	ObjectManager::Get().Release();
	return true;
}


bool LobbyScene::FirstInit() noexcept
{
	if (m_isFirstInit)
	{
		return true;
	}
	return false;
}


void LobbyScene::LoadUI() noexcept
{
	static auto pToGuest = [](void* pScene) {
		if (PacketManager::Get().InputIP.empty())
		{
			MessageBox(Window::m_hWnd, L"IP 뒷자리를 입력하세염.", L"삐빅-", 0);
			return;
		}
		((LobbyScene*)pScene)->m_pBackHero->SetANIM_OneTime(Guard_DASHJUMP);
		((LobbyScene*)pScene)->m_pStartEffect->EffectPlay();
		SoundManager::Get().Play("SV_Guard_Shout.mp3");
		PacketManager::Get().isHost = false;
		((LobbyScene*)pScene)->m_isStart = true;
		/*((LobbyScene*)pScene)->StartupClient();
		((LobbyScene*)pScene)->SetScene(ESceneName::Main);

		PacketManager::Get().SendPacket('\0', 0, PACKET_ReqAddPlayer);
		PacketManager::Get().SendPacket('\0', 0, PACKET_ReqSyncSpawns);*/
	};
	static auto pToHost = [](void* pScene) {
		((LobbyScene*)pScene)->m_pBackHero->SetANIM_OneTime(Guard_DASHJUMP);
		((LobbyScene*)pScene)->m_pStartEffect->EffectPlay();
		SoundManager::Get().Play("SV_Guard_Shout.mp3");
		PacketManager::Get().isHost = true;
		((LobbyScene*)pScene)->m_isStart = true;
		/*((LobbyScene*)pScene)->StartupServer();
		((LobbyScene*)pScene)->SetScene(ESceneName::Main);*/
	};
	static auto pToExit = [](void* pScene) {
		exit(0); pScene;
	};

	JState::SetState(DxManager::GetDevice());
	JPanel* pUIRoot = new JPanel(L"UI_IntroRoot");
	pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/UI_Intro", *pUIRoot);

	// 호스트 입장
	auto pPanel = pUIRoot->find_child(L"D_Host"); // D_Host
	pPanel->EventClick.first = pToHost;
	pPanel->EventClick.second = this;
	// 게스트 입장
	pPanel = (JTextCtrl*)pUIRoot->find_child(L"G_Enter"); // G_Enter
	pPanel->EventClick.first = pToGuest;
	pPanel->EventClick.second = this;
	// Exit
	pPanel = (JTextCtrl*)pUIRoot->find_child(L"D_Exit");
	pPanel->EventClick.first = pToExit;
	pPanel->EventClick.second = this;
	// IP 창
	m_toGuestIP = (JTextCtrl*)pUIRoot->find_child(L"G_IP"); // G_IP
	m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().InputIP;
	// 패널창
	m_toGuestPanel = m_toGuestIP->m_pParent;
	// 매칭, 시작 이펙
	JTextCtrl* AutoMatching = (JTextCtrl*)pUIRoot->find_child(L"D_AutoMatching");
	m_pStartEffect = (JPanel*)pUIRoot->find_child(L"effect4"); //1234
	
	ObjectManager::Get().PushObject(pUIRoot);
	UI::IntroEvent(pUIRoot);	
}