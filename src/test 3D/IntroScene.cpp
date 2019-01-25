#include "IntroScene.h"
#include "JEventBind.h"
#include "PacketManager.h"
#include "PlayerController.h"
#include "EventManager.h"
#include "AIZombie.h"
#include "AIZombieCrawl.h"
#include "AIZombieCast.h"
#include "AIZombieEx.h"
#include "AIZombieKing.h"

bool IntroScene::Init() noexcept
{
	FirstInit();
	// UI
	LoadSound();
	LoadUI();
	// 서버 연결
	//ConnectMatchingServer();
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
	SoundManager::Get().Load("SV_gogogo.mp3");
	SoundManager::Get().Load("SV_jocena.mp3");
	SoundManager::Get().Load("SE_dead.mp3");
	SoundManager::Get().Load("SE_HIT.mp3");
	SoundManager::Get().Load("SE_Rudy.mp3");
}

bool IntroScene::FirstInit() noexcept
{
	if (m_isFirstInit)
	{
		m_isFirstInit = false;
		ErrorMessage(__FUNCTION__ + " -> Start."s);
		// 폰트, 사운드 등
		WriteManager::Get().SetText({ 0, 0 }, L"", D2D1::ColorF::Black, 20, L"휴면둥근헤드라인");
		WriteManager::Get().SetFontSizeAlign(20, EAlign::Center, EAlign::Center);
		m_pParser = new MaxImporter();
		///
		GameObject* pObject = nullptr;
		Collider*   pCollider = nullptr;
		//// Effect 로드
		ObjectManager::Get().SetProtoObject(new GameObject(L"Slash", m_pParser->CreateFromParticle(L"Slash.eff", L"../../data/script"), EObjType::Effect));
		ObjectManager::Get().SetProtoObject(new GameObject(L"ZDead", m_pParser->CreateFromParticle(L"ZombieDead.eff", L"../../data/script"), EObjType::Effect));
		ObjectManager::Get().SetProtoObject(new GameObject(L"ZAttack", m_pParser->CreateFromParticle(L"ZombieAttack.eff", L"../../data/script"), EObjType::Effect));
		ObjectManager::Get().SetProtoObject(new GameObject(L"PAttack", m_pParser->CreateFromParticle(L"Boom2.eff", L"../../data/script"), EObjType::Effect));
		ObjectManager::Get().SetProtoObject(new GameObject(L"PDead", m_pParser->CreateFromParticle(L"Boom3.eff", L"../../data/script"), EObjType::Effect));
		ObjectManager::Get().SetProtoObject(new GameObject(L"Fly", m_pParser->CreateFromParticle(L"Fire.eff", L"../../data/script"), EObjType::Effect));
		ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"Fire.eff", L"../../data/script"));
		// 아이템
		pCollider = new Collider(80.0f);
		pObject = new GameObject(L"Atom", { pCollider, m_pParser->CreateFromParticle(L"Bigbang.eff", L"../../data/script"), new CTransformer(Vector3::Zero, {3.0f, 5.0f, 7.0f, 0.0f}) }, EObjType::Effect);
		pCollider->SetGravityScale(0.0f);
		pCollider->usePhysics(false);
		pCollider->CollisionEvent = MyEvent::GiantItem;
		pCollider->m_eTag = ETag::Dummy;
		ObjectManager::Get().SetProtoObject(pObject);
		
		/// 컴포넌트 등록
		//ObjectManager::Get().SetProtoComponent(new RPlane(L"Plane", L"None.png"));
		ObjectManager::Get().SetProtoComponent(new RCube(L"Cube", L"None.png"));
		//ObjectManager::Get().SetProtoComponent(new RSphere(20, L"Sphere", L"None.png"));
		ObjectManager::Get().SetProtoComponent(new RSphere(10, L"RowSphere", L"None.png"));
		//ObjectManager::Get().SetProtoComponent(new RLine(L"Line"));
		//ObjectManager::Get().SetProtoComponent(new ColliderAABB(2.0f, -Vector3::One, Vector3::One));
		//ObjectManager::Get().SetProtoComponent(new ColliderOBB(2.0f, -Vector3::One, Vector3::One));
		//ObjectManager::Get().SetProtoComponent(new Collider(1));

		// 라이트
		auto pTrans = new CTransformer(Vector3::Up * 400.0f, Quaternion::Up * PI * 0.35f, Vector3::One);
		pTrans->TransEvent = [](Transform* pParent, Transform* pTrans, const float& spf, const float& accTime) {
			pParent->SetTransform(*pTrans);
			pParent->Translate({ cosf(0.5f * accTime) * 200.0f, 0.0f, sinf(0.5f * accTime) * 200.0f });
			return; spf; accTime; pTrans;
		};
		ObjectManager::Get().Lights.front()->AddComponent({ pTrans });
		// 라이트 랜더러
		auto pShpere = (Renderer*)ObjectManager::GetInstance().TakeComponent(L"RowSphere");
		pShpere->SetShaderLayout("VS_Basic", "PS_Basic");
		pObject = new GameObject(L"Sun", pShpere);
		pObject->isGlobal(true);
		pObject->SetScale(Vector3::One * 7);
		pObject->SetParent(ObjectManager::Get().Lights.front());


		// ========================= 캐릭터 초기화 ===========================
		ErrorMessage(__FUNCTION__ + " -> Character Loading."s);
		I_CHARMGR.Init();
		if (!I_CHARMGR.Load(DxManager::GetDevice(), DxManager::GetDContext(), TablePATH))		//경로 중요함
		{
			return false;
		}
		ErrorMessage(__FUNCTION__ + " -> Object Setting."s);
		// 단검
		auto pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(ITEM_Dagger);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->m_myName = L"Dagger";
		pHeroObj->m_objType = EObjType::Dummy;
		pHeroObj->SetScale(Vector3::One * 0.6f);
		pCollider = new Collider(15.0f);
		pHeroObj->AddComponent({ pCollider, ObjectManager::Get().TakeComponent(L"Fire") });
		pCollider->m_pivot = Vector3::Up * 6.0f + Vector3::Forward * 2.5f;
		pCollider->CollisionEvent = MyEvent::DaggerHit;
		pCollider->m_eTag = ETag::Dummy;
		pHeroObj->m_pPhysics->UserSocket = (UINT)-1;
		pHeroObj->SetHP(100.0f);
		ObjectManager::Get().SetProtoObject(pHeroObj);

		// 닭
		pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(NPC_Chicken);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->m_myName = L"Chicken";
		pHeroObj->m_objType = EObjType::Dummy;
		pHeroObj->SetScale(Vector3::One * 0.4f);
		pCollider = new Collider(15.0f);
		pHeroObj->AddComponent({ pCollider, ObjectManager::Get().TakeComponent(L"Fire") });
		pCollider->m_pivot = Vector3::Up * 4.0f + Vector3::Forward * 2.5f;
		pCollider->CollisionEvent = MyEvent::ZombieThrow;
		pCollider->m_eTag = ETag::Dummy;
		pHeroObj->m_pPhysics->UserSocket = (UINT)-1;
		pHeroObj->SetHP(100.0f);
		ObjectManager::Get().SetProtoObject(pHeroObj);

		// 기사 
		pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(Guard);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->SetANIM_Loop(Guard_IDLE);
		pHeroObj->m_myName = L"Guard";
		pHeroObj->m_objType = EObjType::Character;
		pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
		pHeroObj->AddComponent({ pCollider/*, ObjectManager::Get().TakeComponent(L"Fire")*/ });
		pCollider->m_eTag = ETag::Ally;
		pHeroObj->m_pPhysics->UserSocket = (UINT)-1;
		ObjectManager::Get().SetProtoObject(pHeroObj);
		// 좀비
		pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(Zombie);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->SetANIM_Loop(Zombie_IDLE);
		pHeroObj->m_myName = L"Zombie";
		pHeroObj->m_objType = EObjType::Enemy;
		pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
		pHeroObj->AddComponent({ pCollider, new AIZombie() });
		pCollider->CollisionEvent = MyEvent::ZombieHit;
		pCollider->m_eTag = ETag::Enemy;
		pHeroObj->m_pPhysics->UserSocket = (UINT)-1;
		pHeroObj->m_pPhysics->m_damage = 0.08f;
		ObjectManager::Get().SetProtoObject(pHeroObj);
		// 좀비 Cast
		pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(Zombie);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->SetANIM_Loop(Zombie_IDLE);
		pHeroObj->m_myName = L"Caster";
		pHeroObj->m_objType = EObjType::Enemy;
		pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
		pHeroObj->AddComponent({ pCollider, new AIZombieCast() });
		pCollider->CollisionEvent = MyEvent::ZombieHit;
		pCollider->m_eTag = ETag::Enemy;
		pHeroObj->m_pPhysics->UserSocket = (UINT)-1;
		pHeroObj->m_pPhysics->m_damage = 0.08f;
		ObjectManager::Get().SetProtoObject(pHeroObj);
		// 좀비 Crawl
		pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(Zombie);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->SetANIM_Loop(Zombie_CRAWL);
		pHeroObj->m_myName = L"Crawler";
		pHeroObj->m_objType = EObjType::Enemy;
		pCollider = new ColliderOBB({ -13.0f, 0.0f , -40.0f }, { 13.0f, 25.0f , 40.0f });
		pHeroObj->AddComponent({ pCollider, new AIZombieCrawl() });
		pCollider->CollisionEvent = MyEvent::ZombieHit;
		pCollider->m_pPhysics->m_mass = 0.2f;
		pCollider->m_pPhysics->m_damping = 1.0f;
		pCollider->m_eTag = ETag::Enemy;
		pHeroObj->m_pPhysics->UserSocket = (UINT)-1;
		pHeroObj->m_pPhysics->m_damage = 0.3f;
		ObjectManager::Get().SetProtoObject(pHeroObj);
		// 좀비 Ex
		pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(ZombieEX);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->SetANIM_Loop(ZombieEX_IDLE);
		pHeroObj->m_myName = L"Mutant";
		pHeroObj->m_objType = EObjType::Enemy;
		pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
		pHeroObj->AddComponent({ pCollider, new AIZombieEx() });
		pCollider->CollisionEvent = MyEvent::ZombieHit;
		pCollider->m_pPhysics->m_mass = 0.2f;
		pCollider->m_pPhysics->m_damping = 1.0f;
		pCollider->m_eTag = ETag::Enemy;
		pHeroObj->m_pPhysics->UserSocket = (UINT)-1;
		pHeroObj->m_pPhysics->m_damage = 0.3f;
		ObjectManager::Get().SetProtoObject(pHeroObj);
		// 좀비 King
		pHeroObj = new AHeroObj();
		pHeroObj->SetPlayerCharacter(Zombie_KING);
		pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		pHeroObj->SetANIM_Loop(Zombie_KING_IDLE);
		pHeroObj->m_myName = L"Tank";
		pHeroObj->m_objType = EObjType::Enemy;
		pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
		pHeroObj->AddComponent({ pCollider, new AIZombieKing() });
		pCollider->CollisionEvent = MyEvent::ZombieHit;
		pCollider->m_pPhysics->m_mass = 0.1f;
		pCollider->m_pPhysics->m_damping = 2.0f;
		pCollider->m_eTag = ETag::Enemy;
		pHeroObj->m_pPhysics->UserSocket = (UINT)-1;
		pHeroObj->m_pPhysics->m_damage = 0.4f;
		ObjectManager::Get().SetProtoObject(pHeroObj);

		//// 새
		//pHeroObj = new AHeroObj();
		//pHeroObj->SetPlayerCharacter(NPC_Bird);
		//pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
		//pHeroObj->m_myName = L"Bird";
		//pHeroObj->m_objType = EObjType::Enemy;
		//pHeroObj->SetScale(Vector3::One * 4.0f);
		//pCollider = new ColliderOBB({ -1.0f, 0.0f , -1.0f }, { 1.0f, 2.0f , 1.0f });
		//pHeroObj->AddComponent({ pCollider/*, ObjectManager::Get().TakeComponent(L"Fire")*/ });
		//pCollider->m_pivot *= pHeroObj->GetScaleAverage();
		//pCollider->SetGravityScale(0.3f);
		//pHeroObj->m_pPhysics->UserSocket = (UINT)-1;
		//ObjectManager::Get().SetProtoObject(pHeroObj);

		// =============================== 맵 생성 =================================
		ErrorMessage(__FUNCTION__ + " -> Map Loading."s);
		m_Importer.Import();
		m_pMap = new XMap();
		m_pMap->Create(DxManager::Get().GetDevice(), DxManager::Get().GetDContext(), &m_Importer, _T("../../Data/Map/Shader/MapShader_Specular.hlsl"), _T("../../Data/Map/Shader/MapShader_Color_Specular.hlsl"), "VS", "PS");
		m_pMapTree = new XQuadTreeIndex();
		m_pMapTree->Build(m_pMap);
		m_pMap->m_objType = EObjType::Map;
		m_pMap->isGlobal(true);
		m_pMap->isStatic(true);
		//ObjectManager::Get().PushObject(m_pMap);
		return true;
	}
	return false;
}


void IntroScene::LoadUI() noexcept
{
	ErrorMessage(__FUNCTION__ + " -> Start."s);
	JPanel* pUIRoot = new JPanel(L"Intro");
	pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/Intro", *pUIRoot);
	ObjectManager::Get().PushObject(pUIRoot);
	static auto GotoLobby = [](void* pScene) {
		auto pIntro = (IntroScene*)pScene;

		//if (m_loginCheck == -99)
		{
			PacketManager::Get().pMyInfo = new UserInfo();
			PacketManager::Get().pMyInfo->DataSize = (UCHAR)pIntro->m_pID->GetString().size() * 2;
			PacketManager::Get().pMyInfo->DataSize = PacketManager::Get().pMyInfo->DataSize > 12 ? 12 : PacketManager::Get().pMyInfo->DataSize;
			memcpy(PacketManager::Get().pMyInfo->UserID, pIntro->m_pID->GetString().c_str(), PacketManager::Get().pMyInfo->DataSize);
			PacketManager::Get().pMyInfo->UserSocket = 0;
			///
			pIntro->SetScene(ESceneName::Lobby);
			ErrorMessage("서버 미접속");
			return;
		}

		// 로그인 요청
		pIntro->RequestSignIn(pIntro->m_pID->GetString().c_str(), pIntro->m_pPW->GetString().c_str());
		pIntro->m_loginCheck = 0;
		while (pIntro->m_loginCheck == 0)
		{
			if (pIntro->m_loginCheck == 1)
			{
				PacketManager::Get().pMyInfo = new UserInfo();
				PacketManager::Get().pMyInfo->DataSize = (UCHAR)pIntro->m_pID->GetString().size() * 2;
				PacketManager::Get().pMyInfo->DataSize = PacketManager::Get().pMyInfo->DataSize > 12 ? 12 : PacketManager::Get().pMyInfo->DataSize;
				memcpy(PacketManager::Get().pMyInfo->UserID, pIntro->m_pID->GetString().c_str(), PacketManager::Get().pMyInfo->DataSize);
				PacketManager::Get().pMyInfo->UserSocket = 0;
				///
				pIntro->SetScene(ESceneName::Lobby);
				break;
			}
			if (pIntro->m_loginCheck == -1)
			{
				break;
			}
		}
		ErrorMessage("로그인 종료");
	};

	m_pID = (JEditCtrl*)pUIRoot->find_child(L"Login_ID");
	m_pPW = (JEditCtrl*)pUIRoot->find_child(L"Login_PW");

	JButtonCtrl* pBtn = (JButtonCtrl*)pUIRoot->find_child(L"Login_Enter");
	pBtn->EventClick.first = GotoLobby;
	pBtn->EventClick.second = this;

	/// 회원가입
	//JEditCtrl* pSignUpID = (JEditCtrl*)pUIRoot->find_child(L"SignUp_ID");
	//JEditCtrl* pSignUpPW = (JEditCtrl*)pUIRoot->find_child(L"SignUp_PW");
	//JEditCtrl* pSignUpPWCheck = (JEditCtrl*)pUIRoot->find_child(L"SignUp_PWCheck");
	//JButtonCtrl* pSignUpEnter = (JButtonCtrl*)pUIRoot->find_child(L"SignUp_Enter");

	SoundManager::Get().SetBGM("SE_Rudy.mp3");
	UI::IntroEvent(pUIRoot);
}
