#include "AIZombieKing.h"
#include "ObjectManager.h"
//#include "GameObject.h"
#include "AHeroObj.h"
#include "CEventTimer.h"
#include "EventManager.h"
#include "PlayerController.h"
#include "SoundManager.h"
#include "ColliderOBB.h"

AIZombieKing::AIZombieKing()
{
	m_myName = L"AI";
	m_comptType = EComponent::Etc;
	//Init();
}


bool AIZombieKing::Init() noexcept
{
	m_isEnable = true;
	m_attackRange = m_pParent->GetScaleAverage() * 3000.0f;
	m_moveSpeed = RandomNormal() * 5.0f + 15.0f;
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
	m_delayBreath += spf;
	m_delayStump += spf;
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
			 m_pParent->m_pPhysics->m_mass = 0.1f;
			 m_pParent->m_pPhysics->m_damping = 2.0f;
		 	((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_KING_WALK);
		 	m_pParent->SetFocus(m_Target = PlayerController::Get().m_pHome->GetPosition());
		 }	break;
		 case EState::Attack:
		 {
		 	m_delay = 0.45f;
		 	((AHeroObj*)m_pParent)->SetANIM_OneTime(Zombie_KING_ATTACK);
		 }	break;
		 case EState::Action1:
		 {
		 	m_delay = 0.9f;
		 	((AHeroObj*)m_pParent)->SetANIM_OneTime(Zombie_KING_ROAR);
		 }	break;
		 case EState::Action2:
		 {
			 // 브레스
		 	if (m_delayBreath >= 3.8f)
		 	{
		 		ObjectManager::Get().RemoveObject(m_Breath);
		 		m_Breath = nullptr;
		 		m_delayBreath = 0.0f;
		 		m_delay = 4.0f;
		 		m_eDirState = EState::Move;
		 		((AHeroObj*)m_pParent)->SetANIM_OneTime(Zombie_KING_IDLE);
		 		return true;
		 	}
			m_eState = EState::Action1;
		 	m_Breath->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 30.0f + m_pParent->GetUp() * 50.0f);
		 	//if (sinf(m_delayBreath) >= 0.0f)
		 	//	m_Breath->Rotate(Quaternion::Left * spf);
		 	//else
		 	//	m_Breath->Rotate(Quaternion::Right * spf);
		 }	break;
		 case EState::Action3:
		 {
			 // 점핑
			 m_delay = 4.0f;
			 m_pParent->SetForce(m_Target);
			 auto pEffect = ObjectManager::Get().TakeObject(L"ZStump");
			 pEffect->SetPosition(m_pParent->GetPosition() + Vector3::Up * 5.0f);
			 m_eDirState = EState::Move;
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
			else if (m_delayStump >= 6.0f)
			{
				// 점프 어택
				m_delayStump = 0.0f;
				m_pParent->m_pPhysics->m_mass = 1.0f;
				m_pParent->m_pPhysics->m_damping = 0.25f;
				m_pParent->SetFocus(iter->GetPosition());
				m_Target = (iter->GetPosition() - m_pParent->GetPosition()) * 1.5f + Vector3::Up * 100.0f;
				m_eDirState = EState::Action3;
				m_delay = 0.5f;
				((AHeroObj*)m_pParent)->SetANIM_OneTime(Zombie_KING_JUMP_ATTACK);
				return true;
			}
		}
		if (VectorLengthSq(m_Target - m_pParent->GetPosition()) <= m_attackRange + PlayerController::Get().HomeRadius)
		{
			m_eDirState = EState::Attack;
			return true;
		}
		// 브레스
		else if(m_delayBreath >= 8.0f)
		{
			m_delayBreath = 0.0f;
			m_pParent->SetFocus(m_Target = PlayerController::Get().m_pHome->GetPosition());
			m_eDirState = EState::Action1;
			return true;
		}
		// 이동
		m_pParent->Translate(Normalize(m_Target = PlayerController::Get().m_pHome->GetPosition() - m_pParent->GetPosition()) * m_moveSpeed * spf);
	}	break;
	case EState::Attack:
	{
		SoundManager::Get().PlayQueue("SE_zombie_hit02.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);

		// 공격
		auto pCollider = new Collider(m_pParent->GetScale().x * 55.0f);
		auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack3");
		pEffect->AddComponent(pCollider);
		pEffect->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 60.0f + m_pParent->GetUp() * 35.0f);
		pEffect->m_pPhysics->m_damage = 0.9f;
		pCollider->CollisionEvent = MyEvent::ZombieAttack;
		pCollider->m_eTag = ETag::Dummy;
		pCollider->SetGravityScale(0.0f);
		pCollider->usePhysics(false);
		///
		m_delay = 4.0f;
		m_eDirState = EState::Move;
	}	break;
	case EState::Action1:
	{
		//SoundManager::Get().PlayQueue("SE_zombie_hit02.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);

		// 브레스
		auto pCollider = new ColliderOBB({-30.0f, -40.0f, -200.0f}, { 30.0f, 40.0f, 0.0f });
		m_Breath = ObjectManager::Get().TakeObject(L"ZBreath");
		m_Breath->AddComponent(pCollider);
		m_Breath->SetRotation(m_pParent->GetRotation());
		m_Breath->m_pPhysics->m_damage = 1.0f;
		pCollider->CollisionEvent = MyEvent::ZombieAttack;
		pCollider->m_eTag = ETag::Dummy;
		pCollider->SetGravityScale(0.0f);
		pCollider->usePhysics(false);
		///
		m_delayBreath = 0.0f;
		m_eDirState = EState::Action2;
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