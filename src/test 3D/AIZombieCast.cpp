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
	m_attackRange = m_pParent->GetScaleAverage() * 10000.0f;
	m_moveSpeed = RandomNormal() * 10.0f + 20.0f;
	m_delay = 0.0f;
	m_eState = EState::Idle;
	m_eDirState = EState::Idle;
	return true;
}

bool AIZombieCast::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable) return false;

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
		}	break;
		case EState::Move:
		{
			((AHeroObj*)m_pParent)->SetANIM_Loop(ZombieR_RUN);
			m_pParent->SetFocus(m_Target = PlayerController::Get().m_pHome->GetPosition());
		}	break;
		case EState::Attack:
		{
			m_delay = 1.1f;
			((AHeroObj*)m_pParent)->SetANIM_OneTime(ZombieR_SHOT);
		}	break;
		}
		return true;
	}
	// 행동
	switch (m_eState)
	{
	case EState::Idle:
	{
		m_eDirState = EState::Move;
	}	break;
	case EState::Move:
	{
		for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Character))
		{
			if (VectorLengthSq(iter->GetPosition() - m_pParent->GetPosition()) <= m_attackRange)
			{
				m_pParent->SetRotationY(m_pParent->GetFocusY(m_Target = iter->GetPosition()) - PI * 0.5f);
				m_eDirState = EState::Attack;
				return true;
			}
		}
		if (VectorLengthSq(m_Target - m_pParent->GetPosition()) <= m_attackRange + PlayerController::Get().HomeRadius)
		{
			m_pParent->SetFocus(m_Target);
			m_eDirState = EState::Attack;
			return true;
		}
		else	// 이동
		{
			m_pParent->Translate(Normalize(m_Target - m_pParent->GetPosition()) * m_moveSpeed * spf);
		}
	}	break;
	case EState::Attack:
	{
		SoundManager::Get().PlayQueue("SE_chicken.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);

		auto pChicken = ObjectManager::Get().TakeObject(L"Chicken");
		pChicken->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 40.0f + m_pParent->GetUp() * 65.0f + m_pParent->GetRight() * 20.0f);
		pChicken->SetRotation(m_pParent->GetRotation());
		pChicken->SetScale(m_pParent->GetScale().x * 2.0f * Vector3::One);
		pChicken->SetForce((m_pParent->GetForward() + Vector3::Up * 0.2f) * 250.0f);
		pChicken->m_pPhysics->m_damage = 0.35f;
		///
		m_delay = 5.0f;
		m_eDirState = EState::Move;
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