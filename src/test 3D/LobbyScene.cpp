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
	// to �Խ�Ʈ
	m_toGuest = pRoot->find_child(L"txt_Guest");
	m_toGuest->EventClick.first = pToGuest;
	m_toGuest->EventClick.second = this;
	// to ȣ��Ʈ
	m_toHost = pRoot->find_child(L"txt_Host");
	m_toHost->EventClick.first = pToHost;
	m_toHost->EventClick.second = this;

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