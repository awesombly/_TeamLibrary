#include "PlayerState.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "PlayerController.h"
#include "PacketManager.h"
#include "UIManager.h"


bool PlayerState::CommonProcess(const float& spf) noexcept
{
	return true;
}

// 기본
void PlayerStateBasic::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->UpdateStatus(false);
}
bool PlayerStateBasic::Process(const float& spf) noexcept
{
	if (m_pOwner->m_berserkFrame >= 1.0f)
	{
		if ((m_pOwner->m_berserkFrame -= spf) <= 1.0f)
		{
			// 변수 설정
			m_pOwner->GetParent()->SetArmor(m_pOwner->m_defencePoint -= 10);
			m_pOwner->SendPhysicsInfo();

			m_pOwner->m_berserkFrame = 0.0f;
			//m_pOwner->m_DelayFrame = 0.5f;
			m_pOwner->m_curDelayRSkill = m_pOwner->m_DelayRSkill;
			m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack2;
			return true;
		}
	}
	if (m_pOwner->m_DelayFrame >= 0.0f)
	{
		m_pOwner->m_DelayFrame -= spf;
		return false;
	}
	/// 공통 작업
	//CommonProcess(spf);

	m_pOwner->m_eAction = PlayerController::EAction::Idle;
	if (Input::GetKeyState('W') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Forward);
	}
	if (Input::GetKeyState('S') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Backward);
	}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Left);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Right);
	}
	if (Input::GetKeyState('1') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot1->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot1->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot1->DelItem();
		}
	}
	if (Input::GetKeyState('2') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot2->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot2->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot2->DelItem();
		}
	}
	if (Input::GetKeyState('3') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot3->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot3->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot3->DelItem();
		}
	}

	//static bool isFly = false;
	//if (Input::GetKeyState(VK_SPACE) == EKeyState::DOWN)
	//{
	//	if (m_pOwner->m_pParent->isGround())
	//	{
	//		m_pOwner->m_eAction = PlayerController::EAction::Jump;
	//	}
		//else
		//{
		//	// 날기
		//	isFly = true;
		//	m_pOwner->m_eAction = PlayerController::EAction::Fly;
		//	m_pOwner->m_pEffectFly = ObjectManager::Get().TakeObject(L"EFly");
		//	m_pOwner->m_pEffectFly->SetParent(m_pOwner->m_pParent);
		//}
	//}
	//if (isFly && Input::GetKeyState(VK_SPACE) == EKeyState::HOLD)
	//{
	//	m_pOwner->m_curMP -= (0.65f + m_pOwner->m_RegenMP) * spf;
	//	if (m_pOwner->m_curMP <= 0.0f)
	//	{
	//		isFly = false;
	//		m_pOwner->m_eAction = PlayerController::EAction::FlyEnd;
	//		if (m_pOwner->m_pEffectFly != nullptr)
	//		{
	//			ObjectManager::Get().DisableObject(m_pOwner->m_pEffectFly);
	//			m_pOwner->m_pEffectFly = nullptr;
	//		}
	//	}
	//}
	//if (Input::GetKeyState(VK_SPACE) == EKeyState::UP)
	//{
	//	isFly = false;
	//	m_pOwner->m_eAction = PlayerController::EAction::FlyEnd;
	//	if (m_pOwner->m_pEffectFly != nullptr)
	//	{
	//		ObjectManager::Get().DisableObject(m_pOwner->m_pEffectFly);
	//		m_pOwner->m_pEffectFly = nullptr;
	//	}
	//}


	// L 클릭
	if (Input::GetKeyState(EMouseButton::Left) == EKeyState::DOWN &&
		m_pOwner->m_curDelayLSkill <= 0.0f &&
		m_pOwner->m_curMP >= 0.4f)
	{
		m_pOwner->m_curMP -= 0.4f;
		m_pOwner->m_curDelayLSkill = m_pOwner->m_DelayLSkill;
		m_pOwner->SetState(EPlayerState::LSkill);
		return true;
	}

	// R 클릭
	if (Input::GetKeyState(VK_RBUTTON) == EKeyState::DOWN &&
		m_pOwner->m_curDelayRSkill <= 0.0f &&
		m_pOwner->m_berserkFrame <= 0.0f)
	{
		//m_pOwner->m_curDelayRSkill = m_pOwner->m_DelayRSkill;
		//m_curDelayDash += 0.3f;
		//m_pOwner->m_eAction = PlayerController::EAction::RSkill;
		m_pOwner->SetState(EPlayerState::RSkill);
		return true;
	}

	// 스페이스
	if (Input::GetKeyState(VK_SPACE) == EKeyState::DOWN &&
		m_pOwner->m_curDelayDash <= 0.0f)
	{
		m_pOwner->m_curDelayDash = m_pOwner->m_DelayDash;
		m_pOwner->SetState(EPlayerState::Special);
		return true;
	}

	// 대쉬로
	if (Input::GetKeyState(VK_SHIFT) == EKeyState::DOWN)
	{
		m_pOwner->SetState(EPlayerState::Run);
		return true;
	}

	return true;
}

// LSkill
void PlayerStateLSkill::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->m_eAction = PlayerController::EAction::LSkill;
	m_pOwner->m_DelayFrame = 0.5f;
	m_pOwner->m_moveSpeed *= 0.4f;
}
bool PlayerStateLSkill::Process(const float& spf) noexcept
{
	if (m_pOwner->m_berserkFrame >= 1.0f)
	{
		if ((m_pOwner->m_berserkFrame -= spf) <= 1.0f)
		{
			// 변수 설정
			m_pOwner->GetParent()->SetArmor(m_pOwner->m_defencePoint -= 10);
			m_pOwner->SendPhysicsInfo();

			m_pOwner->m_berserkFrame = 0.0f;
			//m_pOwner->m_DelayFrame = 0.5f;
			m_pOwner->m_curDelayRSkill = m_pOwner->m_DelayRSkill;
			m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack2;
			return true;
		}
	}
	m_pOwner->m_DelayFrame -= spf;
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->SetState(EPlayerState::Basic);
		m_pOwner->m_eAction = PlayerController::EAction::Attack;
		m_pOwner->m_DelayFrame = 0.5f;
		m_pOwner->m_curDelayLSkill = m_pOwner->m_DelayLSkill;
		return true;
	}

	m_pOwner->m_eAction = PlayerController::EAction::NIdle;
	if (Input::GetKeyState('W') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NForward);
	}
	if (Input::GetKeyState('S') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NBackward);
	}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NLeft);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NRight);
	}
	return true;
}


// RSkill
void PlayerStateRSkill::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->m_eAction = PlayerController::EAction::RSkill;
	m_pOwner->m_DelayFrame = 0.3f;
	m_pOwner->m_moveSpeed = 0.0f;
}
bool PlayerStateRSkill::Process(const float& spf) noexcept
{
	m_pOwner->m_DelayFrame -= spf;
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->SetState(EPlayerState::Basic);
		m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack;
		// 변수 설정
		m_pOwner->GetParent()->SetArmor(m_pOwner->m_defencePoint += 10);
		m_pOwner->SendPhysicsInfo();

		m_pOwner->m_berserkFrame = 18.0f;
		m_pOwner->m_DelayFrame = 0.7f;
		m_pOwner->m_curDelayRSkill = m_pOwner->m_DelayRSkill;

		return true;
	}
	
	m_pOwner->m_eAction = PlayerController::EAction::NIdle;
	//if (Input::GetKeyState('W') == EKeyState::HOLD)
	//{
	//	m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NForward);
	//}
	//if (Input::GetKeyState('S') == EKeyState::HOLD)
	//{
	//	m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NBackward);
	//}
	//if (Input::GetKeyState('A') == EKeyState::HOLD)
	//{
	//	m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NLeft);
	//}
	//if (Input::GetKeyState('D') == EKeyState::HOLD)
	//{
	//	m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NRight);
	//}
	return true;
}

// 방어자세
void PlayerStateGuard::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->m_eAction = PlayerController::EAction::Special;
	m_pOwner->m_DelayFrame = 0.7f;
	m_pOwner->m_moveSpeed *= 0.8f;
	// 변수 설정
	m_pOwner->GetParent()->SetGravityScale(3.0f);
	m_pOwner->GetParent()->m_pPhysics->m_damping = 2.0f;
	m_pOwner->GetParent()->SetArmor(m_pOwner->m_defencePoint += 45);
	m_pOwner->SendPhysicsInfo();
	
}
bool PlayerStateGuard::Process(const float& spf) noexcept
{
	if (m_pOwner->m_DelayFrame >= 0.0f)
	{
		m_pOwner->m_DelayFrame -= spf;
		return true;
	}

	m_pOwner->m_eAction = PlayerController::EAction::Idle;
	//if (Input::GetKeyState('W') == EKeyState::HOLD)
	//{
	//	m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Forward);
	//}
	//if (Input::GetKeyState('S') == EKeyState::HOLD)
	//{
	//	m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Backward);
	//}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Left);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Right);
	}

	// 기본으로
	if (Input::GetKeyState(VK_SPACE) == EKeyState::FREE)
	{
		if (m_pOwner->GetParent() != nullptr)
		{
			m_pOwner->GetParent()->SetGravityScale(3.0f);
			m_pOwner->GetParent()->m_pPhysics->m_damping = 0.3f;
			m_pOwner->GetParent()->SetArmor(max(m_pOwner->m_defencePoint -= 45, 2));
			m_pOwner->SendPhysicsInfo();
		}
		m_pOwner->SetState(EPlayerState::Basic);
	}
	return true;
}


// 대쉬
void PlayerStateRun::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->m_eAction = PlayerController::EAction::Run;
	if (m_pOwner->m_pEffectFly != nullptr)
		ObjectManager::Get().DisableObject(m_pOwner->m_pEffectFly);
	m_pOwner->m_pEffectFly = ObjectManager::Get().TakeObject(L"EPDustStay", false);
	m_pOwner->m_pEffectFly->SetParent(pOwner);
	m_pOwner->m_pEffectFly->SetPosition(Vector3::Up * 5.0f);
}
bool PlayerStateRun::Process(const float& spf) noexcept
{
	if (m_pOwner->m_berserkFrame >= 1.0f)
	{
		if ((m_pOwner->m_berserkFrame -= spf) <= 1.0f)
		{
			// 변수 설정
			m_pOwner->GetParent()->SetArmor(m_pOwner->m_defencePoint -= 10);
			m_pOwner->SendPhysicsInfo();

			m_pOwner->m_berserkFrame = 0.0f;
			//m_pOwner->m_DelayFrame = 0.5f;
			m_pOwner->m_curDelayRSkill = m_pOwner->m_DelayRSkill;
			m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack2;
			return true;
		}
	}
	m_pOwner->m_eAction = PlayerController::EAction::Run;

	if (m_pOwner->m_DelayFrame >= 0.0f)
	{
		m_pOwner->m_DelayFrame -= spf;
		return false;
	}
	/// 공통 작업
	//CommonProcess(spf);
	                                                     
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = PlayerController::EAction::RunLeft;
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = PlayerController::EAction::RunRight;
	}

	// 기본으로
	if (Input::GetKeyState(VK_SHIFT) == EKeyState::UP)
	{
		m_pOwner->SetState(EPlayerState::Basic);
		if (m_pOwner->m_pEffectFly != nullptr)
			ObjectManager::Get().DisableObject(m_pOwner->m_pEffectFly);
		m_pOwner->m_pEffectFly = nullptr;
	}

	return true;
}


/// ============================= 궁수 ===================================

// 기본
void ArcherStateBasic::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->UpdateStatus(false);
}
bool ArcherStateBasic::Process(const float& spf) noexcept
{
	m_pOwner->m_eAction = PlayerController::EAction::Idle;
	if (m_pOwner->m_DelayFrame >= 0.0f)
	{
		m_pOwner->m_DelayFrame -= spf;
		return false;
	}

	if (Input::GetKeyState('W') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Forward);
	}
	if (Input::GetKeyState('S') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Backward);
	}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Left);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Right);
	}
	if (Input::GetKeyState('1') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot1->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot1->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot1->DelItem();
		}
	}
	if (Input::GetKeyState('2') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot2->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot2->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot2->DelItem();
		}
	}
	if (Input::GetKeyState('3') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot3->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot3->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot3->DelItem();
		}
	}


	// L 클릭
	if (Input::GetKeyState(EMouseButton::Left) == EKeyState::DOWN &&
		m_pOwner->m_curDelayLSkill <= 0.0f &&
		m_pOwner->m_curMP >= 0.3f)
	{
		//m_pOwner->m_curMP -= 0.4f;
		//m_pOwner->m_curDelayLSkill = m_pOwner->m_DelayLSkill;
		m_pOwner->SetState(EPlayerState::LSkill);
		return true;
	}


	// 구르기
	if (Input::GetKeyState(VK_SHIFT) == EKeyState::DOWN &&
		m_pOwner->m_pParent->isGround() &&
		m_pOwner->m_curDelayDash <= 0.0f)
	{
		m_pOwner->m_curDelayDash = m_pOwner->m_DelayDash;

		m_pOwner->m_eAction = PlayerController::EAction::Dash;
		if (Input::GetKeyState('A') == EKeyState::HOLD)
			m_pOwner->m_eAction = PlayerController::EAction::DashLeft;
		if (Input::GetKeyState('D') == EKeyState::HOLD)
			m_pOwner->m_eAction = PlayerController::EAction::DashRight;
		m_pOwner->SetState(EPlayerState::Dash);
		return true;
	}

	// 화살비
	if (Input::GetKeyState(VK_RBUTTON) == EKeyState::DOWN &&
		m_pOwner->m_curDelayRSkill <= 0.0f)
	{
		m_pOwner->m_curDelayRSkill = m_pOwner->m_DelayRSkill;
		m_pOwner->SetState(EPlayerState::RSkill);
		return true;
	}

	return true;
}


// LSkill
void ArcherStateLSkill::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->m_eAction = PlayerController::EAction::LSkill;
	m_pOwner->m_DelayFrame = 0.7f;
	m_pOwner->m_moveSpeed *= 0.4f;
	m_pOwner->m_chargeCount = 0.0f;
}
bool ArcherStateLSkill::Process(const float& spf) noexcept
{
	m_pOwner->m_DelayFrame -= (spf + spf * PacketManager::Get().pMyInfo->StatDex * 0.1f);
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->m_chargeCount += (spf + spf * PacketManager::Get().pMyInfo->StatDex * 0.1f);
		// 차징
		if (m_pOwner->m_chargeCount >= 3.0f)
		{
			m_pOwner->m_curMP = max(m_pOwner->m_curMP - 0.4f, 0.0f);
			// 강제 발사
			m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack2;
			m_pOwner->m_DelayFrame = 0.5f;
			m_pOwner->SetState(EPlayerState::Wait);
			return true;
		}
		else if (m_pOwner->m_chargeCount >= 2.2f)
		{
			m_pOwner->m_curMP -= 0.2f;
			m_pOwner->m_eAction = PlayerController::EAction::LCharge2;
			m_pOwner->m_DelayFrame = 1.0f;
			return true;
		}
		else if (m_pOwner->m_chargeCount >= 1.2f)
		{
			m_pOwner->m_curMP -= 0.2f;
			m_pOwner->m_eAction = PlayerController::EAction::LCharge1;
			m_pOwner->m_DelayFrame = 1.0f;
			return true;
		}
		else
		{
			m_pOwner->m_eAction = PlayerController::EAction::LCharging;
			m_pOwner->m_DelayFrame = 0.55f;
			return true;
		}
	}

	m_pOwner->m_chargeCount += (spf + spf * PacketManager::Get().pMyInfo->StatDex * 0.1f);
	if (Input::GetKeyState(VK_LBUTTON) == EKeyState::FREE || 
		m_pOwner->m_curMP <= 0.0f)
	{
		if (m_pOwner->m_chargeCount >= 0.6f)
		{
			m_pOwner->m_curMP = max(m_pOwner->m_curMP - 0.4f, 0.0f);
			if (m_pOwner->m_chargeCount <= 1.1f)
			{
				m_pOwner->m_eAction = PlayerController::EAction::Attack;
			}
			else if (m_pOwner->m_chargeCount <= 2.1f)
			{
				m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack;
			}
			else
			{
				m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack2;
			}
			m_pOwner->m_DelayFrame = 0.55f;
			m_pOwner->SetState(EPlayerState::Wait);
			return true;
		}
	}


	m_pOwner->m_eAction = PlayerController::EAction::NIdle;
	if (Input::GetKeyState('W') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NForward);
	}
	if (Input::GetKeyState('S') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NBackward);
	}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NLeft);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NRight);
	}
	return true;
}

// RSkill
void ArcherStateRSkill::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->m_eAction = PlayerController::EAction::RSkill;
	m_pOwner->m_DelayFrame = 0.3f;
	m_pOwner->m_moveSpeed *= 0.4f;
}
bool ArcherStateRSkill::Process(const float& spf) noexcept
{
	m_pOwner->m_DelayFrame -= spf;
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->SetState(EPlayerState::Basic);
		m_pOwner->m_eAction = PlayerController::EAction::Special;
		m_pOwner->m_DelayFrame = 0.5f;
		return true;
	}

	m_pOwner->m_eAction = PlayerController::EAction::NIdle;
	if (Input::GetKeyState('W') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NForward);
	}
	if (Input::GetKeyState('S') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NBackward);
	}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NLeft);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NRight);
	}
	return true;
}

// Dash
void ArcherStateDash::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->m_DelayFrame = 1.25f;
	//m_pOwner->m_moveSpeed *= 0.15f;
}
bool ArcherStateDash::Process(const float& spf) noexcept
{
	m_pOwner->m_DelayFrame -= spf;
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->SetState(EPlayerState::Basic);
		return true;
	}

	m_pOwner->m_eAction = PlayerController::EAction::NIdle;
	//if (Input::GetKeyState('W') == EKeyState::HOLD)
	//{
	//	m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NForward);
	//}
	//if (Input::GetKeyState('S') == EKeyState::HOLD)
	//{
	//	m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NBackward);
	//}
	//if (Input::GetKeyState('A') == EKeyState::HOLD)
	//{
	//	m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NLeft);
	//}
	//if (Input::GetKeyState('D') == EKeyState::HOLD)
	//{
	//	m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NRight);
	//}
	return true;
}

// 대기
void ArcherStateWait::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
}
bool ArcherStateWait::Process(const float& spf) noexcept
{
	m_pOwner->m_DelayFrame -= (spf + spf * PacketManager::Get().pMyInfo->StatDex * 0.05f);
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->SetState(EPlayerState::Basic);
		return true;
	}

	m_pOwner->m_eAction = PlayerController::EAction::NIdle;
	if (Input::GetKeyState('W') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NForward);
	}
	if (Input::GetKeyState('S') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NBackward);
	}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NLeft);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NRight);
	}
	return true;
}


/// ============================= 법사 ===================================

// 기본
void MageStateBasic::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->UpdateStatus(false);
}
bool MageStateBasic::Process(const float& spf) noexcept
{
	if (m_pOwner->m_DelayFrame >= 0.0f)
	{
		m_pOwner->m_DelayFrame -= spf;
		return false;
	}

	m_pOwner->m_eAction = PlayerController::EAction::Idle;
	if (Input::GetKeyState('W') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Forward);
	}
	if (Input::GetKeyState('S') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Backward);
	}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Left);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::Right);
	}
	if (Input::GetKeyState('1') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot1->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot1->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot1->DelItem();
		}
	}
	if (Input::GetKeyState('2') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot2->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot2->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot2->DelItem();
		}
	}
	if (Input::GetKeyState('3') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot3->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot3->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot3->DelItem();
		}
	}

	// L 클릭
	if (Input::GetKeyState(EMouseButton::Left) == EKeyState::DOWN &&
		m_pOwner->m_curDelayLSkill <= 0.0f &&
		m_pOwner->m_curMP >= 0.4f)
	{
		m_pOwner->m_curMP -= 0.4f;
		m_pOwner->SetState(EPlayerState::LSkill);
		return true;
	}

	// R 클릭
	if (Input::GetKeyState(EMouseButton::Right) == EKeyState::DOWN &&
		m_pOwner->m_curDelayRSkill <= 0.0f &&
		m_pOwner->m_curMP >= 0.8f)
	{
		m_pOwner->m_curMP -= 0.8f;
		m_pOwner->SetState(EPlayerState::RSkill);
		return true;
	}

	// 텔포
	if (Input::GetKeyState(VK_SHIFT) == EKeyState::DOWN &&
		m_pOwner->m_curDelayDash <= 0.0f)
	{
		//m_pOwner->m_curMP -= 0.4f;
		m_pOwner->SetState(EPlayerState::Dash);
		return true;
	}

	return true;
}


// LSkill
void MageStateLSkill::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->m_eAction = PlayerController::EAction::LSkill;
	m_pOwner->m_DelayFrame = 0.6f;
	m_pOwner->m_moveSpeed *= 0.6f;
}
bool MageStateLSkill::Process(const float& spf) noexcept
{
	m_pOwner->m_DelayFrame -= spf;

	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->m_eAction = PlayerController::EAction::Attack;
		m_pOwner->SetState(EPlayerState::Basic);
		m_pOwner->m_curDelayLSkill = m_pOwner->m_DelayLSkill;
		m_pOwner->m_DelayFrame = 0.6f;
		return true;
	}

	m_pOwner->m_eAction = PlayerController::EAction::NIdle;
	if (Input::GetKeyState('W') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NForward);
	}
	if (Input::GetKeyState('S') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NBackward);
	}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NLeft);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NRight);
	}
	return true;
}

// RSkill
void MageStateRSkill::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->m_eAction = PlayerController::EAction::RSkill;
	m_pOwner->m_DelayFrame = 1.1f;
	m_pOwner->m_moveSpeed *= 0.1f;
}
bool MageStateRSkill::Process(const float& spf) noexcept
{
	m_pOwner->m_DelayFrame -= spf;

	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack;
		m_pOwner->SetState(EPlayerState::Basic);
		m_pOwner->m_curDelayRSkill = m_pOwner->m_DelayRSkill;
		m_pOwner->m_DelayFrame = 1.0f;
		return true;
	}

	m_pOwner->m_eAction = PlayerController::EAction::NIdle;
	if (Input::GetKeyState('W') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NForward);
	}
	if (Input::GetKeyState('S') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NBackward);
	}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NLeft);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NRight);
	}
	return true;
}

// 텔포
void MageStateDash::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->m_eAction = PlayerController::EAction::Special;
	m_pOwner->m_DelayFrame = 0.7f;
	m_pOwner->m_moveSpeed *= 0.2f;
}
bool MageStateDash::Process(const float& spf) noexcept
{
	m_pOwner->m_DelayFrame -= spf;
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		// 준비
		m_pOwner->m_eAction = PlayerController::EAction::Special2;
		m_pOwner->m_DelayFrame = 10.0f;
		return true;
	}
	if (m_pOwner->m_DelayFrame <= 8.4f &&
		m_pOwner->m_DelayFrame > 5.0f)
	{
		// 이동
		m_pOwner->SetState(EPlayerState::Basic);
		m_pOwner->m_eAction = PlayerController::EAction::Special3;
		m_pOwner->m_curDelayDash = m_pOwner->m_DelayDash;
		m_pOwner->m_DelayFrame = 1.0f;
		return true;
	}

	m_pOwner->m_eAction = PlayerController::EAction::NIdle;
	if (Input::GetKeyState('W') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NForward);
	}
	if (Input::GetKeyState('S') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NBackward);
	}
	if (Input::GetKeyState('A') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NLeft);
	}
	if (Input::GetKeyState('D') == EKeyState::HOLD)
	{
		m_pOwner->m_eAction = (PlayerController::EAction)(m_pOwner->m_eAction + PlayerController::EAction::NRight);
	}
	return true;
}