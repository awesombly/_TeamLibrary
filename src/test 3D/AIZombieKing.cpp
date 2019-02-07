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
	AIZombie::Init();
	if (m_Breath != nullptr)
	{
		ObjectManager::Get().DisableObject(m_Breath);
		m_Breath = nullptr;
	}
	m_isEnable = true;
	m_attackRange = m_pParent->GetScaleAverage() * 3000.0f;
	m_moveSpeed = RandomNormal() * 5.0f + 15.0f;
	m_delayBreath = 0.0f;
	m_delayStump = 0.0f;
	m_delay = 0.0f;
	m_eState = EState::Idle;
	m_eDirState = EState::Idle;
	return true;
}

bool AIZombieKing::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable) return false;
	m_pParent->Rotate(Quaternion::Left * spf);
	return false;
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
		 }	break;
		 case EState::Move:
		 {
			 m_pParent->m_pPhysics->m_mass = 0.05f;
			 m_pParent->m_pPhysics->m_damping = 3.0f;
		 	((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_KING_WALK);
		 	m_pParent->SetFocus(m_Target = PlayerController::Get().m_pHome->GetPosition());
		 }	break;
		 case EState::Attack:
		 {
		 	m_delay = 0.45f;
		 	((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_KING_ATTACK);
		 }	break;
		 case EState::Action1:
		 {
		 	m_delay = 0.9f;
		 	((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_KING_ROAR);
		 }	break;
		 case EState::Action2:
		 {
			 // 브레스 끝
		 	if (m_delayBreath >= 3.8f)
		 	{
		 		ObjectManager::Get().DisableObject(m_Breath);
		 		m_Breath = nullptr;
		 		m_delayBreath = 0.0f;
		 		m_delay = 1.0f;
				m_dealyAttack = 4.0f;
		 		m_eDirState = EState::Move;
		 		((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_KING_IDLE);
		 		return true;
		 	}
			m_eState = EState::Action1;
			if (m_Breath != nullptr)
			{
				m_Breath->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 30.0f + m_pParent->GetUp() * 55.0f);
			}
		 	//if (sinf(m_delayBreath) >= 0.0f)
		 	//	m_Breath->Rotate(Quaternion::Left * spf);
		 	//else
		 	//	m_Breath->Rotate(Quaternion::Right * spf);
		 }	break;
		 case EState::Action3:
		 {
			 // 점핑
			 m_delay = 3.5f;
			 m_pParent->SetForce(m_Target);
			 auto pEffect = ObjectManager::Get().TakeObject(L"EZStump");
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
				else if (m_delayStump >= 6.0f)
				{
					// 점프 어택
					m_delayStump = 0.0f;
					m_pParent->m_pPhysics->m_mass = 1.0f;
					m_pParent->m_pPhysics->m_damping = 0.25f;
					m_pParent->SetFocus(iter->GetPosition());
					m_Target = (iter->GetPosition() - m_pParent->GetPosition()) * 1.6f + Vector3::Up * 200.0f;
					m_eDirState = EState::Action3;
					m_delay = 0.5f;
					((AHeroObj*)m_pParent)->SetANIM_OneTime(Zombie_KING_JUMP_ATTACK);
					SoundManager::Get().PlayQueue("SV_zombie_king_shout.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);
					// 카메라 진동
					std::thread vibrator(&PlayerController::StartVibration, &PlayerController::Get(), 1.5f, 7.0f);
					vibrator.detach();
					return true;
				}
			}
			if (VectorLengthSq(m_Target - m_pParent->GetPosition()) <= m_attackRange + PlayerController::Get().HomeRadius)
			{
				m_eDirState = EState::Attack;
				return true;
			}
			// 브레스
			else if (m_delayBreath >= 8.0f)
			{
				m_delayBreath = 0.0f;
				m_pParent->SetFocus(m_Target = PlayerController::Get().m_pHome->GetPosition());
				m_eDirState = EState::Action1;
				return true;
			}
			m_pParent->Translate(Normalize(m_Target - m_pParent->GetPosition()) * m_moveSpeed * spf);
			return true;
		}
		// 이동
		if (VectorLengthSq(m_Target - m_pParent->GetPosition()) >= m_attackRange + PlayerController::Get().HomeRadius)
		{
			m_pParent->Translate(Normalize(m_Target = PlayerController::Get().m_pHome->GetPosition() - m_pParent->GetPosition()) * m_moveSpeed * spf);
		}
	}	break;
	case EState::Attack:
	{
		SoundManager::Get().PlayQueue("SV_zombie_king_attack1.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);

		// 공격
		auto pEffect = ObjectManager::Get().TakeObject(L"ZAttack3");
		pEffect->SetPosition(m_pParent->GetPosition() + m_pParent->GetForward() * 60.0f + m_pParent->GetUp() * 35.0f);
		pEffect->m_pPhysics->m_damage = 0.9f;
		pEffect->SetScale(m_pParent->GetScale());
		m_dealyAttack = 4.0f;
		///
		m_delay = 1.5f;
		m_eDirState = EState::Move;
	}	break;
	case EState::Action1:
	{
		SoundManager::Get().PlayQueue("SV_zombie_king_shout.mp3", m_pParent->GetPosition(), PlayerController::Get().SoundRange);

		// 브레스
		m_Breath = ObjectManager::Get().TakeObject(L"ZBreath");
		//m_Breath->SetParent(m_pParent);
		m_Breath->SetRotation(m_pParent->GetRotation());
		m_Breath->m_pPhysics->m_damage = 1.0f;
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

void AIZombieKing::DeadEvent() noexcept
{
	if (m_Breath != nullptr)
	{
		ObjectManager::Get().DisableObject(m_Breath);
		m_Breath = nullptr;
	}
	//if (RandomNormal() >= 0.2f)
	{
		auto pObject = ObjectManager::Get().TakeObject(L"ItemBox");
		pObject->SetPosition(m_pParent->GetCollider()->GetCenter());
		pObject->SetHP(10000.0f);
	}
	PlayerController::Get().OperEXP(1.0f);
	auto pEffect = ObjectManager::Get().TakeObject(L"EZDead3");
	pEffect->SetPosition(m_pParent->GetCollider()->GetCenter());
}

Component* AIZombieKing::clone() noexcept
{
	auto pAI = new AIZombieKing(*this);
	pAI->Init();
	return (Component*)pAI;
}