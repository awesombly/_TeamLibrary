#include "AIZombie.h"
#include "ObjectManager.h"
//#include "GameObject.h"
#include "AHeroObj.h"
#include "EventManager.h"
#include "PlayerController.h"
#include "SoundManager.h"
#include "Collider.h"

AIZombie::AIZombie()
{
	m_myName = L"AI";
	m_comptType = EComponent::Etc;
	//Init();
}


bool AIZombie::Init() noexcept
{
	//Update();
	//
	m_attackRange = m_pParent->GetScaleAverage() * 4000.0f;
	m_moveSpeed   = RandomNormal() * 50.0f + 80.0f;
	return true;
}

bool AIZombie::Frame(const float& spf, const float& accTime)	noexcept
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
			((AHeroObj*)m_pParent)->SetANIM_Loop(ZombieR_WALK);
			m_pParent->SetDirectionForce(Normalize((m_Target = PlayerController::Get().m_HomePos) - m_pParent->GetPosition()) * m_moveSpeed);
			m_pParent->SetFocus(m_Target);
		}	break;
		case EState::Attack:
		{
			((AHeroObj*)m_pParent)->SetANIM_Loop(ZombieR_ATTACK);
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
		 SoundManager::Get().PlayQueue("SV_zombieR_atk1.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);
		 // 공격
		 auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack");
		 pEffect->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 50.0f + m_pParent->GetUp() * 45.0f);
		 pEffect->m_pPhysics->m_damage = 0.2f;
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

bool AIZombie::Render(ID3D11DeviceContext* pDContext) noexcept
{
	return true;
	pDContext;
}

bool AIZombie::Release()	noexcept
{
	return true;
}

void AIZombie::Update()	noexcept
{
	m_isEnable = true;
	auto pCollider = m_pParent->GetCollider();
	pCollider->m_eTagArray[ETag::Ally] = true;
	pCollider->m_eTagArray[ETag::Enemy] = true;
	pCollider->m_eTagArray[ETag::Dummy] = true;
	pCollider->m_eTagArray[ETag::Collider] = true;

	m_delay = 0.0f;
	m_eState = EState::Idle;
	m_eDirState = EState::Idle;
	((AHeroObj*)m_pParent)->SetHeroAnimSpeed(1.0f);
	Init();
}


Component* AIZombie::clone() noexcept
{
	auto pAI = new AIZombie(*this);
	pAI->Update();
	return (Component*)pAI;
}