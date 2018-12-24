#include "LobbyScene.h"
#include "JParser.h"
#include "JState.h"
#include "JEventList.h"

bool LobbyScene::Init() noexcept
{
	static auto pToGuest = [](void* pScene) {
		isHost = false;
		((MainClass*)pScene)->SetScene(ESceneName::Main);
	};
	static auto pToHost = [](void* pScene) {
		isHost = true;
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

	// to 호스트
	m_toHost = pUIRoot->find_child(L"txt_Host");
	m_toHost->EventClick.first = pToHost;
	m_toHost->EventClick.second = this;
	// to 게스트
	m_toGuest = pUIRoot->find_child(L"Guest_IP_9");
	m_toGuest->EventClick.first = pToGuest;
	m_toGuest->EventClick.second = this;

	ObjectManager::Get().PushObject(pUIRoot);
	return true;
}


// 프레임
bool LobbyScene::Frame() noexcept
{
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