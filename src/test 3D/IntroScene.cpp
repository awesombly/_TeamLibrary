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
#include "CEventTimer.h"

bool IntroScene::Init() noexcept
{
	ErrorMessage(__FUNCTION__ + " -> Start."s);
	FirstInit();
	// 사운드, UI
	LoadSound();
	LoadUI();
	///
	//SoundManager::Get().Play("bgm_Intro_Theme.mp3");
	SoundManager::Get().Load("bgm_Intro_Theme.mp3", true);
	m_isLoading = false;
	return true;
}


// 프레임
bool IntroScene::Frame() noexcept
{
	// Click
	if (Input::GetKeyState(VK_LBUTTON) == EKeyState::DOWN)
	{
		SoundManager::Get().Play("SE_Click01.mp3");
	}
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
	//SoundManager::Get().Load("bgm_Intro_Theme.mp3");
	SoundManager::Get().Load("bgm_Lobby_Theme.mp3");
	SoundManager::Get().Load("bgm_InGame_Theme.mp3");
	//////////////////////////////////////////////PLAYER_SV
	SoundManager::Get().Load("SV_paladin_atk1.mp3");
	SoundManager::Get().Load("SV_paladin_atk2.mp3");
	SoundManager::Get().Load("SV_paladin_atk3.mp3");
	SoundManager::Get().Load("SV_paladin_atk4.mp3");
	SoundManager::Get().Load("SV_paladin_shout.mp3");
	SoundManager::Get().Load("SV_paladin_lowHP.mp3");
	SoundManager::Get().Load("SV_paladin_dead.mp3");

	SoundManager::Get().Load("SV_archer_atk1.mp3");
	SoundManager::Get().Load("SV_archer_atk2.mp3");
	SoundManager::Get().Load("SV_archer_atk3.mp3");
	SoundManager::Get().Load("SV_archer_atk4.mp3");
	SoundManager::Get().Load("SV_archer_lowHP.mp3");
	SoundManager::Get().Load("SV_archer_dead.mp3");

	SoundManager::Get().Load("SV_mage_atk1.mp3");
	SoundManager::Get().Load("SV_mage_atk2.mp3");
	SoundManager::Get().Load("SV_mage_atk3.mp3");
	SoundManager::Get().Load("SV_mage_atk4.mp3");
	SoundManager::Get().Load("SV_mage_lowHP.mp3");
	SoundManager::Get().Load("SV_mage_dead.mp3");

	//////////////////////////////////////////////MOB_SV
	SoundManager::Get().Load("SV_zombieR_idle1.mp3");
	SoundManager::Get().Load("SV_zombieR_idle2.mp3");
	SoundManager::Get().Load("SV_zombieR_atk1.mp3");
	SoundManager::Get().Load("SV_zombieR_shot.mp3");

	SoundManager::Get().Load("SV_zombieEX_idle.mp3");
	SoundManager::Get().Load("SV_zombieEX_atk1.mp3");
	SoundManager::Get().Load("SV_zombieEX_atk2.mp3");

	SoundManager::Get().Load("SV_zombie_king_idle.mp3");
	SoundManager::Get().Load("SV_zombie_king_attack1.mp3");
	SoundManager::Get().Load("SV_zombie_king_attack2.mp3");
	SoundManager::Get().Load("SV_zombie_king_shout.mp3");

	SoundManager::Get().Load("SV_zombie_dead.mp3");
	///////////////////////////////////////////////SE
	SoundManager::Get().Load("SE_chicken.mp3");
	SoundManager::Get().Load("SE_Click01.mp3");
	SoundManager::Get().Load("SE_dead.mp3");
	SoundManager::Get().Load("SE_bomb.mp3");
	SoundManager::Get().Load("SE_dash.mp3");
	SoundManager::Get().Load("SE_dash2.mp3");
	SoundManager::Get().Load("SE_wave.mp3");
	SoundManager::Get().Load("SE_pickup.mp3");
	
	SoundManager::Get().Load("SE_blacksmith.mp3");
	SoundManager::Get().Load("SE_church.mp3");
	SoundManager::Get().Load("SE_windmill.mp3");
	SoundManager::Get().Load("SE_towerround.mp3");
	SoundManager::Get().Load("SE_levelup.mp3");
	

	SoundManager::Get().Load("SE_fire1.mp3");
	SoundManager::Get().Load("SE_flare_shot.mp3");

	SoundManager::Get().Load("SE_hit_impact_01.mp3");
	SoundManager::Get().Load("SE_hit_impact_02.mp3");

	//paladin
	SoundManager::Get().Load("SE_shildup.mp3");
	SoundManager::Get().Load("SE_Sword_slash1.mp3");
	SoundManager::Get().Load("SE_Sword_slash2.mp3");

	//archer
	SoundManager::Get().Load("SE_bow_ready.mp3");
	SoundManager::Get().Load("SE_bow_shot.mp3");
	SoundManager::Get().Load("SE_dive.mp3");

	//mage
	SoundManager::Get().Load("SE_astro.mp3");
	SoundManager::Get().Load("SE_light.mp3");
	SoundManager::Get().Load("SE_dark.mp3");
	SoundManager::Get().Load("SE_flame.mp3");
	SoundManager::Get().Load("SE_freeze.mp3");
	SoundManager::Get().Load("SE_healing.mp3");
	///
	SoundManager::Get().Load("SE_drink.mp3");
}

bool IntroScene::FirstInit() noexcept
{
	if (m_isFirstInit)
	{
		m_isFirstInit = false;
		// 폰트, 사운드 등
		WriteManager::Get().SetText({ 0, 0 }, L"", D2D1::ColorF::Black, 20, L"휴면둥근헤드라인");
		WriteManager::Get().SetFontSizeAlign(20, EAlign::Center, EAlign::Center);
		// 스프라이트
		ObjectManager::Get().ReadSpriteScript();
		// ===================================== 캐릭터 초기화 ==============================================
		//ErrorMessage(__FUNCTION__ + " -> Character Loading."s);
		I_CHARMGR.Init();
		if (!I_CHARMGR.Load(DxManager::GetDevice(), DxManager::GetDContext(), TablePATH))		//경로 중요함
		{
			return false;
		}
		// ===================================== 오브젝트 초기화 ==============================================
		SetObjects();
		return true;
	}
	return false;
}


void IntroScene::SetObjects() noexcept
{
	GameObject* pObject = nullptr;
	Collider*   pCollider = nullptr;
	// 스카이 박스
	m_pSkyBox = new GameObject(L"SkyBox", { new SkySphere(20, L"SkySphere", L"CubeMap/A_nightsky.dds")/*, new CTransformer(Vector3::Zero, Quaternion::Right * 0.02f, Vector3::Zero)*/ }, EObjType::Dummy);
	m_pSkyBox->SetScale(Vector3::One * 100);
	m_pSkyBox->isGlobal(true);
	// 컴포넌트 등록
	ObjectManager::Get().SetProtoComponent(new RCube(L"Cube", L"None.png"));
	//ObjectManager::Get().SetProtoComponent(new RSphere(20, L"Sphere", L"None.png"));
	ObjectManager::Get().SetProtoComponent(new RSphere(10, L"RowSphere", L"None.png"));
	//ObjectManager::Get().Lights.front()->SetPosition(0.0f, 350.0f, 0.0f);
	// ======================================= Load =====================================================
	const auto urlEffect = L"../../data/script";
	m_pParser = new MaxImporter();
	//ObjectManager::Get().SetProtoObject(new GameObject(L"EZDead", m_pParser->CreateFromParticle(L"ZombieDead.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"EZDead2", m_pParser->CreateFromParticle(L"ZombieDead2.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"EZDead3", m_pParser->CreateFromParticle(L"ZombieDead3.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"EZDead", m_pParser->CreateFromParticle(L"DeadEffect.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"EZHit", m_pParser->CreateFromParticle(L"ZombieAttack.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"EZStump", m_pParser->CreateFromParticle(L"Stump.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"ZBoom", pZBoom, EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"ZAttack2", m_pParser->CreateFromParticle(L"ZombieAttack2.eff", urlEffect), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"ZAttack3", m_pParser->CreateFromParticle(L"ZombieAttack3.eff", urlEffect), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"ZBreath", m_pParser->CreateFromParticle(L"Breath.eff", urlEffect), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"PDead", m_pParser->CreateFromParticle(L"Boom3.eff", urlEffect), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"EHeal", m_pParser->CreateFromParticle(L"Heal.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"EPAttack", m_pParser->CreateFromParticle(L"Boom2.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"EPSlash", m_pParser->CreateFromParticle(L"Slash.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"EPSlash2", m_pParser->CreateFromParticle(L"Slash2.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"EPDust", m_pParser->CreateFromParticle(L"Dust.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"EPDustStay", m_pParser->CreateFromParticle(L"DustStay.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"EHit", m_pParser->CreateFromParticle(L"Hit.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"EHit2", m_pParser->CreateFromParticle(L"Hit2.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"EHit3", m_pParser->CreateFromParticle(L"Hit3.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"EHitLight", m_pParser->CreateFromParticle(L"HitLight.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"ELight", m_pParser->CreateFromParticle(L"Lightning.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"EEmission2", m_pParser->CreateFromParticle(L"Emission2.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"EPLevelUp", m_pParser->CreateFromParticle(L"LevelUp.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	//ObjectManager::Get().SetProtoObject(new GameObject(L"EFly", m_pParser->CreateFromParticle(L"Fire.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"ETeleport", m_pParser->CreateFromParticle(L"Teleport.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"EFire", m_pParser->CreateFromParticle(L"CampFire.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoObject(new GameObject(L"EBerserk", m_pParser->CreateFromParticle(L"Berserk.eff", urlEffect)->SetEffectScale(3.0f), EObjType::Effect));
	ObjectManager::Get().SetProtoComponent(m_pParser->CreateFromParticle(L"Fire.eff", urlEffect)->SetEffectScale(3.0f));

	Renderer* pRerderEnvi = new Renderer(L"EnviR");
	pRerderEnvi->SetEnviromentMap(((Renderer*)m_pSkyBox->GetComponent(EComponent::Renderer))->m_srcName, EEnviType::Refraction);
	Renderer* pRenderer = pRerderEnvi;// new Renderer(L"BaseR");


	// ======================================== Effect =====================================================
	// 플레이어 사망
	pCollider = new Collider(80.0f);
	pObject = new GameObject(L"PDead", { pCollider, m_pParser->CreateFromParticle(L"Boom3.eff", urlEffect)->SetEffectScale(3.0f) }, EObjType::Effect);
	pCollider->CollisionEvent = MyEvent::ForceWave;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	ObjectManager::Get().SetProtoObject(pObject);

	// 좀비 던짐
	pCollider = new Collider(20.0f);
	pObject = new GameObject(L"ZThrow", { pCollider, m_pParser->CreateFromParticle(L"Fire2.eff", urlEffect)->SetEffectScale(3.0f) }, EObjType::Effect);
	pCollider->CollisionEvent = MyEvent::ZombieThrow;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Enemy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	ObjectManager::Get().SetProtoObject(pObject);

	// 좀비 어택
	pCollider = new Collider(40.0f);
	pObject = new GameObject(L"ZAttack", { pCollider, m_pParser->CreateFromParticle(L"ZombieAttack.eff", urlEffect)->SetEffectScale(3.0f) }, EObjType::Effect);
	pCollider->CollisionEvent = MyEvent::ZombieAttack;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Enemy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	ObjectManager::Get().SetProtoObject(pObject);

	// 좀비 어택2
	pCollider = new Collider(50.0f);
	pObject = new GameObject(L"ZAttack2", { pCollider, m_pParser->CreateFromParticle(L"ZombieAttack2.eff", urlEffect)->SetEffectScale(3.0f) }, EObjType::Effect);
	pCollider->CollisionEvent = MyEvent::ZombieAttack;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Enemy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	ObjectManager::Get().SetProtoObject(pObject);

	// 좀비 어택3
	pCollider = new Collider(50.0f);
	pObject = new GameObject(L"ZAttack3", { pCollider, m_pParser->CreateFromParticle(L"ZombieAttack3.eff", urlEffect)->SetEffectScale(3.0f) }, EObjType::Effect);
	pCollider->CollisionEvent = MyEvent::ZombieAttack;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Enemy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	ObjectManager::Get().SetProtoObject(pObject);

	// 점프 어택
	pCollider = new Collider(55.0f);
	pObject = new GameObject(L"EZStump", { pCollider, m_pParser->CreateFromParticle(L"Stump.eff", urlEffect)->SetEffectScale(4.0f) }, EObjType::Effect);
	pCollider->CollisionEvent = MyEvent::ZombieAttack;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Enemy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	ObjectManager::Get().SetProtoObject(pObject);
	
	// 브레스
	pCollider = new ColliderOBB({ -90.0f, -170.0f, -350.0f }, { 90.0f, 170.0f, 350.0f });
	pObject = new GameObject(L"ZBreath", { pCollider, m_pParser->CreateFromParticle(L"Breath.eff", urlEffect)->SetEffectScale(3.0f) }, EObjType::Effect);
	//new GameObject(L"ZBreath", { pCollider }, EObjType::Effect);
	//pCollider->CollisionEvent = MyEvent::TickDamage;
	//pCollider = new ColliderOBB({ -90.0f, -210.0f, -300.0f }, { 90.0f, 120.0f, 300.0f });
	//pObject = new GameObject(L"ZBreath", { m_pParser->CreateFromParticle(L"Breath.eff", urlEffect)->SetEffectScale(3.0f) }, EObjType::Effect);
	//(new GameObject(L"ZC", { pCollider }, EObjType::Effect))->SetParent(pObject);
	//pCollider->m_pParent->SetPosition(Vector3::Forward * 10.0f);
	//pCollider->SetDamage(2.0f);
	pCollider->CollisionEvent = MyEvent::TickDamage;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Enemy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	ObjectManager::Get().SetProtoObject(pObject);

	auto pZBoom = m_pParser->CreateFromParticle(L"ZombieBoom.eff", urlEffect)->SetEffectScale(3.0f);
	// 좀비 붐
	pCollider = new Collider(60.0f);
	pObject = new GameObject(L"ZBoom", { pCollider, pZBoom }, EObjType::Effect);
	pCollider->CollisionEvent = MyEvent::ZombieAttack;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Enemy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	ObjectManager::Get().SetProtoObject(pObject);

	// 충격파
	pCollider = new Collider(1.0f);
	pObject = new GameObject(L"PShock", { pCollider, m_pParser->CreateFromParticle(L"Emission.eff", urlEffect)->SetEffectScale(3.0f), new CTransformer(Vector3::Zero, Quaternion::Zero, Vector3::One * 150.0f) }, EObjType::Effect);
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	pCollider->CollisionEvent = MyEvent::PlayerAttack;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->m_eTagArray[ETag::Ally] = false;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	ObjectManager::Get().SetProtoObject(pObject);

	// 밀리 어택
	pCollider = new Collider(70.0f);
	pObject = new GameObject(L"Melee", { pCollider, m_pParser->CreateFromParticle(L"Hit4.eff", urlEffect)->SetEffectScale(3.0f) }, EObjType::Effect);
	pCollider->CollisionEvent = MyEvent::MeleeHit;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	ObjectManager::Get().SetProtoObject(pObject);

	// 힐폭
	pCollider = new Collider(25.0f);
	auto pParticle = m_pParser->CreateFromParticle(L"Flash.eff", urlEffect)->SetEffectScale(3.0f);
	pParticle->isFollow(true);
	pObject = new GameObject(L"Magic", { pCollider, pParticle }, EObjType::Effect);
	pCollider->CollisionEvent = MyEvent::EnergyBall;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	//pCollider->SetGravityScale(0.5f);
	ObjectManager::Get().SetProtoObject(pObject);

	// 메테오
	pCollider = new Collider(1.1f);
	pParticle = m_pParser->CreateFromParticle(L"HellFire.eff", urlEffect)->SetEffectScale(2.0f);
	auto pSphere = new RSphere(7, L"CMeteor", L"None.png");
	pSphere->SetColor(DxManager::GetDContext(), Color::Red);
	//pParticle->isFollow(true);
	pObject = new GameObject(L"Meteor", { pCollider, pParticle, pSphere }, EObjType::Object);
	pObject->SetScale(Vector3::One * 50.0f);
	pCollider->CollisionEvent = MyEvent::NuclearBoom;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->m_eTagArray[ETag::Ally] = false;
	pCollider->m_eTagArray[ETag::Enemy] = false;
	//pCollider->SetGravityScale(0.5f);
	ObjectManager::Get().SetProtoObject(pObject);

	// 버프 웨이브 
	pCollider = new Collider(1.0f);
	pObject = new GameObject(L"BuffWave", { pCollider, m_pParser->CreateFromParticle(L"Emission2.eff", urlEffect)->SetEffectScale(3.0f), new CTransformer(Vector3::Zero, Quaternion::Zero, Vector3::One * 150.0f) }, EObjType::Effect);
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	pCollider->CollisionEvent = MyEvent::BuffWave;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	ObjectManager::Get().SetProtoObject(pObject);

	// 폭탄 폭발
	pCollider = new Collider(100.0f);
	pObject = new GameObject(L"PBoom", { pCollider, m_pParser->CreateFromParticle(L"RedBoom.eff", urlEffect)->SetEffectScale(3.0f) }, EObjType::Effect);
	pCollider->SetGravityScale(0.0f);
	pCollider->usePhysics(false);
	pCollider->CollisionEvent = MyEvent::MeleeHit;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	ObjectManager::Get().SetProtoObject(pObject);
	// ========================================== 오브젝트 ==================================================================

	// 템 상자
	auto pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_Box);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"ItemBox";
	pHeroObj->m_objType = EObjType::AObject;
	pHeroObj->SetScale(Vector3::One * 0.9f);
	pCollider = new Collider(13.0f);
	//pCollider = new ColliderOBB(-Vector3::One, Vector3::One);
	pHeroObj->AddComponent({ pCollider, pRenderer, new CTransformer(Vector3::Zero, Quaternion::Left * 3.0f) });
	pHeroObj->SetHP(10000.0f);
	pHeroObj->m_pPhysics->m_repulsion = 1.3f;
	pHeroObj->SetGravityScale(2.0f);
	pCollider->CollisionEvent = MyEvent::ItemBox;
	pCollider->m_eTag = ETag::Enemy;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 폭탄
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_Grenade);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"PBomb";
	pHeroObj->m_objType = EObjType::AObject;
	pHeroObj->SetScale(Vector3::One * 2.5f);
	pCollider = new Collider(17.0f);
	pHeroObj->AddComponent({ pCollider, pRenderer, ObjectManager::Get().TakeComponent(L"Fire") });
	pCollider->m_pivot = Vector3::Up * 6.0f;
	pCollider->CollisionEvent = MyEvent::PlayerBomb;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 시한폭탄
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_Bomb);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"TimeBomb";
	pHeroObj->m_objType = EObjType::AObject;
	pCollider = new Collider(5.0f);
	pHeroObj->SetScale(Vector3::One * 3.0f);
	auto pTimer = new CEventTimer(3.0f);
	pTimer->TimerEvent = { TimeEvent::TimeBomb, nullptr };
	pHeroObj->AddComponent({ pCollider, pRenderer, pTimer, ObjectManager::Get().TakeComponent(L"Fire") });
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Collider] = false;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 충격 폭탄
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_Chicken);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"ShockBomb";
	pHeroObj->m_objType = EObjType::AObject;
	pCollider = new Collider(28.0f);
	pHeroObj->SetScale(Vector3::One * 1.2f);
	pHeroObj->AddComponent({ pCollider, pRenderer });
	pCollider->m_eTag = ETag::Enemy;
	pCollider->m_eTagArray[ETag::Collider] = false;
	//pCollider->CollisionEvent = MyEvent::ShockBoom;
	pHeroObj->m_pPhysics->DeadEvent	 = DyingEvent::ShockBoomDead;
	pHeroObj->m_pPhysics->UserSocket = ESocketType::EDummy;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 지뢰
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_Mine);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"Mine";
	pHeroObj->m_objType = EObjType::AObject;
	pCollider = new Collider(15.0f);
	pHeroObj->SetScale(Vector3::One * 3.0f);
	pHeroObj->AddComponent({ pCollider, pRenderer });
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->CollisionEvent = MyEvent::MineBoom;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 미사일
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_Missile);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"Missile";
	pHeroObj->m_objType = EObjType::AObject;
	pHeroObj->SetScale(Vector3::One * 0.3f);
	pCollider = new Collider(15.0f);
	pTimer = new CEventTimer(1.2f);
	pTimer->TimerEvent = { TimeEvent::MissileShot, (void*)&Vector3::Zero };
	pHeroObj->AddComponent({pCollider, pRenderer, pTimer});
	pCollider->m_pivot = Vector3::Up * 8.0f;
	pCollider->SetGravityScale(0.2f);
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Ally] = false;
	pCollider->CollisionEvent = MyEvent::MissileCollision;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 핵 투하
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_Bird);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"SkyShip";
	pHeroObj->m_objType = EObjType::AObject;
	pHeroObj->SetScale(Vector3::One * 180.0f);
	pCollider = new Collider(0.0001f);
	pTimer = new CEventTimer(1.5f);
	pTimer->TimerEvent = { TimeEvent::NuclearLaunch, nullptr };
	pHeroObj->AddComponent({ pCollider, pRenderer, pTimer, new CEventTimer(6.0f) });
	//pCollider->m_pivot = Vector3::Up * 8.0f;
	pCollider->SetGravityScale(0.0f);
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Ally] = false;
	pCollider->m_eTagArray[ETag::Enemy] = false;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 핵
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_Missile);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"Nuclear";
	pHeroObj->m_objType = EObjType::AObject;
	pCollider = new Collider(10.0f);
	pHeroObj->AddComponent({ pCollider, pRenderer });
	pCollider->m_pivot = Vector3::Down * 200.0f;
	//pCollider->SetGravityScale(0.0f);
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->CollisionEvent = MyEvent::NuclearBoom;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 대거
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_Dagger);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"Dagger";
	pHeroObj->m_objType = EObjType::AObject;
	pCollider = new Collider(25.0f);
	pHeroObj->SetScale(Vector3::One * 0.9f);
	pHeroObj->AddComponent({ pCollider, pRenderer });
	pCollider->m_pivot = Vector3::Up * 8.0f + Vector3::Forward * 3.0f;
	//pCollider->SetGravityScale(0.5f);
	pCollider->CollisionEvent = MyEvent::DaggerHit;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Ally] = false;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	///
	// 화살
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_ARROW);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"Arrow";
	pHeroObj->m_objType = EObjType::AObject;
	pCollider = new Collider(3.5f);
	pHeroObj->AddComponent({ pCollider, pRenderer, ObjectManager::Get().TakeComponent(L"Fire") });
	//pCollider->m_pivot = Vector3::Up * 6.0f + Vector3::Forward * 2.5f;
	pCollider->SetGravityScale(0.5f);
	pCollider->CollisionEvent = MyEvent::OneTimeHit;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 관통 화살
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_ARROW);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"ArrowP";
	pHeroObj->m_objType = EObjType::AObject;
	pCollider = new Collider(4.0f);
	pHeroObj->AddComponent({ pCollider, pRenderer, new CEventTimer(3.0f), ObjectManager::Get().TakeComponent(L"Fire") });
	//pCollider->m_pivot = Vector3::Up * 6.0f + Vector3::Forward * 2.5f;
	pCollider->SetGravityScale(0.5f);
	//pCollider->usePhysics(false);
	pCollider->CollisionEvent = MyEvent::PiercingHit;
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 화살비
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_Bomb);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"ArrowR";
	pHeroObj->m_objType = EObjType::AObject;
	pCollider = new Collider(1.0f);
	pHeroObj->SetScale(Vector3::One * 3.0f);
	pTimer = new CEventTimer(1.3f);
	pTimer->TimerEvent = { TimeEvent::ArrowRain, nullptr };
	pHeroObj->AddComponent({ pCollider, pRenderer, pTimer });
	pCollider->m_eTag = ETag::Dummy;
	pCollider->m_eTagArray[ETag::Collider] = false;
	pCollider->m_eTagArray[ETag::Dummy] = false;
	pCollider->m_eTagArray[ETag::Enemy] = false;
	pCollider->m_eTagArray[ETag::Ally] = false;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 폭약통
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ITEM_BarrelRed);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->m_myName = L"Explosive";
	pHeroObj->m_objType = EObjType::AObject;
	pCollider = new ColliderOBB({ -18.0f, -20.0f, -18.0f }, { 18.0f, 20.0f, 18.0f });
	pHeroObj->AddComponent({ pCollider, pRenderer });
	//pCollider->m_pivot = Vector3::Up * 4.0f + Vector3::Forward * 2.5f;
	//pCollider->CollisionEvent = MyEvent::ZombieThrow;
	pHeroObj->m_pPhysics->DeadEvent = DyingEvent::BarrelDead;
	pCollider->m_eTag = ETag::Enemy;
	//pCollider->m_pPhysics->m_mass = 0.15f;
	pCollider->m_pPhysics->m_damping = 1.5f;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// ======================================= Character =====================================================
	//I_MAPMGR.Init();
	//I_MAPMGR.Load(DxManager::GetDevice(), DxManager::GetDContext(), L"../../Data/MAPOBJ/OBJTable.cit");
	//I_MAPMGR.GetPtr(L"test")->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	//ObjectManager::Get().PushObject(I_MAPMGR.GetPtr(L"test"));
	//AMapObj* test = (AMapObj*)I_MAPMGR.GetPtr(L"test")->clone();
	//test->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	//test->m_myName = L"MObj";
	//test->m_objType = EObjType::Character;
	//test->SetPositionY(150.0f);
	//test->SetScale(1.0f, 1.0f, 1.0f);
	//test->AddComponent( pRenderer);
	////ObjectManager::Get().PushObject(test);
	//ObjectManager::Get().SetProtoObject(test);

	// 기사 
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(Paladin);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->SetANIM_Loop(Paladin_IDLE);
	pHeroObj->m_myName = L"Paladin";
	pHeroObj->m_objType = EObjType::Character;
	pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	pHeroObj->AddComponent({ pCollider, pRerderEnvi });
	pCollider->m_eTag = ETag::Ally;
	ObjectManager::Get().SetProtoObject(pHeroObj);
	
	// 궁수 
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(Archer);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->SetANIM_Loop(Archer_IDLE);
	pHeroObj->m_myName = L"Archer";
	pHeroObj->m_objType = EObjType::Character;
	pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	pHeroObj->AddComponent({ pCollider, pRerderEnvi });
	pCollider->m_eTag = ETag::Ally;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 법사 
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(Mage);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->SetANIM_Loop(Mage_IDLE);
	pHeroObj->m_myName = L"Mage";
	pHeroObj->m_objType = EObjType::Character;
	pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	pHeroObj->AddComponent({ pCollider, pRerderEnvi });
	pCollider->m_eTag = ETag::Ally;
	ObjectManager::Get().SetProtoObject(pHeroObj);

	// 좀비
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ZombieR);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->SetANIM_Loop(ZombieR_IDLE);
	pHeroObj->m_myName = L"Zombie";
	pHeroObj->m_objType = EObjType::Enemy;
	pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	pHeroObj->AddComponent({ pCollider, pRerderEnvi, new AIZombie() });
	pCollider->CollisionEvent = MyEvent::ZombieHit;
	pCollider->m_eTag = ETag::Enemy;
	pHeroObj->m_pPhysics->DeadEvent = DyingEvent::ZombieDead;
	pHeroObj->m_pPhysics->UserSocket = ESocketType::EZombie;
	pHeroObj->m_pPhysics->m_damage = 0.08f;
	ObjectManager::Get().SetProtoObject(pHeroObj);
	// 좀비 Cast
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ZombieR);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->SetANIM_Loop(ZombieR_IDLE);
	pHeroObj->m_myName = L"Caster";
	pHeroObj->m_objType = EObjType::Enemy;
	pCollider = new ColliderOBB({ -13.0f, 0.0f , -13.0f }, { 13.0f, 80.0f , 13.0f });
	pHeroObj->AddComponent({ pCollider, pRerderEnvi, new AIZombieCast() });
	pCollider->CollisionEvent = MyEvent::ZombieHit;
	pCollider->m_eTag = ETag::Enemy;
	pHeroObj->m_pPhysics->DeadEvent = DyingEvent::ZombieDead;
	pHeroObj->m_pPhysics->UserSocket = ESocketType::ECaster;
	pHeroObj->m_pPhysics->m_damage = 0.08f;
	ObjectManager::Get().SetProtoObject(pHeroObj);
	// 좀비 Crawl
	pHeroObj = new AHeroObj();
	pHeroObj->SetPlayerCharacter(ZombieR);
	pHeroObj->SetMatrix(0, &ObjectManager::Get().Cameras[ECamera::Main]->m_matView, &ObjectManager::Get().Cameras[ECamera::Main]->m_matProj);
	pHeroObj->SetANIM_Loop(ZombieR_CRAWL);
	pHeroObj->m_myName = L"Crawler";
	pHeroObj->m_objType = EObjType::Enemy;
	pCollider = new ColliderOBB({ -13.0f, 0.0f , -40.0f }, { 13.0f, 25.0f , 40.0f });
	pHeroObj->AddComponent({ pCollider, pRerderEnvi, new AIZombieCrawl() });
	pCollider->CollisionEvent = MyEvent::ZombieHit;
	//pCollider->m_pPhysics->m_mass = 0.15f;
	pCollider->SetGravityScale(15.0f);
	pCollider->m_pPhysics->m_damping = 1.5f;
	pCollider->m_eTag = ETag::Enemy;
	pHeroObj->m_pPhysics->DeadEvent = DyingEvent::ZombieCrawlDead;
	pHeroObj->m_pPhysics->UserSocket = ESocketType::ECrawler;
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
	pHeroObj->AddComponent({ pCollider, pRerderEnvi, new AIZombieEx() });
	pCollider->CollisionEvent = MyEvent::ZombieHit;
	//pCollider->m_pPhysics->m_mass = 0.15f;
	pCollider->SetGravityScale(15.0f);
	pCollider->m_pPhysics->m_damping = 1.5f;
	pCollider->m_eTag = ETag::Enemy;
	pHeroObj->m_pPhysics->DeadEvent = DyingEvent::ZombieExDead;
	pHeroObj->m_pPhysics->UserSocket = ESocketType::EMutant;
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
	pHeroObj->AddComponent({ pCollider, pRerderEnvi, new AIZombieKing() });
	pCollider->CollisionEvent = MyEvent::ZombieHit;
	//pCollider->m_pPhysics->m_mass = 0.05f;
	pCollider->SetGravityScale(100.0f);
	pCollider->m_pPhysics->m_damping = 3.0f;
	pCollider->m_eTag = ETag::Enemy;
	pHeroObj->m_pPhysics->DeadEvent = DyingEvent::ZombieKingDead;
	pHeroObj->m_pPhysics->UserSocket = ESocketType::ETank;
	pHeroObj->m_pPhysics->m_damage = 0.4f;
	ObjectManager::Get().SetProtoObject(pHeroObj);
}


void GotoLobby2(void* pScene)
{
	IntroScene* pIntro = (IntroScene*)pScene;

	if (!pIntro->m_pPW->GetString().empty())
	{
		// 로그인이 처음이고 비번 입력시 서버 연결
		int retValue = 0;
		if (pIntro->m_loginCheck == 0)
		{
			ErrorMessage(__FUNCTION__ + "서버 접속"s);
			retValue = pIntro->ConnectMatchingServer();
			pIntro->m_loginCheck = 99;
		}
		if (retValue == 0)
		{
			// 로그인 요청
			pIntro->m_loginCheck = 0;
			pIntro->RequestSignIn(pIntro->m_pID->GetString().c_str(), pIntro->m_pPW->GetString().c_str());
			while (pIntro->m_loginCheck == 0)
			{
				ErrorMessage(__FUNCTION__ + " -> 로그인 루프"s);
				if (pIntro->m_loginCheck == 1)
				{
					ErrorMessage(__FUNCTION__ + " -> 로그인 성공"s);
					PacketManager::Get().pMyInfo = new UserInfo();
					PacketManager::Get().pMyInfo->DataSize = (UCHAR)pIntro->m_pID->GetString().size() * 2;
					PacketManager::Get().pMyInfo->DataSize = PacketManager::Get().pMyInfo->DataSize > 12 ? 12 : PacketManager::Get().pMyInfo->DataSize;
					memcpy(PacketManager::Get().pMyInfo->UserID, pIntro->m_pID->GetString().c_str(), PacketManager::Get().pMyInfo->DataSize);
					PacketManager::Get().pMyInfo->UserSocket = 0;
					///
					pIntro->SetScene(ESceneName::Lobby);
					return;
				}
				if (pIntro->m_loginCheck == -1)
				{
					pIntro->m_pHelpText->SetString(L"로그인에 실패 하였습니다..");
					pIntro->m_pHelpText->m_bRender = true;
					ErrorMessage(__FUNCTION__ + " -> 로그인 실패"s);
					return;
				}
			}
		}
		else
		{
			pIntro->m_pHelpText->SetString(L"서버 접속에 실패하였습니다.");
			pIntro->m_pHelpText->m_bRender = true;
		}
	}
	// 비번 없거나 서버 미접속시
	PacketManager::Get().pMyInfo = new UserInfo();
	PacketManager::Get().pMyInfo->DataSize = (UCHAR)pIntro->m_pID->GetString().size() * 2;
	PacketManager::Get().pMyInfo->DataSize = PacketManager::Get().pMyInfo->DataSize > 12 ? 12 : PacketManager::Get().pMyInfo->DataSize;
	memcpy(PacketManager::Get().pMyInfo->UserID, pIntro->m_pID->GetString().c_str(), PacketManager::Get().pMyInfo->DataSize);
	PacketManager::Get().pMyInfo->UserSocket = 0;
	///
	pIntro->SetScene(ESceneName::Lobby);
	ErrorMessage(__FUNCTION__ + " -> 서버 미접속"s);
}

void IntroScene::LoadUI() noexcept
{
	//ErrorMessage(__FUNCTION__ + " -> Start."s);
	JPanel* pUIRoot = new JPanel(L"Intro");
	pUIRoot->m_objType = EObjType::UI;
	JParser par;
	par.FileLoad(DxManager::GetDevice(), L"../../data/ui/Intro", *pUIRoot);
	ObjectManager::Get().PushObject(pUIRoot);

	m_pID = (JEditCtrl*)pUIRoot->find_child(L"Login_ID");
	m_pPW = (JEditCtrl*)pUIRoot->find_child(L"Login_PW");

	JButtonCtrl* pBtn = (JButtonCtrl*)pUIRoot->find_child(L"Login_Enter");
	pBtn->EventClick.first = GotoLobby2;
	pBtn->EventClick.second = this;

	/// 회원가입
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
			// 널 아니고 비번 같으면 가입
			if (!pIntro->m_pSignUpID->GetString().empty() &&
				!pIntro->m_pSignUpPW->GetString().empty() &&
				pIntro->m_pSignUpPW->GetString() == pIntro->m_pSignUpPWCheck->GetString())
			{
				pIntro->RequestSignUp(pIntro->m_pSignUpID->GetString().c_str(), pIntro->m_pSignUpPW->GetString().c_str());
			}
			else
			{
				pIntro->m_pHelpText->SetString(L"잘못 입력된 값이 있습니다.");
				pIntro->m_pHelpText->m_bRender = true;
			}
		}
		ErrorMessage("회원가입 시도");
	};
	pSignUpEnter->EventClick.second = this;
	// 알림창
	m_pHelpText = (JTextCtrl*)pUIRoot->find_child(L"Help_txt");

	UI::IntroEvent(pUIRoot);
	UI::Loading(pUIRoot);
}