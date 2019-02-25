#include "AIZombieCrawl.h"
#include "ObjectManager.h"
//#include "GameObject.h"
#include "AHeroObj.h"
#include "EventManager.h"
#include "PlayerController.h"
#include "SoundManager.h"

AIZombieCrawl::AIZombieCrawl()
{
	m_myName = L"AI";
	m_comptType = EComponent::Etc;
	//Init();
}


bool AIZombieCrawl::Init() noexcept
{
	//Update();
	//
	m_attackRange = m_pParent->GetScaleAverage() * 2500.0f;
	m_moveSpeed = RandomNormal() * 15.0f + 30.0f;
	return true;
}

bool AIZombieCrawl::Frame(const float& spf, const float& accTime)	noexcept
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
			m_pParent->isMoving(false);
		}	break;
		case EState::Move:
		{
			((AHeroObj*)m_pParent)->SetANIM_Loop(ZombieR_CRAWL);
			m_pParent->SetDirectionForce(Normalize((m_Target = PlayerController::Get().m_HomePos) - m_pParent->GetPosition()) * m_moveSpeed);
			m_pParent->SetFocus(m_Target = PlayerController::Get().m_HomePos);
		}	break;
		case EState::Attack:
		{
			m_delay = 1.5f;
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
		/*for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Character))
		{
			if (VectorLengthSq(iter->GetPosition() - m_pParent->GetPosition()) <= m_attackRange)
			{
				m_Target = iter->GetPosition();
				m_inAttackRange = true;
				break;
			}
		}*/

		if (VectorLengthSq((m_Target = PlayerController::Get().m_HomePos) - m_pParent->GetPosition()) <= m_attackRange + PlayerController::Get().HomeRadius)
		{
			m_eDirState = EState::Attack;
			return true;
		}
		m_pParent->SetDirectionForce(Normalize((PlayerController::Get().m_HomePos) - m_pParent->GetPosition()) * m_moveSpeed);
	}	break;
	case EState::Attack:
	{
		SoundManager::Get().PlayQueue("SE_bomb.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);
		// 공격
		auto pEffect = ObjectManager::Get().TakeObject(L"ZBoom");
		pEffect->SetPosition(m_pParent->GetPosition());
		pEffect->SetScale(Vector3::One * 1.5f);
		pEffect->m_pPhysics->m_damage = 0.8f;

		ObjectManager::Get().DisableObject(m_pParent);
	}	break;
	}
	return true;
	accTime;
}

bool AIZombieCrawl::Render(ID3D11DeviceContext* pDContext) noexcept
{
	return true;
	pDContext;
}

bool AIZombieCrawl::Release()	noexcept
{
	return true;
}

Component* AIZombieCrawl::clone() noexcept
{
	auto pAI = new AIZombieCrawl(*this);
	pAI->Update();
	return (Component*)pAI;
}