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
	///
	m_isLoading = false;
	return true;
}


// ������
bool IntroScene::Frame() noexcept
{
	DxManager::Get().Frame();
	ObjectManager::Get().Frame(Timer::SPF, Timer::AccumulateTime);
	SoundManager::Get().Frame();
	return true;
}

// ����
bool IntroScene::Render() noexcept
{
	DxManager::Get().Render();
	ObjectManager::Get().Render(DxManager::GetDContext());
	SoundManager::Get().Render();
	return true;
}

// ������
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
		// ��Ʈ, ���� ��
		WriteManager::Get().SetText({ 0, 0 }, L"", D2D1::ColorF::Black, 20, L"�޸�ձ�������");
		WriteManager::Get().SetFontSizeAlign(20, EAlign::Center, EAlign::Center);
		m_pParser = new MaxImporter();

		// ===================================== ĳ���� �ʱ�ȭ ==============================================
		ErrorMessage(__FUNCTION__ + " -> Character Loading."s);
		I_CHARMGR.Init();
		if (!I_CHARMGR.Load(DxManager::GetDevice(), DxManager::GetDContext(), TablePATH))		//��� �߿���
		{
			return false;
		}
		// ===================================== ������Ʈ �ʱ�ȭ ==============================================
		ErrorMessage(__FUNCTION__ + " -> Object Setting."s);
		SetObjects();
		// =============================== �� ���� =================================
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


void IntroScene::SetObjects() noexcept
{
	GameObject* pObject = nullptr;
	Collider*   pCollider = nullptr;
	
	// ������Ʈ ���
	ObjectManager::Get().SetProtoComponent(new RCube(L"Cube", L"None.png"));
	//ObjectManager::Get().SetProtoComponent(new RSphere(20, L"Sphere", L"None.png"));
	ObjectManager::Get().SetProtoComponent(new RSphere(10, L"RowSphere", L"None.png"));
	// ����Ʈ
	auto pTrans = new CTransformer(Vector3::Up * 400.0f, Quaternion::Up * PI * 0.35f, Vector3::One);
	pTrans->TransEvent = [](Transform* pParent, Transform* pTrans, const float& spf, const float& accTime) {
		pParent->SetTransform(*pTrans);
		pParent->Translate({ cosf(0.5f * accTime) * 200.0f, 0.0f, sinf(0.5f * accTime) * 200.0f });
		return; spf; accTime; pTrans;
	};
	ObjectManager::Get().Lights.front()->AddComponent({ pTrans });
	// ����Ʈ ������
	//auto pShpere = (Renderer*)ObjectManager::GetInstance().TakeComponent(L"RowSphere");
	//pShpere->SetShaderLayout("VS_Basic", "PS_Basic");
	//pObject = new GameObject(L"Sun", pShpere);
	//pObject->isGlobal(true);
	//pObject->SetScale(Vector3::One * 7);
	//pObject->SetParent(ObjectManager::Get().Lights.front());

	// ======================================= Effect =====================================================
	const auto urlEffect = L"../../data/script";
	auto pZBoom = m_pParser->CreateFromParticle(L"ZombieBoom.eff", urlEffect);
	ObjectManager::Get().SetProtoObject(new GameObject(L"ZDead", m_pParser->CreateFromParticle(L"ZombieDead.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"ZDead2", m_pParser->CreateFromParticle(L"ZombieDead2.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"ZDead3", m_pParser->CreateFromParticle(L"ZombieDead3.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"ZBoom", pZBoom, EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"ZAttack", m_pParser->CreateFromParticle(L"ZombieAttack.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"ZAttack2", m_pParser->CreateFromParticle(L"ZombieAttack2.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"ZAttack3", m_pParser->CreateFromParticle(L"ZombieAttack3.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"ZStump", m_pParser->CreateFromParticle(L"Stump.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"ZBreath", m_pParser->CreateFromParticle(L"Breath.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"PDead", m_pParser->CreateFromParticle(L"Boom3.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"PAttack", m_pParser->CreateFromParticle(L"Boom2.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"PSlash", m_pParser->CreateFromParticle(L"Slash.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"PLevelUp", m_pParser->CreateFromParticle(L"LevelUp.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"Fly", m_pParser->CreateFromParticle(L"Fire.eff", urlEffect), EObjType::Effect));
	ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"Fire.eff", urlEffect));
	// ====================================== Item =====================================================
	// ������
	pCollider = new Collider(80.0f);
	pObject = new GameObject(L"Atom", { pCollider, m_pParser->CreateFromParticle(L"Bigbang.eff", urlEffect), new CTransformer(Vector3::Zero, {3.0f, 5.0f, 7.0f, 0.0f}) }, EObjType::Effect);
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	pCollider->CollisionEvent = MyEvent::GiantItem;
	pCollider->m_eTag = ETag::Dummy;
	ObjectManager::Get().SetProtoObject(pObject);

	// �����
	pCollider = new Collider(1.0f);
	pObject = new GameObject(L"PShock", { pCollider, m_pParser->CreateFromParticle(L"Emission.eff", urlEffect), new CTransformer(Vector3::Zero, Quaternion::Zero, Vector3::One * 30.0f) }, EObjType::Effect);
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	pCollider->CollisionEvent = MyEvent::PlayerAttack;
	pCollider->m_eTag = ETag::Dummy;
	ObjectManager::Get().SetProtoObject(pObject);

	// ��ź
	pCollider = new Collider(1.0f);
	pObject = new GameObject(L"PBomb", { pCollider, ObjectManager::Get().TakeComponent(L"Fire"), ObjectManager::GetInstance().TakeComponent(L"RowSphere") });
	//pObject->m_pPhysics->m_damage = 0.5f;
	pObject->SetScale(Vector3::One * 10.0f);
	pCollider->CollisionEvent = MyEvent::PlayerBomb;
	pCollider->m_eTag = ETag::Dummy;
	ObjectManager::Get().SetProtoObject(pObject);

	// ��ź ����
	pCollider = new Collider(60.0f);
	pObject = new GameObject(L"PBoom", { pCollider, pZBoom->clone() }, EObjType::Effect);
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	pCollider->CollisionEvent = MyEvent::DaggerHit;
	pCollider->m_eTag = ETag::Dummy;
	ObjectManager::Get().SetProtoObject(pObject);

	// �ܰ�
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
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// ��
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
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// ======================================= Character =====================================================
	// ��� 
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(Guard);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->SetANIM_Loop(Guard_IDLE);
	pHeroObj->m_myName = L"Guard";
	pHeroObj->m_objType = EObjType::Character;
	pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	pHeroObj->AddComponent(pCollider);
	pCollider->m_eTag = ETag::Ally;
	ObjectManager::Get().SetProtoObject(pHeroObj);
	// ����
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
	pHeroObj->m_pPhysics->UserSocket = ESocketType::EZombie;
	pHeroObj->m_pPhysics->m_damage = 0.08f;
	ObjectManager::Get().SetProtoObject(pHeroObj);
	// ���� Cast
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
	pHeroObj->m_pPhysics->UserSocket = ESocketType::ECaster;
	pHeroObj->m_pPhysics->m_damage = 0.08f;
	ObjectManager::Get().SetProtoObject(pHeroObj);
	// ���� Crawl
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(Zombie);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->SetANIM_Loop(Zombie_CRAWL);
	pHeroObj->m_myName = L"Crawler";
	pHeroObj->m_objType = EObjType::Enemy;
	pCollider = new ColliderOBB({ -13.0f, 0.0f , -40.0f }, { 13.0f, 25.0f , 40.0f });
	pHeroObj->AddComponent({ pCollider, new AIZombieCrawl() });
	pCollider->CollisionEvent = MyEvent::ZombieHit;
	pCollider->m_pPhysics->m_mass = 0.15f;
	pCollider->m_pPhysics->m_damping = 1.5f;
	pCollider->m_eTag = ETag::Enemy;
	pHeroObj->m_pPhysics->UserSocket = ESocketType::ECrawler;
	pHeroObj->m_pPhysics->m_damage = 0.3f;
	ObjectManager::Get().SetProtoObject(pHeroObj);
	// ���� Ex
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ZombieEX);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->SetANIM_Loop(ZombieEX_IDLE);
	pHeroObj->m_myName = L"Mutant";
	pHeroObj->m_objType = EObjType::Enemy;
	pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	pHeroObj->AddComponent({ pCollider, new AIZombieEx() });
	pCollider->CollisionEvent = MyEvent::ZombieHit;
	pCollider->m_pPhysics->m_mass = 0.15f;
	pCollider->m_pPhysics->m_damping = 1.5f;
	pCollider->m_eTag = ETag::Enemy;
	pHeroObj->m_pPhysics->UserSocket = ESocketType::EMutant;
	pHeroObj->m_pPhysics->m_damage = 0.3f;
	ObjectManager::Get().SetProtoObject(pHeroObj);
	// ���� King
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(Zombie_KING);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->SetANIM_Loop(Zombie_KING_IDLE);
	pHeroObj->m_myName = L"Tank";
	pHeroObj->m_objType = EObjType::Enemy;
	pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	pHeroObj->AddComponent({ pCollider, new AIZombieKing() });
	pCollider->CollisionEvent = MyEvent::ZombieHit;
	pCollider->m_pPhysics->m_mass = 0.05f;
	pCollider->m_pPhysics->m_damping = 3.0f;
	pCollider->m_eTag = ETag::Enemy;
	pHeroObj->m_pPhysics->UserSocket = ESocketType::ETank;
	pHeroObj->m_pPhysics->m_damage = 0.4f;
	ObjectManager::Get().SetProtoObject(pHeroObj);
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

		if (!pIntro->m_pPW->GetString().empty())
		{
			// �α����� ó���̰� ��� �Է½� ���� ����
			int retValue = 0;
			if (pIntro->m_loginCheck == 0)
			{
				retValue = pIntro->ConnectMatchingServer();
				pIntro->m_loginCheck = 99;
			}
			if (retValue == 0)
			{
				// �α��� ��û
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
				ErrorMessage("�α��� ����");
				return;
			}
		}
		// ��� ���ų� ���� �����ӽ�
		PacketManager::Get().pMyInfo = new UserInfo();
		PacketManager::Get().pMyInfo->DataSize = (UCHAR)pIntro->m_pID->GetString().size() * 2;
		PacketManager::Get().pMyInfo->DataSize = PacketManager::Get().pMyInfo->DataSize > 12 ? 12 : PacketManager::Get().pMyInfo->DataSize;
		memcpy(PacketManager::Get().pMyInfo->UserID, pIntro->m_pID->GetString().c_str(), PacketManager::Get().pMyInfo->DataSize);
		PacketManager::Get().pMyInfo->UserSocket = 0;
		///
		pIntro->SetScene(ESceneName::Lobby);
		ErrorMessage("���� ������");
	};

	m_pID = (JEditCtrl*)pUIRoot->find_child(L"Login_ID");
	m_pPW = (JEditCtrl*)pUIRoot->find_child(L"Login_PW");

	JButtonCtrl* pBtn = (JButtonCtrl*)pUIRoot->find_child(L"Login_Enter");
	pBtn->EventClick.first = GotoLobby;
	pBtn->EventClick.second = this;

	/// ȸ������
	m_pSignUpID = (JEditCtrl*)pUIRoot->find_child(L"SignUp_ID");
	m_pSignUpPW = (JEditCtrl*)pUIRoot->find_child(L"SignUp_PW");
	m_pSignUpPWCheck = (JEditCtrl*)pUIRoot->find_child(L"SignUp_PWCheck");
	JButtonCtrl* pSignUpEnter = (JButtonCtrl*)pUIRoot->find_child(L"SignUp_Enter");
	pSignUpEnter->EventClick.first = [](void* pVoid) {
		auto pIntro = (IntroScene*)pVoid;

		int retValue = 0;
		if (pIntro->m_loginCheck == 0)
		{
			retValue = pIntro->ConnectMatchingServer();
			pIntro->m_loginCheck = 99;
		}
		if (retValue == 0)
		{
			// �� �ƴϰ� ��� ������ ����
			if (!pIntro->m_pSignUpID->GetString().empty() &&
				!pIntro->m_pSignUpPW->GetString().empty() &&
				pIntro->m_pSignUpPW->GetString() == pIntro->m_pSignUpPWCheck->GetString())
			{
				pIntro->RequestSignUp(pIntro->m_pSignUpID->GetString().c_str(), pIntro->m_pSignUpPW->GetString().c_str());
			}
		}
		ErrorMessage("ȸ������ �õ�");
	};
	pSignUpEnter->EventClick.second = this;

	//SoundManager::Get().SetBGM("SE_Rudy.mp3");
	// ��ʿ����� ���� ���� ��� �ؽ�Ʈ �־������
	// m_pSignUpID = (JTextCtrl*)pUIRoot->find_child(L"Help_txt");
	UI::IntroEvent(pUIRoot);
}
