#include "LobbyScene.h"
#include "PacketManager.h"
#include "JState.h"


bool LobbyScene::Init() noexcept
{
	FirstInit();
	// UI
	LoadUI();
	///
	SoundManager::Get().SetBGM("bgm_Title01.mp3");
	m_isLoading = false;
	return true;
}


// 프레임
bool LobbyScene::Frame() noexcept
{
	if (m_toGuestPanel->m_bRender == true)
	{
		if (Input::GetKeyState(VK_BACK) == EKeyState::DOWN &&
			!PacketManager::Get().InputIP.empty())
		{
			// 글자 지우기
			PacketManager::Get().InputIP.erase(--PacketManager::Get().InputIP.end());
			m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().InputIP;
		}
		// 채팅 입력
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


	if (Input::GetKeyState(EMouseButton::Left) == EKeyState::DOWN)
	{
		SoundManager::Get().Play("SE_Click01.mp3");
	}

	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();
	return true;
}

// 랜더
bool LobbyScene::Render() noexcept
{
	DxManager::Get().Render();
	ObjectManager::Get().Render(DxManager::GetDContext());
	SoundManager::Get().Render();
	return true;
}

// 릴리즈
bool LobbyScene::Release() noexcept
{
	ObjectManager::Get().Release();
	return true;
}



bool LobbyScene::FirstInit() noexcept
{
	if (m_isFirstInit)
	{
		m_isFirstInit = false;
		// =============================== 맵 생성 =================================
		m_Importer.Import();
		m_pMap = new XMap();
		//m_pMap = (XMap*)new GameObject(L"");
		m_pMap->Create(DxManager::Get().GetDevice(), DxManager::Get().GetDContext(), &m_Importer, _T("../../Data/Map/Shader/MapShader_Specular.hlsl"), _T("../../Data/Map/Shader/MapShader_Color_Specular.hlsl"), "VS", "PS");
		m_pMapTree = new XQuadTreeIndex();
		m_pMapTree->Build(m_pMap);
		m_pMap->m_objType = EObjType::Map;
		m_pMap->isGlobal();
		m_pMap->isStatic();
		//ObjectManager::Get().PushObject(m_pMap);
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
		PacketManager::Get().isHost = false;
		((MainClass*)pScene)->StartupClient();
		((MainClass*)pScene)->SetScene(ESceneName::Main);

		PacketManager::Get().SendPacket('\0', 0, PACKET_ReqAddPlayer);
		PacketManager::Get().SendPacket('\0', 0, PACKET_ReqSyncSpawns);
	};
	static auto pToHost = [](void* pScene) {
		PacketManager::Get().isHost = true;
		((MainClass*)pScene)->StartupServer();
		((MainClass*)pScene)->SetScene(ESceneName::Main);
	};
	static auto pToExit = [](void* pScene) {
		exit(0); pScene;
	};

	JState::SetState(DxManager::GetDevice());
	JPanel* pUIRoot = new JPanel(L"UI_IntroRoot");
	pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/UI_Intro", *pUIRoot);

	// to 호스트
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

	ObjectManager::Get().PushObject(pUIRoot);
}