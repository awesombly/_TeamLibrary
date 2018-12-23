#include "IntroScene.h"



bool IntroScene::Init() noexcept
{
	// ====================================================================================================
#pragma region Basic
	GameObject* pObject = nullptr;
	Renderer*	pRenderer = nullptr;
	Collider*   pCollider = nullptr;

	if (m_isFirstInit)
	{
		m_isFirstInit = false;
		LoadSound();
		// 컴포넌트 등록
		ObjectManager::Get().SetProtoComponent(new RPlane(L"Plane", L"None.png"));
		ObjectManager::Get().SetProtoComponent(new RCube(L"Cube", L"None.png"));
		ObjectManager::Get().SetProtoComponent(new RSphere(20, L"Sphere", L"None.png"));
		ObjectManager::Get().SetProtoComponent(new RLine(L"Line"));
		ObjectManager::Get().SetProtoComponent(new ColliderAABB(-Vector3::One, Vector3::One));
		ObjectManager::Get().SetProtoComponent(new ColliderOBB(-Vector3::One, Vector3::One));
		ObjectManager::Get().SetProtoComponent(new ColliderSphere(1));

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

		// 높이 맵
		pCollider = (Collider*)ObjectManager::Get().TakeComponent(L"ColliderOBB");
		//pCollider->m_pivot = Vector3::Up * 25.0f;
		pCollider->useGravity(false);
		pCollider->usePhysics(false);
		((ColliderOBB*)pCollider)->SetMinMax({ -300.0f, -50.0f, -300.0f }, { 300.0f, 50.0f, 300.0f });
		auto mapMap = new HeightMap(L"HeightMap", EComponent::Renderer, L"mounds.jpg");
		auto pHeightMap = new GameObject(L"HeightMap", { mapMap, pCollider }, EObjType::Map);
		mapMap->CreateHeightMap(DxManager::GetDContext(), L"HeightMap/HEIGHT_MOUNDS.bmp", 10, 1.0f);
		mapMap->SetNormalMap(L"NormalMap/wall_NM_height.dds");
		mapMap->SetEnviromentMap(L"CubeMap/grassenvmap1024.dds", EEnviType::Fresnel);
		pHeightMap->Translate(Vector3::Down * 100.0f);
		//ObjectManager::Get().PushObject(pHeightMap);
		ObjectManager::Get().SetProtoObject(pHeightMap);

		// 파티클
		auto pParticle = new ParticleSystem(L"ParticleSystem", new Particle(), L"UI/cat.png");
		pParticle->m_maxParticleCount = 9999;
		pParticle->m_spawnInterval = 0.013f;
		pParticle->m_minLifeCycle = 7.0f;
		pParticle->m_maxLifeCycle = 7.0f;
		pParticle->m_minInitPosition = Vector3::One * 200;
		pParticle->m_maxInitPosition = -Vector3::One * 200;
		pParticle->m_minDirection = -Vector3::One;
		pParticle->m_maxDirection = Vector3::One;
		pParticle->m_minMaxMoveSpeed = 2.0;
		pParticle->m_maxMaxMoveSpeed = 20.0;
		pParticle->m_minAccMoveSpeed = 1.0f;
		pParticle->m_maxAccMoveSpeed = 2.0f;
		pParticle->m_minCurMoveSpeed = 1.0f;
		pParticle->m_maxCurMoveSpeed = 10.0f;
		pParticle->m_minDirAngle = Quaternion::Zero;
		pParticle->m_maxDirAngle = Quaternion::Zero;
		pParticle->m_minRotateSpeed = 0.0f;
		pParticle->m_maxRotateSpeed = 0.0f;
		pParticle->m_minColor = { 0.5f, 0.5f, 0.5f, 0.5f };
		pParticle->m_maxColor = Color::White * 2;
		pParticle->m_minGravityPower = 5.0f;
		pParticle->m_maxGravityPower = 10.0f;

		auto pParticleObject = new GameObject(L"ParticleSystem", { pParticle });
		pParticleObject->Translate(Vector3::Up * 200);
		//ObjectManager::Get().PushObject(pParticleObject);
		ObjectManager::Get().SetProtoObject(pParticleObject);

		// 노말 환경
		pRenderer = (Renderer*)ObjectManager::Get().TakeComponent(L"Cube");
		pRenderer->SetNormalMap(L"NormalMap/tileADOT3.jpg");
		pRenderer->SetEnviromentMap(L"CubeMap/grassenvmap1024.dds", EEnviType::Refraction);
		pCollider = (Collider*)ObjectManager::Get().TakeComponent(L"ColliderOBB");
		pObject = new GameObject(L"Object1", { pRenderer, pCollider, new CTransformer(Vector3::Zero, Quaternion::Right * 2.0f, Vector3::One) }, EObjType::Object);
		pObject->Translate(Vector3::Backward * 30.0f + Vector3::Up * 200.0f);
		pObject->SetScale(Vector3::One * 4);
		//ObjectManager::Get().PushObject(pObject);
		ObjectManager::Get().SetProtoObject(pObject);

		pRenderer = (Renderer*)ObjectManager::Get().TakeComponent(L"Cube");
		pRenderer->SetNormalMap(L"NormalMap/wall_NM_height.dds");
		pRenderer->SetEnviromentMap(L"CubeMap/grassenvmap1024.dds", EEnviType::Fresnel);
		pCollider = (Collider*)ObjectManager::Get().TakeComponent(L"ColliderOBB");
		pObject = new GameObject(L"Object2", { pRenderer, pCollider, new CTransformer(Vector3::Zero, Quaternion::Up, Vector3::One) }, EObjType::Object);
		pObject->Translate(Vector3::Backward * 30.0f + Vector3::Left * 140.0f + Vector3::Up * 300);
		pObject->SetScale(Vector3::One * 10);
		//ObjectManager::Get().PushObject(pObject);
		ObjectManager::Get().SetProtoObject(pObject);

		pRenderer = (Renderer*)ObjectManager::Get().TakeComponent(L"Sphere");
		pRenderer->SetNormalMap(L"NormalMap/Earth_NormalMap.dds");
		pRenderer->SetEnviromentMap(L"CubeMap/grassenvmap1024.dds", EEnviType::Basic);
		pCollider = (Collider*)ObjectManager::Get().TakeComponent(L"ColliderSphere");
		pObject = new GameObject(L"Object3", { pRenderer, pCollider, new CTransformer(Vector3::Up, Quaternion::Base, Vector3::One) }, EObjType::Object);
		pObject->Translate(Vector3::Backward * 30.0f + Vector3::Right * 140.0f + Vector3::Up * 250);
		pObject->SetScale(Vector3::One * 15);
		ObjectManager::Get().SetProtoObject(pObject);
		//ObjectManager::Get().PushObject(pObject);
	}

	//m_pTakeObject = ObjectManager::Get().TakeObject(L"Object1");
	//m_pTakeObject2 = ObjectManager::Get().TakeObject(L"Object2");
	//m_pTakeObject3 = ObjectManager::Get().TakeObject(L"Object3");
#pragma endregion
	// ====================================================================================================


	return true;
}


// 프레임
bool IntroScene::Frame() noexcept
{
	SetScene(ESceneName::Main);
	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();
	return true;
}
 
// 랜더
bool IntroScene::Render() noexcept
{
	DxManager::Get().Render();
	ObjectManager::Get().Render(DxManager::Get().GetDContext());
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
	//SoundManager::Get().Load("BGM/Mischievous Masquerade", false, FMOD_LOOP_NORMAL);
	//SoundManager::Get().Load("BGM/Sandbag Mini Game", false, FMOD_LOOP_NORMAL);
	//SoundManager::Get().Load("BGM/Brawl Breaks", false, FMOD_LOOP_NORMAL);
	//SoundManager::Get().Load("BGM/Lobby", false, FMOD_LOOP_NORMAL);
	SoundManager::Get().Load("BGM/PLAY ROUGH", false, FMOD_LOOP_NORMAL);
	SoundManager::Get().SetBGM("BGM/PLAY ROUGH");
	//
	SoundManager::Get().Load("bash1");
}