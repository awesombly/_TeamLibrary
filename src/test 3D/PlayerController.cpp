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
			pObject->SetANIM_OneTime(Guard_DANCE1);
		}	break;
		case EAction::Dance2:
		{
			pObject->SetANIM_OneTime(Guard_DANCE2);
		}	break;
		case EAction::Dance3:
		{
			pObject->SetANIM_OneTime(Guard_DANCE3);
		}	break;
		case EAction::Throw:
		{
			pObject->SetANIM_OneTime(Guard_THROW);
			auto pDagger = ObjectManager::Get().TakeObject(L"Dagger");
			pDagger->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
			pDagger->SetRotation(pObject->GetRotation() + Quaternion::Up * 0.5f);
			pDagger->SetScale(Vector3::One * 4.0f);
			((Collider*)pDagger->GetComponentList(EComponent::Collider)->front())->SetForce((pObject->GetForward() + Vector3::Up * 0.5f) * 300.0f);
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
			//pObject->SetANIM_OneTime(Zombie_FLY);
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
			pObject->SetANIM(Zombie_RUN);
		}	break;
		case EAction::Backward:
		{
			pObject->SetANIM(Zombie_BACKWARD);
		}	break;
		case EAction::Dance1:
		{
			pObject->SetANIM_OneTime(Zombie_DANCE1);
		}	break;
		case EAction::Dance2:
		{
			pObject->SetANIM_OneTime(Zombie_DANCE2);
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
		//m_direction = Vector3::Zero;
		//m_pCollider = (Collider*)m_pParent->GetComponentList(EComponent::Collider)->front();

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

		m_curDelayThrow += spf;
		if (m_curDelayThrow > m_DelayThrow)
		{
			//if (m_curAnim == EAction::Idle)
				//SetAnim((AHeroObj*)m_pParent, m_curCharacter, EAction::Idle);
			if (Input::GetMouseState(EMouseButton::Left) == EKeyState::DOWN)
			{
				m_curDelayThrow = 0.0f;
				eAction = EAction::Throw;
			}
		}



		if (eAction != m_curAction)
		{
			static Packet_AnimTransform p_AnimTransform;
			// 이동 처리
			p_AnimTransform.Position = m_pParent->GetPosition();
			switch (eAction)
			{
			case EAction::Jump:
			{
				p_AnimTransform.Position += Vector3::Up * 15.0f;
				p_AnimTransform.Direction = Vector3::Up * m_jumpPower;
			}	break;
			case EAction::Left:
			{
				p_AnimTransform.Direction = m_pParent->GetLeft() * m_moveSpeed;
			}	break;
			case EAction::Right:
			{
				p_AnimTransform.Direction = m_pParent->GetRight() * m_moveSpeed;
			}	break;
			case EAction::Forward:
			{
				p_AnimTransform.Direction = m_pParent->GetForward() * m_moveSpeed;
			}	break;
			case EAction::ForwardLeft:
			{
				p_AnimTransform.Direction = (m_pParent->GetForward() + m_pParent->GetLeft()) * 0.7f * m_moveSpeed;
			}	break;
			case EAction::ForwardRight:
			{
				p_AnimTransform.Direction = (m_pParent->GetForward() + m_pParent->GetRight()) * 0.7f * m_moveSpeed;
			}	break;
			case EAction::Backward:
			{
				p_AnimTransform.Direction = m_pParent->GetBackward() * m_moveSpeed;
			}	break;
			case EAction::BackwardLeft:
			{
				p_AnimTransform.Direction = (m_pParent->GetBackward() + m_pParent->GetLeft()) * 0.7f * m_moveSpeed;
			}	break;
			case EAction::BackwardRight:
			{
				p_AnimTransform.Direction = (m_pParent->GetBackward() + m_pParent->GetRight()) * 0.7f * m_moveSpeed;
			}	break;
			default:
			{
				p_AnimTransform.Direction = Vector3::Zero;
			}	break;
			}
			p_AnimTransform.KeyValue = m_pParent->m_keyValue;
			p_AnimTransform.Rotation = m_pParent->GetRotation();
			p_AnimTransform.EAnimState = m_curAnim = eAction;
			p_AnimTransform.ECharacter = m_curCharacter;
			PacketManager::Get().SendPacket((char*)&p_AnimTransform, sizeof(Packet_AnimTransform), PACKET_SetAnimTransform);
		}
		m_curAction = eAction;

		if (!PacketManager::Get().isHost &&
			Input::GetKeyState(VK_CONTROL) == EKeyState::DOWN)
		{
			PacketManager::Get().SendPacket((char*)&PI, sizeof(PI), PACKET_ReqSync);
		}


		////m_pHero = (AHeroObj*)m_pParent;
		//
		//// 이동 처리
		//switch (eAction)
		//{
		//case EAction::Idle:
		//{
		//	if (m_isLoopAnim)
		//	{
		//		m_pCollider->isMoving(false);
		//		m_isLoopAnim = false;
		//	}
		//}	break;
		//case EAction::Jump:
		//{
		//	m_pParent->Translate(Vector3::Up * 15.0f);
		//	m_pCollider->SetForce(Vector3::Up * m_jumpPower);
		//	m_isLoopAnim = false;
		//}	break;
		//case EAction::Dance1:
		//case EAction::Dance2:
		//case EAction::Dance3:
		//case EAction::Throw:
		//{
		//	m_isLoopAnim = false;
		//}	break;
		//case EAction::Left:
		//{
		//	m_pCollider->SetDirectionForce(m_pParent->GetLeft() * m_moveSpeed);
		//	m_isLoopAnim = true;
		//}	break;
		//case EAction::Right:
		//{
		//	m_pCollider->SetDirectionForce(m_pParent->GetRight() * m_moveSpeed);
		//	m_isLoopAnim = true;
		//}	break;
		//case EAction::Forward:
		//{
		//	m_pCollider->SetDirectionForce(m_pParent->GetForward() * m_moveSpeed);
		//	m_isLoopAnim = true;
		//}	break;
		//case EAction::ForwardLeft:
		//{
		//	m_pCollider->SetDirectionForce((m_pParent->GetForward() + m_pParent->GetLeft()) * 0.7f * m_moveSpeed);
		//	m_isLoopAnim = true;
		//}	break;
		//case EAction::ForwardRight:
		//{
		//	m_pCollider->SetDirectionForce((m_pParent->GetForward() + m_pParent->GetRight()) * 0.7f * m_moveSpeed);
		//	m_isLoopAnim = true;
		//}	break;
		//case EAction::Backward:
		//{
		//	m_pCollider->SetDirectionForce(m_pParent->GetBackward() * m_moveSpeed);
		//	m_isLoopAnim = true;
		//}	break;
		//case EAction::BackwardLeft:
		//{
		//	m_pCollider->SetDirectionForce((m_pParent->GetBackward() + m_pParent->GetLeft()) * 0.7f * m_moveSpeed);
		//	m_isLoopAnim = true;
		//}	break;
		//case EAction::BackwardRight:
		//{
		//	m_pCollider->SetDirectionForce((m_pParent->GetBackward() + m_pParent->GetRight()) * 0.7f * m_moveSpeed);
		//	m_isLoopAnim = true;
		//}	break;
		//}
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
	if (Input::GetKeyState('R') == EKeyState::DOWN)
	{
		Input::isDebug = true;
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





////bool PlayerController::isCharacter() noexcept
//{
//	return m_isCharacter;
//}
////void PlayerController::isCharacter(const bool& isCharacter) noexcept
//{
//	m_isCharacter = isCharacter;
//}
