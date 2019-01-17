#include "IntroScene.h"
#include "JEventBind.h"
#include "PacketManager.h"
#include "PlayerController.h"

bool IntroScene::Init() noexcept
{
	FirstInit();
	// UI
	LoadUI();
	///
	m_isLoading = false;
	return true;
}


// 프레임
bool IntroScene::Frame() noexcept
{
	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();
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
	SoundManager::Get().Load("SE_footstep.mp3");
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
	SoundManager::Get().Load("SV_Guard_Shout.mp3");
	SoundManager::Get().Load("SV_Guard_Punch.mp3");
}

bool IntroScene::FirstInit() noexcept
{
	if (m_isFirstInit)
	{
		m_isFirstInit = false;
		ErrorMessage("Intro Loading Start.");
		// 폰트 설정
		WriteManager::Get().SetText({ 0, 0 }, L"", D2D1::ColorF::Black, 20, L"Yu Gothic");
		WriteManager::Get().SetFontSizeAlign(20, EAlign::Center, EAlign::Center);
		//
		m_pParser = new MaxImporter();
		
		LoadSound();
		GameObject* pObject = nullptr;
		Collider*   pCollider = nullptr;
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
		auto pTrans = new CTransformer(Vector3::Up * 400.0f, Quaternion::Up * PI * 0.35f, Vector3::One);
		pTrans->TransEvent = [](Transform* pParent, Transform* pTrans, const float& spf, const float& accTime) {
			pParent->SetTransform(*pTrans);
			pParent->Translate({ cosf(0.5f * accTime) * 200.0f, 0.0f, sinf(0.5f * accTime) * 200.0f });
			return; spf; accTime; pTrans;
		};
		ObjectManager::Get().Lights.front()->AddComponent({ pTrans });
		// 라이트 랜더러
		auto pShpere = (Renderer*)ObjectManager::GetInstance().TakeComponent(L"Sphere");
		pShpere->SetShaderLayout("VS_Basic", "PS_Basic");
		pObject = new GameObject(L"Sun", pShpere);
		pObject->isGlobal(true);
		pObject->SetScale(Vector3::One * 7);
		pObject->SetParent(ObjectManager::Get().Lights.front());

		// Effect 로드
		//ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"Snow.eff", L"../../data/script"));
		ObjectManager::Get().SetProtoObject(new GameObject(L"Boom", m_pParser->CreateFromParticle(L"Boom.eff", L"../../data/script"), EObjType::Effect));
		ObjectManager::Get().SetProtoObject(new GameObject(L"Boom2", m_pParser->CreateFromParticle(L"Boom2.eff", L"../../data/script"), EObjType::Effect));
		ObjectManager::Get().SetProtoObject(new GameObject(L"Boom3", m_pParser->CreateFromParticle(L"Boom3.eff", L"../../data/script"), EObjType::Effect));
		//ObjectManager::Get().SetProtoObject(new GameObject(L"Fire", m_pParser->CreateFromParticle(L"Fire.eff", L"../../data/script"), EObjType::Effect));
		ObjectManager::Get().SetProtoObject(new GameObject(L"Fly", m_pParser->CreateFromParticle(L"Fly.eff", L"../../data/script"), EObjType::Effect));
		//ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"Boom.eff", L"../../data/script"));
		//ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"Boom2.eff", L"../../data/script"));
		//ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"Boom3.eff", L"../../data/script"));
		ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"Fire.eff", L"../../data/script"));
		//ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"Fly.eff", L"../../data/script"));
		//ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"Bigbang.eff", L"../../data/script"));
		//ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"Shock.eff", L"../../data/script"));
		//ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"Atom.eff", L"../../data/script"));
		//ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"WheelWind.eff", L"../../data/script"));
		///

		// ========================= 캐릭터 초기화 ===========================
		ErrorMessage("Intro Character Loading.");
		I_CHARMGR.Init();
		if (!I_CHARMGR.Load(DxManager::GetDevice(), DxManager::GetDContext(), TablePATH))		//경로 중요함
		{
			return false;
		}
		// 단검 충돌시
		static auto pDaggerHit = [](Collider* pA, Collider* pB) {
			if (pB == nullptr)
			{
				auto pEffect = ObjectManager::Get().TakeObject(L"Boom2");// new GameObject(L"HitEffect", ObjectManager::Get().TakeComponent(L"Boom2"));
				pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
				//ObjectManager::Get().PushObject(pEffect);
				//pA->ClearIgnoreList();
				ObjectManager::Get().DisableObject(pA->m_pParent);
			}
			else
			{
				if (pB->m_eTag != ETag::Collider) return;

				pB->SetForce((Normalize(-pA->GetTotalForce()) + Vector3::Up) * 120.0f);
				pB->m_pParent->OperHP(-0.15f);
				if (pB->m_pParent == PlayerController::Get().GetParent())
				{
					((JPanel*)PlayerController::Get().m_pHitEffect)->EffectPlay();
				}
				if (pB->m_pParent->GetHP() <= 0.0f)
				{
					Packet_PlayerDead p_PlayerDead;
					p_PlayerDead.KeyValue = pB->m_pParent->m_keyValue;
					p_PlayerDead.KillUser = pA->m_pPhysics->UserSocket;
					PacketManager::Get().SendPacket((char*)&p_PlayerDead, (USHORT)sizeof(Packet_PlayerDead), PACKET_PlayerDead);
				}
				else if (PacketManager::Get().pMyInfo->UserSocket == pA->m_pPhysics->UserSocket)
				{
					PacketManager::Get().pMyInfo->Score += 100;
					PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
				}
				auto pEffect = ObjectManager::Get().TakeObject(L"Boom2");
				pEffect->SetPosition(pA->m_pParent->GetWorldPosition());
				//ObjectManager::Get().PushObject(pEffect);
				//pA->ClearIgnoreList();
				ObjectManager::Get().DisableObject(pA->m_pParent);
			}
		};

		// 단검
		auto pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(ITEM_Dagger);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->m_myName = L"Dagger";
		pHeroObj->m_objType = EObjType::Object;
		pHeroObj->SetScale(Vector3::One * 0.8f);
		pCollider = new Collider(15.0f);
		pCollider->m_pivot = Vector3::Up * 8.0f + Vector3::Forward * 2.5f;
		pCollider->CollisionEvent = pDaggerHit;
		pHeroObj->AddComponent({ pCollider, ObjectManager::Get().TakeComponent(L"Fire") });
		ObjectManager::Get().SetProtoObject(pHeroObj);

		// 닭
		pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(NPC_Chicken);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->m_myName = L"Chicken";
		pHeroObj->m_objType = EObjType::Object;
		pHeroObj->SetScale(Vector3::One * 0.8f);
		pCollider = new Collider(15.0f);
		pCollider->m_pivot = Vector3::Up * 8.0f + Vector3::Forward * 2.5f;
		pCollider->CollisionEvent = pDaggerHit;
		pHeroObj->AddComponent({ pCollider, ObjectManager::Get().TakeComponent(L"Fire") });
		ObjectManager::Get().SetProtoObject(pHeroObj);

		// 기사 
		pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(Guard, 0.0f, 100.0f, 0.0f);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->SetANIM_Loop(Guard_IDLE);
		pHeroObj->m_myName = L"Guard";
		pHeroObj->m_objType = EObjType::Object;
		pHeroObj->SetScale(Vector3::One * 0.5f);
		pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
		pHeroObj->AddComponent({ pCollider, ObjectManager::Get().TakeComponent(L"Fire") });
		pCollider->m_pivot *= 0.5f;
		ObjectManager::Get().SetProtoObject(pHeroObj);

		// 좀비
		pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(Zombie, 80.0f, 200.0f, -300.0f);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->SetANIM_Loop(Zombie_IDLE);
		pHeroObj->m_myName = L"Zombie";
		pHeroObj->m_objType = EObjType::Object;
		pHeroObj->SetScale(Vector3::One * 0.5f);
		pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
		pHeroObj->AddComponent({ pCollider, ObjectManager::Get().TakeComponent(L"Fire") });
		pCollider->m_pivot *= 0.5f;
		ObjectManager::Get().SetProtoObject(pHeroObj);

		// 새
		pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(NPC_Bird, 0.0f, 80.0f, 300.0f);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->m_myName = L"Bird";
		pHeroObj->m_objType = EObjType::Object;
		pHeroObj->SetScale(Vector3::One * 7.0f);
		pCollider = new ColliderOBB({ -1.0f, 0.0f , -1.0f }, { 1.0f, 2.0f , 1.0f });
		pHeroObj->AddComponent({ pCollider, ObjectManager::Get().TakeComponent(L"Fire") });
		pCollider->m_pivot *= 7.0f;
		pCollider->SetGravityScale(0.3f);
		ObjectManager::Get().SetProtoObject(pHeroObj);

		// =============================== 맵 생성 =================================
		ErrorMessage("Intro Map Loading.");
		m_Importer.Import();
		m_pMap = new XMap();
		//m_pMap = (XMap*)new GameObject(L"");
		m_pMap->Create(DxManager::Get().GetDevice(), DxManager::Get().GetDContext(), &m_Importer, _T("../../Data/Map/Shader/MapShader_Specular.hlsl"), _T("../../Data/Map/Shader/MapShader_Color_Specular.hlsl"), "VS", "PS");
		m_pMapTree = new XQuadTreeIndex();
		m_pMapTree->Build(m_pMap);
		m_pMap->m_objType = EObjType::Map;
		m_pMap->isGlobal();
		m_pMap->isStatic();
		//ObjectManager::Get().PushObject(m_pMap);
		return true;
	}
	return false;
}


void IntroScene::LoadUI() noexcept
{
	JPanel* pUIRoot = new JPanel(L"Intro");
	pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/Intro", *pUIRoot);
	ObjectManager::Get().PushObject(pUIRoot);
	static auto GotoLobby = [](void* pScene) {
		auto pIntro = (IntroScene*)pScene;

		PacketManager::Get().pMyInfo = new UserInfo();
		PacketManager::Get().pMyInfo->DataSize = (UCHAR)pIntro->m_pID->GetString().size() * 2;
		PacketManager::Get().pMyInfo->DataSize = PacketManager::Get().pMyInfo->DataSize > 20 ? 20 : PacketManager::Get().pMyInfo->DataSize;
		memcpy(PacketManager::Get().pMyInfo->UserID, pIntro->m_pID->GetString().c_str(), PacketManager::Get().pMyInfo->DataSize);
		PacketManager::Get().pMyInfo->UserSocket = 0;
		//m_pPW->GetString();

		((IntroScene*)pScene)->SetScene(ESceneName::Lobby);
	};

	m_pID = (JEditCtrl*)pUIRoot->find_child(L"Login_ID");
	m_pPW = (JEditCtrl*)pUIRoot->find_child(L"Login_PW");

	JButtonCtrl* pBtn = (JButtonCtrl*)pUIRoot->find_child(L"Login_Enter");
	pBtn->EventClick.first = GotoLobby;
	pBtn->EventClick.second = this;



	UI::IntroEvent(pUIRoot);
}