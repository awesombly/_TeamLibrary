#include "PlayerController.h"
#include "AHeroObj.h"
#include "ColliderOBB.h"
#include "ObjectManager.h"
#include "PacketManager.h"
#include "SoundManager.h"
#include "EventManager.h"
#include "CEventTimer.h"
#include "AIZombie.h"
#include "UIManager.h"

bool PlayerController::Init() noexcept
{
	GameObject::Init();
	if (pUIManager == nullptr)
	{
		pUIManager = &UIManager::Get();
	}
	auto& ItemIndex = JItem::Get()->m_pItemList;
	// ≈€ º≥¡§
	// ShockWave
	m_ItemList[ItemIndex[L"Bomb1"]] = ActiveEvent::ThrowNuclear;
	m_ItemList[ItemIndex[L"Bomb2"]] = ActiveEvent::ThrowShockBoom;
	m_ItemList[ItemIndex[L"Bomb3"]] = ActiveEvent::ThrowBomb;
	m_ItemList[ItemIndex[L"LandMine"]] = ActiveEvent::ThrowTimeBomb;
	m_ItemList[ItemIndex[L"Barge"]] = ActiveEvent::ThrowBarricade;
	m_ItemList[ItemIndex[L"Missile"]] = ActiveEvent::ThrowMissile;
	m_ItemList[ItemIndex[L"Nuclear"]] = ActiveEvent::ThrowMine;
	m_ItemList[ItemIndex[L"TNT"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Potion"]] = ActiveEvent::UsePotion;
	return true;
}

bool PlayerController::Frame(const float& spf, const float& accTime)	noexcept
{
	GameObject::Frame(spf, accTime);
	m_curMP = min(m_curMP + spf * m_RegenMP, m_maxMP);
	

	if (!pUIManager->m_pChat->m_bRender &&
		m_pParent != nullptr)
	{
		m_pParent->HealHP(spf * m_RegenHP);
		// HP, MP πŸ
		m_pParent->m_pPhysics->m_disHP = max<float>(m_pParent->m_pPhysics->m_disHP - spf * m_pParent->m_pPhysics->m_maxHP, m_pParent->GetHP());
		m_disMP = max<float>(m_disMP - spf * m_maxMP, m_curMP);

		PlayerInput(spf);
		CheckTownCollision();
		if (pUIManager->m_pMouseIcon->m_bRender)
		{
			UpdateStatus();
		}
		else
		{
			CameraInput(spf);

			// √ ±‚»≠
			if (Input::GetKeyState('R') == EKeyState::DOWN)
			{
				ResetOption();
			}
			if (Input::GetKeyState(VK_SUBTRACT) == EKeyState::DOWN)
			{
				m_curDelayRespawn = -9999.9f;
				CutParent(false, true);
				ObjectManager::Cameras[ECamera::Main]->CutParent(false, true);
			}
		}
	}
	else if (m_pParent == nullptr)
	{
		// ∏ÆΩ∫∆˘
		m_curDelayRespawn += spf;
		if (m_curDelayRespawn >= m_DelayRespawn)
		{
			SendReqRespawn(m_curCharacter);
			return true;
		}
	}

	// ¿˚ √º∑¬πŸ
	if (pUIManager->m_pEnemyPanel->m_bRender)
	{
		m_curDelayEnemyPanel -= spf;
		if (m_curDelayEnemyPanel <= 0.0f)
		{
			pUIManager->m_pEnemyPanel->m_bRender = false;
		}
		//else if (m_pTargetEnemy->m_pPhysics->m_disHP > m_pTargetEnemy->GetHP())
		//{
		m_pTargetEnemy->m_pPhysics->m_disHP = max<float>(m_pTargetEnemy->m_pPhysics->m_disHP - spf * 0.5f * m_pTargetEnemy->m_pPhysics->m_maxHP, m_pTargetEnemy->GetHP());
		pUIManager->m_pEnemyHPText->SetString(to_wstring((int)(m_pTargetEnemy->GetHP() * 100.0f)) + L" / " + to_wstring((int)(m_pTargetEnemy->m_pPhysics->m_maxHP * 100.0f)));
		//}
	}
	// ∞Êƒ°πŸ
	//if (m_disEXP < m_EXP)
	//{
	m_disEXP = min<float>(m_disEXP + spf * 0.4f, m_EXP);
	//}
	return true;
}


bool PlayerController::Render(ID3D11DeviceContext* pDContext) noexcept
{
	GameObject::Render(pDContext);
	return true;
}

bool PlayerController::Release() noexcept
{
	GameObject::Release();
	return true;
}


void PlayerController::SetAnim(AHeroObj* pObject, const UINT& socket, const ECharacter& eCharacter, const EAction& eAction, const D3DXVECTOR3& forward) noexcept
{
	static D3DXVECTOR3 missileTarget;
	pObject->SetHeroAnimSpeed(PacketManager::Get().UserList[socket]->MotionRate);
	switch (eCharacter)
	{
		// ==================================== ∆»∂ÛµÚ =======================================
	case ECharacter::EGuard:
	{
		switch (eAction)
		{
		case EAction::Idle:
		{
			pObject->SetANIM_Loop(Paladin_IDLE);
		}	break;
		case EAction::Left:
		case EAction::BackwardLeft:
		{
			pObject->SetANIM_Loop(Paladin_LEFT);
		}	break;
		case EAction::Right:
		case EAction::BackwardRight:
		{
			pObject->SetANIM_Loop(Paladin_RIGHT);
		}	break;
		case EAction::Forward:
		case EAction::ForwardLeft:
		case EAction::ForwardRight:
		{
			pObject->SetANIM_Loop(Paladin_WALK);
		}	break;
		case EAction::Backward:
		{
			pObject->SetANIM_Loop(Paladin_BACK);
		}	break;
		case EAction::Run:
		{
			SoundManager::Get().PlayQueue("SE_dash2.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			pObject->SetANIM_Loop(Paladin_RUN);
		}	break;
		case EAction::LSkill:
		{
			pObject->SetANIM_Loop(Paladin_ATTACK);
			pObject->SetGravityScale(50.0f);
		}	break;
		case EAction::LCharge1:
		{
			pObject->SetANIM_Loop(Paladin_SWING);
		}	break;
		case EAction::LCharge2:
		{
			pObject->SetANIM_Loop(Paladin_SLASH);
		}	break;
		case EAction::Attack:
		{
			auto pItem = ObjectManager::Get().TakeObject(L"Melee");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 60.0f + pObject->GetUp() * 45.0f);
			pItem->SetScale(pObject->GetScale());
			pItem->m_pPhysics->UserSocket = socket;
			pItem->SetDamage(0.4f * PacketManager::Get().UserList[socket]->AttackRate);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());

			SoundManager::Get().PlayVariation("SV_paladin_atk", false, 4);
			SoundManager::Get().PlayQueue("SE_Sword_slash1.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::ChargeAttack:
		{
			auto pItem = ObjectManager::Get().TakeObject(L"Melee");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 60.0f + pObject->GetUp() * 45.0f);
			pItem->SetScale(pObject->GetScale());
			pItem->m_pPhysics->UserSocket = socket;
			pItem->SetDamage(0.6f * PacketManager::Get().UserList[socket]->AttackRate);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());

			SoundManager::Get().PlayVariation("SV_paladin_atk", false, 4);
			SoundManager::Get().PlayQueue("SE_Sword_slash1.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::ChargeAttack2:
		{
			auto pItem = ObjectManager::Get().TakeObject(L"Melee");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 60.0f + pObject->GetUp() * 45.0f);
			pItem->SetScale(pObject->GetScale());
			pItem->m_pPhysics->UserSocket = socket;
			pItem->SetDamage(0.8f * PacketManager::Get().UserList[socket]->AttackRate);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());

			SoundManager::Get().PlayVariation("SV_paladin_atk", false, 4);
			SoundManager::Get().PlayQueue("SE_Sword_slash1.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::Wait:
		{
			pObject->SetGravityScale(pObject->GetScale().x * 1.5f);
		}	break;
		case EAction::RSkill:
		{
			pObject->SetANIM_Loop(Paladin_POWERUP);
		}	break;
		case EAction::Special2:
		{
			// ±§»≠
			SoundManager::Get().PlayQueue("SV_paladin_shout.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			auto pItem = ObjectManager::Get().TakeObject(L"EBerserk");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 60.0f);

			pObject->SetDamage(0.2f * PacketManager::Get().UserList[socket]->AttackRate);
			pObject->GetCollider()->CollisionEvent = MyEvent::BerserkMode;

			pItem = ObjectManager::Get().TakeObject(L"EFire");
			pItem->SetParent(pObject);
			///
			PacketManager::Get().UserList[socket]->MotionRate += 0.65f;
			if (PlayerController::Get().GetParent() == pObject)
			{
				PlayerController::Get().m_pEffectBerserk = pItem;
				PlayerController::Get().m_motionBuff = 0.65f;
				PlayerController::Get().m_cooltimeBuff = 2;
			}
		}	break;
		case EAction::Special3:
		{
			// ±§»≠ ¡æ∑·
			pObject->SetDamage(0.0f, 0);
			pObject->GetCollider()->CollisionEvent = nullptr;
			///
			PacketManager::Get().UserList[socket]->MotionRate -= 0.65f;
			if (PlayerController::Get().GetParent() == pObject)
			{
				if(PlayerController::Get().m_pEffectBerserk->isEnable())
					ObjectManager::Get().DisableObject(PlayerController::Get().m_pEffectBerserk);
				PlayerController::Get().m_pEffectBerserk = nullptr;
				PlayerController::Get().m_motionBuff = 0.0f;
				PlayerController::Get().m_cooltimeBuff = 0;
			}
		}	break;
		case Special:
		{
			pObject->SetANIM_Loop(Paladin_BLOCK);
			SoundManager::Get().PlayQueue("SE_shildup.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);

			// ¿Ã∆Â
			auto pItem = ObjectManager::Get().TakeObject(L"EHit");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 30.0f);

			for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Enemy))
			{
				if (auto pController = iter->GetComponent(EComponent::Etc);
					pController != nullptr)
				{
					((AIZombie*)pController)->m_Target = pObject->GetPosition();
				}
			}
		}	break;
		// ================================== ≈€ ªÁøÎ =========================================
		case EAction::ShockWave:
		{
			// √Ê∞›∆ƒ
			pObject->SetANIM_Loop(Paladin_POWERUP);
			auto pItem = ObjectManager::Get().TakeObject(L"PShock");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 100.0f);
			pItem->SetScale(Vector3::One);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->m_pPhysics->m_damage = 0.5f;
			SoundManager::Get().PlayQueue("SV_paladin_atk3.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			//SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::ThrowBomb:
		{
			// ∆¯≈∫
			pObject->SetANIM_Loop(Paladin_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"PBomb");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
			pItem->SetForce((forward * 0.6f + Vector3::Up) * 300.0f);
			pItem->m_pPhysics->m_damage = 0.6f;
			pItem->m_pPhysics->UserSocket = socket;
			//pItem->SetDamage(1.0f, PacketManager::Get().UserList[socket]->StatLuk);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());

			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::ITimeBomb:
		{
			// Ω√«—∆¯≈∫
			pObject->SetANIM_Loop(Paladin_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"TimeBomb");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 50.0f + pObject->GetRight() * 20.0f);
			pItem->SetForce(forward * 90.0f);
			pItem->m_pPhysics->m_damage = 0.7f;
			pItem->m_pPhysics->UserSocket = socket;

			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::IShockBomb:
		{
			// √Ê∞›∆¯≈∫
			pObject->SetANIM_Loop(Paladin_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"ShockBomb");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 50.0f + pObject->GetRight() * 20.0f);
			pItem->SetForce(forward * 350.0f);
			//pItem->m_pPhysics->UserSocket = socket;
			pItem->SetHP(0.3f);
			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::IMine:
		{
			// ¡ˆ∑⁄
			pObject->SetANIM_Loop(Paladin_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"Mine");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 45.0f + pObject->GetUp() * 50.0f);
			pItem->SetForce(forward * 30.0f);
			pItem->m_pPhysics->m_damage = 0.8f;
			pItem->m_pPhysics->UserSocket = socket;
			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::IMissile:
		{
			// πÃªÁ¿œ
			pObject->SetANIM_Loop(Paladin_POWERUP);
			missileTarget = forward * 600.0f;
			for (int i = 0; i < 13; ++i)
			{
				auto pItem = ObjectManager::Get().TakeObject(L"Missile");
				pItem->SetPosition(pObject->GetPosition() + pObject->GetBackward() * 60.0f + pObject->GetUp() * 55.0f);
				pItem->SetRotation(pObject->GetRotation() + Quaternion::Down * (RandomNormal() * 0.3f + 1.42f));
				pItem->SetForce({ RandomNormal() * 360.0f - 180.0f, RandomNormal() * 210.0f - 30.0f, (RandomNormal() * 200.0f + 60.0f) * pObject->GetBackward().z });
				pItem->m_pPhysics->UserSocket = socket;
				pItem->m_pPhysics->m_damage = 0.3f;
				((CEventTimer*)pItem->GetComponent(EComponent::Timer))->m_EventDelay = RandomNormal() * 1.0f + 0.5f;
				((CEventTimer*)pItem->GetComponent(EComponent::Timer))->TimerEvent = { TimeEvent::MissileShot, (void*)&missileTarget };
			}
			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::INuclear:
		{
			// «Ÿ
			pObject->SetANIM_Loop(Paladin_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"SkyShip");
			pItem->SetPosition(pObject->GetPosition() + Vector3::Up * 800.0f + pObject->GetBackward() * 450.0f);
			pItem->SetRotation(pObject->GetRotation());
			pItem->SetDirectionForce(pObject->GetForward() * 480.0f);
			pItem->m_pPhysics->UserSocket = socket;
			SoundManager::Get().PlayQueue("SV_paladin_atk2.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			//SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::IPotion:
		{
			// π∞æ‡
			pObject->SetANIM_Loop(Paladin_THROW);
			pObject->HealHP(pObject->GetHP() * 0.5f);
			SoundManager::Get().Play("SE_drink.mp3");
		}	break;
		case EAction::IBarricade:
		{
			// πŸ∏Æƒ…¿ÃµÂ
			pObject->SetANIM_Loop(Paladin_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"Barricade", false);
			pItem->m_objType = EObjType::Character;
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 45.0f + pObject->GetUp() * 50.0f);
			pItem->SetScale(4.0f, 1.5f, 4.0f);
			pItem->SetRotation(pObject->GetRotation() + Quaternion::Left * PI * 0.5f);
			auto pCollider = new ColliderOBB({ -7.0f, 0.0f, -25.0f }, {7.0f, 50.0f, 25.0f});
			pCollider->m_eTag = ETag::Ally;
			pItem->AddComponent(pCollider);
			//pCollider->usePhysics(false);
			pItem->SetGravityScale(10.0f);
			pItem->m_pPhysics->m_mass = 0.1f;
			pItem->SetHP(10.0f);
			pItem->SetForce(forward * 50.0f);
			//pItem->m_pPhysics->m_damage = 0.8f;
			pItem->m_pPhysics->UserSocket = socket;
			ObjectManager::Get().PushObject(pItem);
			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		}
	}	break;
	// ==================================== æ∆√≥ =======================================
	case ECharacter::EArcher:
	{
		switch (eAction)
		{
		case EAction::Idle:
		{
			pObject->SetANIM_Loop(Archer_IDLE);
		}	break;
		case EAction::Left:
		case EAction::BackwardLeft:
		{
			pObject->SetANIM_Loop(Archer_LEFT);
		}	break;
		case EAction::Right:
		case EAction::BackwardRight:
		{
			pObject->SetANIM_Loop(Archer_RIGHT);
		}	break;
		case EAction::Forward:
		case EAction::ForwardLeft:
		case EAction::ForwardRight:
		{
			pObject->SetANIM_Loop(Archer_WALK);
		}	break;
		case EAction::Backward:
		{
			pObject->SetANIM_Loop(Archer_BACK);
		}	break;
		case EAction::LSkill:
		{
			pObject->SetANIM_Loop(Archer_AIM_READY);
		}	break;
		case EAction::LCharging:
		{
			SoundManager::Get().PlayQueue("SE_bow_ready.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			pObject->SetANIM_Loop(Archer_AIM_IDLE);
		}	break;
		case EAction::LCharge1:
		{
			SoundManager::Get().PlayQueue("SE_bow_ready.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			///SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			auto pItem = ObjectManager::Get().TakeObject(L"EPSlash");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 55.0f);
		}	break;
		case EAction::LCharge2:
		{
			SoundManager::Get().PlayQueue("SE_bow_ready.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			auto pItem = ObjectManager::Get().TakeObject(L"EPAttack");
			//SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 55.0f);
		}	break;
		case EAction::Attack:
		{
			pObject->SetANIM_Loop(Archer_AIM_SHOT);

			auto pItem = ObjectManager::Get().TakeObject(L"Arrow");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
			pItem->SetRotation(pObject->GetRotation());
			pItem->SetScale(Vector3::One * 3.0f);
			pItem->SetForce(forward * 600.0f);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->SetDamage(0.3f * PacketManager::Get().UserList[socket]->AttackRate);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			//pItem->GetCollider()->CollisionEvent = MyEvent::ArrowHit;
			//SoundManager::Get().Play("SV_archer_atk2.mp3");
			SoundManager::Get().PlayQueue("SE_bow_shot.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::ChargeAttack:
		{
			pObject->SetANIM_Loop(Archer_AIM_SHOT);

			auto pItem = ObjectManager::Get().TakeObject(L"Arrow");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
			pItem->SetRotation(pObject->GetRotation());
			pItem->SetScale(Vector3::One * 6.0f);
			pItem->SetForce(forward * 1050.0f);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->SetDamage(0.8f * PacketManager::Get().UserList[socket]->AttackRate);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			SoundManager::Get().PlayQueue("SE_bow_shot.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::ChargeAttack2:
		{
			pObject->SetANIM_Loop(Archer_AIM_SHOT);

			auto pItem = ObjectManager::Get().TakeObject(L"Arrow");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
			pItem->SetRotation(pObject->GetRotation());
			pItem->SetScale(Vector3::One * 9.0f);
			pItem->SetForce(forward * 1800.0f);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->SetDamage(1.6f * PacketManager::Get().UserList[socket]->AttackRate);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			SoundManager::Get().PlayQueue("SE_bow_shot.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::RSkill:
		{
			SoundManager::Get().PlayQueue("SV_archer_atk1.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			pObject->SetANIM_Loop(Archer_THROW);
		}	break;
		case EAction::Attack2:
		{
			auto pItem = ObjectManager::Get().TakeObject(L"ArrowR");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 50.0f);
			pItem->SetRotation(pObject->GetRotation());
			pItem->SetScale(Vector3::One * 3.0f);
			pItem->SetForce((forward * 0.4f + Vector3::Up) * 540.0f);
			pItem->SetDamage(0.4f * PacketManager::Get().UserList[socket]->AttackRate);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
		}	break;
		case EAction::Dash:
		case EAction::DashLeft:
		case EAction::DashRight:
		{
			pObject->GetCollider()->m_eTagArray[ETag::Enemy] = false;
			pObject->GetCollider()->m_eTagArray[ETag::Dummy] = false;
			//
			SoundManager::Get().PlayQueue("SE_dive.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			SoundManager::Get().PlayQueue("SV_archer_atk2.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			pObject->SetANIM_Loop(Archer_DIVE);
			auto pEffect = ObjectManager::Get().TakeObject(L"EPDust");
			pEffect->SetPosition(pObject->GetPosition() + Vector3::Up * 30.0f);
		}	break;
		case EAction::Special:
		{
			pObject->SetHeroAnimSpeed(1.0f);
			PacketManager::Get().UserList[socket]->MotionRate = 1.0f;
			pObject->GetCollider()->m_eTagArray[ETag::Enemy] = false;
			pObject->GetCollider()->m_eTagArray[ETag::Dummy] = false;
			//
			pObject->SetANIM_Loop(Archer_BACKDIVE);
			SoundManager::Get().PlayQueue("SV_archer_atk2.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::Special2:
		{
			//pObject->SetANIM_Loop(Archer_AIM_SHOT);

			auto pItem = ObjectManager::Get().TakeObject(L"Arrow");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 35.0f + pObject->GetRight() * 15.0f);
			pItem->SetRotation(pObject->GetRotation());
			pItem->SetScale(Vector3::One * 3.0f);
			pItem->SetForce(forward * 800.0f);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->SetDamage(0.2f * PacketManager::Get().UserList[socket]->AttackRate);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			//SoundManager::Get().Play("SV_archer_atk2.mp3");
			SoundManager::Get().PlayQueue("SE_bow_shot.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::Wait:
		{
			pObject->GetCollider()->m_eTagArray[ETag::Enemy] = true;
			pObject->GetCollider()->m_eTagArray[ETag::Dummy] = true;
		}	break;
		// ================================== ≈€ ªÁøÎ =========================================
		case EAction::ShockWave:
		{
			// √Ê∞›∆ƒ
			pObject->SetANIM_Loop(Archer_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"PShock");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 100.0f);
			pItem->SetScale(Vector3::One);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->m_pPhysics->m_damage = 0.5f;
			SoundManager::Get().PlayQueue("SV_archer_atk2.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			//SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::ThrowBomb:
		{
			// ∆¯≈∫
			pObject->SetANIM_Loop(Archer_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"PBomb");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
			pItem->SetForce((forward * 0.6f + Vector3::Up) * 300.0f);
			pItem->m_pPhysics->m_damage = 0.6f;
			pItem->m_pPhysics->UserSocket = socket;
			//pItem->SetDamage(1.0f, PacketManager::Get().UserList[socket]->StatLuk);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());

			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::ITimeBomb:
		{
			// Ω√«—∆¯≈∫
			pObject->SetANIM_Loop(Archer_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"TimeBomb");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 50.0f + pObject->GetRight() * 20.0f);
			pItem->SetForce(forward * 90.0f);
			pItem->m_pPhysics->m_damage = 0.7f;
			pItem->m_pPhysics->UserSocket = socket;

			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::IShockBomb:
		{
			// √Ê∞›∆¯≈∫
			pObject->SetANIM_Loop(Archer_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"ShockBomb");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 50.0f + pObject->GetRight() * 20.0f);
			pItem->SetForce(forward * 350.0f);
			//pItem->m_pPhysics->UserSocket = socket;
			pItem->SetHP(0.3f);
			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::IMine:
		{
			// ¡ˆ∑⁄
			pObject->SetANIM_Loop(Archer_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"Mine");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 45.0f + pObject->GetUp() * 50.0f);
			pItem->SetForce(forward * 30.0f);
			pItem->m_pPhysics->m_damage = 0.8f;
			pItem->m_pPhysics->UserSocket = socket;
			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::IMissile:
		{
			// πÃªÁ¿œ
			pObject->SetANIM_Loop(Archer_THROW);
			missileTarget = forward * 600.0f;
			for (int i = 0; i < 13; ++i)
			{
				auto pItem = ObjectManager::Get().TakeObject(L"Missile");
				pItem->SetPosition(pObject->GetPosition() + pObject->GetBackward() * 60.0f + pObject->GetUp() * 55.0f);
				pItem->SetRotation(pObject->GetRotation() + Quaternion::Down * (RandomNormal() * 0.3f + 1.42f));
				pItem->SetForce({ RandomNormal() * 360.0f - 180.0f, RandomNormal() * 210.0f - 30.0f, (RandomNormal() * 200.0f + 60.0f) * pObject->GetBackward().z });
				pItem->m_pPhysics->UserSocket = socket;
				pItem->m_pPhysics->m_damage = 0.3f;
				((CEventTimer*)pItem->GetComponent(EComponent::Timer))->m_EventDelay = RandomNormal() * 1.0f + 0.5f;
				((CEventTimer*)pItem->GetComponent(EComponent::Timer))->TimerEvent = { TimeEvent::MissileShot, (void*)&missileTarget };
			}
			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::INuclear:
		{
			// «Ÿ
			pObject->SetANIM_Loop(Archer_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"SkyShip");
			pItem->SetPosition(pObject->GetPosition() + Vector3::Up * 800.0f + pObject->GetBackward() * 562.5f);
			pItem->SetRotation(pObject->GetRotation());
			pItem->SetDirectionForce(pObject->GetForward() * 600.0f);
			pItem->m_pPhysics->UserSocket = socket;
			SoundManager::Get().PlayQueue("SV_archer_atk4.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			//SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::IPotion:
		{
			// π∞æ‡
			pObject->SetANIM_Loop(Archer_THROW);
			pObject->HealHP(pObject->GetHP() * 0.5f);
			SoundManager::Get().Play("SE_drink.mp3");
		}	break;
		}
	}	break;
	// ==================================== π˝ªÁ =======================================
	case ECharacter::EMage:
	{
		switch (eAction)
		{
		case EAction::Idle:
		{
			pObject->SetANIM_Loop(Mage_IDLE);
		}	break;
		case EAction::Jump:
		{
		}	break;
		case EAction::Left:
		case EAction::BackwardLeft:
		{
			pObject->SetANIM_Loop(Mage_LEFT);
		}	break;
		case EAction::Right:
		case EAction::BackwardRight:
		{
			pObject->SetANIM_Loop(Mage_RIGHT);
		}	break;
		case EAction::Forward:
		case EAction::ForwardLeft:
		case EAction::ForwardRight:
		{
			pObject->SetANIM_Loop(Mage_WALK);
		}	break;
		case EAction::Backward:
		{
			pObject->SetANIM_Loop(Mage_BACK);
		}	break;
		case EAction::LSkill:
		{
			pObject->SetANIM_Loop(Mage_ATK);
		}	break;
		case EAction::LCharge1:
		{
			pObject->SetANIM_Loop(Mage_ATK_DU);
		}	break;
		case EAction::LCharge2:
		{
			pObject->SetANIM_Loop(Mage_ATK_UD);
		}	break;
		case EAction::Attack:
		{
			auto pItem = ObjectManager::Get().TakeObject(L"Magic");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
			pItem->SetScale(pObject->GetScale());
			pItem->SetForce((forward + Vector3::Up * 0.8f) * 240.0f);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->SetDamage(0.25f * PacketManager::Get().UserList[socket]->AttackRate);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());

			SoundManager::Get().PlayQueue("SE_flare_shot.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			SoundManager::Get().PlayQueue("SV_mage_atk1.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::ChargeAttack:
		{
			SoundManager::Get().PlayQueue("SV_mage_atk2.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::LCharging:
		{
			auto pItem = ObjectManager::Get().TakeObject(L"Magic");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 50.0f + pObject->GetRight() * 20.0f);
			pItem->SetScale(pObject->GetScale());
			pItem->SetForce((forward + Vector3::Up * 0.2f) * 850.0f);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->SetDamage(0.15f * PacketManager::Get().UserList[socket]->AttackRate);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			SoundManager::Get().PlayQueue("SE_flare_shot.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::ChargeAttack2:
		{
			for (int i = 1; i < 10; ++i)
			{
				auto pItem = ObjectManager::Get().TakeObject(L"Magic");
				pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 80.0f * i + pObject->GetUp() * 800.0f + pObject->GetRight() * 20.0f);
				pItem->SetForce(Vector3::Down * 400.0f + Vector3::Down * 150.0f * (10 - i));
				pItem->m_pPhysics->UserSocket = socket;
				pItem->SetDamage(0.25f * PacketManager::Get().UserList[socket]->AttackRate);
				pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			}

			SoundManager::Get().PlayQueue("SE_flare_shot.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			SoundManager::Get().PlayQueue("SV_mage_atk3.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::RSkill:
		{
			pObject->SetANIM_Loop(Mage_BUFF);
		}	break;
		case EAction::Attack2:
		{
			// πˆ«¡ ø˛¿Ã∫Í
			auto pItem = ObjectManager::Get().TakeObject(L"BuffWave");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 75.0f);
			pItem->SetRotation(pObject->GetRotation());
			pItem->SetScale(Vector3::One);
			//pItem->SetForce((forward + Vector3::Up * 0.8f) * 80.0f);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->SetDamage(0.5f * PacketManager::Get().UserList[socket]->AttackRate);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());

			SoundManager::Get().PlayQueue("SE_fire1.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			SoundManager::Get().PlayQueue("SV_mage_atk4.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::Special:
		{
			pObject->SetANIM_Loop(Mage_ATK_DU);
		}	break;
		case EAction::Special2:
		{
			auto pEffect = ObjectManager::Get().TakeObject(L"ETeleport");
			pEffect->SetPosition(pObject->GetPosition());
			pObject->SetHeroRender(false);
			pObject->GetCollider()->m_eTagArray[ETag::Enemy] = false;
			pObject->GetCollider()->m_eTagArray[ETag::Dummy] = false;
			SoundManager::Get().PlayQueue("SE_healing.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::Special3:
		{
			pObject->SetHeroRender(true);
			pObject->GetCollider()->m_eTagArray[ETag::Enemy] = true;
			pObject->GetCollider()->m_eTagArray[ETag::Dummy] = true;

			pObject->SetPosition(pObject->GetPosition() + forward * 360.0f);
			auto pEffect = ObjectManager::Get().TakeObject(L"EHit2");
			pEffect->SetPosition(pObject->GetPosition() + Vector3::Up * 25.5f);
			pObject->SetANIM_Loop(Mage_ATK_UD);
		}	break;
		case EAction::Motion1:
		{
			pObject->SetANIM_Loop(Mage_ATK_UD);
		}	break;
		case EAction::Motion2:
		{
			auto targetPos = pObject->GetPosition() + forward * 300.0f;
			targetPos.y = 500.0f;
			auto pItem = ObjectManager::Get().TakeObject(L"Magic");
			pItem->SetPosition(targetPos);
			pItem->SetScale(Vector3::One);
			pItem->SetForce(Vector3::Down * 300.0f);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->SetDamage(0.55f * PacketManager::Get().UserList[socket]->AttackRate);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			SoundManager::Get().PlayQueue("SV_mage_atk4.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		// ================================== ≈€ ªÁøÎ =========================================
		case EAction::ShockWave:
		{
			// √Ê∞›∆ƒ
			pObject->SetANIM_Loop(Mage_ATK_UD);
			auto pItem = ObjectManager::Get().TakeObject(L"PShock");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 100.0f);
			pItem->SetScale(Vector3::One);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->m_pPhysics->m_damage = 0.5f;
			SoundManager::Get().PlayQueue("SV_mage_atk3.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			//SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::ThrowBomb:
		{
			// ∆¯≈∫
			pObject->SetANIM_Loop(Mage_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"PBomb");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
			pItem->SetForce((forward * 0.6f + Vector3::Up) * 300.0f);
			pItem->m_pPhysics->m_damage = 0.6f;
			pItem->m_pPhysics->UserSocket = socket;
			//pItem->SetDamage(1.0f, PacketManager::Get().UserList[socket]->StatLuk);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());

			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::ITimeBomb:
		{
			// Ω√«—∆¯≈∫
			pObject->SetANIM_Loop(Mage_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"TimeBomb");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 50.0f + pObject->GetRight() * 20.0f);
			pItem->SetForce(forward * 90.0f);
			pItem->m_pPhysics->m_damage = 0.7f;
			pItem->m_pPhysics->UserSocket = socket;

			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::IShockBomb:
		{
			// √Ê∞›∆¯≈∫
			pObject->SetANIM_Loop(Mage_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"ShockBomb");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 50.0f + pObject->GetRight() * 20.0f);
			pItem->SetForce(forward * 350.0f);
			//pItem->m_pPhysics->UserSocket = socket;
			pItem->SetHP(0.3f);
			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::IMine:
		{
			// ¡ˆ∑⁄
			pObject->SetANIM_Loop(Mage_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"Mine");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 45.0f + pObject->GetUp() * 50.0f);
			pItem->SetForce(forward * 30.0f);
			pItem->m_pPhysics->m_damage = 0.8f;
			pItem->m_pPhysics->UserSocket = socket;
			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::IMissile:
		{
			// πÃªÁ¿œ
			pObject->SetANIM_Loop(Mage_ATK_UD);
			missileTarget = forward * 600.0f;
			for (int i = 0; i < 13; ++i)
			{
				auto pItem = ObjectManager::Get().TakeObject(L"Missile");
				pItem->SetPosition(pObject->GetPosition() + pObject->GetBackward() * 60.0f + pObject->GetUp() * 55.0f);
				pItem->SetRotation(pObject->GetRotation() + Quaternion::Down * (RandomNormal() * 0.3f + 1.42f));
				pItem->SetForce({ RandomNormal() * 360.0f - 180.0f, RandomNormal() * 210.0f - 30.0f, (RandomNormal() * 200.0f + 60.0f) * pObject->GetBackward().z });
				pItem->m_pPhysics->UserSocket = socket;
				pItem->m_pPhysics->m_damage = 0.3f;
				((CEventTimer*)pItem->GetComponent(EComponent::Timer))->m_EventDelay = RandomNormal() * 1.0f + 0.5f;
				((CEventTimer*)pItem->GetComponent(EComponent::Timer))->TimerEvent = { TimeEvent::MissileShot, (void*)&missileTarget };
			}
			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::INuclear:
		{
			// «Ÿ
			pObject->SetANIM_Loop(Mage_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"SkyShip");
			pItem->SetPosition(pObject->GetPosition() + Vector3::Up * 800.0f + pObject->GetBackward() * 562.5f);
			pItem->SetRotation(pObject->GetRotation());
			pItem->SetDirectionForce(pObject->GetForward() * 600.0f);
			pItem->m_pPhysics->UserSocket = socket;
			SoundManager::Get().PlayQueue("SV_mage_atk4.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			//SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		case EAction::IPotion:
		{
			// π∞æ‡
			pObject->SetANIM_Loop(Mage_THROW);
			pObject->HealHP(pObject->GetHP() * 0.5f);
			SoundManager::Get().Play("SE_drink.mp3");
		}	break;
		case EAction::IBarricade:
		{
			// πŸ∏Æƒ…¿ÃµÂ
			pObject->SetANIM_Loop(Paladin_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"Barricade", false);
			pItem->m_objType = EObjType::Character;
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 45.0f + pObject->GetUp() * 50.0f);
			pItem->SetScale(4.0f, 1.5f, 4.0f);
			pItem->SetRotation(pObject->GetRotation() + Quaternion::Left * PI * 0.5f);
			auto pCollider = new ColliderOBB({ -7.0f, 0.0f, -25.0f }, { 7.0f, 50.0f, 25.0f });
			pCollider->m_eTag = ETag::Ally;
			pItem->AddComponent(pCollider);
			//pCollider->usePhysics(false);
			pItem->SetGravityScale(10.0f);
			pItem->m_pPhysics->m_mass = 0.1f;
			pItem->SetHP(10.0f);
			pItem->SetForce(forward * 50.0f);
			//pItem->m_pPhysics->m_damage = 0.8f;
			pItem->m_pPhysics->UserSocket = socket;
			ObjectManager::Get().PushObject(pItem);
			SoundManager::Get().PlayQueue("SE_dash.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
		}
	}	break;
	}
}

void PlayerController::PlayerInput(const float& spf) noexcept
{
	m_curDelayLSkill = max(m_curDelayLSkill - spf, 0.0f);
	m_curDelayRSkill = max(m_curDelayRSkill - spf, 0.0f);
	m_curDelayDash = max(m_curDelayDash - spf, 0.0f);
	// «‡µø
	m_curState->Process(spf);

	if (m_eAction != m_preAction)
	{
		// ¡§∫∏ ¿¸º€
		SendAnimTransform(m_eAction, m_curCharacter);
	}
	m_preAction = m_eAction;
}

void PlayerController::SetState(const EPlayerState& eState) noexcept
{
	m_preState = m_curState;
	m_curState = m_stateList[eState];
	m_curState->StateInit(this);
}


void PlayerController::CameraInput(const float& spf) noexcept
{
	static const float MinCameraY = -PI * 0.2f;
	static const float MaxCameraY = PI * 0.4f;

	// ∏∂øÏΩ∫ ∞Ì¡§
	static POINT prevPoint = { 0, 0 };
	prevPoint = Input::GetCursor();
	SetCursorPos((int)m_setMouseScreen.x, (int)m_setMouseScreen.y);

	Input::OperMoveMousePos({ (float)(-m_setMouseClient.x + prevPoint.x), (float)(-m_setMouseClient.y + prevPoint.y) });

	// ƒ´∏ﬁ∂Û Arm ¡∂¿˝
	if (!pUIManager->m_pChat->m_bRender)
	{
		m_pCamera->m_armLength = std::clamp(m_pCamera->m_armLength - Input::GetWheelScroll() * m_mouseSense * spf * 5.0f, 0.0f, 320.0f);
	}
	// »∏¿¸
	m_pCamera->SetRotationX(std::clamp(m_pCamera->GetRotation().x + Input::GetMouseMovePos().y * m_mouseSense * 0.004f, MinCameraY, MaxCameraY));
	m_pParent->Rotate(0.0f, Input::GetMouseMovePos().x * m_mouseSense * 0.004f);
	// »∏¿¸ µø±‚»≠
	if (abs(m_prevRotY - m_pParent->GetRotation().y) > 0.08f)
	{
		SendAnimTransform(m_curAnim, ECharacter::EDummy);
	}

	//static Packet_MouseRotate p_MouseRotate;
	//if (Input::GetMouseMovePos().x > 0.0f &&
	//	m_MouseDirection != EDirection::Right)
	//{
	//	m_MouseDirection = EDirection::Right;
	//	m_prevMouseDir = p_MouseRotate.RotateSpeed = m_mouseSense * 0.1f;
	//	p_MouseRotate.KeyValue = m_pParent->m_keyValue;
	//	PacketManager::Get().SendPacket((char*)&p_MouseRotate, sizeof(Packet_MouseRotate), PACKET_MouseRotate);
	//	
	//	ErrorMessage("»∏¿¸ : " + to_string(Input::GetMouseMovePos().x));
	//}
	//else if (Input::GetMouseMovePos().x < 0.0f &&
	//		 m_MouseDirection != EDirection::Left)
	//{
	//	m_MouseDirection = EDirection::Left;
	//	m_prevMouseDir = p_MouseRotate.RotateSpeed = -m_mouseSense * 0.1f;
	//	p_MouseRotate.KeyValue = m_pParent->m_keyValue;
	//	PacketManager::Get().SendPacket((char*)&p_MouseRotate, sizeof(Packet_MouseRotate), PACKET_MouseRotate);
	//	ErrorMessage("»∏¿¸ : " + to_string(Input::GetMouseMovePos().x));
	//}
	//else if (Input::GetMouseMovePos().x == 0.0f &&
	//		 m_MouseDirection != EDirection::Middle)
	//{
	//	m_MouseDirection = EDirection::Middle;
	//	m_prevMouseDir = p_MouseRotate.RotateSpeed = 0.0f;
	//	p_MouseRotate.KeyValue = m_pParent->m_keyValue;
	//	PacketManager::Get().SendPacket((char*)&p_MouseRotate, sizeof(Packet_MouseRotate), PACKET_MouseRotate);
	//	ErrorMessage("»∏¿¸ : " + to_string(Input::GetMouseMovePos().x));
	//}
}

void PlayerController::ResetOption() noexcept
{
	Window::UpdateWindowRect();
	m_setMouseScreen = { (LONG)Window::GetWinCenter().x, (LONG)Window::GetWinCenter().y };
	m_setMouseClient = m_setMouseScreen;
	ScreenToClient(Window::m_hWnd, &m_setMouseClient);
	///
	SetPosition(Vector3::Zero);
	SetRotation(Quaternion::Base);
	m_pCamera = ObjectManager::Cameras[ECamera::Main];
	m_pCamera->SetRotation(Quaternion::Left * PI + Quaternion::Up * PI * 0.2f);
	m_pCamera->m_lerpMoveSpeed = 6.0f;
	m_pCamera->m_lerpRotateSpeed = 6.0f;
	if (m_pParent == nullptr)
		return;
	m_pCamera->SetPosition(Vector3::Up * 85.0f * m_pParent->GetScale().x);
	m_pCamera->m_armLength = 10.0f * m_pParent->GetScale().x;
}

void PlayerController::UpdateStatus(const bool& infoUpdate) noexcept
{
	auto pUserInfo = PacketManager::Get().pMyInfo;
	m_moveSpeed = MoveSpeed + MoveSpeed * pUserInfo->StatDex * 0.05f;
	m_jumpPower = JumpPower;
	pUserInfo->MotionRate = 1.3f + pUserInfo->StatDex * 0.05f + m_motionBuff;
	switch (m_curCharacter)
	{
	case PlayerController::EGuard:
		pUserInfo->AttackRate = 1.0f + m_upgradeWeapon * 0.15f + pUserInfo->StatStr * 0.15f + pUserInfo->StatDex * 0.05f + +pUserInfo->StatInt * 0.05f;
		break;
	case PlayerController::EArcher:
		pUserInfo->AttackRate = 1.0f + m_upgradeWeapon * 0.15f + pUserInfo->StatStr * 0.05f + pUserInfo->StatDex * 0.15f + +pUserInfo->StatInt * 0.05f;
		break;
	case PlayerController::EMage:
		pUserInfo->AttackRate = 1.0f + m_upgradeWeapon * 0.15f + pUserInfo->StatStr * 0.05f + pUserInfo->StatDex * 0.05f + +pUserInfo->StatInt * 0.15f;
		break;
	default:
		break;
	}
	m_DelayEnemyPanel = 3.0f;
	m_DelayRespawn = 8.0f * 5.0f / (5.0f + pUserInfo->StatLuk);
	m_DelayLSkill = 0.7f * 5.0f / (5.0f + m_upgradeAcce1);
	m_DelayRSkill = 4.0f * 5.0f / (5.0f + m_upgradeAcce1);
	m_DelayDash = 2.5f * 5.0f / (5.0f + m_upgradeAcce1);
	pUIManager->m_pLeftIcon->SetValue(m_curDelayDash, m_DelayDash, m_curDelayDash);
	pUIManager->m_pRightIcon->SetValue(m_curDelayRSkill, m_DelayRSkill, m_curDelayRSkill);

	m_maxMP = 1.0f + pUserInfo->StatInt * 0.2f;
	m_RegenMP = 0.2f + pUserInfo->StatInt * 0.03f;
	m_RegenHP = 0.04f + pUserInfo->StatStr * 0.006f;
	
	pUIManager->m_pMpBar->SetValue(m_curMP, m_maxMP, m_disMP);
	pUIManager->m_pExpProgress->SetValue(m_EXP, m_NeedEXP, m_disEXP);
	if (m_pParent != nullptr)
	{
		m_pParent->m_pPhysics->m_maxHP = 1.0f + pUserInfo->StatStr * 0.2f;
		pUIManager->m_pHpBar->SetValue(m_pParent->GetHP(), m_pParent->m_pPhysics->m_maxHP, m_pParent->m_pPhysics->m_disHP);
		pUIManager->m_pInfoHP->SetString(to_wstring((int)(m_pParent->GetHP() * 100.0f)) + L" / " + to_wstring((int)(m_pParent->m_pPhysics->m_maxHP * 100.0f)));
		//pUIManager->m_pInfoTitle->SetString(m_pParent->m_myName);
		pUIManager->m_pInfoArmor->SetString(to_wstring((1.0f - m_pParent->m_pPhysics->m_armor) * 100.0f).substr(0, 4) + L" %");
	}
	if (infoUpdate)
	{
		if (pUIManager->m_pInfoPanel->m_bRender)
		{
			pUIManager->m_pInfoMP->SetString(to_wstring((int)(m_curMP * 100.0f)) + L" / " + to_wstring((int)(m_maxMP * 100.0f)));
			pUIManager->m_pInfoMP->SetString(to_wstring((int)(m_curMP * 100.0f)) + L" / " + to_wstring((int)(m_maxMP * 100.0f)));
			pUIManager->m_pInfoEXP->SetString(to_wstring((int)(m_EXP * 100.0f)) + L" / " + to_wstring((int)(m_NeedEXP * 100.0f)));
			//pUIManager->m_pInfoName->SetString(pUserInfo->UserID);
			pUIManager->m_pInfoAttackSpeed->SetString(to_wstring((pUserInfo->MotionRate * 0.77f) * 100.0f).substr(0, 5) + L" %");
			pUIManager->m_pInfoMoveSpeed->SetString(to_wstring((2.0f - 5.0f / (5.0f + pUserInfo->StatLuk)) * 100.0f).substr(0, 5) + L" %");
			pUIManager->m_pInfoLevel->SetString(to_wstring(pUserInfo->Level));
			if (m_upgradeWeapon >= 1)
				pUIManager->m_pInfoDamage->SetString(to_wstring(100.0f * (pUserInfo->AttackRate - m_upgradeWeapon * 0.15f + 0.001f)).substr(0, 5) + L" + " + to_wstring(m_upgradeWeapon * 15.0f).substr(0, 5) + L" %");
			else
				pUIManager->m_pInfoDamage->SetString(to_wstring(100.0f * pUserInfo->AttackRate).substr(0, 5) + L" %");
			pUIManager->m_pInfoSP->SetString(to_wstring(m_statPoint));
			if (m_upgradeAcce2 >= 1)
			{
				pUIManager->m_pInfoStr->SetString(to_wstring(pUserInfo->StatStr - m_upgradeAcce2) + L" + " + to_wstring(m_upgradeAcce2));
				pUIManager->m_pInfoDex->SetString(to_wstring(pUserInfo->StatDex - m_upgradeAcce2) + L" + " + to_wstring(m_upgradeAcce2));
				pUIManager->m_pInfoInt->SetString(to_wstring(pUserInfo->StatInt - m_upgradeAcce2) + L" + " + to_wstring(m_upgradeAcce2));
				pUIManager->m_pInfoLuk->SetString(to_wstring(pUserInfo->StatLuk - m_upgradeAcce2) + L" + " + to_wstring(m_upgradeAcce2));
			}
			else
			{
				pUIManager->m_pInfoStr->SetString(to_wstring(pUserInfo->StatStr));
				pUIManager->m_pInfoDex->SetString(to_wstring(pUserInfo->StatDex));
				pUIManager->m_pInfoInt->SetString(to_wstring(pUserInfo->StatInt));
				pUIManager->m_pInfoLuk->SetString(to_wstring(pUserInfo->StatLuk));
			}
		}
		if (pUIManager->m_pInvenPanel->m_bRender)
		{
			pUIManager->m_pInvenMoney->SetString(to_wstring(m_money));
			if (pUIManager->m_pInvenWeapon->CheckHovered())
			{
				//m_pInvenInfoName = 
				pUIManager->m_pInvenInfoEffect->SetString(L"Damage +" + to_wstring(m_upgradeWeapon * 15) + L"%");
				pUIManager->m_pInvenInfoReinforce->SetString(L"+" + to_wstring(m_upgradeWeapon));
			}
			if (pUIManager->m_pInvenArmor->CheckHovered())
			{
				pUIManager->m_pInvenInfoEffect->SetString(L"Armor +" + to_wstring((1.0f - (5.0f / (5.0f + m_upgradeArmor))) * 100.0f).substr(0, 4) + L"%");
				pUIManager->m_pInvenInfoReinforce->SetString(L"+" + to_wstring(m_upgradeArmor));
			}
			if (pUIManager->m_pInvenAcce1->CheckHovered())
			{
				pUIManager->m_pInvenInfoEffect->SetString(L"Cooltime +" + to_wstring((1.0f - (5.0f / (5.0f + m_upgradeAcce1 + 1))) * 100.0f).substr(0, 4) + L"%");
				pUIManager->m_pInvenInfoReinforce->SetString(L"+" + to_wstring(m_upgradeAcce1));
			}
			if (pUIManager->m_pInvenAcce2->CheckHovered())
			{
				pUIManager->m_pInvenInfoEffect->SetString(L"All Stat +" + to_wstring(m_upgradeAcce2));
				pUIManager->m_pInvenInfoReinforce->SetString(L"+" + to_wstring(m_upgradeAcce2));
			}
		}
	}
}


void PlayerController::Possess(GameObject* pObject) noexcept
{
	if (pObject == nullptr)
		return;
	SetParent(pObject);

	static auto pEvent = [](void* pVoid, void* pVoid2) {
		auto pPlayer = (PlayerController*)pVoid;
		auto pObj = (GameObject*)pVoid2;
		if (pObj->m_myName == L"Paladin")
		{
			pPlayer->m_curCharacter = PlayerController::ECharacter::EGuard;
			pPlayer->m_pCollider = pObj->GetCollider();
			{
				pPlayer->m_defencePoint = 2;
				pPlayer->pUIManager->m_pMpBar->SetColor({ 0.7f, 0.0f, 0.7f, 1.0f });
				pPlayer->m_pCollider->CollisionEvent = nullptr;
				pPlayer->m_pCollider->m_pivot = Vector3::Up * 40.0f * pObj->GetScale().x;
			}
		}
		else if (pObj->m_myName == L"Archer")
		{
			pPlayer->m_curCharacter = PlayerController::ECharacter::EArcher;
			pPlayer->m_pCollider = pObj->GetCollider();
			{
				pPlayer->m_defencePoint = 1;
				pPlayer->pUIManager->m_pMpBar->SetColor({0.7f, 0.7f, 0.0f, 1.0f});
				pPlayer->m_pCollider->CollisionEvent = nullptr;
				pPlayer->m_pCollider->m_pivot = Vector3::Up * 40.0f * pObj->GetScale().x;
			}
		}
		else if (pObj->m_myName == L"Mage")
		{
			pPlayer->m_curCharacter = PlayerController::ECharacter::EMage;
			pPlayer->m_pCollider = pObj->GetCollider();
			{
				pPlayer->m_defencePoint = 0;
				pPlayer->pUIManager->m_pMpBar->SetColor(Color::Blue);
				pPlayer->m_pCollider->CollisionEvent = nullptr;
				pPlayer->m_pCollider->m_pivot = Vector3::Up * 40.0f * pObj->GetScale().x;
			}
		}
		pObj->SetArmor(pPlayer->m_defencePoint + pPlayer->m_upgradeArmor);
		pPlayer->SendPhysicsInfo();
		if (pPlayer->m_pEffectFly != nullptr)
			ObjectManager::Get().DisableObject(pPlayer->m_pEffectFly);
		if(pPlayer->m_pEffectBerserk != nullptr)
			ObjectManager::Get().DisableObject(pPlayer->m_pEffectBerserk);

		pPlayer->ResetOption();
		pPlayer->UpdateStatus();
		pPlayer->SetState(EPlayerState::Basic);

		pPlayer->m_curDelayDash = 0.0f;
		pPlayer->m_curDelayLSkill = 0.0f;
		pPlayer->m_curDelayRSkill = 0.0f;
		pPlayer->m_chargeCount = 0.0f;
		pPlayer->m_berserkFrame = 0.0f;
		pPlayer->m_motionBuff = 0.0f;
		pPlayer->m_cooltimeBuff = 0.0f;
		// ªÛ≈¬√¢
		UIManager::Get().m_pInfoName->SetString(PacketManager::Get().pMyInfo->UserID);
		UIManager::Get().m_pInfoTitle->SetString(pObj->m_myName);

		SoundManager::Get().m_pListenerPos = &pObj->GetRoot()->GetPosition();
		PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
	};

	ObjectManager::PostFrameEvent.emplace(pEvent, this, pObject);
}

void PlayerController::DeadEvent() noexcept
{
	if (m_pParent == nullptr)
		return;
	SoundManager::Get().m_pListenerPos = &GetPosition();
	SoundManager::Get().Play("SE_dead.mp3");
	switch (m_curCharacter)
	{
	case PlayerController::EGuard:
		PacketManager::Get().SendPlaySound("SV_paladin_dead.mp3", m_pParent->GetPosition(), SoundRange);
		break;
	case PlayerController::EArcher:
		PacketManager::Get().SendPlaySound("SV_archer_dead.mp3", m_pParent->GetPosition(), SoundRange);
		break;
	case PlayerController::EMage:
		PacketManager::Get().SendPlaySound("SV_mage_dead.mp3", m_pParent->GetPosition(), SoundRange);
		break;
	}
	m_pParent->SetHP(0.0f);
	SetPosition(m_pParent->GetPosition());
	SetRotation(m_pParent->GetRotation());
	CutParent(true, true);
	m_curDelayRespawn = 0.0f;
	pUIManager->m_pRespawn->m_bRender = true;
	pUIManager->m_pRespawnBar->SetValue(m_curDelayRespawn, m_DelayRespawn, m_curDelayRespawn);
}

void PlayerController::HitEvent(Collider* pTarget) noexcept
{
	m_curDelayEnemyPanel = m_DelayEnemyPanel;
	pUIManager->m_pEnemyPanel->m_bRender = true;

	m_pTargetEnemy = pTarget->m_pParent;
	pUIManager->m_pEnemyHP->SetValue(pTarget->m_pPhysics->m_curHP, pTarget->m_pPhysics->m_maxHP, pTarget->m_pPhysics->m_disHP);
	pUIManager->m_pEnemyName->SetString(pTarget->m_pParent->m_myName);
}

void PlayerController::OperEXP(const float& value) noexcept
{
	m_money += (int)(value * 5000.0f + value * PacketManager::Get().pMyInfo->StatLuk * 750.0f);
	m_EXP += value;
	if (m_EXP >= m_NeedEXP && m_pParent != nullptr)
	{
		// LevelUp
		m_EXP = min(m_EXP - m_NeedEXP, m_NeedEXP);
		m_statPoint += 4;
		++PacketManager::Get().pMyInfo->Level;
		m_NeedEXP = 1.0f + PacketManager::Get().pMyInfo->Level * 0.3f;
		PacketManager::Get().pMyInfo->KeyValue = m_pParent->m_keyValue;
		PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendLevelUp);
	}
}

void PlayerController::CheckTownCollision() noexcept
{
	for (int i = 0; i < 4; ++i)
	{
		if (VectorLengthSq(m_CarpetPos[i] - GetWorldPosition()) <= 5000.0f)
		{
			switch (i)
			{
			case ECarpet::Smithy:
			{
				if (Input::GetKeyState('X') == EKeyState::DOWN)
				{
					if (pUIManager->m_pSmithyPanel->m_bRender)
					{
						// ≤˚
						pUIManager->m_pSmithyPanel->m_bRender = false;
						pUIManager->m_pMouseIcon->m_bRender = false;
					}
					else
					{
						// ≈¥
						SoundManager::Get().Play("SE_blacksmith.mp3");
						pUIManager->m_pSmithyPanel->m_bRender = true;
						pUIManager->m_pMouseIcon->m_bRender = true;
					}
				}
				pUIManager->m_pSmithyBtnWeapon->SetString(to_wstring((m_upgradeWeapon + 1) * 1000) + L" KG");
				pUIManager->m_pSmithyBtnArmor->SetString(to_wstring((m_upgradeArmor + 1) * 1000) + L" KG");
				pUIManager->m_pSmithyBtnAcce1->SetString(to_wstring((m_upgradeAcce1 + 1) * 1000) + L" KG");
				pUIManager->m_pSmithyBtnAcce2->SetString(to_wstring((m_upgradeAcce2 + 1) * 5000) + L" KG");

				pUIManager->m_pSmithyInfo1Weapon->SetString(L"Level " + to_wstring(m_upgradeWeapon) + L" °Ê Level " + to_wstring(m_upgradeWeapon + 1));
				pUIManager->m_pSmithyInfo2Weapon->SetString(L"Damage +" + to_wstring((m_upgradeWeapon) * 15) + L"% °Ê +" + to_wstring((m_upgradeWeapon + 1) * 15) + L"%");

				pUIManager->m_pSmithyInfo1Armor->SetString(L"Level " + to_wstring(m_upgradeArmor) + L" °Ê Level " + to_wstring(m_upgradeArmor + 1));
				pUIManager->m_pSmithyInfo2Armor->SetString(L"Armor +" + to_wstring((1.0f - (5.0f / (5.0f + m_defencePoint + m_upgradeArmor))) * 100.0f).substr(0, 4) + L"% °Ê +" + to_wstring((1.0f - (5.0f / (5.0f + m_defencePoint + m_upgradeArmor + 1))) * 100.0f).substr(0, 4) + L"%"); 

				pUIManager->m_pSmithyInfo1Acce1->SetString(L"Level " + to_wstring(m_upgradeAcce1) + L" °Ê Level " + to_wstring(m_upgradeAcce1 + 1));
				pUIManager->m_pSmithyInfo2Acce1->SetString(L"Cooltime +" + to_wstring((1.0f - (5.0f / (5.0f + m_upgradeAcce1))) * 100.0f).substr(0, 4) + L"% °Ê +" + to_wstring((1.0f - (5.0f / (5.0f + m_upgradeAcce1 + 1))) * 100.0f).substr(0, 4) + L"%");

				pUIManager->m_pSmithyInfo1Acce2->SetString(L"Level " + to_wstring(m_upgradeAcce2) + L" °Ê Level " + to_wstring(m_upgradeAcce2 + 1));
				pUIManager->m_pSmithyInfo2Acce2->SetString(L"All Stat +" + to_wstring(m_upgradeAcce2) + L" °Ê +" + to_wstring(m_upgradeAcce2 + 1));
			}	break;
			case ECarpet::Church:
			{
				if (PlayerController::Get().m_canChurh)
				{
					if (Input::GetKeyState('X') == EKeyState::DOWN)
					{
						SoundManager::Get().Play("SE_church.mp3");
						PlayerController::Get().m_canChurh = false;
						pUIManager->m_pRespawnEffect->SetEventTime(1.5f);
						pUIManager->m_pRespawnEffect->EffectPlay();

						Packet_Float p_HealHP;
						p_HealHP.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
						p_HealHP.Value = 10000.0f;
						PacketManager::Get().SendPacket((char*)&p_HealHP, (USHORT)sizeof(Packet_Float), PACKET_HealHP);
						//SoundManager::Get().Play("SE_dark.mp3");
						// + ∆˜º«
						UIManager::Get().AddSlotItem(L"Potion");
					}
				}
			}	break;
			case ECarpet::Shop:
			{
				if (Input::GetKeyState('X') == EKeyState::DOWN)
				{
					if (pUIManager->m_pShopPanel->m_bRender)
					{
						// ≤˚
						pUIManager->m_pShopPanel->m_bRender = false;
						pUIManager->m_pMouseIcon->m_bRender = false;
						pUIManager->m_pInvenPanel->m_bRender = false;
					}
					else
					{
						// ≈¥
						SoundManager::Get().Play("SE_windmill.mp3");
						pUIManager->m_pShopPanel->m_bRender = true;
						pUIManager->m_pMouseIcon->m_bRender = true;
						pUIManager->m_pInvenPanel->m_bRender = true;

						pUIManager->m_pShopItem0->SetString(L"500 KG");
						pUIManager->m_pShopItem1->SetString(L"500 KG");
						pUIManager->m_pShopItem2->SetString(L"500 KG");
						pUIManager->m_pShopItem3->SetString(L"500 KG");
						pUIManager->m_pShopItem4->SetString(L"500 KG");
						pUIManager->m_pShopItem5->SetString(L"500 KG");
						pUIManager->m_pShopItem6->SetString(L"500 KG");
						pUIManager->m_pShopItem7->SetString(L"500 KG");
					}
				}
			}	break;
			case ECarpet::Tower:
			{
				if (Input::GetKeyState('X') == EKeyState::DOWN)
				{
					if (pUIManager->m_pTowerPanel->m_bRender)
					{
						// ≤˚
						pUIManager->m_pTowerPanel->m_bRender = false;
						pUIManager->m_pMouseIcon->m_bRender = false;
					}
					else
					{
						// ≈¥
						SoundManager::Get().Play("SE_towerround.mp3");
						pUIManager->m_pTowerPanel->m_bRender = true;
						pUIManager->m_pMouseIcon->m_bRender = true;

					}
				}
				pUIManager->m_pTowerUpgrade->SetString(to_wstring(1000 + PacketManager::Get().TowerLevel * 1000) + L" KG");
				pUIManager->m_pTowerCurLevel->SetString(to_wstring(PacketManager::Get().TowerLevel));
				pUIManager->m_pTowerCurAtkDamage->SetString(to_wstring(PacketManager::Get().TowerDamage * 100.0f).substr(0, 4));
				pUIManager->m_pTowerCurAtkSpeed->SetString(to_wstring(PacketManager::Get().TowerDelayShot).substr(0, 4));
				if(PacketManager::Get().TowerLevel >= 5)
					pUIManager->m_pTowerText1->SetString(L"∆¯≈∫ ∞¯∞›");
				else
					pUIManager->m_pTowerText1->SetString(L"");
						
				pUIManager->m_pTowerNextLevel->SetString(to_wstring(PacketManager::Get().TowerLevel + 1));
				pUIManager->m_pTowerNextAtkDamage->SetString(to_wstring((0.2f + PacketManager::Get().TowerLevel * 0.05f) * 100.0f).substr(0, 4));
				if(PacketManager::Get().TowerLevel == 0)
					pUIManager->m_pTowerNextAtkSpeed->SetString(to_wstring(8.0f).substr(0, 4));
				else
					pUIManager->m_pTowerNextAtkSpeed->SetString(to_wstring(PacketManager::Get().TowerDelayShot * 0.85f).substr(0, 4));

				if (PacketManager::Get().TowerLevel == 4)
					pUIManager->m_pTowerText2->SetString(L"∆¯≈∫ ∞¯∞›");
				else
					pUIManager->m_pTowerText2->SetString(L"");
			}	break;
			}
			pUIManager->m_pXPush->m_bRender = true;
			return;
		}
	}
	if (pUIManager->m_pXPush->m_bRender)
	{
		pUIManager->m_pXPush->m_bRender = false;
		pUIManager->m_pShopPanel->m_bRender = false;
		pUIManager->m_pSmithyPanel->m_bRender = false;
		pUIManager->m_pTowerPanel->m_bRender = false;
		pUIManager->m_pMouseIcon->m_bRender = false;
		pUIManager->m_pInvenPanel->m_bRender = false;
	}
}


void PlayerController::SendAnimTransform(const EAction& eAction, const ECharacter& eCharacter) noexcept
{
	static Packet_AnimTransform p_AnimTransform;

	p_AnimTransform.KeyValue = m_pParent->m_keyValue;
	p_AnimTransform.UserSocket = PacketManager::Get().pMyInfo->UserSocket;
	p_AnimTransform.Position = m_pParent->GetPosition();
	p_AnimTransform.Rotation = m_pParent->GetRotation();
	m_prevRotY = p_AnimTransform.Rotation.y;
	p_AnimTransform.EAnimState = m_curAnim = eAction;
	p_AnimTransform.ECharacter = eCharacter;
	p_AnimTransform.Force = m_pParent->GetForce();

	// ¿Ãµø √≥∏Æ
	switch (eAction)
	{
	case EAction::Left:
	case EAction::RLeft:
	{
		p_AnimTransform.Direction = m_pParent->GetLeft();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::Right:
	case EAction::RRight:
	{
		p_AnimTransform.Direction = m_pParent->GetRight();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::Forward:
	case EAction::RForward:
	{
		p_AnimTransform.Direction = m_pParent->GetForward();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::Backward:
	case EAction::RBackward:
	{
		p_AnimTransform.Direction = m_pParent->GetBackward();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::ForwardLeft:
	case EAction::NForwardLeft:
	{
		p_AnimTransform.Direction = m_pParent->GetForward() + m_pParent->GetLeft();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::ForwardRight:
	case EAction::NForwardRight:
	{
		p_AnimTransform.Direction = m_pParent->GetForward() + m_pParent->GetRight();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::BackwardLeft:
	case EAction::NBackwardLeft:
	{
		p_AnimTransform.Direction = m_pParent->GetBackward() + m_pParent->GetLeft();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::BackwardRight:
	case EAction::NBackwardRight:
	{
		p_AnimTransform.Direction = m_pParent->GetBackward() + m_pParent->GetRight();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::Run:
	{
		p_AnimTransform.Direction = m_pParent->GetForward();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed * 2.0f;
	}	break;
	case EAction::RunLeft:
	{
		p_AnimTransform.Direction = m_pParent->GetForward() + m_pParent->GetLeft() * 0.5f;
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed * 2.0f;
	}	break;
	case EAction::RunRight:
	{
		p_AnimTransform.Direction = m_pParent->GetForward() + m_pParent->GetRight() * 0.5f;
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed * 2.0f;
	}	break;
	case EAction::Dash:
	case EAction::DashLeft:
	case EAction::DashRight:
	{
		p_AnimTransform.Direction = Vector3::Zero;
	}	break;
	default:
	{
		p_AnimTransform.Direction = ObjectManager::Cameras[ECamera::Main]->GetForward();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction);
	}	break;
	}
	PacketManager::Get().SendPacket((char*)&p_AnimTransform, sizeof(Packet_AnimTransform), PACKET_SetAnimTransform);
}

void PlayerController::SendReqRespawn(const ECharacter& eCharacter) noexcept
{
	m_curDelayRespawn = 0.0f;
	pUIManager->m_pRespawn->m_bRender = false;

	// ¥Ÿ∏• ƒ≥∏Ø¿œΩ√
	if (m_curCharacter != eCharacter)
	{
		m_curCharacter = eCharacter;
		if (m_curState != nullptr)
		{
			for (auto& iter : m_stateList)
			{
				delete iter.second;
			}
			m_stateList.clear();
		}
		// ªÛ≈¬ º≥¡§
		switch (eCharacter)
		{
		case PlayerController::EDummy:
			break;
		case PlayerController::EGuard:
			m_stateList.try_emplace(EPlayerState::Basic, new  PlayerStateBasic());
			m_stateList.try_emplace(EPlayerState::LSkill, new PlayerStateLSkill());
			m_stateList.try_emplace(EPlayerState::RSkill, new PlayerStateRSkill());
			m_stateList.try_emplace(EPlayerState::Run, new PlayerStateRun());
			m_stateList.try_emplace(EPlayerState::Special, new PlayerStateGuard());
			m_stateList.try_emplace(EPlayerState::Wait, new PlayerStateWait());
			//m_stateList.try_emplace(EPlayerState::Dead, new PlayerStateDead());
			break;
		case PlayerController::EArcher:
			m_stateList.try_emplace(EPlayerState::Basic, new ArcherStateBasic());
			m_stateList.try_emplace(EPlayerState::LSkill, new ArcherStateLSkill());
			m_stateList.try_emplace(EPlayerState::RSkill, new ArcherStateRSkill());
			m_stateList.try_emplace(EPlayerState::Dash, new ArcherStateDash());
			m_stateList.try_emplace(EPlayerState::Special, new ArcherStateTumbling());
			m_stateList.try_emplace(EPlayerState::Wait, new ArcherStateWait());
			break;
		case PlayerController::EMage:
			m_stateList.try_emplace(EPlayerState::Basic, new MageStateBasic());
			m_stateList.try_emplace(EPlayerState::LSkill, new MageStateLSkill());
			m_stateList.try_emplace(EPlayerState::RSkill, new MageStateRSkill());
			m_stateList.try_emplace(EPlayerState::Dash, new MageStateDash());
			m_stateList.try_emplace(EPlayerState::Special, new MageStateSpecial());
			m_stateList.try_emplace(EPlayerState::Wait, new MageStateWait());
			break;
		default:
			break;
		}
		m_curState = m_stateList[EPlayerState::Basic];
		SetState(EPlayerState::Basic);
	}

	static Packet_ReqAddPlayer p_ReqAddPlayer;
	p_ReqAddPlayer.ECharacter = eCharacter;
	PacketManager::Get().ReqSendPacket((char*)&p_ReqAddPlayer, (USHORT)sizeof(Packet_ReqAddPlayer), PACKET_ReqAddPlayer);
}


void PlayerController::SendGiantMode(const float& spf) noexcept
{
	Packet_Vector3 p_SetScale;
	p_SetScale.KeyValue = m_pParent->m_keyValue;
	p_SetScale.Vec3 = m_pParent->GetScale() + Vector3::One * spf;
	PacketManager::Get().SendPacket((char*)&p_SetScale, (USHORT)sizeof(Packet_Vector3), PACKET_SetScale);
}

void PlayerController::StartGiantMode() noexcept
{
	//PacketManager::Get().SendPlaySound("SE_jajan.mp3", m_pParent->GetPosition(), SoundRange);
	pUIManager->m_pRespawn->EffectPlay();
	// Heal
	Packet_Float p_SetHP;
	p_SetHP.KeyValue = PlayerController::Get().GetParent()->m_keyValue;
	p_SetHP.Value = 100.0f;
	PacketManager::Get().SendPacket((char*)&p_SetHP, (USHORT)sizeof(Packet_Float), PACKET_HealHP);
	//// Score
	//PacketManager::Get().pMyInfo->Score += 777;

	float frameCount = 0.0f;
	while (frameCount <= 3.0f)
	{
		if (m_pParent == nullptr)
			break;
		SendGiantMode(0.032f);
		frameCount += 0.12f;
		this_thread::sleep_for(chrono::milliseconds(120));
	}
	m_moveSpeed = MoveSpeed * 3.0f + MoveSpeed * PacketManager::Get().pMyInfo->StatDex * 0.15f;
	m_jumpPower = JumpPower * 2.0f;
}

void PlayerController::StartVibration(float seconds, const float& shakePower) noexcept
{
	//pUIManager->m_pRespawn->EffectPlay();

	auto prevPosition = m_pCamera->GetPosition();
	float randValue = shakePower * 2.0f;
	while (seconds >= 0.0f)
	{
		m_pCamera->SetPosition(prevPosition);
		m_pCamera->Translate(randValue * RandomNormal() - shakePower, randValue * RandomNormal() - shakePower, randValue * RandomNormal() - shakePower);

		seconds -= 0.03f;
		this_thread::sleep_for(chrono::milliseconds(30));
	}
	m_pCamera->SetPosition(prevPosition);
}

//void PlayerController::StartTaunt(GameObject* pTarget, float seconds) noexcept
//{
//	while (seconds >= 0.0f)
//	{
//
//		seconds -= 0.03f;
//		this_thread::sleep_for(chrono::milliseconds(30));
//	}
//}

void PlayerController::SendPhysicsInfo() noexcept
{
	Packet_Physics p_Physics;
	p_Physics.KeyValue = m_pParent->m_keyValue;
	p_Physics.Mass = m_pParent->m_pPhysics->m_mass;
	p_Physics.Damping = m_pParent->m_pPhysics->m_damping;
	p_Physics.Repulsion = m_pParent->m_pPhysics->m_repulsion;
	p_Physics.MaxHP = m_pParent->m_pPhysics->m_maxHP;
	p_Physics.Armor = m_pParent->m_pPhysics->m_armor;
	p_Physics.GravityScale = m_pParent->m_pPhysics->m_GravityScale;
	PacketManager::Get().SendPacket((char*)&p_Physics, sizeof(Packet_Physics), PACKET_SetPhysicsInfo);
}

void PlayerController::SendGameStart() noexcept
{
	UIManager::Get().m_FightPanel->m_bRender = true;
	PlayerController::Get().m_GameFrameCount = 3.0f;
	this_thread::sleep_for(chrono::seconds(4));

	PlayerController::Get().SendReqRespawn(PlayerController::Get().m_selectCharacter);
	PacketManager::Get().pChatList->push_string(L"========================== ∞‘¿” Ω√¿€ ============================");
}