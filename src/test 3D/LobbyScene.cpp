#include "LobbyScene.h"
#include "JParser.h"
#include "JState.h"
#include "JEventList.h"
#include "PacketManager.h"

bool LobbyScene::Init() noexcept
{
	static auto pToGuest = [](void* pScene) {
		PacketManager::Get().isHost = false;
		((MainClass*)pScene)->StartupClient();
		((MainClass*)pScene)->SetScene(ESceneName::Main);
		PacketManager::Get().SendPacket((char*)&PI, sizeof(PI), PACKET_RequestSync);
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
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/test_1.txt", *pUIRoot);
	pUIRoot->find_child(L"panel1")->m_bRender = false;
	pUIRoot->find_child(L"txt_Guest")->EventClick.first = UI::E_INTRO_SHOW_GUEST;
	pUIRoot->find_child(L"txt_Guest")->EventClick.second = pUIRoot->find_child(L"panel1");

	pUIRoot->find_child(L"Guest_Exit")->EventClick.first = UI::E_INTRO_NOTSHOW_GUEST;
	pUIRoot->find_child(L"Guest_Exit")->EventClick.second = pUIRoot->find_child(L"panel1");

	// to ȣ��Ʈ
	m_toHost = pUIRoot->find_child(L"txt_Host");
	m_toHost->EventClick.first = pToHost;
	m_toHost->EventClick.second = this;
	// to �Խ�Ʈ
	m_toGuest = pUIRoot->find_child(L"Guest_IP_9");
	m_toGuest->EventClick.first = pToGuest;
	m_toGuest->EventClick.second = this;

	ObjectManager::Get().PushObject(pUIRoot);
	return true;
}


// ������
bool LobbyScene::Frame() noexcept
{
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