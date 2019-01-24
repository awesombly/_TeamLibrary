#include "AIController.h"
//#include "GameObject.h"
#include "AHeroObj.h"

AIController::AIController()
{
	m_myName = L"AI";
	m_comptType = EComponent::Etc;
	//Init();
}


bool AIController::Init() noexcept
{
	m_isEnable = true;
	m_attackRange = m_pParent->GetScaleAverage() * 3000.0f;
	m_moveSpeed   = RandomNormal() * 35.0f + 15.0f;
	//m_damage	  = 0.2f;// RandomNormal() * 0.3f + 0.1f;
	return true;
}

bool AIController::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable) return false;
	
	switch (m_eState)
	{
	 case EState::Idle:
	 {
		 Init();
	 	((AHeroObj*)m_pParent)->SetANIM_Loop(Zombie_FORWARD);
	 	m_eState = EState::Move;
		m_pParent->SetFocus(m_Target);
	 }	break;
	 case EState::Move:
	 {
	 	if (VectorLengthSq(m_Target - m_pParent->GetPosition()) >= m_attackRange)
	 	{
	 		m_pParent->Translate(Normalize(m_Target - m_pParent->GetPosition()) * m_moveSpeed * spf);
	 	}
		//m_pParent->SetFocus(m_Target);
	 	//m_dirRotY = m_pParent->GetFocusY(m_Target);
	 	//m_pParent->SetRotationY(Lerp(m_dirRotY, m_pParent->GetRotation().y, spf * 5.0f));
	 }	break;
	 case EState::Attack:
	 {
	 }	break;
	}
	return true;
	accTime;
}

bool AIController::Render(ID3D11DeviceContext* pDContext) noexcept
{
	return true;
	pDContext;
}

bool AIController::Release()	noexcept
{
	return true;
}

Component* AIController::clone() noexcept
{
	auto pAI = new AIController(*this);
	pAI->m_eState = EState::Idle;
	return (Component*)pAI;
}