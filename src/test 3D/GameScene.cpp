#include "GameScene.h"



bool GameScene::Init() noexcept
{

	
	return true;
}


// 프레임
bool GameScene::Frame() noexcept
{
	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();
	return true;
}

// 랜더
bool GameScene::Render() noexcept
{
	DxManager::Get().Render();
	ObjectManager::Get().Render(DxManager::Get().GetDContext());
	SoundManager::Get().Render();
	return true;
}

// 릴리즈
bool GameScene::Release() noexcept
{
	ObjectManager::Get().Release();
	return true;
}