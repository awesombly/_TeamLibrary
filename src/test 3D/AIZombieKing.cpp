#include "AIZombieKing.h"
#include "ObjectManager.h"
//#include "GameObject.h"
#include "AHeroObj.h"
#include "CEventTimer.h"
#include "EventManager.h"
#include "PlayerController.h"
#include "SoundManager.h"
#include "Collider.h"

AIZombieKing::AIZombieKing()
{
	m_myName = L"AI";
	m_comptType = EComponent::Etc;
	//Init();
}


bool AIZombieKing::Init() noexcept
{
	//Update();
	//
	m_pCollider = m_pParent->GetCollider();
	if (m_Breath != nullptr)
	{
		ObjectManager::Get().DisableObject(m_Breath);
		m_Breath = nullptr;
	}
	m_attackRange = m_pParent->GetScaleAverage() * 27000.0f;
	m_moveSpeed = RandomNormal() * 15.0f + 45.0f;
	return true;
}

bool AIZombieKing::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable) return false;

	m_dealyAttack -= spf;
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
			 ((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_KING_IDLE);
			 m_pParent->isMoving(false);
			 //m_pParent->m_pPhysics->m_mass = 0.05f;
			 m_pParent->SetGravityScale(200.0f);
			 m_pParent->m_pPhysics->m_damping = 10.0f;
			 m_pCollider->m_eTagArray[ETag::Enemy] = true;
			 m_pCollider->m_eTagArray[ETag::Dummy] = true;
			 m_pCollider->m_eTagArray[ETag::Collider] = true;
		 }	break;
		 case EState::Move:
		 {
		 	((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_KING_WALK);
			m_pParent->SetDirectionForce(Normalize((m_Target = PlayerController::Get().m_HomePos) - m_pParent->GetPosition()) * m_moveSpeed);
			m_pParent->SetFocus(m_Target);
		 }	break;
		 case EState::Attack:
		 {
		 	((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_KING_ATTACK);
		 	m_delay = 0.45f;
			m_pParent->isMoving(false);
			m_pParent->SetFocus(m_Target);
		 }	break;
		 case EState::Action1:
		 {
		 	((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_KING_ROAR);
		 	m_delay = 0.9f;
			m_pParent->isMoving(false);
			m_pParent->SetFocus(m_Target);
		 }	break;
		 case EState::Action2:
		 {
			 // 브레스 끝
		 	if (m_delayBreath >= 3.1f)
		 	{
		 		ObjectManager::Get().DisableObject(m_Breath);
		 		m_Breath = nullptr;
		 		m_delayBreath = 0.0f;
		 		m_delay = 1.0f;
				m_dealyAttack = 3.5f;
		 		m_eDirState = EState::Idle;
		 		((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_KING_IDLE);
		 		return true;
		 	}
			m_eState = EState::Action1;
			//if (m_Breath != nullptr)
			//{
			//	m_Breath->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 50.0f + m_pParent->GetUp() * 55.0f);
			//}
		 	//if (sinf(m_delayBreath) >= 0.0f)
		 	//	m_Breath->Rotate(Quaternion::Left * spf);
		 	//else
		 	//	m_Breath->Rotate(Quaternion::Right * spf);
		 }	break;
		 case EState::Action3:
		 {
			 // 점핑
			 //m_delay = 2.5f;
			 //m_dealyAttack = 3.5f;
			 m_pParent->isMoving(false);
			 m_pParent->isGround(false);
			 m_pParent->SetForce(m_Target);
			 auto pEffect = ObjectManager::Get().TakeObject(L"EZStump");
			 pEffect->SetPosition(m_pParent->GetPosition() + Vector3::Up * 3.0f);
			 pEffect->SetScale(m_pParent->GetScale() * 0.6f);
			 pEffect->SetDamage(0.3f);
			 //m_eDirState = EState::Idle;
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
		//if (m_dealyAttack <= 0.0f)
		//{
			for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Character))
			{
				if (VectorLengthSq(iter->GetPosition() - m_pParent->GetPosition()) <= m_attackRange)
				{
					m_Target = iter->GetPosition();
					m_eDirState = EState::Attack;
					return true;
				}
			}
		//}
		if (m_delayStump >= 6.0f &&
			!ObjectManager::Get().GetObjectList(EObjType::Character)->empty())
		{
			auto targetPos = ObjectManager::Get().GetObjectList(EObjType::Character)->front()->GetPosition();
			// 점프 어택
			m_delayStump = 0.0f;
			m_pParent->SetFocus(targetPos);
			//m_pParent->m_pPhysics->m_mass = 1.0f;
			m_pParent->SetGravityScale(7.0f);
			m_pParent->m_pPhysics->m_damping = 0.65f;
			m_pCollider->m_eTagArray[ETag::Enemy] = false;
			m_pCollider->m_eTagArray[ETag::Dummy] = false;
			m_pCollider->m_eTagArray[ETag::Collider] = false;
			m_Target = (targetPos - m_pParent->GetPosition()) + Vector3::Up * 1600.0f;
			m_eDirState = EState::Action3;
			m_delay = 0.5f;
			((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_KING_JUMP_ATTACK);
			//SoundManager::Get().PlayQueue("SV_zombie_king_shout.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange * 3.0f);
			SoundManager::Get().Play("SV_zombie_king_shout.mp3");
			// 카메라 진동
			std::thread vibrator(&PlayerController::StartVibration, &PlayerController::Get(), 1.5f, 7.0f);
			vibrator.detach();
			return true;
		}
		///
		if (VectorLengthSq((m_Target = PlayerController::Get().m_HomePos) - m_pParent->GetPosition()) <= m_attackRange + PlayerController::Get().HomeRadius)
		{
			m_eDirState = EState::Attack;
			return true;
		}
		// 브레스
		else if (m_delayBreath >= 8.0f)
		{
			m_delayBreath = 0.0f;
			m_eDirState = EState::Action1;
			return true;
		}
		return true;
	}	break;
	case EState::Attack:
	{
		SoundManager::Get().PlayQueue("SV_zombie_king_attack1.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);

		// 공격
		auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack3");
		pEffect->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 60.0f + m_pParent->GetUp() * 35.0f);
		pEffect->SetScale(m_pParent->GetScale());
		pEffect->m_pPhysics->m_damage = 0.9f;
		m_dealyAttack = 3.5f;
		///
		m_delay = 0.75f;
		m_eDirState = EState::Idle;
	}	break;
	case EState::Action1:
	{
		SoundManager::Get().PlayQueue("SV_zombie_king_shout.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);

		// 브레스
		m_Breath = ObjectManager::Get().TakeObject(L"ZBreath");
		//m_Breath->SetParent(m_pParent);
		m_Breath->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 50.0f + m_pParent->GetUp() * 55.0f);
		m_Breath->SetRotation(m_pParent->GetRotation());
		m_Breath->GetCollider()->m_pivot = m_pParent->GetForward() * 50.0f + m_pParent->GetDown() * 35.0f;
		//m_Breath->SetFocus(m_pParent->GetPosition() + m_pParent->GetForward() * 1000.0f);
		m_Breath->m_pPhysics->m_damage = 0.4f;		// 체력 비례 딜
		///
		m_delayBreath = 0.0f;
		m_eDirState = EState::Action2;
	}	break;
	case EState::Action3:
	{
		if (m_pParent->isGround())
		{
			m_delay = 0.6f;
			m_dealyAttack = 2.0f;
			auto pEffect = ObjectManager::Get().TakeObject(L"EZStump");
			pEffect->SetPosition(m_pParent->GetPosition() + Vector3::Up * 3.0f);
			pEffect->SetScale(m_pParent->GetScale());
			pEffect->m_pPhysics->m_damage = 0.75f;
			m_eDirState = EState::Idle;
		}
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
	pAI->Update();
	return (Component*)pAI;
}