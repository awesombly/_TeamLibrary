#include "PlayerController.h"
#include "Collider.h"
#include "AHeroObj.h"
#include "ObjectManager.h"
#include "PacketManager.h"
#include "SoundManager.h"
#include "EventManager.h"
#include "CEventTimer.h"
#include "AIZombie.h"
#include "UIManager.h"

#include "CTransformer.h"

bool PlayerController::Init() noexcept
{
	GameObject::Init();
	if (pUIManager == nullptr)
	{
		pUIManager = &UIManager::Get();
	}
	auto& ItemIndex = JItem::Get()->m_pItemList;
	// 템 설정
	m_ItemList[ItemIndex[L"Berry_0"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Berry_1"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Book_0"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Book_1"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Cloak_0"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Cloak_1"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Coin_0"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Ball_0"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Ball_1"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Essence_0"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Flower_0"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Gems_0"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Gems_1"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"MetalCase_0"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"MetalCase_1"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Plate_0"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Sword_0"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Necklace_0"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Necklace_1"]] = ActiveEvent::ShockWave;
	m_ItemList[ItemIndex[L"Ornament_0"]] = ActiveEvent::ThrowBomb;
	m_ItemList[ItemIndex[L"Ornament_1"]] = ActiveEvent::ThrowBomb;
	m_ItemList[ItemIndex[L"Parchment_0"]] = ActiveEvent::ThrowBomb;
	m_ItemList[ItemIndex[L"Stone_0"]] = ActiveEvent::ThrowBomb;
	m_ItemList[ItemIndex[L"Stone_1"]] = ActiveEvent::ThrowBomb;
	m_ItemList[ItemIndex[L"Shirt_0"]] = ActiveEvent::ThrowBomb;
	m_ItemList[ItemIndex[L"Shirt_1"]] = ActiveEvent::ThrowBomb;
	m_ItemList[ItemIndex[L"Potion_0"]] = ActiveEvent::ThrowBomb;
	m_ItemList[ItemIndex[L"Potion_1"]] = ActiveEvent::ThrowBomb;
	m_ItemList[ItemIndex[L"Wood_0"]] = ActiveEvent::ThrowBomb;
	m_ItemList[ItemIndex[L"Wood_1"]] = ActiveEvent::ThrowBomb;
	return true;
}

bool PlayerController::Frame(const float& spf, const float& accTime)	noexcept
{
	GameObject::Frame(spf, accTime);
	m_curMP = min(m_curMP + spf * m_RegenMP, m_maxMP);

	if (!pUIManager->m_pChat->m_bRender &&
		m_pParent != nullptr)
	{
		if (pUIManager->m_pMouseIcon->m_bRender)
		{
			UpdateStatus();
		}
		else
		{
			PlayerInput(spf);
			CameraInput(spf);

			// 초기화
			if (Input::GetKeyState('R') == EKeyState::DOWN)
			{
				Input::isDebug = !Input::isDebug;
				ResetOption();
			}
			if (Input::GetKeyState(VK_SUBTRACT) == EKeyState::DOWN)
			{
				m_curDelayRespawn = -9999.9f;
				CutParent(true, true);
				ObjectManager::Cameras[ECamera::Main]->CutParent(true, true);
			}
		}
	}
	else if (m_pParent == nullptr)
	{
		// 리스폰
		m_curDelayRespawn += spf;
		if (m_curDelayRespawn >= m_DelayRespawn)
		{
			pUIManager->m_pRespawn->m_bRender = false;
			m_curDelayRespawn = 0.0f;

			SendReqRespawn(m_curCharacter);
			return true;
		}
	}

	// 적 체력바
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
	// 경치바
	//if (m_disEXP < m_EXP)
	//{
		m_disEXP = min<float>(m_disEXP + spf * 0.4f, m_EXP);
	//}

	/// 재생성
	if (Input::GetKeyState('Z') == EKeyState::DOWN)
	{
		if (m_pParent != nullptr)
			m_pParent->m_pPhysics->UserSocket = (UINT)-1;
		SendReqRespawn(ECharacter::EGuard);
	}
	if (Input::GetKeyState('X') == EKeyState::DOWN)
	{
		if (m_pParent != nullptr)
			m_pParent->m_pPhysics->UserSocket = (UINT)-1;
		SendReqRespawn(ECharacter::EArcher);
	}
	if (Input::GetKeyState('C') == EKeyState::DOWN)
	{
		if (m_pParent != nullptr)
			m_pParent->m_pPhysics->UserSocket = (UINT)-1;
		SendReqRespawn(ECharacter::EMage);
	}
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
	switch (eCharacter)
	{
	// ==================================== 팔라딘 =======================================
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
			 SoundManager::Get().Play("SE_dash.mp3");
			 pObject->SetANIM_Loop(Paladin_RUN);
		 }	break;
	 	 case EAction::LSkill:
	 	 {
			pObject->SetANIM_Loop(Paladin_ATTACK);
		}	break;
	 	 case EAction::Attack:
	 	 {
			 auto pItem = ObjectManager::Get().TakeObject(L"Melee");
			 ///
	 	 	//auto pCollider = new Collider(pObject->GetScale().x * 55.0f);
	 	 	//auto pMelee = new GameObject(L"Melee", { pCollider, new CEventTimer(0.5f) });
	 	 	pItem->SetParent(pObject);
	 	 	pItem->SetPosition(pObject->GetForward() * 50.0f + pObject->GetUp() * 45.0f);
	 	 	//pItem->UpdateMatrix();
	 	 	pItem->m_pPhysics->UserSocket = socket;
	 	 	pItem->SetDamage(0.5f, PacketManager::Get().UserList[socket]->StatStr);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			
			SoundManager::Get().Play("SV_paladin_atk1.mp3");
			SoundManager::Get().PlayQueue("SE_Sword_slash1.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
	 	 }	break;
	 	 case RSkill:
	 	 {
			 pObject->SetANIM_Loop(Paladin_POWERUP);
		}	break;
		 case ChargeAttack:
		 {
			 pObject->SetANIM_Loop(Paladin_POWERUP);
		 }	break;
	 	 case Special:
	 	 {
	 	 	pObject->SetANIM_Loop(Paladin_BLOCK);
			SoundManager::Get().PlayQueue("SE_shildup.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
	 	 	for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Enemy))
	 	 	{
	 	 		if (auto pController = iter->GetComponent(EComponent::Etc);
	 	 			pController != nullptr)
				{
					((AIZombie*)pController)->m_Target = pObject->GetPosition();
				}
	 	 	}
	 	 }	break;
	 	 // ================================== 템 사용 =========================================
	 	 case EAction::ShockWave:
	 	 {
			// 충격파
			pObject->SetANIM_OneTime(Paladin_POWERUP);
			auto pItem = ObjectManager::Get().TakeObject(L"PShock");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 100.0f);
			pItem->SetScale(Vector3::One);
			pItem->m_pPhysics->UserSocket = socket;
			pItem->SetDamage(0.5f, PacketManager::Get().UserList[socket]->StatLuk);
			SoundManager::Get().PlayQueue("SV_paladin_shout.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			//SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
	 	 case EAction::ThrowBomb:
	 	 {
			// 폭탄
			pObject->SetANIM_OneTime(Paladin_THROW);
			auto pItem = ObjectManager::Get().TakeObject(L"PBomb");
			pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
			pItem->SetForce((forward * 0.6f + Vector3::Up) * 100.0f);
			pItem->m_pPhysics->UserSocket = socket;
			//pItem->SetDamage(1.0f, PacketManager::Get().UserList[socket]->StatLuk);
			pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			//SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		}	break;
	 	 }
	 }	break;
	 // ==================================== 아처 =======================================
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
	 	 	pObject->SetANIM_OneTime(Archer_AIM_READY);
	 	 }	break;
	 	 case EAction::LCharging:
	 	 {
			SoundManager::Get().Play("SE_bow_ready.mp3");
	 	 	pObject->SetANIM_OneTime(Archer_AIM_IDLE);
	 	 }	break;
		 case EAction::LCharge1:
		 {
			 auto pItem = ObjectManager::Get().TakeObject(L"EPSlash");
			 SoundManager::Get().Play("SE_bow_ready.mp3");
			///SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			 pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 55.0f);
		 }	break;
		 case EAction::LCharge2:
		 {
			 auto pItem = ObjectManager::Get().TakeObject(L"EPAttack");
			 SoundManager::Get().Play("SE_bow_ready.mp3");
			 //SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
			 pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 55.0f);
		 }	break;
	 	 case EAction::Attack:
	 	 {
	 	 	pObject->SetANIM_Loop(Archer_AIM_SHOT);
	 	 
	 	 	auto pItem = ObjectManager::Get().TakeObject(L"Arrow");
	 	 	pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
	 	 	pItem->SetRotation(pObject->GetRotation());
			pItem->SetScale(Vector3::One);
	 	 	pItem->SetForce(forward * 200.0f);
	 	 	pItem->m_pPhysics->UserSocket = socket;
	 	 	pItem->SetDamage(0.3f, PacketManager::Get().UserList[socket]->StatStr);
	 	 	pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			//SoundManager::Get().Play("SV_archer_atk2.mp3");
	 	 	SoundManager::Get().PlayQueue("SE_bow_shot.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
	 	 }	break;
	 	 case EAction::ChargeAttack:
	 	 {
	 	 	pObject->SetANIM_Loop(Archer_AIM_SHOT);
	 	 
	 	 	auto pItem = ObjectManager::Get().TakeObject(L"Arrow");
	 	 	pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
	 	 	pItem->SetRotation(pObject->GetRotation());
			pItem->SetScale(Vector3::One * 2.0f);
	 	 	pItem->SetForce(forward * 350.0f);
	 	 	pItem->m_pPhysics->UserSocket = socket;
	 	 	pItem->SetDamage(0.8f, PacketManager::Get().UserList[socket]->StatStr);
	 	 	pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			SoundManager::Get().PlayQueue("SE_bow_shot.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
	 	 	//SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
	 	 }	break;
	 	 case EAction::ChargeAttack2:
	 	 {
	 	 	pObject->SetANIM_Loop(Archer_AIM_SHOT);
	 	 
	 	 	auto pItem = ObjectManager::Get().TakeObject(L"Arrow");
	 	 	pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
	 	 	pItem->SetRotation(pObject->GetRotation());
			pItem->SetScale(Vector3::One * 3.0f);
	 	 	pItem->SetForce(forward * 600.0f);
	 	 	pItem->m_pPhysics->UserSocket = socket;
	 	 	pItem->SetDamage(1.6f, PacketManager::Get().UserList[socket]->StatStr);
	 	 	pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			SoundManager::Get().PlayQueue("SE_bow_shot.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
	 	 	//SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
	 	 }	break;
		 case EAction::RSkill:
		 {
			 SoundManager::Get().Play("SV_archer_atk1.mp3");
			 pObject->SetANIM_OneTime(Archer_THROW);
		 }	break;
		 case EAction::Special:
		 {
			 auto pItem = ObjectManager::Get().TakeObject(L"ArrowR");
			 pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 50.0f);
			 pItem->SetRotation(pObject->GetRotation());
			 pItem->SetScale(Vector3::One);
			 pItem->SetForce((forward * 0.4f + Vector3::Up) * 180.0f);
			 pItem->m_pPhysics->UserSocket = socket;
			 //pItem->SetDamage(0.3f, PacketManager::Get().UserList[socket]->StatStr);
			 pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
		 }	break;
		 case Dash:
		 case DashLeft:
		 case DashRight:
		 {
			 SoundManager::Get().Play("SE_dash.mp3");
			 pObject->SetANIM_Loop(Archer_DIVE);
			 auto pEffect = ObjectManager::Get().TakeObject(L"EPDust");
			 pEffect->SetPosition(pObject->GetPosition() + Vector3::Up * 10.0f);
		 }	break;
	 	 // ================================== 템 사용 =========================================
	 	 case EAction::ShockWave:
	 	 {
	 	 	// 충격파
	 	 	pObject->SetANIM_OneTime(Archer_AIM_IDLE);
	 	 	auto pItem = ObjectManager::Get().TakeObject(L"PShock");
	 	 	pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 100.0f);
	 	 	pItem->SetScale(Vector3::One);
	 	 	pItem->m_pPhysics->UserSocket = socket;
	 	 	pItem->SetDamage(0.5f, PacketManager::Get().UserList[socket]->StatLuk);
	 	 	//SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
	 	 }	break;
	 	 case EAction::ThrowBomb:
	 	 {
	 	 	// 폭탄
	 	 	pObject->SetANIM_OneTime(Archer_THROW);
	 	 	auto pItem = ObjectManager::Get().TakeObject(L"PBomb");
	 	 	pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
	 	 	pItem->SetForce((forward * 0.6f + Vector3::Up) * 100.0f);
	 	 	pItem->m_pPhysics->UserSocket = socket;
	 	 	//pItem->SetDamage(1.0f, PacketManager::Get().UserList[socket]->StatLuk);
	 	 	pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
	 	 	//SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
	 	 }	break;
	 	}
	 }	break;
	 // ==================================== 법사 =======================================
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
		 case EAction::Attack:
		 {
			 auto pItem = ObjectManager::Get().TakeObject(L"Magic");
			 pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
			 pItem->SetRotation(pObject->GetRotation());
			 //pItem->SetScale(Vector3::One);
			 pItem->SetForce((forward + Vector3::Up * 0.8f) * 80.0f);
			 pItem->m_pPhysics->UserSocket = socket;
			 pItem->SetDamage(0.25f, PacketManager::Get().UserList[socket]->StatStr);
			 pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			 
			 SoundManager::Get().Play("SV_mage_atk2.mp3");
			 SoundManager::Get().PlayQueue("SE_flare_shot.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		 }	break;
		 case EAction::RSkill:
		 {
			 pObject->SetANIM_Loop(Mage_BUFF);
		 }	break;
		 case EAction::ChargeAttack:
		 {
			 auto pItem = ObjectManager::Get().TakeObject(L"BuffWave");
			 pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 75.0f);
			 pItem->SetRotation(pObject->GetRotation());
			 pItem->SetScale(Vector3::One);
			 //pItem->SetForce((forward + Vector3::Up * 0.8f) * 80.0f);
			 pItem->m_pPhysics->UserSocket = socket;
			 pItem->SetDamage(0.5f, PacketManager::Get().UserList[socket]->StatStr);
			 pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());

			SoundManager::Get().Play("SV_mage_atk4.mp3");
			 SoundManager::Get().PlayQueue("SE_fire1.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
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
			 pObject->GetCollider()->m_eTag = ETag::Dummy;
			 SoundManager::Get().PlayQueue("SE_healing.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange); 
		 }	break;
		 case EAction::Special3:
		 {
			 pObject->SetHeroRender(true);
			 pObject->GetCollider()->m_eTag = ETag::Ally;

			 pObject->SetPosition(pObject->GetPosition() + forward * 120.0f);
			 auto pEffect = ObjectManager::Get().TakeObject(L"EHit2");
			 pEffect->SetPosition(pObject->GetPosition() + Vector3::Up * 10.0f);
			 pObject->SetANIM_Loop(Mage_ATK_DU);
		 }	break;
		 // ================================== 템 사용 =========================================
		 case EAction::ShockWave:
		 {
			 // 충격파
			 pObject->SetANIM_OneTime(Mage_ATK_DU);
			 auto pItem = ObjectManager::Get().TakeObject(L"PShock");
			 pItem->SetPosition(pObject->GetPosition() + pObject->GetUp() * 100.0f);
			 pItem->SetScale(Vector3::One);
			 pItem->m_pPhysics->UserSocket = socket;
			 pItem->SetDamage(0.5f, PacketManager::Get().UserList[socket]->StatLuk);
			// SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
		 }	break;
		 case EAction::ThrowBomb:
		 {
			 // 폭탄
			 pObject->SetANIM_OneTime(Mage_THROW);
			 auto pItem = ObjectManager::Get().TakeObject(L"PBomb");
			 pItem->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
			 pItem->SetForce((forward * 0.6f + Vector3::Up) * 100.0f);
			 pItem->m_pPhysics->UserSocket = socket;
			 //pItem->SetDamage(1.0f, PacketManager::Get().UserList[socket]->StatLuk);
			 pItem->GetCollider()->AddIgnoreList(pObject->GetCollider());
			// SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetPosition(), PlayerController::Get().SoundRange);
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
	// 행동
	m_curState->Process(spf);

	if (m_eAction != m_preAction)
	{
		// 정보 전송
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

	// 마우스 고정
	static POINT prevPoint = { 0, 0 };
	prevPoint = Input::GetCursor();
	SetCursorPos((int)m_setMouseScreen.x, (int)m_setMouseScreen.y);

	Input::OperMoveMousePos({ (float)(-m_setMouseClient.x + prevPoint.x), (float)(-m_setMouseClient.y + prevPoint.y) });

	// 카메라 Arm 조절
	if (!pUIManager->m_pChat->m_bRender)
	{
		m_pCamera->m_armLength = std::clamp(m_pCamera->m_armLength - Input::GetWheelScroll() * m_mouseSense * spf, 0.0f, 80.0f);
	}
	// 회전
	m_pCamera->SetRotationX(std::clamp(m_pCamera->GetRotation().x + Input::GetMouseMovePos().y * m_mouseSense * 0.004f, MinCameraY, MaxCameraY));
	m_pParent->Rotate(0.0f, Input::GetMouseMovePos().x * m_mouseSense * 0.004f);
	// 회전 동기화
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
	//	ErrorMessage("회전 : " + to_string(Input::GetMouseMovePos().x));
	//}
	//else if (Input::GetMouseMovePos().x < 0.0f &&
	//		 m_MouseDirection != EDirection::Left)
	//{
	//	m_MouseDirection = EDirection::Left;
	//	m_prevMouseDir = p_MouseRotate.RotateSpeed = -m_mouseSense * 0.1f;
	//	p_MouseRotate.KeyValue = m_pParent->m_keyValue;
	//	PacketManager::Get().SendPacket((char*)&p_MouseRotate, sizeof(Packet_MouseRotate), PACKET_MouseRotate);
	//	ErrorMessage("회전 : " + to_string(Input::GetMouseMovePos().x));
	//}
	//else if (Input::GetMouseMovePos().x == 0.0f &&
	//		 m_MouseDirection != EDirection::Middle)
	//{
	//	m_MouseDirection = EDirection::Middle;
	//	m_prevMouseDir = p_MouseRotate.RotateSpeed = 0.0f;
	//	p_MouseRotate.KeyValue = m_pParent->m_keyValue;
	//	PacketManager::Get().SendPacket((char*)&p_MouseRotate, sizeof(Packet_MouseRotate), PACKET_MouseRotate);
	//	ErrorMessage("회전 : " + to_string(Input::GetMouseMovePos().x));
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
	Input::isDebug = false;
}

void PlayerController::UpdateStatus(const bool& infoUpdate) noexcept
{
	auto pUserInfo = PacketManager::Get().pMyInfo;
	m_moveSpeed = MoveSpeed + MoveSpeed * pUserInfo->StatDex * 0.1f;
	m_jumpPower = JumpPower;

	m_DelayEnemyPanel = 3.0f;
	m_DelayRespawn = 8.0f * 5.0f / (5.0f + pUserInfo->StatLuk);
	m_DelayLSkill = 0.7f * 5.0f / (5.0f + pUserInfo->StatDex);
	m_DelayDash = 2.5f * 5.0f / (5.0f + pUserInfo->StatDex);
	m_DelayRSkill = 4.0f * 5.0f / (5.0f + pUserInfo->StatDex);
	pUIManager->m_pLeftIcon->SetValue(m_curDelayDash, m_DelayDash);
	pUIManager->m_pRightIcon->SetValue(m_curDelayRSkill, m_DelayRSkill);
	m_RegenMP = 0.3f + pUserInfo->StatInt * 0.045f;
	m_maxMP = 1.0f + pUserInfo->StatInt * 0.2f;
	pUIManager->m_pMpBar->SetValue(m_curMP, m_maxMP);
	pUIManager->m_pExpProgress->SetValue(m_disEXP, m_NeedEXP);
	if (m_pParent != nullptr)
	{
		m_pParent->m_pPhysics->m_maxHP = 1.0f + pUserInfo->StatLuk * 0.2f;
		pUIManager->m_pHpBar->SetValue(m_pParent->GetHP(), m_pParent->m_pPhysics->m_maxHP);
		pUIManager->m_pInfoHP->SetString(to_wstring((int)(m_pParent->GetHP() * 100.0f)) + L" / " + to_wstring((int)(m_pParent->m_pPhysics->m_maxHP * 100.0f)));
		//pUIManager->m_pInfoTitle->SetString(m_pParent->m_myName);
		pUIManager->m_pInfoArmor->SetString(to_wstring((1.0f - m_pParent->m_pPhysics->m_armor) * 100.0f).substr(0, 4) + L" %");
	}
	if (infoUpdate)
	{
		pUIManager->m_pInfoMP->SetString(to_wstring((int)(m_curMP * 100.0f)) + L" / " + to_wstring((int)(m_maxMP * 100.0f)));
		pUIManager->m_pInfoMP->SetString(to_wstring((int)(m_curMP * 100.0f)) + L" / " + to_wstring((int)(m_maxMP * 100.0f)));
		pUIManager->m_pInfoEXP->SetString(to_wstring((int)(m_EXP * 100.0f)) + L" / " + to_wstring((int)(m_NeedEXP * 100.0f)));
		//pUIManager->m_pInfoName->SetString(pUserInfo->UserID);
		pUIManager->m_pInfoAttackSpeed->SetString(to_wstring(1.0f / (5.0f / (5.0f + pUserInfo->StatDex))).substr(0, 4));
		pUIManager->m_pInfoMoveSpeed->SetString(to_wstring(1.0f + pUserInfo->StatDex * 0.15f).substr(0, 4));
		pUIManager->m_pInfoLevel->SetString(to_wstring(pUserInfo->Level));
		pUIManager->m_pInfoDamage->SetString(to_wstring(1.0f + pUserInfo->StatStr * 0.15f).substr(0, 4));
		pUIManager->m_pInfoSP->SetString(to_wstring(m_statPoint));
		pUIManager->m_pInfoStr->SetString(to_wstring(pUserInfo->StatStr));
		pUIManager->m_pInfoDex->SetString(to_wstring(pUserInfo->StatDex));
		pUIManager->m_pInfoInt->SetString(to_wstring(pUserInfo->StatInt));
		pUIManager->m_pInfoLuk->SetString(to_wstring(pUserInfo->StatLuk));
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
			auto pCollider = pObj->GetCollider();
			{
				pPlayer->m_defencePoint = 2;
				((Collider*)pCollider)->CollisionEvent = nullptr;
				((Collider*)pCollider)->m_pivot = Vector3::Up * 40.0f * pObj->GetScale().x;
			}
		}
		else if (pObj->m_myName == L"Archer")
		{
			pPlayer->m_curCharacter = PlayerController::ECharacter::EArcher;
			auto pCollider = pObj->GetCollider();
			{
				pPlayer->m_defencePoint = 1;
				((Collider*)pCollider)->CollisionEvent = nullptr;
				((Collider*)pCollider)->m_pivot = Vector3::Up * 40.0f * pObj->GetScale().x;
			}
		}
		else if (pObj->m_myName == L"Mage")
		{
			pPlayer->m_curCharacter = PlayerController::ECharacter::EMage;
			auto pCollider = pObj->GetCollider();
			{
				pPlayer->m_defencePoint = 0;
				((Collider*)pCollider)->CollisionEvent = nullptr;
				((Collider*)pCollider)->m_pivot = Vector3::Up * 40.0f * pObj->GetScale().x;
			}
		}
		pObj->SetArmor(pPlayer->m_defencePoint);
		pPlayer->SendPhysicsInfo();
		if (pPlayer->m_pEffectFly != nullptr)
			ObjectManager::Get().DisableObject(pPlayer->m_pEffectFly);

		pPlayer->ResetOption();
		pPlayer->UpdateStatus();
		pPlayer->SetState(EPlayerState::Basic);
		// 상태창
		UIManager::Get().m_pInfoName->SetString(PacketManager::Get().pMyInfo->UserID);
		UIManager::Get().m_pInfoTitle->SetString(pObj->m_myName);

		SoundManager::Get().m_pListenerPos = &pObj->GetRoot()->GetPosition();
		PacketManager::Get().SendPacket((char*)PacketManager::Get().pMyInfo, (USHORT)(PS_UserInfo + PacketManager::Get().pMyInfo->DataSize), PACKET_SendUserInfo);
	};

	ObjectManager::PostFrameEvent.emplace(pEvent, this, pObject);
}

void PlayerController::DeadEvent() noexcept
{
	PacketManager::Get().SendPlaySound("SE_dead.mp3", m_pParent->GetPosition(), SoundRange);
	m_pParent->SetHP(0.0f);
	SetPosition(m_pParent->GetPosition());
	SetRotation(m_pParent->GetRotation());
	CutParent(true, true);
	m_curDelayRespawn = 0.0f;
	pUIManager->m_pRespawn->m_bRender = true;
	pUIManager->m_pRespawnBar->SetValue(m_curDelayRespawn, m_DelayRespawn);
}

void PlayerController::HitEvent(Collider* pTarget) noexcept
{
	m_curDelayEnemyPanel = m_DelayEnemyPanel;
	pUIManager->m_pEnemyPanel->m_bRender = true;

	m_pTargetEnemy = pTarget->m_pParent;
	pUIManager->m_pEnemyHP->SetValue(pTarget->m_pPhysics->m_disHP, pTarget->m_pPhysics->m_maxHP);
	pUIManager->m_pEnemyName->SetString(pTarget->m_pParent->m_myName);
}

void PlayerController::OperEXP(const float& value) noexcept
{
	m_EXP += value /*+ PacketManager::Get().pMyInfo->StatLuk * 0.1f*/;
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


void PlayerController::SendAnimTransform(const EAction& eAction, const ECharacter& eCharacter) noexcept
{
	static Packet_AnimTransform p_AnimTransform;

	// 이동 처리
	switch (eAction)
	{
	case EAction::Jump:
	{
		p_AnimTransform.Force = Vector3::Up * m_jumpPower;
	}	break;
	case EAction::Left:
	case EAction::RLeft:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetLeft();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::Right:
	case EAction::RRight:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetRight();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::Forward:
	case EAction::RForward:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetForward();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::Backward:
	case EAction::RBackward:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetBackward();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::ForwardLeft:
	case EAction::NForwardLeft:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetForward() + m_pParent->GetLeft();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::ForwardRight:
	case EAction::NForwardRight:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetForward() + m_pParent->GetRight();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::BackwardLeft:
	case EAction::NBackwardLeft:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetBackward() + m_pParent->GetLeft();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::BackwardRight:
	case EAction::NBackwardRight:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetBackward() + m_pParent->GetRight();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::Run:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetForward();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed * 2.0f;
	}	break;
	case EAction::RunLeft:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetForward() + m_pParent->GetLeft() * 0.5f;
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed * 2.0f;
	}	break;
	case EAction::RunRight:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetForward() + m_pParent->GetRight() * 0.5f;
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed * 2.0f;
	}	break;
	case EAction::Dash:
	{
		p_AnimTransform.Force = (Normalize(m_pParent->GetForward()) * m_moveSpeed * 2.2f + Vector3::Up * 30.0f);
		p_AnimTransform.Direction = Vector3::Zero;
	}	break;
	case EAction::DashLeft:
	{
		p_AnimTransform.Force = (Normalize(m_pParent->GetForward() + m_pParent->GetLeft() * 0.5f) * m_moveSpeed * 2.2f + Vector3::Up * 30.0f);
		p_AnimTransform.Direction = Vector3::Zero;
	}	break;
	case EAction::DashRight:
	{
		p_AnimTransform.Force = (Normalize(m_pParent->GetForward() + m_pParent->GetRight() * 0.5f) * m_moveSpeed * 2.2f + Vector3::Up * 30.0f);
		p_AnimTransform.Direction = Vector3::Zero;
	}	break;
	//case EAction::LSkill:
	default:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = ObjectManager::Cameras[ECamera::Main]->GetForward();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction);
		//p_AnimTransform.Direction = Vector3::Zero;
	}	break;
	}
	p_AnimTransform.KeyValue = m_pParent->m_keyValue;
	p_AnimTransform.UserSocket = PacketManager::Get().pMyInfo->UserSocket;
	p_AnimTransform.Position = m_pParent->GetPosition();
	p_AnimTransform.Rotation = m_pParent->GetRotation();
	m_prevRotY = p_AnimTransform.Rotation.y;
	p_AnimTransform.EAnimState = m_curAnim = eAction;
	p_AnimTransform.ECharacter = eCharacter;
	PacketManager::Get().SendPacket((char*)&p_AnimTransform, sizeof(Packet_AnimTransform), PACKET_SetAnimTransform);
}

void PlayerController::SendReqRespawn(const ECharacter& eCharacter) noexcept
{
	// 다른 캐릭일시
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
		// 상태 설정
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
			//m_stateList.try_emplace(EPlayerState::Dead, new PlayerStateDead());
			break;
		case PlayerController::EArcher:
			m_stateList.try_emplace(EPlayerState::Basic, new ArcherStateBasic());
			m_stateList.try_emplace(EPlayerState::LSkill, new ArcherStateLSkill());
			m_stateList.try_emplace(EPlayerState::RSkill, new ArcherStateRSkill());
			m_stateList.try_emplace(EPlayerState::Dash, new ArcherStateDash());
			//m_stateList.try_emplace(EPlayerState::Dead, new PlayerStateDead());
			break;
		case PlayerController::EMage:
			m_stateList.try_emplace(EPlayerState::Basic,  new MageStateBasic());
			m_stateList.try_emplace(EPlayerState::LSkill, new MageStateLSkill());
			m_stateList.try_emplace(EPlayerState::RSkill, new MageStateRSkill());
			m_stateList.try_emplace(EPlayerState::Dash,   new MageStateDash());
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