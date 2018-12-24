#include "LobbyScene.h"
#include "JParser.h"


bool LobbyScene::Init() noexcept
{
	//JPanel* pRoot = new JPanel(L"IntroRoot");
	//JParser Par;
	//Par.FileLoad(DxManager::GetDevice(), L"../../data/ui/ui_intro_test.txt", *pRoot);
	//ObjectManager::Get().PushObject(pRoot);
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