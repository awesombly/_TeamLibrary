#include "LobbyScene.h"
#include "JParser.h"
#include "JState.h"

bool LobbyScene::Init() noexcept
{
	JState::SetState(DxManager::GetDevice());
	JPanel* pRoot = new JPanel(L"IntroRoot");
	//pRoot->isGlobal();
	JParser Par;
	Par.FileLoad(DxManager::GetDevice(), L"../../data/ui/ui_intro_test.txt", *pRoot);
	ObjectManager::Get().PushObject(pRoot);
	
	static auto pToGuest = [](void* pScene) {
		isHost = false;
		((MainClass*)pScene)->SetScene(ESceneName::Main);
	};
	static auto pToHost = [](void* pScene) {
		isHost = true;
		((MainClass*)pScene)->SetScene(ESceneName::Main);
	};
	// to 게스트
	m_toGuest = pRoot->find_child(L"txt_Guest");
	m_toGuest->EventClick.first = pToGuest;
	m_toGuest->EventClick.second = this;
	// to 호스트
	m_toHost = pRoot->find_child(L"txt_Host");
	m_toHost->EventClick.first = pToHost;
	m_toHost->EventClick.second = this;

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