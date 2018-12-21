#include "PlayerController.h"
#include "Collider.h"
#include "AHeroObj.h"
#include "ObjectManager.h"


bool PlayerController::Init() noexcept
{
	GameObject::Init();
	return true;
}

bool PlayerController::Frame(const float& spf, const float& accTime)	noexcept
{
	GameObject::Frame(spf, accTime);

	if (m_pParent != nullptr)
	{
		PlayerInput(spf);
		CameraInput(spf);
	}
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


bool PlayerController::isCharacter() noexcept
{
	return m_isCharacter;
}
void PlayerController::isCharacter(const bool& isCharacter) noexcept
{
	m_isCharacter = isCharacter;
}


void PlayerController::SetAnimation(const EAction& eAction) noexcept
{
	m_toIdle = false;
	if (!m_isCharacter ||
		m_pParent == nullptr ||
		m_curAction == eAction)	return;

	AHeroObj* pHero = (AHeroObj*)m_pParent;
	m_curAction = eAction;
	switch(eAction)
	{
	case EAction::Idle:
	 {
		m_isLoopAnim = false;
	 	pHero->SetANIM(Guard_IDLE);
	 }	break;
	case EAction::Jump:
	 {
		m_isLoopAnim = false;
	 	///pHero->SetANIM(Guard_DANCE3);
	 }	break;
	case EAction::Left:
	 {
		m_isLoopAnim = true;
	 	pHero->SetANIM(Guard_LEFT);
	 }	break;
	case EAction::Right:
	 {
		m_isLoopAnim = true;
	 	pHero->SetANIM(Guard_RIGHT);
	 }	break;
	case EAction::Forward:
	 {
		m_isLoopAnim = true;
	 	pHero->SetANIM(Guard_FORWARD);
	 }	break;
	case EAction::Backward:
	 {
		m_isLoopAnim = true;
	 	pHero->SetANIM(Guard_BACKWARD);
	 }	break;
	case EAction::Dance1:
	 {
		m_isLoopAnim = false;
	 	pHero->SetANIM(Guard_DANCE1);
	 }	break;
	case EAction::Dance2:
	 {
		m_isLoopAnim = false;
	 	pHero->SetANIM(Guard_DANCE2);
	 }	break;
	case EAction::Dance3:
	 {
		m_isLoopAnim = false;
	 	pHero->SetANIM(Guard_DANCE3);
	 }	break;
	}
}

void PlayerController::PlayerInput(const float& spf) noexcept
{
	if (Input::GetKeyState(VK_SHIFT) != EKeyState::HOLD)
	{
		m_toIdle = true;
		if (Input::GetKeyState('W') == EKeyState::HOLD)
		{
			SetAnimation(EAction::Forward);
			m_pParent->Translate(m_pParent->GetForward() * Timer::SPF * 70);
		}
		if (Input::GetKeyState('S') == EKeyState::HOLD)
		{
			SetAnimation(EAction::Backward);
			m_pParent->Translate(m_pParent->GetBackward() * Timer::SPF * 70);
		}
		if (Input::GetKeyState('A') == EKeyState::HOLD)
		{
			SetAnimation(EAction::Left);
			m_pParent->Translate(m_pParent->GetLeft() * Timer::SPF * 70);
		}
		if (Input::GetKeyState('D') == EKeyState::HOLD)
		{
			SetAnimation(EAction::Right);
			m_pParent->Translate(m_pParent->GetRight() * Timer::SPF * 70);
		}
		if (Input::GetKeyState('Q') == EKeyState::HOLD)
		{
			m_pParent->Rotate(Quaternion::Left * Timer::SPF);
		}
		if (Input::GetKeyState('E') == EKeyState::HOLD)
		{
			m_pParent->Rotate(Quaternion::Right * Timer::SPF);
		}
		if (Input::GetKeyState('1') == EKeyState::HOLD)
		{
			SetAnimation(EAction::Dance1);
		}
		if (Input::GetKeyState('2') == EKeyState::HOLD)
		{
			SetAnimation(EAction::Dance2);
		}
		if (Input::GetKeyState('3') == EKeyState::HOLD)
		{
			SetAnimation(EAction::Dance3);
		}
		if (Input::GetKeyState(VK_SPACE) == EKeyState::DOWN)
		{
			SetAnimation(EAction::Dance3);
			m_pParent->Translate(Vector3::Up * 5.0f);
			
			m_pCollider = (Collider*)m_pParent->GetComponentList(EComponent::Collider)->front();
			if (m_pCollider != nullptr)
			{
				m_pCollider->SetForce(Vector3::Up * 180.0f);
			}
		}
		// 입력 없을시 Idle
		if (m_toIdle && m_isLoopAnim)
		{
			SetAnimation(EAction::Idle);
		}
	}

	spf;
}

void PlayerController::CameraInput(const float& spf) noexcept
{
	static const float MinCameraY = -PI * 0.2f;
	static const float MaxCameraY =  PI * 0.4f;

	if (!Input::isDebug)
	{
		// 마우스 고정
		static POINT prevPoint = { 0, 0 };
		prevPoint = Input::GetCursor();
		SetCursorPos((int)m_setMouseScreen.x, (int)m_setMouseScreen.y);

		Input::OperMoveMousePos({ (float)(-m_setMouseClient.x + prevPoint.x), (float)(-m_setMouseClient.y + prevPoint.y) });

		// 카메라 Arm 조절
		m_pCamera->m_armLength = std::clamp(m_pCamera->m_armLength - Input::GetWheelScroll() * 0.3f * spf, 0.0f, 30.0f);
		// 회전
		m_pCamera->SetRotationX(std::clamp(m_pCamera->GetRotation().x + Input::GetMouseMovePos().y * 0.002f, MinCameraY, MaxCameraY));
		m_pParent->Rotate(0.0f, Input::GetMouseMovePos().x * 0.002f);
		// 초기화
		if (Input::GetInstance().GetKeyState('R') == EKeyState::DOWN)
		{
			ResetOption();
		}
	}
}

void PlayerController::ResetOption() noexcept
{
	m_setMouseScreen = { (LONG)Window::GetWinCenter().x, (LONG)Window::GetWinCenter().y };
	m_setMouseClient = m_setMouseScreen;
	ScreenToClient(Window::m_hWnd, &m_setMouseClient);
	///
	m_pCamera = ObjectManager::Cameras[ECamera::Main];
	m_pCamera->SetPosition(Vector3::Up * 75.0f);
	m_pCamera->SetRotation(Quaternion::Left * PI + Quaternion::Up * PI * 0.2f);
	m_pCamera->m_armLength		 = 6.0f;
	m_pCamera->m_lerpMoveSpeed   = 5.0f;
	m_pCamera->m_lerpRotateSpeed = 6.0f;
}