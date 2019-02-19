#include "PlayerState.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "PlayerController.h"
#include "PacketManager.h"
#include "UIManager.h"

bool BerserkFrame(PlayerController* pOwner, const float& spf)	noexcept
{
	pOwner->GetParent()->m_pPhysics->m_curHP -= spf * (0.06f + pOwner->m_RegenHP);
	if ((pOwner->m_berserkFrame -= spf) <= 1.0f || pOwner->GetParent()->GetHP() <= 0.1f)
	{
		// 변수 설정
		pOwner->m_defencePoint = max(pOwner->m_defencePoint - 10, 2);
		pOwner->GetParent()->SetArmor(pOwner->m_defencePoint + pOwner->m_upgradeArmor);
		pOwner->SendPhysicsInfo();
	
		pOwner->m_berserkFrame = 0.0f;
		pOwner->m_curDelayRSkill = pOwner->m_DelayRSkill;
		pOwner->m_eAction = PlayerController::EAction::Special3;
		return true;
	}
	return false;
}

// 기본
void PlayerStateBasic::StateInit(PlayerController* pOwner) noexcept
{
	m_pOwner = pOwner;
	m_pOwner->UpdateStatus(false);
}
bool PlayerStateBasic::Process(const float& spf) noexcept
{
	if (m_pOwner->m_berserkFrame >= 1.0f && 
		BerserkFrame(m_pOwner, spf))
	{
		return true;
	}

	if (m_pOwner->m_DelayFrame >= 0.0f)
	{
		m_pOwner->m_DelayFrame -= spf * PacketManager::Get().pMyInfo->MotionRate;
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
			m_pOwner->m_DelayFrame = 1.0f;
		}
	}
	if (Input::GetKeyState('2') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot2->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot2->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot2->DelItem();
			m_pOwner->m_DelayFrame = 1.0f;
		}
	}
	if (Input::GetKeyState('3') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot3->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot3->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot3->DelItem();
			m_pOwner->m_DelayFrame = 1.0f;
		}
	}


	// L 클릭
	if (Input::GetKeyState(VK_LBUTTON) == EKeyState::DOWN &&
		m_pOwner->m_curDelayLSkill <= 0.0f &&
		m_pOwner->m_curMP >= 0.3f &&
		!UIManager::Get().m_pMouseIcon->m_bRender)
	{
		m_pOwner->m_curMP -= 0.3f;
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
	m_pOwner->m_DelayFrame = 0.3f;
	m_pOwner->m_moveSpeed *= 0.25f;
	m_pOwner->m_inputCombo = false;
	m_pOwner->m_comboCount = 0;
}
bool PlayerStateLSkill::Process(const float& spf) noexcept
{
	if (m_pOwner->m_berserkFrame >= 1.0f &&
		BerserkFrame(m_pOwner, spf))
	{
		return true;
	}
	
	if (Input::GetKeyState(VK_LBUTTON) == EKeyState::DOWN &&
		!UIManager::Get().m_pMouseIcon->m_bRender)
	{
		m_pOwner->m_inputCombo = true;
	}
	m_pOwner->m_DelayFrame -= spf * PacketManager::Get().pMyInfo->MotionRate;
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		static float forwardRate = 1.0f;
		forwardRate = 1.0f;
		if (Input::GetKeyState('W') == EKeyState::HOLD)
			forwardRate = 1.5f;
		if (Input::GetKeyState('S') == EKeyState::HOLD)
			forwardRate = 0.35f;
		switch (m_pOwner->m_comboCount)
		{
		case 0:
		{
			// 전진
			++m_pOwner->m_comboCount;
			m_pOwner->m_DelayFrame = 0.2f;
			m_pOwner->GetParent()->AddForce(m_pOwner->GetParent()->GetForward() * 600.0f * forwardRate);
		}	break;
		case 1:
		{
			// 어택1
			++m_pOwner->m_comboCount;
			m_pOwner->m_eAction = PlayerController::EAction::Attack;
			m_pOwner->m_DelayFrame = 0.45f;
		}	break;
		case 2:
		{
			// 어택2동작
			if (m_pOwner->m_inputCombo)
			{
				m_pOwner->m_inputCombo = false;
				++m_pOwner->m_comboCount;
				m_pOwner->m_eAction = PlayerController::EAction::LCharge1;
				m_pOwner->m_DelayFrame = 0.35f;
				m_pOwner->m_curMP -= 0.25f;
			}
			else
			{
				m_pOwner->SetState(EPlayerState::Basic);
				m_pOwner->m_eAction = PlayerController::EAction::Wait;
				m_pOwner->m_DelayFrame = 0.2f;
				m_pOwner->m_curDelayLSkill = m_pOwner->m_DelayLSkill;
			}
		}	break;
		case 3:
		{
			// 전진
			++m_pOwner->m_comboCount;
			m_pOwner->m_DelayFrame = 0.2f;
			m_pOwner->GetParent()->AddForce(m_pOwner->GetParent()->GetForward() * 800.0f * forwardRate);
		}	break;
		case 4:
		{
			// 어택2
			++m_pOwner->m_comboCount;
			m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack;
			m_pOwner->m_DelayFrame = 0.45f;
		}	break;
		case 5:
		{
			// 어택3동작
			if (m_pOwner->m_inputCombo)
			{
				m_pOwner->m_inputCombo = false;
				++m_pOwner->m_comboCount;
				m_pOwner->m_eAction = PlayerController::EAction::LCharge2;
				m_pOwner->m_DelayFrame = 0.6f;
				m_pOwner->m_moveSpeed = 0.0f;
				m_pOwner->m_curMP -= 0.25f;
			}
			else
			{
				m_pOwner->SetState(EPlayerState::Basic);
				m_pOwner->m_eAction = PlayerController::EAction::Wait;
				m_pOwner->m_DelayFrame = 0.2f;
				m_pOwner->m_curDelayLSkill = m_pOwner->m_DelayLSkill * 1.2f;
			}
		}	break;
		case 6:
		{
			// 전진
			++m_pOwner->m_comboCount;
			m_pOwner->m_DelayFrame = 0.2f;
			m_pOwner->GetParent()->AddForce(m_pOwner->GetParent()->GetForward() * 1000.0f * forwardRate);
		}	break;
		case 7:
		{
			// 어택3
			++m_pOwner->m_comboCount;
			m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack2;
			m_pOwner->m_DelayFrame = 0.85f;
		}	break;
		case 8:
		{
			// 끝
			m_pOwner->SetState(EPlayerState::Basic);
			m_pOwner->m_eAction = PlayerController::EAction::Wait;
			m_pOwner->m_curDelayLSkill = m_pOwner->m_DelayLSkill * 1.4f;
		}	break;
		default:
			break;
		}
		
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
	m_pOwner->m_DelayFrame -= spf * PacketManager::Get().pMyInfo->MotionRate;
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->SetState(EPlayerState::Basic);
		m_pOwner->m_eAction = PlayerController::EAction::Special2;
		// 변수 설정
		m_pOwner->m_defencePoint += 10;
		m_pOwner->GetParent()->SetArmor(m_pOwner->m_defencePoint + m_pOwner->m_upgradeArmor);
		m_pOwner->SendPhysicsInfo();

		m_pOwner->m_berserkFrame = 18.0f;
		m_pOwner->m_DelayFrame = 0.7f;
		m_pOwner->m_curDelayRSkill = m_pOwner->m_DelayRSkill;

		return true;
	}
	
	m_pOwner->m_eAction = PlayerController::EAction::NIdle;
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
	m_pOwner->m_defencePoint += 45;
	m_pOwner->GetParent()->SetArmor(m_pOwner->m_defencePoint + m_pOwner->m_upgradeArmor);
	m_pOwner->SendPhysicsInfo();
	
}
bool PlayerStateGuard::Process(const float& spf) noexcept
{
	if (m_pOwner->m_DelayFrame >= 0.0f)
	{
		m_pOwner->m_DelayFrame -= spf * PacketManager::Get().pMyInfo->MotionRate;
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
			m_pOwner->m_defencePoint = max(m_pOwner->m_defencePoint - 45, 2);
			m_pOwner->GetParent()->SetArmor(m_pOwner->m_defencePoint + m_pOwner->m_upgradeArmor);
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
	m_pOwner->m_pEffectFly->SetPosition(Vector3::Up * 15.0f);
}
bool PlayerStateRun::Process(const float& spf) noexcept
{
	if (m_pOwner->m_berserkFrame >= 1.0f &&
		BerserkFrame(m_pOwner, spf))
	{
		return true;
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
		m_pOwner->m_DelayFrame -= spf * PacketManager::Get().pMyInfo->MotionRate;
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
			m_pOwner->m_DelayFrame = 1.0f;
		}
	}
	if (Input::GetKeyState('2') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot2->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot2->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot2->DelItem();
			m_pOwner->m_DelayFrame = 1.0f;
		}
	}
	if (Input::GetKeyState('3') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot3->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot3->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot3->DelItem();
			m_pOwner->m_DelayFrame = 1.0f;
		}
	}


	// L 클릭
	if (Input::GetKeyState(VK_LBUTTON) == EKeyState::DOWN &&
		m_pOwner->m_curDelayLSkill <= 0.0f &&
		m_pOwner->m_curMP >= 0.3f &&
		!UIManager::Get().m_pMouseIcon->m_bRender)
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
	m_pOwner->m_DelayFrame -= (spf * PacketManager::Get().pMyInfo->MotionRate);
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->m_chargeCount += (spf * PacketManager::Get().pMyInfo->MotionRate);
		// 차징
		if (m_pOwner->m_chargeCount >= 3.0f)
		{
			m_pOwner->m_curMP = max(m_pOwner->m_curMP - 0.4f, 0.0f);
			// 강제 발사
			m_pOwner->SetState(EPlayerState::Wait);
			m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack2;
			m_pOwner->m_DelayFrame = 0.5f;
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

	m_pOwner->m_chargeCount += (spf * PacketManager::Get().pMyInfo->MotionRate);
	if (Input::GetKeyState(VK_LBUTTON) == EKeyState::FREE || 
		m_pOwner->m_curMP <= 0.0f &&
		!UIManager::Get().m_pMouseIcon->m_bRender)
	{
		if (m_pOwner->m_chargeCount >= 0.6f)
		{
			m_pOwner->m_curMP = max(m_pOwner->m_curMP - 0.2f, 0.0f);
			m_pOwner->SetState(EPlayerState::Wait);
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
	m_pOwner->m_DelayFrame = 0.35f;
	m_pOwner->m_moveSpeed *= 0.4f;
}
bool ArcherStateRSkill::Process(const float& spf) noexcept
{
	m_pOwner->m_DelayFrame -= spf * PacketManager::Get().pMyInfo->MotionRate;
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->SetState(EPlayerState::Wait);
		m_pOwner->m_eAction = PlayerController::EAction::Special;
		m_pOwner->m_DelayFrame = 0.65f;
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
	m_pOwner->m_DelayFrame -= spf * PacketManager::Get().pMyInfo->MotionRate;
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->SetState(EPlayerState::Wait);
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
	m_pOwner->m_eAction = PlayerController::EAction::Wait;
}
bool ArcherStateWait::Process(const float& spf) noexcept
{
	m_pOwner->m_DelayFrame -= (spf * PacketManager::Get().pMyInfo->MotionRate);
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
		m_pOwner->m_DelayFrame -= spf * PacketManager::Get().pMyInfo->MotionRate;
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
			m_pOwner->m_DelayFrame = 1.0f;
		}
	}
	if (Input::GetKeyState('2') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot2->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot2->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot2->DelItem();
			m_pOwner->m_DelayFrame = 1.0f;
		}
	}
	if (Input::GetKeyState('3') == EKeyState::DOWN)
	{
		if (!UIManager::Get().m_pSlot3->m_bEmpty)
		{
			m_pOwner->m_ItemList[UIManager::Get().m_pSlot3->m_pItem->m_pIndexList[0]](m_pOwner, nullptr);
			UIManager::Get().m_pSlot3->DelItem();
			m_pOwner->m_DelayFrame = 1.0f;
		}
	}

	// L 클릭
	if (Input::GetKeyState(VK_LBUTTON) == EKeyState::DOWN &&
		m_pOwner->m_curDelayLSkill <= 0.0f &&
		m_pOwner->m_curMP >= 0.3f &&
		!UIManager::Get().m_pMouseIcon->m_bRender)
	{
		m_pOwner->m_curMP -= 0.3f;
		m_pOwner->SetState(EPlayerState::LSkill);
		return true;
	}

	// R 클릭
	if (Input::GetKeyState(VK_RBUTTON) == EKeyState::DOWN &&
		m_pOwner->m_curDelayRSkill <= 0.0f &&
		m_pOwner->m_curMP >= 0.7f)
	{
		m_pOwner->m_curMP -= 0.7f;
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
	m_pOwner->m_inputCombo = false;
	m_pOwner->m_comboCount = 0;
}
bool MageStateLSkill::Process(const float& spf) noexcept
{
	if (Input::GetKeyState(VK_LBUTTON) == EKeyState::DOWN &&
		!UIManager::Get().m_pMouseIcon->m_bRender)
	{
		m_pOwner->m_inputCombo = true;
	}
	m_pOwner->m_DelayFrame -= spf * PacketManager::Get().pMyInfo->MotionRate;
	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		switch (m_pOwner->m_comboCount)
		{
		case 0:
		{
			// 어택1
			++m_pOwner->m_comboCount;
			m_pOwner->m_eAction = PlayerController::EAction::Attack;
			m_pOwner->m_DelayFrame = 0.8f;
		}	break;
		case 1:
		{
			// 어택2동작
			if (m_pOwner->m_inputCombo)
			{
				m_pOwner->m_inputCombo = false;
				++m_pOwner->m_comboCount;
				m_pOwner->m_eAction = PlayerController::EAction::LCharge1;
				m_pOwner->m_DelayFrame = 0.8f;
				m_pOwner->m_moveSpeed *= 0.4f;
				m_pOwner->m_curMP -= 0.3f;
			}
			else
			{
				m_pOwner->SetState(EPlayerState::Basic);
				m_pOwner->m_DelayFrame = 0.2f;
				m_pOwner->m_curDelayLSkill = m_pOwner->m_DelayLSkill;
			}
		}	break;
		case 2:
		{
			// 어택2
			++m_pOwner->m_comboCount;
			m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack;
			m_pOwner->m_DelayFrame = 0.05f;
		}	break;
		case 3:
		case 4:
		case 5:
		{
			// 어택2 발사
			++m_pOwner->m_comboCount;
			m_pOwner->m_eAction = PlayerController::EAction::LCharging;
			m_pOwner->m_DelayFrame = 0.25f;
		}	break;
		case 6:
		{
			// 어택3동작
			if (m_pOwner->m_inputCombo)
			{
				m_pOwner->m_inputCombo = false;
				++m_pOwner->m_comboCount;
				m_pOwner->m_eAction = PlayerController::EAction::LCharge2;
				m_pOwner->m_DelayFrame = 1.1f;
				m_pOwner->m_moveSpeed *= 0.3f;
				m_pOwner->m_curMP -= 0.3f;
			}
			else
			{
				m_pOwner->SetState(EPlayerState::Basic);
				m_pOwner->m_DelayFrame = 0.2f;
				m_pOwner->m_curDelayLSkill = m_pOwner->m_DelayLSkill * 1.2f;
			}
		}	break;
		case 7:
		{
			// 어택3
			++m_pOwner->m_comboCount;
			m_pOwner->m_eAction = PlayerController::EAction::ChargeAttack2;
			m_pOwner->m_DelayFrame = 1.05f;
		}	break;
		case 8:
		{
			// 끝
			m_pOwner->SetState(EPlayerState::Basic);
			m_pOwner->m_curDelayLSkill = m_pOwner->m_DelayLSkill * 1.4f;
		}	break;
		default:
			break;
		}

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
	m_pOwner->m_DelayFrame -= spf * PacketManager::Get().pMyInfo->MotionRate;

	if (m_pOwner->m_DelayFrame <= 0.0f)
	{
		m_pOwner->m_eAction = PlayerController::EAction::Attack2;
		m_pOwner->SetState(EPlayerState::Basic);
		m_pOwner->m_curDelayRSkill = m_pOwner->m_DelayRSkill;
		m_pOwner->m_DelayFrame = 1.05f;
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
	m_pOwner->m_DelayFrame -= spf * PacketManager::Get().pMyInfo->MotionRate;
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
		m_pOwner->m_DelayFrame = 0.8f;
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