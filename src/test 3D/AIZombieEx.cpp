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
	//Update();
	//
	m_attackRange = m_pParent->GetScaleAverage() * 5000.0f;
	m_moveSpeed = RandomNormal() * 15.0f + 105.0f;
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
	// ù �ൿ
	if (m_eState != m_eDirState)
	{
		switch (m_eState = m_eDirState)
		{
		case EState::Idle:
		{
			((AHeroObj*)m_pParent)->SetANIM_Loop(ZombieEX_IDLE);
			m_pParent->isMoving(false);
		}	break;
		case EState::Move:
		{
			((AHeroObj*)m_pParent)->SetANIM_Loop(ZombieEX_RUN);
			m_pParent->SetDirectionForce(Normalize((m_Target = PlayerController::Get().m_HomePos) - m_pParent->GetPosition()) * m_moveSpeed);
			m_pParent->SetFocus(m_Target);
		}	break;
		case EState::Attack:
		{
			((AHeroObj*)m_pParent)->SetANIM_Loop(ZombieEX_ATTACK);
			m_delay = 1.3f;
			m_pParent->isMoving(false);
			m_pParent->SetFocus(m_Target);
		}	break;
		}
		return true;
	}

	// �ൿ
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
			if (VectorLengthSq((m_Target = PlayerController::Get().m_HomePos) - m_pParent->GetPosition()) <= m_attackRange + PlayerController::Get().HomeRadius)
			{
				m_eDirState = EState::Attack;
				return true;
			}
		}
		m_pParent->SetDirectionForce(Normalize((PlayerController::Get().m_HomePos) - m_pParent->GetPosition()) * m_moveSpeed);
	}	break;
	case EState::Attack:
	{
		SoundManager::Get().PlayQueue("SV_zombieEX_atk2.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);

		// ����
		auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack2");
		pEffect->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 60.0f + m_pParent->GetUp() * 45.0f);
		pEffect->m_pPhysics->m_damage = 0.6f;
		pEffect->SetScale(m_pParent->GetScale());
		m_dealyAttack = 3.0f;
		///
		m_delay = 2.0f;
		m_eDirState = EState::Idle;
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

Component* AIZombieEx::clone() noexcept
{
	auto pAI = new AIZombieEx(*this);
	pAI->Update();
	return (Component*)pAI;
}