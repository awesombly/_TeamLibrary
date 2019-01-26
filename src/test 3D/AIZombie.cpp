#include "AIZombie.h"
#include "ObjectManager.h"
//#include "GameObject.h"
#include "AHeroObj.h"
#include "EventManager.h"
#include "PlayerController.h"
#include "SoundManager.h"


AIZombie::AIZombie()
{
	m_myName = L"AI";
	m_comptType = EComponent::Etc;
	//Init();
}


bool AIZombie::Init() noexcept
{
	m_isEnable = true;
	m_attackRange = m_pParent->GetScaleAverage() * 1500.0f;
	m_moveSpeed   = RandomNormal() * 25.0f + 25.0f;
	return true;
}

bool AIZombie::Frame(const float& spf, const float& accTime)	noexcept
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
			((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_WALK);
			m_pParent->SetFocus(m_Target = PlayerController::Get().m_pHome->GetPosition());
		}	break;
		case EState::Attack:
		{
			m_delay = 1.1f;
			((AHeroObj*)m_pParent)->SetANIM_OneTime(Zombie_ATTACK);
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
	 	//m_dirRotY = m_pParent->GetFocusY(m_Target);
	 	//m_pParent->SetRotationY(Lerp(m_dirRotY, m_pParent->GetRotation().y, spf * 5.0f));
	 }	break;
	 case EState::Attack:
	 {
		 SoundManager::Get().PlayQueue("SE_zombie_hit02.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);

		 // 공격
		 auto pCollider = new Collider(m_pParent->GetScale().x * 40.0f);
		 auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack");
		 pEffect->AddComponent(pCollider);
		 pEffect->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 50.0f + m_pParent->GetUp() * 45.0f);
		 pEffect->m_pPhysics->m_damage = 0.2f;
		 pCollider->CollisionEvent = MyEvent::ZombieAttack;
		 pCollider->m_eTag = ETag::Dummy;
		 pCollider->SetGravityScale(0.0f);
		 pCollider->usePhysics(false);
		 ///
		 m_delay = 3.5f;
		 m_eDirState = EState::Move;
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

Component* AIZombie::clone() noexcept
{
	auto pAI = new AIZombie(*this);
	pAI->m_eState = EState::Idle;
	return (Component*)pAI;
}