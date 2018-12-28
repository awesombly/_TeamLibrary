#include "IntroScene.h"


bool IntroScene::Init() noexcept
{
	// ====================================================================================================
#pragma region Basic
	GameObject* pObject = nullptr;
	//Renderer*	pRenderer = nullptr;
	Collider*   pCollider = nullptr;

	if (m_isFirstInit)
	{
		m_isFirstInit = false;
		LoadSound();
		// 컴포넌트 등록
		ObjectManager::Get().SetProtoComponent(new RPlane(L"Plane", L"None.png"));
		ObjectManager::Get().SetProtoComponent(new RCube(L"Cube", L"None.png"));
		ObjectManager::Get().SetProtoComponent(new RSphere(20, L"Sphere", L"None.png"));
		ObjectManager::Get().SetProtoComponent(new RSphere(10, L"RowSphere", L"None.png"));
		ObjectManager::Get().SetProtoComponent(new RLine(L"Line"));
		ObjectManager::Get().SetProtoComponent(new ColliderAABB(2.0f, -Vector3::One, Vector3::One));
		ObjectManager::Get().SetProtoComponent(new ColliderOBB(2.0f, -Vector3::One, Vector3::One));
		ObjectManager::Get().SetProtoComponent(new Collider(1));

		// 월드 중심축
		RLine*		pSelectLines[3];
		pSelectLines[0] = (RLine*)ObjectManager::Get().TakeComponent(L"Line");
		pSelectLines[1] = (RLine*)ObjectManager::Get().TakeComponent(L"Line");
		pSelectLines[2] = (RLine*)ObjectManager::Get().TakeComponent(L"Line");
		pSelectLines[0]->SetLineInfo(DxManager::Get().GetDContext(), Vector3::Left * 1000, Vector3::Right * 1000, Color::Red, true);
		pSelectLines[1]->SetLineInfo(DxManager::Get().GetDContext(), Vector3::Up * 1000, Vector3::Down * 1000, Color::Green, true);
		pSelectLines[2]->SetLineInfo(DxManager::Get().GetDContext(), Vector3::Forward * 1000, Vector3::Backward * 1000, Color::Blue, true);
		pObject = new GameObject(L"Center", { pSelectLines[0], pSelectLines[1], pSelectLines[2] });
		pObject->isGlobal(true);
		ObjectManager::Get().PushObject(pObject);

		// 라이트
		auto pTrans = new CTransformer(Vector3::Up * 150.0f, Quaternion::Up * PI * 0.35f, Vector3::One);
		pTrans->TransEvent = [](Transform* pParent, Transform* pTrans, const float& spf, const float& accTime) {
			pParent->SetTransform(*pTrans);
			pParent->Translate({ cosf(0.5f * accTime) * 200.0f, 0.0f, sinf(0.5f * accTime) * 200.0f });
			return; spf; accTime; pTrans;
		};
		ObjectManager::Get().Lights.front()->AddComponent({ pTrans });
		// 라이트 랜더러
		auto pShpere = (Renderer*)ObjectManager::GetInstance().TakeComponent(L"Sphere");
		pShpere->SetShaderLayout("VS_PNCT", "PS_PCT");
		pObject = new GameObject(L"Sun", pShpere);
		pObject->isGlobal(true);
		pObject->SetScale(Vector3::One * 7);
		pObject->SetParent(ObjectManager::Get().Lights.front());

		// 파티클
		auto pParticle = new ParticleSystem(L"ParticleSystem", new Particle(), L"UI/cat.png");
		pParticle->m_maxParticleCount = 9999;
		pParticle->m_spawnInterval = 0.013f;
		pParticle->m_minLifeCycle = 5.0f;
		pParticle->m_maxLifeCycle = 6.0f;
		pParticle->m_minInitPosition = Vector3::Zero;// *200;
		pParticle->m_maxInitPosition = Vector3::Zero;// *200;
		pParticle->m_minDirection = Vector3::Forward;
		pParticle->m_maxDirection = Vector3::Backward;
		pParticle->m_minMaxMoveSpeed = 100.0;
		pParticle->m_maxMaxMoveSpeed = 100.0;
		pParticle->m_minAccMoveSpeed = 5.0f;
		pParticle->m_maxAccMoveSpeed = 10.0f;
		pParticle->m_minCurMoveSpeed = 20.0f;
		pParticle->m_maxCurMoveSpeed = 40.0f;
		pParticle->m_minDirAngle = Quaternion::Zero;
		pParticle->m_maxDirAngle = Quaternion::Zero;
		pParticle->m_minRotateSpeed = 0.0f;
		pParticle->m_maxRotateSpeed = 0.0f;
		pParticle->m_minColor = { 0.5f, 0.5f, 0.5f, 0.5f };
		pParticle->m_maxColor = Color::White * 2;
		pParticle->m_minGravityPower = 15.0f;
		pParticle->m_maxGravityPower = 25.0f;
		pParticle->m_minInitScale = Vector3::One * 5.0f;
		pParticle->m_minInitScale = Vector3::One * 5.0f;
		pParticle->m_minScalePerLife = Vector3::Zero;
		pParticle->m_maxScalePerLife = Vector3::Zero;

		auto pParticleObject = new GameObject(L"ParticleSystem", { pParticle, new CTransformer(Vector3::Zero, Quaternion::Left * 1.5f) });
		pParticleObject->Translate(Vector3::Up * 250);
		//ObjectManager::Get().PushObject(pParticleObject);
		ObjectManager::Get().SetProtoObject(pParticleObject);

		// 단검 충돌시
		static auto pDaggerHit = [](Collider* pA, Collider* pB) {
			if (pB != nullptr)
			{
				pB->OperHP(-0.15f);
				pB->SetForce(Normalize((pB->GetCenter() - pA->GetCenter()) + Vector3::Up) * 200.0f);
			}
			ObjectManager::Get().DisableObject(pA->m_pParent);
			//ObjectManager::Get().RemoveComponent(pA);
			//ObjectManager::Get().PopCollider(pA);
		};
		// 단검
		auto pDagger = new AHeroObj();
		pDagger->SetPlayerCharacter(ITEM_Dagger);
		pDagger->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pDagger->m_myName = L"Dagger";
		pDagger->m_objType = EObjType::Object;
		pCollider = new Collider(15.0f);
		pCollider->m_pivot = Vector3::Up * 10.0f + Vector3::Forward * 5.0f;
		pCollider->CollisionEvent = pDaggerHit;
		pDagger->AddComponent(pCollider);
		ObjectManager::Get().SetProtoObject(pDagger);

		// 닭 생성
		auto pChicken = new AHeroObj();
		pChicken->SetPlayerCharacter(NPC_Chicken);
		pChicken->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pChicken->m_myName = L"Chicken";
		pChicken->m_objType = EObjType::Object;
		pCollider = new Collider(15.0f);//new ColliderOBB(23.0f, { -15.0f, 0.0f , -15.0f }, { 15.0f, 30.0f , 15.0f });
		pCollider->m_pivot = Vector3::Up * 10.0f + Vector3::Forward * 5.0f;
		pCollider->CollisionEvent = pDaggerHit;
		pChicken->AddComponent(pCollider);
		ObjectManager::Get().SetProtoObject(pChicken);
	}
#pragma endregion
	// ====================================================================================================
	m_isLoading = false;
	return true;
}


// 프레임
bool IntroScene::Frame() noexcept
{
	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();
	SetScene(ESceneName::Lobby);
	return true;
}
 
// 랜더
bool IntroScene::Render() noexcept
{
	DxManager::Get().Render();
	ObjectManager::Get().Render(DxManager::GetDContext());
	SoundManager::Get().Render();
	return true;
}

// 릴리즈
bool IntroScene::Release() noexcept
{
	ObjectManager::Get().Release();
	return true;
}


void IntroScene::LoadSound() noexcept
{
	SoundManager::Get().Load("BGM/PLAY ROUGH.mp3", false, FMOD_LOOP_NORMAL);
	SoundManager::Get().Load("bgm_ingame01.mp3", false, FMOD_LOOP_NORMAL);
	SoundManager::Get().Load("bgm_ingame02.mp3", false, FMOD_LOOP_NORMAL);
	SoundManager::Get().Load("bgm_Title01.mp3", false, FMOD_LOOP_NORMAL);
	SoundManager::Get().Load("bgm_Title02.mp3", false, FMOD_LOOP_NORMAL);
	SoundManager::Get().Load("SE_bird_01.mp3");
	SoundManager::Get().Load("SE_chicken.mp3");
	SoundManager::Get().Load("SE_Click01.mp3");
	SoundManager::Get().Load("SE_Click02.mp3");
	SoundManager::Get().Load("SE_Dance01.mp3");
	SoundManager::Get().Load("SE_Dance02.mp3");
	SoundManager::Get().Load("SE_Dance03.mp3");
	SoundManager::Get().Load("SE_Dance04.mp3");
	SoundManager::Get().Load("SE_Dance05.mp3");
	SoundManager::Get().Load("SE_Dance06.mp3");
	SoundManager::Get().Load("SE_Dance07.mp3");
	SoundManager::Get().Load("SE_Dance08.mp3");
	SoundManager::Get().Load("SE_footstep.mp3");
	SoundManager::Get().Load("SE_game_win.mp3");
	SoundManager::Get().Load("SE_game_lose.mp3");
	SoundManager::Get().Load("SE_game_time_start.mp3");
	SoundManager::Get().Load("SE_game_time_end.mp3");
	SoundManager::Get().Load("SE_jajan.mp3");
	SoundManager::Get().Load("SE_jump01.mp3");
	SoundManager::Get().Load("SE_jump02.mp3");
	SoundManager::Get().Load("SE_throw01.mp3");
	SoundManager::Get().Load("SE_throw02.mp3");
	SoundManager::Get().Load("SE_throw03.mp3");
	SoundManager::Get().Load("SE_zombie_fly.mp3");
	SoundManager::Get().Load("SE_zombie_hit01.mp3");
	SoundManager::Get().Load("SE_zombie_hit02.mp3");
}