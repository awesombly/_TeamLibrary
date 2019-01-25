#include "AIZombieKing.h"
#include "ObjectManager.h"
//#include "GameObject.h"
#include "AHeroObj.h"
#include "CEventTimer.h"
#include "EventManager.h"
#include "PlayerController.h"

AIZombieKing::AIZombieKing()
{
	m_myName = L"AIEx";
	m_comptType = EComponent::Etc;
	//Init();
}


bool AIZombieKing::Init() noexcept
{
	m_isEnable = true;
	m_attackRange = m_pParent->GetScaleAverage() * 3000.0f;
	m_moveSpeed = RandomNormal() * 15.0f + 5.0f;
	return true;
}

bool AIZombieKing::Frame(const float& spf, const float& accTime)	noexcept
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
			((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_KING_WALK);
			m_pParent->SetFocus(m_Target = PlayerController::Get().m_pHome->GetPosition());
		}	break;
		case EState::Attack:
		{
			m_delay = 0.7f;
			((AHeroObj*)m_pParent)->SetANIM_OneTime(Zombie_KING_ATTACK);
		}	break;
		}
		return true;
	}
	// 행동
	switch (m_eState)
	{
	case EState::Idle:
	{
		Init();
		m_eDirState = EState::Move;
	}	break;
	case EState::Move:
	{
		for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Character))
		{
			if (VectorLengthSq(iter->GetPosition() - m_pParent->GetPosition()) <= m_attackRange)
			{
				m_pParent->SetRotationY(m_pParent->GetFocusY(m_Target = iter->GetPosition()) + PI * 0.25f);
				m_eDirState = EState::Attack;
				return true;
			}
		}
		if (VectorLengthSq(m_Target - m_pParent->GetPosition()) <= m_attackRange + PlayerController::Get().HomeRadius)
		{
			m_pParent->SetFocus(m_Target = PlayerController::Get().m_pHome->GetPosition());
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
		//SoundManager::Get().PlayQueue("SV_Guard_Punch.mp3", pObject->GetWorldPosition(), 1000.0f);

		auto pCollider = new Collider(m_pParent->GetScale().x * 50.0f);
		auto pMelee = new GameObject(L"Melee", { pCollider, new CEventTimer(0.3f) });
		pMelee->SetParent(m_pParent);
		pMelee->SetPosition(m_pParent->GetForward() * 60.0f + m_pParent->GetUp() * 40.0f);
		pMelee->SetRotation(m_pParent->GetRotation());
		pMelee->UpdateMatrix();
		pMelee->m_pPhysics->UserSocket = (UINT)-1;
		pMelee->SetHP(100.0f);
		pMelee->m_pPhysics->m_damage = 0.4f;
		pCollider->CollisionEvent = MyEvent::ZombieAttack;
		pCollider->m_eTag = ETag::Dummy;
		pCollider->SetGravityScale(0.0f);
		pCollider->usePhysics(false);
		///
		m_delay = 3.0f;
		m_eDirState = EState::Move;
	}	break;
	}
	return true;
	accTime;
}

bool AIZombieKing::Render(ID3D11DeviceContext* pDContext) noexcept
{
	return true;
	pDContext;
}

bool AIZombieKing::Release()	noexcept
{
	return true;
}

Component* AIZombieKing::clone() noexcept
{
	auto pAI = new AIZombieKing(*this);
	pAI->m_eState = EState::Idle;
	return (Component*)pAI;
}