#include "AIZombieEx.h"
#include "ObjectManager.h"
//#include "GameObject.h"
#include "AHeroObj.h"
#include "EventManager.h"
#include "PlayerController.h"
#include "SoundManager.h"

AIZombieEx::AIZombieEx()
{
	m_myName = L"AI";
	m_comptType = EComponent::Etc;
	//Init();
}


bool AIZombieEx::Init() noexcept
{
	m_isEnable = true;
	m_attackRange = m_pParent->GetScaleAverage() * 2600.0f;
	m_moveSpeed = RandomNormal() * 5.0f + 35.0f;
	m_delay = 0.0f;
	m_eState = EState::Idle;
	m_eDirState = EState::Idle;
	return true;
}

bool AIZombieEx::Frame(const float& spf, const float& accTime)	noexcept
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
		}	break;
		case EState::Move:
		{
			((AHeroObj*)m_pParent)->SetANIM_Loop(ZombieEX_RUN);
			m_pParent->SetFocus(m_Target = PlayerController::Get().m_pHome->GetPosition());
		}	break;
		case EState::Attack:
		{
			m_delay = 1.3f;
			((AHeroObj*)m_pParent)->SetANIM_Loop(ZombieEX_ATTACK);
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
		if (m_dealyAttack <= 0.0f)
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
			m_pParent->Translate(Normalize(m_Target - m_pParent->GetPosition()) * m_moveSpeed * spf);
			return true;
		}
		// 이동
		if (VectorLengthSq(m_Target - m_pParent->GetPosition()) >= m_attackRange + PlayerController::Get().HomeRadius)
		{
			m_pParent->Translate(Normalize(m_Target - m_pParent->GetPosition()) * m_moveSpeed * spf);
		}
	}	break;
	case EState::Attack:
	{
		SoundManager::Get().PlayQueue("SV_zombieEX_atk2.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);

		// 공격
		auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack2");
		pEffect->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 60.0f + m_pParent->GetUp() * 45.0f);
		pEffect->m_pPhysics->m_damage = 0.6f;
		pEffect->SetScale(m_pParent->GetScale());
		m_dealyAttack = 3.0f;
		///
		m_delay = 2.0f;
		m_eDirState = EState::Move;
	}	break;
	}
	return true;
	accTime;
}

bool AIZombieEx::Render(ID3D11DeviceContext* pDContext) noexcept
{
	return true;
	pDContext;
}

bool AIZombieEx::Release()	noexcept
{
	return true;
}

void AIZombieEx::DeadEvent() noexcept
{
	if (RandomNormal() >= 0.4f)
	{
		auto pObject = ObjectManager::Get().TakeObject(L"ItemBox");
		pObject->SetPosition(m_pParent->GetCollider()->GetCenter());
		pObject->SetHP(10000.0f);
	}
	PlayerController::Get().OperEXP(0.2f);
	auto pEffect = ObjectManager::Get().TakeObject(L"EZDead2");
	pEffect->SetPosition(m_pParent->GetCollider()->GetCenter());
}



Component* AIZombieEx::clone() noexcept
{
	auto pAI = new AIZombieEx(*this);
	pAI->Init();
	return (Component*)pAI;
}