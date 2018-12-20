#include "PlayerController.h"
#include "Input.h"


bool PlayerController::Init() noexcept
{
	GameObject::Init();
	return true;
}

bool PlayerController::Frame(const float& spf, const float& accTime)	noexcept
{
	GameObject::Frame(spf, accTime);

	PlayerController();
	return true;
}


bool PlayerController::Render(ID3D11DeviceContext* pDContext) noexcept
{
	GameObject::Render(pDContext);
	return true;
}

bool PlayerController::Release() noexcept
{
	GameObject::Release();
	return true;
}


void PlayerController::PlayerInput() noexcept
{

#pragma region PlayerInput
	//if (Input::GetKeyState(VK_SHIFT) != EKeyState::HOLD)
	//{
	//	if (Input::GetKeyState('W') == EKeyState::HOLD)
	//	{
	//		m_Hero.SetANIM(Guard_FORWARD);
	//		//m_Hero.SetANIM(Guard_DANCE3);
	//		m_Hero.Translate(m_Hero.GetForward() * Timer::SPF * 50);
	//	}
	//	if (Input::GetKeyState('S') == EKeyState::HOLD)
	//	{
	//		m_Hero.SetANIM(Guard_BACKWARD);
	//		m_Hero.Translate(m_Hero.GetBackward() * Timer::SPF * 50);
	//	}
	//	if (Input::GetKeyState('A') == EKeyState::HOLD)
	//	{
	//		m_Hero.SetANIM(Guard_LEFT);
	//		m_Hero.Translate(m_Hero.GetLeft() * Timer::SPF * 50);
	//	}
	//	if (Input::GetKeyState('D') == EKeyState::HOLD)
	//	{
	//		m_Hero.SetANIM(Guard_RIGHT);
	//		m_Hero.Translate(m_Hero.GetRight() * Timer::SPF * 50);
	//	}
	//	if (Input::GetKeyState('Q') == EKeyState::HOLD)
	//	{
	//		m_Hero.SetANIM(Guard_DANCE1);
	//		m_Hero.Rotate(Quaternion::Left * Timer::SPF);
	//	}
	//	if (Input::GetKeyState('E') == EKeyState::HOLD)
	//	{
	//		m_Hero.SetANIM(Guard_DANCE2);
	//		m_Hero.Rotate(Quaternion::Right * Timer::SPF);
	//	}
	//	if (Input::GetKeyState(VK_SPACE) == EKeyState::DOWN)
	//	{
	//		m_Hero.SetANIM(Guard_DANCE3);
	//		m_Hero.Translate(Vector3::Up * 5.0f);
	//		m_pHeroCollider->m_force = Vector3::Up * 180.0f;
	//	}
	//}
#pragma endregion
}


void PlayerController::SetAnimation(const EAction& eAction) noexcept
{
	if (!m_isCharacter ||
		m_curAction == eAction)
		return;

	switch(eAction)
	{
	case EAction::Idle:
	{
		
	}
	}
}