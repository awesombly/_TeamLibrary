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
	m_isEnable = true;
	m_attackRange = m_pParent->GetScaleAverage() * 2000.0f;
	m_moveSpeed = RandomNormal() * 5.0f + 10.0f;
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
		}	break;
		case EState::Move:
		{
			((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_CRAWL);
			m_pParent->SetFocus(m_Target = PlayerController::Get().m_pHome->GetPosition());
		}	break;
		case EState::Attack:
		{
			m_delay = 1.5f;
			//((AHeroObj*)m_pParent)->SetANIM_OneTime(Zombie_ATTACK);
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
		//for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Character))
		//{
		//	if (VectorLengthSq(iter->GetPosition() - m_pParent->GetPosition()) <= m_attackRange)
		//	{
		//		m_pParent->SetRotationY(m_pParent->GetFocusY(m_Target = iter->GetPosition()) - PI * 0.5f);
		//		m_eDirState = EState::Attack;
		//		return true;
		//	}
		//}
		if (VectorLengthSq(m_Target - m_pParent->GetPosition()) <= m_attackRange + PlayerController::Get().HomeRadius)
		{
			m_pParent->SetFocus(m_Target);
			m_eDirState = EState::Attack;
			return true;
		}
		// 이동
		m_pParent->Translate(Normalize(m_Target - m_pParent->GetPosition()) * m_moveSpeed * spf);
	}	break;
	case EState::Attack:
	{
		SoundManager::Get().PlayQueue("SE_zombie_hit01.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);
		// 공격
		auto pCollider = new Collider(m_pParent->GetScale().x * 300.0f);
		auto pEffect = ObjectManager::Get().TakeObject(L"ZBoom");
		pEffect->AddComponent(pCollider);
		pEffect->SetPosition(m_pParent->GetPosition());
		pEffect->m_pPhysics->m_damage = 0.8f;
		pCollider->CollisionEvent = MyEvent::ZombieAttack;
		pCollider->m_eTag = ETag::Dummy;
		pCollider->SetGravityScale(0.0f);
		pCollider->usePhysics(false);

		ObjectManager::Get().DisableObject(m_pParent);
		return false;
		///
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
	pAI->m_eState = EState::Idle;
	return (Component*)pAI;
}