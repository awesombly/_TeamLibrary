#include "GameScene.h"


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
	m_pHero = (AHeroObj*)ObjectManager::Get().TakeObject(L"Guard");
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
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/UI_InGame", *pUIRoot);
	JProgressBar* pProj = (JProgressBar*)pUIRoot->find_child(L"HP_Progress");
	pProj->SetValue(m_pPlayer->m_HP); // 값 bind
	pProj = (JProgressBar*)pUIRoot->find_child(L"MP_Progress");
	pProj->SetValue(m_pPlayer->m_MP); // 값 bind

	// Slider
	m_pVolume = (JSliderCtrl*)pUIRoot->find_child(L"Set_Volum");
	m_pMouseSense = (JSliderCtrl*)pUIRoot->find_child(L"Set_Mouse");
	// Exit
	static auto pGameExit = [](void* pScene) {
		((MainClass*)pScene)->SetScene(ESceneName::Lobby);
	};
	auto pExit = (JTextCtrl*)pUIRoot->find_child(L"Set_GameExit");
	pExit->EventClick.first = pGameExit;
	pExit->EventClick.second = this;
	// 체크 박스
	m_pCheckBox = (JCheckCtrl*)pUIRoot->find_child(L"temp_Check0");

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
	if (Input::GetKeyState('E') == EKeyState::DOWN)
	{
		SendPlaySound("SE_Click01.mp3", Vector3::Zero, 2000.0f);
	}
	static D3DXVECTOR3 ListenPosition;
	SoundManager::Get().m_pListenerPos = &ListenPosition;
	ListenPosition = m_pPlayer->GetWorldPosition();

	// 플레이어 변경
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

		m_pPlayer->ResetOption();
	}
	// 설정 동기화
	SoundManager::Get().SetMasterVolume(*m_pVolume->GetValue());
	m_pPlayer->m_mouseSense = *m_pMouseSense->GetValue();

	m_pMapTree->Frame();
	I_Object.Frame(Timer::SPF, Timer::AccumulateTime);
	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();

	for (auto& iter : ObjectManager::Get().GetColliderList())
	{
		iter->m_mapHeight = m_pMap->GetHeight(iter->m_pParent->GetWorldPosition().x, iter->m_pParent->GetWorldPosition().z);
	}
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

	// 바운딩 박스 표시
	if (m_pCheckBox->m_bCheck)
	{
		static GameObject* pBox = nullptr;
		static GameObject* pSphere = nullptr;
		if (pBox == nullptr)
		{
			pBox = new GameObject(L"DebugBox", ObjectManager::Get().TakeComponent(L"Cube"));
			pBox->isGlobal(true);
			pSphere = new GameObject(L"DebugSphere", ObjectManager::Get().TakeComponent(L"RowSphere"));
			pSphere->isGlobal(true);
		}

		DxManager::Get().SetRasterizerState(ERasterS::Wireframe);
		for (auto& iter : ObjectManager::Get().GetColliderList())
		{
			switch (iter->m_eCollider)
			{
			case ECollider::AABB:
			{
				pBox->SetPosition(iter->GetCenter());
				pBox->SetRotation(Quaternion::Base);
				pBox->SetScale(((ColliderAABB*)iter)->GetLength() * 0.5f);
				pBox->Frame(0.0f, 0.0f);
				pBox->Render(DxManager::GetDContext());
			}	break;
			case ECollider::OBB:
			{
				pBox->SetPosition(iter->GetCenter());
				pBox->SetRotation(iter->m_pParent->GetRotation());
				pBox->SetScale(((ColliderOBB*)iter)->GetExtents());
				pBox->Frame(0.0f, 0.0f);
				pBox->Render(DxManager::GetDContext());
			}	break;
			case ECollider::Sphere:
			{
			}	break;
			}
			pSphere->SetPosition(iter->GetCenter());
			pSphere->SetRotation(Quaternion::Base);
			pSphere->SetScale(iter->GetWorldRadius() * Vector3::One);
			pSphere->Frame(0.0f, 0.0f);
			pSphere->Render(DxManager::GetDContext());
		}
		DxManager::Get().SetRasterizerState(ERasterS::Current);
	}
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