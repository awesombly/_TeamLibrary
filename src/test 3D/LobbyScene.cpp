#include "LobbyScene.h"
#include "PacketManager.h"

#include "JParser.h"
#include "JState.h"
#include "JEventList.h"

bool LobbyScene::Init() noexcept
{
	static auto pToGuest = [](void* pScene) {
		if (PacketManager::Get().m_inputIP.empty())
		{
			MessageBox(Window::m_hWnd, L"IP 뒷자리를 입력하세염.", L"삐빅-", 0);
			return;
		}
		PacketManager::Get().isHost = false;
		((MainClass*)pScene)->StartupClient();
		((MainClass*)pScene)->SetScene(ESceneName::Main);

		PacketManager::Get().SendPacket((char*)&PI, sizeof(PI), PACKET_ReqAddPlayer);
		PacketManager::Get().SendPacket((char*)&PI, sizeof(PI), PACKET_ReqSync);
	};
	static auto pToHost = [](void* pScene) {
		PacketManager::Get().isHost = true;
		((MainClass*)pScene)->StartupServer();
		((MainClass*)pScene)->SetScene(ESceneName::Main);
	};

	JState::SetState(DxManager::GetDevice());
	JPanel* pUIRoot = new JPanel(L"UI_IntroRoot");
	pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/UI_Intro", *pUIRoot);
	//ObjectManager::Get().PushObject(pUIRoot);

	// Host = D_Host
	// IP = G_IP
	// Enter = G_Enter

	// to 호스트
	//m_toHost = pUIRoot->find_child(L"txt_Host"); // D_Host
	m_toHost = pUIRoot->find_child(L"D_Host"); // D_Host
	m_toHost->EventClick.first = pToHost;
	m_toHost->EventClick.second = this;

	// to 게스트
	//m_toGuest = (JTextCtrl*)pUIRoot->find_child(L"Guest_Enter"); // G_Enter
	m_toGuest = (JTextCtrl*)pUIRoot->find_child(L"G_Enter"); // G_Enter
	m_toGuest->EventClick.first = pToGuest;
	m_toGuest->EventClick.second = this;
	// IP 창
	//m_toGuestIP = (JTextCtrl*)pUIRoot->find_child(L"Guest_IP_9"); // G_IP
	m_toGuestIP = (JTextCtrl*)pUIRoot->find_child(L"G_IP"); // G_IP
	m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().m_inputIP;
	// 패널창
	m_toGuestPanel = m_toGuest->m_pParent;
	
	ObjectManager::Get().PushObject(pUIRoot);

	return true;
}




// 프레임
bool LobbyScene::Frame() noexcept
{
	//// 엔터 체크
	//if (Input::GetKeyState(VK_RETURN) == EKeyState::DOWN)
	//{
	//	if (!PacketManager::Get().m_inputIP.empty())
	//	{
	//		//return;
	//	}
	//}
	if (m_toGuestPanel->m_bRender == true)
	{
		if (Input::GetKeyState(VK_BACK) == EKeyState::DOWN &&
			!PacketManager::Get().m_inputIP.empty())
		{
			PacketManager::Get().m_inputIP.erase(--PacketManager::Get().m_inputIP.end());
			m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().m_inputIP;
		}
		// 채팅 입력
		if (PacketManager::Get().m_inputIP.size() < 3)
		{
			for (char i = '0'; i <= '9'; i++)
			{
				if (Input::GetKeyState(i) == EKeyState::DOWN)
				{
					PacketManager::Get().m_inputIP += (char)i;
					m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().m_inputIP;
				}
			}
			for (char i = VK_NUMPAD0; i <= VK_NUMPAD9; i++)
			{
				if (Input::GetKeyState(i) == EKeyState::DOWN)
				{
					switch (i)
					{
					case VK_NUMPAD0:
						PacketManager::Get().m_inputIP += '0';
						break;
					case VK_NUMPAD1:
						PacketManager::Get().m_inputIP += '1';
						break;
					case VK_NUMPAD2:
						PacketManager::Get().m_inputIP += '2';
						break;
					case VK_NUMPAD3:
						PacketManager::Get().m_inputIP += '3';
						break;
					case VK_NUMPAD4:
						PacketManager::Get().m_inputIP += '4';
						break;
					case VK_NUMPAD5:
						PacketManager::Get().m_inputIP += '5';
						break;
					case VK_NUMPAD6:
						PacketManager::Get().m_inputIP += '6';
						break;
					case VK_NUMPAD7:
						PacketManager::Get().m_inputIP += '7';
						break;
					case VK_NUMPAD8:
						PacketManager::Get().m_inputIP += '8';
						break;
					case VK_NUMPAD9:
						PacketManager::Get().m_inputIP += '9';
						break;
					}
					m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().m_inputIP;
				}
			}
		}
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