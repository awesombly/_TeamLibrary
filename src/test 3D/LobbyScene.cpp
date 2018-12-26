#include "LobbyScene.h"
#include "PacketManager.h"
#include "JState.h"
bool LobbyScene::Init() noexcept
{
	static auto pToGuest = [](void* pScene) {
		if (PacketManager::Get().InputIP.empty())
		{
			MessageBox(Window::m_hWnd, L"IP ���ڸ��� �Է��ϼ���.", L"�ߺ�-", 0);
			return;
		}
		PacketManager::Get().isHost = false;
		((MainClass*)pScene)->StartupClient();
		((MainClass*)pScene)->SetScene(ESceneName::Main);

		PacketManager::Get().SendPacket((char*)&PI, sizeof(PI), PACKET_ReqSyncSpawns);
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

	// to ȣ��Ʈ
	m_toHost = pUIRoot->find_child(L"D_Host"); // D_Host
	m_toHost->EventClick.first = pToHost;
	m_toHost->EventClick.second = this;
	// to �Խ�Ʈ
	m_toGuest = (JTextCtrl*)pUIRoot->find_child(L"G_Enter"); // G_Enter
	m_toGuest->EventClick.first = pToGuest;
	m_toGuest->EventClick.second = this;
	// IP â
	m_toGuestIP = (JTextCtrl*)pUIRoot->find_child(L"G_IP"); // G_IP
	m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().InputIP;
	// �г�â
	m_toGuestPanel = m_toGuest->m_pParent;
	
	ObjectManager::Get().PushObject(pUIRoot);
	return true;
}




// ������
bool LobbyScene::Frame() noexcept
{
	//// ���� üũ
	//if (Input::GetKeyState(VK_RETURN) == EKeyState::DOWN)
	//{
	//	if (!PacketManager::Get().InputIP.empty())
	//	{
	//		//return;
	//	}
	//}
	if (m_toGuestPanel->m_bRender == true)
	{
		if (Input::GetKeyState(VK_BACK) == EKeyState::DOWN &&
			!PacketManager::Get().InputIP.empty())
		{
			// ���� �����
			PacketManager::Get().InputIP.erase(--PacketManager::Get().InputIP.end());
			m_toGuestIP->m_Text = L"IP : ~." + PacketManager::Get().InputIP;
		}
		// ä�� �Է�
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

	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();
	return true;
}

// ����
bool LobbyScene::Render() noexcept
{
	DxManager::Get().Render();
	ObjectManager::Get().Render(DxManager::GetDContext());
	SoundManager::Get().Render();
	return true;
}

// ������
bool LobbyScene::Release() noexcept
{
	ObjectManager::Get().Release();
	return true;
}