#include "PlayerController.h"
#include "Collider.h"
#include "AHeroObj.h"
#include "ObjectManager.h"
#include "PacketManager.h"


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




void PlayerController::SetAnim(AHeroObj* pObject, const ECharacter& eCharacter, const EAction& eAction) noexcept
{
	switch (eCharacter)
	{
	case ECharacter::EGuard:
	{
		switch (eAction)
		{
		case EAction::Idle:
		{
			pObject->SetANIM(Guard_IDLE);
		}	break;
		case EAction::Jump:
		{
			///
		}	break;
		case EAction::Left:
		case EAction::BackwardLeft:
		{
			pObject->SetANIM(Guard_LEFT);
		}	break;
		case EAction::Right:
		case EAction::BackwardRight:
		{
			pObject->SetANIM(Guard_RIGHT);
		}	break;
		case EAction::Forward:
		case EAction::ForwardLeft:
		case EAction::ForwardRight:
		{
			pObject->SetANIM(Guard_HAPPYWALK);
		}	break;
		case EAction::Backward:
		{
			pObject->SetANIM(Guard_BACKWARD);
		}	break;
		case EAction::Dance1:
		{
			pObject->SetANIM(Guard_DANCE1);
		}	break;
		case EAction::Dance2:
		{
			pObject->SetANIM(Guard_DANCE2);
		}	break;
		case EAction::Dance3:
		{
			pObject->SetANIM(Guard_DANCE3);
		}	break;
		case EAction::Throw:
		{
			pObject->SetANIM_OneTime(Guard_THROW);
		}	break;
		}
	}	break;
	case ECharacter::EZombie:
	{
		switch (eAction)
		{
		case EAction::Idle:
		{
			pObject->SetANIM(Zombie_IDLE);
		}	break;
		case EAction::Jump:
		{
			pObject->SetANIM(Zombie_FLY);
		}	break;
		case EAction::Left:
		case EAction::ForwardLeft:
		case EAction::BackwardLeft:
		{
			pObject->SetANIM(Zombie_LEFT);
		}	break;
		case EAction::Right:
		case EAction::ForwardRight:
		case EAction::BackwardRight:
		{
			pObject->SetANIM(Zombie_RIGHT);
		}	break;
		case EAction::Forward:
		{
			pObject->SetANIM(Zombie_FORWARD);
		}	break;
		case EAction::Backward:
		{
			pObject->SetANIM(Zombie_BACKWARD);
		}	break;
		}
	}	break;
	}
}

void PlayerController::PlayerInput(const float& spf) noexcept
{
	if (Input::GetKeyState(VK_SHIFT) != EKeyState::HOLD)
	{
		//m_toIdle = true;
		static EAction eAction;
		eAction = EAction::Idle;
		m_direction = Vector3::Zero;
		m_pCollider = (Collider*)m_pParent->GetComponentList(EComponent::Collider)->front();

		if (Input::GetKeyState('W') == EKeyState::HOLD)
		{
			eAction = (EAction)(eAction + EAction::Forward);
		}
		if (Input::GetKeyState('S') == EKeyState::HOLD)
		{
			eAction = (EAction)(eAction + EAction::Backward);
		}
		if (Input::GetKeyState('A') == EKeyState::HOLD)
		{
			eAction = (EAction)(eAction + EAction::Left);
		}
		if (Input::GetKeyState('D') == EKeyState::HOLD)
		{
			eAction = (EAction)(eAction + EAction::Right);
		}
		if (Input::GetKeyState('1') == EKeyState::DOWN)
		{
			eAction = EAction::Dance1;
		}
		if (Input::GetKeyState('2') == EKeyState::DOWN)
		{
			eAction = EAction::Dance2;
		}
		if (Input::GetKeyState('3') == EKeyState::DOWN)
		{
			eAction = EAction::Dance3;
		}
		if (Input::GetKeyState(VK_SPACE) == EKeyState::DOWN)
		{
			eAction = EAction::Jump;
		}
		if (Input::GetMouseState(EMouseButton::Left) == EKeyState::DOWN)
		{
			eAction = EAction::Throw;
		}

		if (eAction != m_curAction)
		{
			//static Packet_AnimState p_AnimState;
			//p_AnimState.KeyValue = m_keyValue;
			//p_AnimState.EAnimState = eAction;
			//PacketManager::Get().SendPacket((char*)&p_AnimState, PACKET_SetAnimation);
			if (m_curCharacter == ECharacter::EDummy)
			{
				static Packet_Vec3Quat p_transform;
				p_transform.KeyValue = m_pParent->m_keyValue;
				p_transform.Vec3 = m_pParent->GetPosition();
				p_transform.Quat = m_pParent->GetRotation();
				PacketManager::Get().SendPacket((char*)&p_transform, PACKET_SetTransform);
			}
			else
			{
				static Packet_AnimTransform p_AnimState;
				p_AnimState.KeyValue = m_pParent->m_keyValue;
				p_AnimState.Vec3 = m_pParent->GetPosition();
				p_AnimState.Quat = m_pParent->GetRotation();
				p_AnimState.EAnimState = eAction;
				p_AnimState.ECharacter = m_curCharacter;
				PacketManager::Get().SendPacket((char*)&p_AnimState, PACKET_SetAnimTransform);
			}
		}

		m_pHero = (AHeroObj*)m_pParent;
		m_curAction = eAction;

		// 이동 처리
		switch (eAction)
		{
		case EAction::Idle:
		{
			if (m_isLoopAnim)
			{
				m_pCollider->isMoving(false);
				m_isLoopAnim = false;
			}
		}	break;
		case EAction::Jump:
		{
			m_pParent->Translate(Vector3::Up * 15.0f);
			m_pCollider->SetForce(Vector3::Up * m_jumpPower);
			m_isLoopAnim = false;
		}	break;
		case EAction::Dance1:
		case EAction::Dance2:
		case EAction::Dance3:
		case EAction::Throw:
		{
			m_isLoopAnim = false;
		}	break;
		case EAction::Left:
		{
			m_pCollider->SetDirectionForce(m_pParent->GetLeft() * m_moveSpeed);
			m_isLoopAnim = true;
		}	break;
		case EAction::Right:
		{
			m_pCollider->SetDirectionForce(m_pParent->GetRight() * m_moveSpeed);
			m_isLoopAnim = true;
		}	break;
		case EAction::Forward:
		{
			m_pCollider->SetDirectionForce(m_pParent->GetForward() * m_moveSpeed);
			m_isLoopAnim = true;
		}	break;
		case EAction::ForwardLeft:
		{
			m_pCollider->SetDirectionForce((m_pParent->GetForward() + m_pParent->GetLeft()) * 0.7f * m_moveSpeed);
			m_isLoopAnim = true;
		}	break;
		case EAction::ForwardRight:
		{
			m_pCollider->SetDirectionForce((m_pParent->GetForward() + m_pParent->GetRight()) * 0.7f * m_moveSpeed);
			m_isLoopAnim = true;
		}	break;
		case EAction::Backward:
		{
			m_pCollider->SetDirectionForce(m_pParent->GetBackward() * m_moveSpeed);
			m_isLoopAnim = true;
		}	break;
		case EAction::BackwardLeft:
		{
			m_pCollider->SetDirectionForce((m_pParent->GetBackward() + m_pParent->GetLeft()) * 0.7f * m_moveSpeed);
			m_isLoopAnim = true;
		}	break;
		case EAction::BackwardRight:
		{
			m_pCollider->SetDirectionForce((m_pParent->GetBackward() + m_pParent->GetRight()) * 0.7f * m_moveSpeed);
			m_isLoopAnim = true;
		}	break;
		}
	}
	spf;
}


//void PlayerController::SetAnimation(const EAction& eAction) noexcept
//{
//	//m_toIdle = false;
//	if (!m_isCharacter ||
//		m_pParent == nullptr)	return;
//
//	//static Packet_Vector3 p_direction;
//	//static Packet_KeyValue p_Key;
//	m_pHero = (AHeroObj*)m_pParent;
//
//	m_curAction = eAction;
//	//if (m_curAction != eAction)
//	//{
//	switch (m_curCharacter)
//	{
//	 case ECharacter::EGuard:
//	 {
//	 	AnimationGuard(eAction);
//	 }	break;
//	 case ECharacter::EZombie:
//	 {
//	 	AnimationZombi(eAction);
//	 }	break;
//	 case ECharacter::EDummy:
//	 {
//	 	switch (eAction)
//	 	 {
//	 	 case EAction::Idle:
//	 	 {
//	 	 	if (m_isLoopAnim)
//	 	 	{
//	 	 		m_pCollider->isMoving(false);
//	 	 		m_isLoopAnim = false;
//	 	 	}
//	 	 }	break;
//	 	 case EAction::Jump:
//	 	 {
//	 	 	m_pParent->Translate(Vector3::Up * 15.0f);
//	 	 	m_pCollider->SetForce(Vector3::Up * m_jumpPower);
//	 	 	m_isLoopAnim = false;
//	 	 }	break;
//		 default:
//		 {
//		 	m_isLoopAnim = true;
//		 }	break;
//	 	}
//	 }	break;
//	}
//}
//
//void PlayerController::AnimationGuard(const EAction& eAction) noexcept
//{
//	switch (eAction)
//	{
//	case EAction::Idle:
//	{
//		if (m_isLoopAnim)
//		{
//			m_pCollider->isMoving(false);
//			m_isLoopAnim = false;
//			m_pHero->SetANIM(Guard_IDLE);
//		}
//	}	break;
//	case EAction::Jump:
//	{
//		m_pParent->Translate(Vector3::Up * 15.0f);
//
//		m_pCollider->SetForce(Vector3::Up * m_jumpPower);
//		m_isLoopAnim = false;
//		///m_pHero->SetANIM(Guard_DANCE3);
//	}	break;
//	case EAction::Left:
//	case EAction::BackwardLeft:
//	{
//		m_isLoopAnim = true;
//		m_pHero->SetANIM(Guard_LEFT);
//	}	break;
//	case EAction::Right:
//	case EAction::BackwardRight:
//	{
//		m_isLoopAnim = true;
//		m_pHero->SetANIM(Guard_RIGHT);
//	}	break;
//	case EAction::Forward:
//	case EAction::ForwardLeft:
//	case EAction::ForwardRight:
//	{
//		m_isLoopAnim = true;
//		m_pHero->SetANIM(Guard_HAPPYWALK);
//	}	break;
//	case EAction::Backward:
//	{
//		m_isLoopAnim = true;
//		m_pHero->SetANIM(Guard_BACKWARD);
//	}	break;
//	case EAction::Dance1:
//	{
//		m_isLoopAnim = false;
//		m_pHero->SetANIM(Guard_DANCE1);
//	}	break;
//	case EAction::Dance2:
//	{
//		m_isLoopAnim = false;
//		m_pHero->SetANIM(Guard_DANCE2);
//	}	break;
//	case EAction::Dance3:
//	{
//		m_isLoopAnim = false;
//		m_pHero->SetANIM(Guard_DANCE3);
//	}	break;
//	case EAction::Throw:
//	{
//		m_isLoopAnim = false;
//		m_pHero->SetANIM_OneTime(Guard_THROW);
//	}	break;
//	}
//}
//
//void PlayerController::AnimationZombi(const EAction& eAction) noexcept
//{
//	switch (eAction)
//	{
//	case EAction::Idle:
//	{
//		if (m_isLoopAnim)
//		{
//			m_pCollider->isMoving(false);
//			m_isLoopAnim = false;
//			m_pHero->SetANIM(Zombie_IDLE);
//		}
//	}	break;
//	case EAction::Jump:
//	{
//		m_pParent->Translate(Vector3::Up * 15.0f);
//
//		m_pCollider->SetForce(Vector3::Up * m_jumpPower);
//		m_isLoopAnim = false;
//		m_pHero->SetANIM(Zombie_FLY);
//	}	break;
//	case EAction::Left:
//	case EAction::ForwardLeft:
//	case EAction::BackwardLeft:
//	{
//		m_isLoopAnim = true;
//		m_pHero->SetANIM(Zombie_LEFT);
//	}	break;
//	case EAction::Right:
//	case EAction::ForwardRight:
//	case EAction::BackwardRight:
//	{
//		m_isLoopAnim = true;
//		m_pHero->SetANIM(Zombie_RIGHT);
//	}	break;
//	case EAction::Forward:
//	{
//		m_isLoopAnim = true;
//		m_pHero->SetANIM(Zombie_FORWARD);
//	}	break;
//	case EAction::Backward:
//	{
//		m_isLoopAnim = true;
//		m_pHero->SetANIM(Zombie_BACKWARD);
//	}	break;
//	}
//}

void PlayerController::CameraInput(const float& spf) noexcept
{
	static const float MinCameraY = -PI * 0.2f;
	static const float MaxCameraY = PI * 0.4f;

	if (!Input::isDebug)
	{
		// 마우스 고정
		static POINT prevPoint = { 0, 0 };
		prevPoint = Input::GetCursor();
		SetCursorPos((int)m_setMouseScreen.x, (int)m_setMouseScreen.y);

		Input::OperMoveMousePos({ (float)(-m_setMouseClient.x + prevPoint.x), (float)(-m_setMouseClient.y + prevPoint.y) });
	}

	// 카메라 Arm 조절
	m_pCamera->m_armLength = std::clamp(m_pCamera->m_armLength - Input::GetWheelScroll() * 0.3f * spf, 0.0f, 50.0f);
	// 회전
	m_pCamera->SetRotationX(std::clamp(m_pCamera->GetRotation().x + Input::GetMouseMovePos().y * 0.002f, MinCameraY, MaxCameraY));
	m_pParent->Rotate(0.0f, Input::GetMouseMovePos().x * 0.002f);
	//static Packet_Quaternion p_rotate;
	//p_rotate.KeyValue = m_pParent->m_keyValue;
	//p_rotate.Quat = m_pParent->GetRotation();
	//PacketManager::Get().SendPacket((char*)&p_rotate, PACKET_Rotate);
	// 초기화
	if (Input::GetInstance().GetKeyState('R') == EKeyState::DOWN)
	{
		ResetOption();
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
	m_pCamera->m_armLength = 6.0f;
	m_pCamera->m_lerpMoveSpeed = 7.0f;
	m_pCamera->m_lerpRotateSpeed = 7.0f;
}





bool PlayerController::isCharacter() noexcept
{
	return m_isCharacter;
}
void PlayerController::isCharacter(const bool& isCharacter) noexcept
{
	m_isCharacter = isCharacter;
}
