#include "GameScene.h"
#include "JParser.h"
#include "JProgressBar.h"


bool GameScene::Init() noexcept
{
#pragma region Basic
	m_pPlayer->m_myName  = L"Player";// = new PlayerController(L"Player", EObjType::Object);
	m_pPlayer->m_objType = EObjType::Object;
	ObjectManager::Cameras[ECamera::Main]->SetParent(m_pPlayer);
	
	// 기사 
	m_pHero = new AHeroObj();
	m_pHero->SetPlayerCharacter(Guard, 0.0f, 0.0f, 500.0f);
	m_pHero->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_pHero->SetANIM(Guard_IDLE);
	m_pHero->m_myName = L"Guard";
	m_pHero->m_objType = EObjType::Object;
	auto pCollider = new ColliderOBB(60.0f, { -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	//pCollider->usePhysics(false);
	m_pHero->AddComponent(pCollider);
	ObjectManager::Get().SetProtoObject(m_pHero);
	m_pHero = ObjectManager::Get().TakeObject(L"Guard");
	m_pPlayer->SetParent(m_pHero);
	m_pPlayer->m_curCharacter = PlayerController::ECharacter::EGuard;
	m_pPlayer->ResetOption();
	//ObjectManager::Get().PushObject(m_pHero);

	// 좀비
	m_pZombie = new AHeroObj();
	m_pZombie->SetPlayerCharacter(Zombie, 80.0f, 200.0f, -300.0f);
	m_pZombie->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	m_pZombie->SetANIM(Zombie_IDLE);
	m_pZombie->m_myName = L"Zombie";
	m_pZombie->m_objType = EObjType::Object;
	pCollider = new ColliderOBB(60.0f, { -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	//pCollider->usePhysics(false);
	m_pZombie->AddComponent(pCollider);
	ObjectManager::Get().PushObject(m_pZombie);

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
	// =================================== UI ========================================
	JPanel* pUIRoot = new JPanel(L"UI_IntroRoot");
	pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/test", *pUIRoot);
	JProgressBar* pProj = (JProgressBar*)pUIRoot->find_child(L"HP_Progress");
	pProj->SetValue(m_pPlayer->m_HP); // 값 bind

	pProj = (JProgressBar*)pUIRoot->find_child(L"MP_Progress");
	pProj->SetValue(m_pPlayer->m_MP); // 값 bind

	ObjectManager::Get().PushObject(pUIRoot);
	return true;
}


// 프레임
bool GameScene::Frame() noexcept
{
	if (Input::GetKeyState('Q') == EKeyState::DOWN)
	{
		SendPlaySound("dead.mp3", Vector3::Zero, 2000.0f);
	}
	static D3DXVECTOR3 ListenPosition;
	SoundManager::Get().m_pListenerPos = &ListenPosition;
	ListenPosition = m_pPlayer->GetWorldPosition();

	if (Input::GetKeyState(VK_TAB) == EKeyState::DOWN)
	{
		static auto curCollider = ObjectManager::Get().GetColliderList().begin();
		if (++curCollider == ObjectManager::Get().GetColliderList().end())
		{
			curCollider = ObjectManager::Get().GetColliderList().begin();
		}
		m_pPlayer->SetParent((*curCollider)->m_pParent);
		if((*curCollider)->m_pParent->m_myName == L"Guard")
			m_pPlayer->m_curCharacter = PlayerController::ECharacter::EGuard;
		else if ((*curCollider)->m_pParent->m_myName == L"Zombie")
			m_pPlayer->m_curCharacter = PlayerController::ECharacter::EZombie;
		else
			m_pPlayer->m_curCharacter = PlayerController::ECharacter::EDummy;
		//if (m_pPlayer->GetParent() == m_pHero)
		//{
		//	m_pPlayer->SetParent(m_pZombie);
		//	m_pPlayer->m_curCharacter = PlayerController::ECharacter::EZombie;
		//}
		//else if (m_pPlayer->GetParent() == m_pZombie)
		//{
		//	m_pPlayer->SetParent(m_pBird);
		//	m_pPlayer->m_curCharacter = PlayerController::ECharacter::EDummy;
		//}
		//else if (m_pPlayer->GetParent() == m_pBird)
		//{
		//	m_pPlayer->SetParent(m_pChicken);
		//	m_pPlayer->m_curCharacter = PlayerController::ECharacter::EDummy;
		//}
		//else if (m_pPlayer->GetParent() == m_pChicken)
		//{
		//	m_pPlayer->SetParent(m_pHero);
		//	m_pPlayer->m_curCharacter = PlayerController::ECharacter::EGuard;
		//}
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

	ObjectManager::Cameras[ECamera::Main]->CutParent();
	ObjectManager::Get().PopObject(ObjectManager::Cameras[ECamera::Main]);
	ObjectManager::Get().Release();
	return true;
}