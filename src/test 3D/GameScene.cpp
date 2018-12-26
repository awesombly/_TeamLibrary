#include "GameScene.h"



bool GameScene::Init() noexcept
{
#pragma region Basic
	m_pPlayer = new PlayerController(L"Player", EObjType::Object);
	ObjectManager::Cameras[ECamera::Main]->SetParent(m_pPlayer);
	
	// 기사 
	m_pHero = new AHeroObj();
	m_pHero->SetPlayerCharacter(Guard, 0.0f, 0.0f, 500.0f);
	m_pHero->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_pHero->SetANIM(Guard_IDLE);
	m_pHero->m_myName = L"Hero";
	m_pHero->m_objType = EObjType::Object;
	auto pCollider = new ColliderOBB(60.0f, { -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	//pCollider->usePhysics(false);
	m_pHero->AddComponent(pCollider);
	m_pPlayer->SetParent(m_pHero);
	m_pPlayer->ResetOption();
	m_pPlayer->m_curCharacter = PlayerController::ECharacter::EGuard;
	ObjectManager::Get().PushObject(m_pHero);

	// 좀비
	m_pZombi = new AHeroObj();
	m_pZombi->SetPlayerCharacter(Zombie, 80.0f, 200.0f, -300.0f);
	m_pZombi->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_pZombi->SetANIM(Zombie_IDLE);
	m_pZombi->m_myName = L"Zombi";
	m_pZombi->m_objType = EObjType::Object;
	pCollider = new ColliderOBB(60.0f, { -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	//pCollider->usePhysics(false);
	m_pZombi->AddComponent(pCollider);
	ObjectManager::Get().PushObject(m_pZombi);

	// 새 생성
	m_pBird = new AHeroObj();
	m_pBird->SetPlayerCharacter(NPC_Bird, 0.0f, 80.0f, 300.0f);
	m_pBird->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_pBird->m_myName = L"Bird";
	m_pBird->m_objType = EObjType::Object;
	m_pBird->SetScale(Vector3::One * 15.0f);
	pCollider = new ColliderOBB(1.5f, { -1.0f, 0.0f , -1.0f }, { 1.0f, 2.0f , 1.0f });
	pCollider->m_pivot *= 15.0f;
	pCollider->useGravity(false);
	m_pBird->AddComponent(pCollider);
	ObjectManager::Get().PushObject(m_pBird);

	// 닭 생성
	m_pChicken = new AHeroObj();
	m_pChicken->SetPlayerCharacter(NPC_Chicken, 0.0f, 300.0f, -400.0f);
	m_pChicken->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_pChicken->m_myName = L"Chicken";
	m_pChicken->m_objType = EObjType::Object;
	//m_pChicken->SetScale(Vector3::One * 15.0f);
	pCollider = new ColliderOBB(23.0f, { -15.0f, 0.0f , -15.0f }, { 15.0f, 30.0f , 15.0f });
	//pCollider->m_pivot *= 15.0f;
	pCollider->useGravity(true);
	m_pChicken->AddComponent(pCollider);
	ObjectManager::Get().PushObject(m_pChicken);


	ObjectManager::Get().TakeObject(L"HeightMap");
	ObjectManager::Get().TakeObject(L"ParticleSystem");
	ObjectManager::Get().TakeObject(L"Object1");
	ObjectManager::Get().TakeObject(L"Object2");
	ObjectManager::Get().TakeObject(L"Object3");
#pragma endregion
	// ======================== 맵 생성 =========================================
	if (m_isFirstInit)
	{
		m_isFirstInit = false;
		m_Importer.Import();
		m_pMap = new XMap;
		m_pMap->Create(DxManager::Get().GetDevice(), DxManager::Get().GetDContext(), &m_Importer, _T("../../Data/Map/Shader/MapShader_Specular.hlsl"), _T("../../Data/Map/Shader/MapShader_Color_Specular.hlsl"), "VS", "PS");
		m_pMapTree = new XQuadTreeIndex;
		m_pMapTree->Build(m_pMap);
		m_pMap->m_objType = EObjType::Map;
		m_pMap->isGlobal();
		ObjectManager::Get().PushObject(m_pMap);
	}
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
			m_pPlayer->SetParent(m_pBird);
			m_pPlayer->m_curCharacter = PlayerController::ECharacter::EDummy;
		}
		else if (m_pPlayer->GetParent() == m_pBird)
		{
			m_pPlayer->SetParent(m_pChicken);
			m_pPlayer->m_curCharacter = PlayerController::ECharacter::EDummy;
		}
		else if (m_pPlayer->GetParent() == m_pChicken)
		{
			m_pPlayer->SetParent(m_pHero);
			m_pPlayer->m_curCharacter = PlayerController::ECharacter::EGuard;
		}
		m_pPlayer->ResetOption();
	}

	m_pMapTree->Frame();
	I_Object.Frame(Timer::SPF, Timer::AccumulateTime);
	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();
	return true;
}

// 랜더
bool GameScene::Render() noexcept
{
	m_pMap->SetMatrix(NULL, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	I_Object.SetMatrix(&ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	I_Object.Render(DxManager::Get().GetDContext());

	DxManager::Get().Render();
	ObjectManager::Get().Render(DxManager::Get().GetDContext());
	SoundManager::Get().Render();
	return true;
}

// 릴리즈
bool GameScene::Release() noexcept
{
	if (m_pMapTree) m_pMapTree->Release();

	m_pHero = m_pZombi = m_pBird = m_pChicken = nullptr;
	ObjectManager::Cameras[ECamera::Main]->CutParent();
	ObjectManager::Get().PopObject(ObjectManager::Cameras[ECamera::Main]);
	ObjectManager::Get().Release();
	return true;
}