#include "AIZombieCast.h"
#include "ObjectManager.h"
#include "AHeroObj.h"
#include "EventManager.h"
#include "PlayerController.h"
#include "SoundManager.h"

AIZombieCast::AIZombieCast()
{
	m_myName = L"AI";
	m_comptType = EComponent::Etc;
}

bool AIZombieCast::Init() noexcept
{
	m_isEnable = true;
	m_attackRange = m_pParent->GetScaleAverage() * 15000.0f;
	m_moveSpeed = RandomNormal() * 30.0f + 60.0f;
	m_delay = 0.0f;
	m_eState = EState::Idle;
	m_eDirState = EState::Idle;
	return true;
}

bool AIZombieCast::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable) return false;

	m_dealyAttack -= spf;
	if (m_delay >= 0.0f)
	{
		m_delay -= spf;
		return false;
	}
	// 첫 행동
	if (m_eState != m_eDirState)
	{
		switch (m_eState = m_eDirState)
		{
		case EState::Idle:
		{
			((AHeroObj*)m_pParent)->SetANIM_Loop(ZombieR_IDLE);
			m_pParent->isMoving(false);
		}	break;
		case EState::Move:
		{
			((AHeroObj*)m_pParent)->SetANIM_Loop(ZombieR_RUN);
			m_pParent->SetDirectionForce(Normalize((m_Target = PlayerController::Get().m_pHome->GetPosition()) - m_pParent->GetPosition()) * m_moveSpeed);
			m_pParent->SetFocus(m_Target);
		}	break;
		case EState::Attack:
		{
			((AHeroObj*)m_pParent)->SetANIM_Loop(ZombieR_SHOT);
			m_delay = 1.1f;
			m_pParent->isMoving(false);
			m_pParent->SetFocus(m_Target);
		}	break;
		}
		return true;
	}

	// 행동
	switch (m_eState)
	{
	case EState::Idle:
	{
		if (m_dealyAttack <= 0.0f)
		{
			m_eDirState = EState::Move;
		}
	}	break;
	case EState::Move:
	{
		if (m_dealyAttack <= 0.0f)
		{
			for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Character))
			{
				if (VectorLengthSq(iter->GetPosition() - m_pParent->GetPosition()) <= m_attackRange)
				{
					m_Target = iter->GetPosition();
					m_eDirState = EState::Attack;
					return true;
				}
			}
			if (VectorLengthSq((m_Target = PlayerController::Get().m_pHome->GetPosition()) - m_pParent->GetPosition()) <= m_attackRange + PlayerController::Get().HomeRadius)
			{
				m_eDirState = EState::Attack;
				return true;
			}
		}
	}	break;
	case EState::Attack:
	{
		SoundManager::Get().PlayQueue("SV_zombieR_shot.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);

		auto pChicken = ObjectManager::Get().TakeObject(L"ZThrow");
		pChicken->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 40.0f + m_pParent->GetUp() * 65.0f);
		pChicken->SetRotation(m_pParent->GetRotation());
		pChicken->SetScale(m_pParent->GetScale().x * 2.0f * Vector3::One);
		pChicken->SetForce((m_pParent->GetForward() + Vector3::Up * 1.2f) * 200.0f);
		pChicken->m_pPhysics->m_damage = 0.35f;
		m_dealyAttack = 4.5f;
		///
		m_delay = 3.0f;
		m_eDirState = EState::Idle;
	}	break;
	}
	return true;
	accTime;
}

bool AIZombieCast::Render(ID3D11DeviceContext* pDContext) noexcept
{
	return true;
	pDContext;
}

bool AIZombieCast::Release()	noexcept
{
	return true;
}

Component* AIZombieCast::clone() noexcept
{
	auto pAI = new AIZombieCast(*this);
	pAI->Init();
	return (Component*)pAI;
}