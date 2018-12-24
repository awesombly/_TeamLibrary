#include "GameScene.h"


////////////////////////////////////////////////////////
// 메쉬가 GameObject로 되어있고, Release시 KeyObject에서 삭제되지 않음,, 
// 위가 원인인진 모르나, 씬 전환시 기사 재생성때 에러가 뜸

// ++ 인트로 화면 호스트, 게스트 전환
// 게스트시 호스트 오브젝트 정보 로딩
// 충돌 물리 처리

bool GameScene::Init() noexcept
{
#pragma region Basic
	m_pPlayer = new PlayerController(L"Player", EObjType::Object);
	ObjectManager::Cameras[ECamera::Main]->SetParent(m_pPlayer);
	
	// 기사
	m_pHero = new AHeroObj();
	m_pHero->SetPlayerCharacter(Guard, 0.0f, 0.0f, 0.0f);
	m_pHero->SetANIM(Guard_IDLE);
	m_pHero->m_myName = L"Hero";
	m_pHero->m_objType = EObjType::Object;
	auto pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });//m_pHero.m_CollsionBox.vMin, m_pHero.m_CollsionBox.vMax));
	//pCollider->m_pivot = Vector3::Up * 40.0f;
	m_pHero->AddComponent(pCollider);
	m_pPlayer->isCharacter(true);
	m_pPlayer->SetParent(m_pHero);
	m_pPlayer->ResetOption();
	m_pPlayer->m_curCharacter = PlayerController::ECharacter::EGuard;
	ObjectManager::Get().PushObject(m_pHero);

	// 좀비
	m_pZombi = new AHeroObj();
	m_pZombi->SetPlayerCharacter(Zombie, 80.0f, 200.0f, -100.0f);
	m_pZombi->SetANIM(Zombie_IDLE);
	m_pZombi->m_myName = L"Zombi";
	m_pZombi->m_objType = EObjType::Object;
	//m_pZombi->SetScale(Vector3::One * 1.5f);
	pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	//pCollider->m_pivot *= 1.5f;
	m_pZombi->AddComponent(pCollider);
	ObjectManager::Get().PushObject(m_pZombi);

	// 새 생성
	m_pBird = new AHeroObj();
	m_pBird->SetPlayerCharacter(NPC_Bird, 0.0f, 80.0f, -100.0f);
	m_pBird->SetANIM(NPC_Bird_IDLE);
	m_pBird->m_myName = L"Bird";
	m_pBird->m_objType = EObjType::Object;
	m_pBird->SetScale(Vector3::One * 15.0f);
	pCollider = new ColliderOBB({ -1.0f, 0.0f , -1.0f }, { 1.0f, 2.0f , 1.0f });
	pCollider->m_pivot *= 15.0f;
	pCollider->useGravity(false);
	m_pBird->AddComponent(pCollider);
	ObjectManager::Get().PushObject(m_pBird);

	ObjectManager::Get().TakeObject(L"HeightMap");
	ObjectManager::Get().TakeObject(L"ParticleSystem");
	ObjectManager::Get().TakeObject(L"Object1");
	ObjectManager::Get().TakeObject(L"Object2");
	ObjectManager::Get().TakeObject(L"Object3");
#pragma endregion
	return true;
}


// 프레임
bool GameScene::Frame() noexcept
{
	if (Input::GetKeyState(VK_TAB) == EKeyState::DOWN)
	{
		if (m_pPlayer->GetParent() == m_pHero)
		{
			m_pPlayer->SetParent(m_pZombi);
			m_pPlayer->m_curCharacter = PlayerController::ECharacter::EZombie;
		}
		else if (m_pPlayer->GetParent() == m_pZombi)
		{
			m_pPlayer->SetParent(m_pHero);
			m_pPlayer->m_curCharacter = PlayerController::ECharacter::EGuard;
		}
		//else if (m_pPlayer->GetParent() == &m_pBird)
		//{
		//	m_pPlayer->SetParent(&m_pHero);
		//	m_pPlayer->isCharacter(false);
		//}
		m_pPlayer->ResetOption();
	}

	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();
	return true;
}

// 랜더
bool GameScene::Render() noexcept
{
	if (m_pBird != nullptr)
	{
		m_pHero->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);//	(월드,뷰,투영)
		m_pZombi->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);//	(월드,뷰,투영)
		m_pBird->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);//	(월드,뷰,투영)
	}

	DxManager::Get().Render();
	ObjectManager::Get().Render(DxManager::Get().GetDContext());
	SoundManager::Get().Render();
	return true;
}

// 릴리즈
bool GameScene::Release() noexcept
{
	I_CHARMGR.Release();
	I_OBJMGR.Release();
	m_pHero = m_pZombi = m_pBird = nullptr;
	ObjectManager::Cameras[ECamera::Main]->CutParent();
	ObjectManager::Get().PopObject(ObjectManager::Cameras[ECamera::Main]);
	ObjectManager::Get().Release();
	return true;
}