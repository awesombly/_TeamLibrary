#include "PlayerController.h"
#include "Collider.h"
#include "AHeroObj.h"
#include "ObjectManager.h"
#include "PacketManager.h"
#include "SoundManager.h"
#include "CEventTimer.h"

#include "uiheader.h"
#include "JPanel.h"
#include "RPlane.h"

bool PlayerController::Init() noexcept
{
	GameObject::Init();
	return true;
}

bool PlayerController::Frame(const float& spf, const float& accTime)	noexcept
{
	GameObject::Frame(spf, accTime);
	if (!m_isChatting &&
		!((JPanel*)m_pOption)->m_bRender &&
		m_pParent != nullptr)
	{
		PlayerInput(spf);
		CameraInput(spf);

		// 초기화
		if (Input::GetKeyState('R') == EKeyState::DOWN)
		{
			Input::isDebug = !Input::isDebug;
			ResetOption();
		}
		if (Input::GetKeyState(VK_SUBTRACT) == EKeyState::DOWN)
		{
			m_curDelayRespawn = -9999.9f;
			CutParentPost();
			ObjectManager::Cameras[ECamera::Main]->CutParentPost();
		}
	}
	else if(m_pParent == nullptr)
	{
		// 리스폰
		m_curDelayRespawn += spf;
		if (m_curDelayRespawn >= m_DelayRespawn)
		{
			((JPanel*)m_pRespawn)->m_bRender = false;
			m_curDelayRespawn = 0.0f;

			SendReqRespawn(m_selectCharacter);
			return true;
		}
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




void PlayerController::SetAnim(AHeroObj* pObject, const UINT& socket, const ECharacter& eCharacter, const EAction& eAction, const D3DXVECTOR3& forward) noexcept
{
	switch (eCharacter)
	{
	case ECharacter::EGuard:
	{
		switch (eAction)
		{
		case EAction::Idle:
		{
			pObject->SetANIM_Loop(Guard_IDLE);
		}	break;
		case EAction::Jump:
		{
			///
		}	break;
		case EAction::Left:
		case EAction::BackwardLeft:
		{
			pObject->SetANIM_Loop(Guard_LEFT);
			//SoundManager::Get().PlayQueue("SE_footstep.mp3", pObject->GetWorldPosition(), 1000.0f);
		}	break;
		case EAction::Right:
		case EAction::BackwardRight:
		{
			pObject->SetANIM_Loop(Guard_RIGHT);
			//SoundManager::Get().PlayQueue("SE_footstep.mp3", pObject->GetWorldPosition(), 1000.0f);
		}	break;
		case EAction::Forward:
		case EAction::ForwardLeft:
		case EAction::ForwardRight:
		{
			pObject->SetANIM_Loop(Guard_HAPPYWALK);
		//	SoundManager::Get().PlayQueue("SE_footstep.mp3", pObject->GetWorldPosition(), 1000.0f);
		}	break;
		case EAction::Backward:
		{
			pObject->SetANIM_Loop(Guard_BACKWARD);
			//SoundManager::Get().PlayQueue("SE_footstep.mp3", pObject->GetWorldPosition(), 1000.0f);
		}	break;
		case EAction::Dance1:
		{
			pObject->SetANIM_OneTime(Guard_DANCE1);
			SoundManager::Get().Play("SE_Dance01.mp3");// , pObject->GetWorldPosition(), 1000.0f);
		}	break;
		case EAction::Dance2:
		{
			pObject->SetANIM_OneTime(Guard_DANCE2);
			SoundManager::Get().Play("SE_Dance02.mp3");//, pObject->GetWorldPosition(), 1000.0f);
		}	break;
		case EAction::Dance3:
		{
			pObject->SetANIM_OneTime(Guard_DANCE3);
			SoundManager::Get().Play("SE_Dance01.mp3");// , pObject->GetWorldPosition(), 1000.0f);
		}	break;
		case EAction::Throw:
		{
			pObject->SetANIM_OneTime(Guard_THROW);
			auto pDagger = ObjectManager::Get().TakeObject(L"Dagger");
			pDagger->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
			pDagger->SetRotation(pObject->GetRotation());
			pDagger->SetForce((forward + Vector3::Up * 0.15f) * 300.0f);
			pDagger->m_pPhysics->UserSocket = socket;
			((Collider*)pDagger->GetComponentList(EComponent::Collider)->front())->AddIgnoreList((Collider*)pObject->GetComponentList(EComponent::Collider)->front());
			SoundManager::Get().PlayQueue("SE_throw01.mp3", pObject->GetWorldPosition(), 1000.0f);
		}	break;
		case EAction::Melee:
		{
			pObject->SetANIM_OneTime(Guard_PUNCH);
			SoundManager::Get().PlayQueue("SV_Guard_Punch.mp3", pObject->GetWorldPosition(), 1000.0f);
			
			auto pCollider = new Collider(8.0f);
			auto pMelee = new GameObject(L"Melee", { pCollider, new CEventTimer(0.5f) });
			pMelee->SetParent(pObject);
			pMelee->SetPosition(pObject->GetForward() * 45.0f + pObject->GetUp() * 45.0f);
			pMelee->SetRotation(pObject->GetRotation());
			pMelee->UpdateMatrix();
			pMelee->m_pPhysics->UserSocket = socket;
			pMelee->SetHP(100.0f);
			pCollider->CollisionEvent = MyEvent::MeleeHit;
			pCollider->AddIgnoreList((Collider*)pObject->GetComponentList(EComponent::Collider)->front());
			pCollider->m_eTag = ETag::Dummy;
			pCollider->SetGravityScale(0.0f);
			pCollider->usePhysics(false);
		}	break;
		}
	}	break;
	case ECharacter::EZombie:
	{
		switch (eAction)
		{
		 case EAction::Idle:
		 {
		 	pObject->SetANIM_Loop(Zombie_IDLE);
		 }	break;
		 case EAction::Jump:
		 {
		 	//pObject->SetANIM_OneTime(Zombie_FLY);
		 }	break;
		 case EAction::Left:
		 case EAction::BackwardLeft:
		 {
		 	pObject->SetANIM_Loop(Zombie_LEFT);
		 }	break;
		 case EAction::Right:
		 case EAction::BackwardRight:
		 {
		 	pObject->SetANIM_Loop(Zombie_RIGHT);
		 }	break;
		 case EAction::Forward:
		 case EAction::ForwardLeft:
		 case EAction::ForwardRight:
		 {
		 	pObject->SetANIM_Loop(Zombie_RUN);
		 }	break;
		 case EAction::Backward:
		 {
		 	pObject->SetANIM_Loop(Zombie_BACKWARD);
		 }	break;
		 case EAction::Dance1:
		 {
		 	pObject->SetANIM_OneTime(Zombie_DANCE1);
			SoundManager::Get().Play("SE_Dance01.mp3");// , pObject->GetWorldPosition(), 1000.0f);
		 }	break;
		 case EAction::Dance2:
		 {
		 	pObject->SetANIM_OneTime(Zombie_DANCE2);
			SoundManager::Get().Play("SE_Dance02.mp3");// pObject->GetWorldPosition(), 1000.0f);
		 }	break;
		 case EAction::Throw:
		 {
		 	pObject->SetANIM_OneTime(Zombie_THROW);
		 	auto pChicken = ObjectManager::Get().TakeObject(L"Chicken");
		 	pChicken->SetPosition(pObject->GetPosition() + pObject->GetForward() * 40.0f + pObject->GetUp() * 65.0f + pObject->GetRight() * 20.0f);
		 	pChicken->SetRotation(pObject->GetRotation());
		 	pChicken->SetForce((forward + Vector3::Up * 0.15f) * 300.0f);
			pChicken->m_pPhysics->UserSocket = socket;
		 	((Collider*)pChicken->GetComponentList(EComponent::Collider)->front())->AddIgnoreList((Collider*)pObject->GetComponentList(EComponent::Collider)->front());
		 	SoundManager::Get().PlayQueue("SE_chicken.mp3", pObject->GetWorldPosition(), 1000.0f);
		 }	break;
		 case EAction::Melee:
		 {
		 	pObject->SetANIM_OneTime(Zombie_PUNCH);
		 	SoundManager::Get().PlayQueue("SE_zombie_hit01.mp3", pObject->GetWorldPosition(), 1000.0f);
		 
			auto pCollider = new Collider(8.0f);
			auto pMelee = new GameObject(L"Melee", { pCollider, new CEventTimer(0.5f) });
			pMelee->SetParent(pObject);
			pMelee->SetPosition(pObject->GetForward() * 45.0f + pObject->GetUp() * 45.0f);
			pMelee->SetRotation(pObject->GetRotation());
			pMelee->UpdateMatrix();
			pMelee->m_pPhysics->UserSocket = socket;
			pMelee->SetHP(100.0f);
			pCollider->CollisionEvent = MyEvent::MeleeHit;
		 	pCollider->AddIgnoreList((Collider*)pObject->GetComponentList(EComponent::Collider)->front());
		 	pCollider->m_eTag = ETag::Dummy;
		 	pCollider->SetGravityScale(0.0f);
		 	pCollider->usePhysics(false);
		 }	break;
		}
	}	break;
	}
}

void PlayerController::PlayerInput(const float& spf) noexcept
{
	static EAction eAction;
	eAction = EAction::Idle;
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
	if (m_pParent->isGround() && Input::GetKeyState(VK_SPACE) == EKeyState::DOWN)
	{
		eAction = EAction::Jump;
		PacketManager::Get().SendPlaySound("SE_jump01.mp3", PlayerController::Get().GetWorldPosition(), 1000.0f);
	}

	if (m_curCharacter != ECharacter::EDummy)
	{
		m_curDelayThrow = max(m_curDelayThrow - spf, 0.0f);
		m_curDelayDash  = max(m_curDelayDash  - spf, 0.0f);
		m_curDelayMelee = max(m_curDelayMelee - spf, 0.0f);
		// 던지기
		if (Input::GetKeyState(EMouseButton::Left) == EKeyState::DOWN &&
			m_curDelayThrow <= 0.0f &&
			m_MP >= 0.2f)
		{
			m_MP -= 0.2f;
			m_curDelayThrow = m_DelayThrow;
			eAction = EAction::Throw;
		}
		// 구르기
		if (Input::GetKeyState(VK_SHIFT) == EKeyState::DOWN &&
			m_curDelayDash <= 0.0f)
		{
			m_curDelayDash = m_DelayDash;
			eAction = EAction::Dash;
			SoundManager::Get().Play("SE_jump02.mp3");
		}
		// 근접 공격
		if (Input::GetKeyState('E') == EKeyState::DOWN &&
			m_curDelayMelee <= 0.0f)
		{
			m_curDelayMelee = m_DelayMelee;
			m_curDelayThrow = 0.6f;
			m_curDelayDash += 0.3f;
			eAction = EAction::Melee;
		}
	}

	static bool isFly = false;
	if (Input::GetKeyState(EMouseButton::Right) == EKeyState::DOWN)
	{
		isFly = true;
		eAction = EAction::Fly;
		m_pEffectFly = ObjectManager::Get().TakeObject(L"Fly"); //ObjectManager::Get().TakeComponent(L"Fly");
		m_pEffectFly->SetParent(m_pParent);
		//m_pParent->AddComponent(m_pEffectFly);
	}
	if (isFly && Input::GetKeyState(EMouseButton::Right) == EKeyState::HOLD)
	{
		m_MP -= 0.65f * spf;
		if (m_MP <= 0.0f)
		{
			isFly = false;
			eAction = EAction::FlyEnd;
			if (m_pEffectFly != nullptr)
			{
				ObjectManager::Get().DisableObject(m_pEffectFly);
				m_pEffectFly = nullptr;
			}
		}
	}
	else
	{
		m_MP = min(m_MP + spf * 0.3f, 1.0f);
	}

	if (Input::GetKeyState(EMouseButton::Right) == EKeyState::UP)
	{
		isFly = false;
		eAction = EAction::FlyEnd;
		if (m_pEffectFly != nullptr)
		{
			ObjectManager::Get().DisableObject(m_pEffectFly);
			m_pEffectFly = nullptr;
		}
	}

	if (eAction != m_curAction)
	{
		// 정보 전송
		SendAnimTransform(eAction, m_curCharacter);
	}
	m_curAction = eAction;

	// 동기화 요청
	if (Input::GetKeyState(VK_ADD) == EKeyState::DOWN &&
		!PacketManager::Get().isHost)
	{
		PacketManager::Get().SendPacket('\0', 1, PACKET_ReqSync);
	}


	if (Input::GetKeyState('X') == EKeyState::DOWN)
	{
		if (m_pParent != nullptr)
			m_pParent->m_pPhysics->UserSocket = (UINT)-1;
		SendReqRespawn(ECharacter::EGuard);
	}
	if (Input::GetKeyState('Z') == EKeyState::DOWN)
	{
		if (m_pParent != nullptr)
			m_pParent->m_pPhysics->UserSocket = (UINT)-1;
		SendReqRespawn(ECharacter::EZombie);
	}

	spf;
}

void PlayerController::SendAnimTransform(const EAction& eAction, const ECharacter& eCharacter) noexcept
{
	static Packet_AnimTransform p_AnimTransform;

	// 이동 처리
	switch (eAction)
	{
	case EAction::Dash:
	{
		if (m_pParent->isMoving())
			p_AnimTransform.Force = (m_pParent->m_pPhysics->m_direction * 1.5f + Vector3::Up * 30.0f);
		else
			p_AnimTransform.Force = (Normalize(m_pParent->GetForward()) * m_moveSpeed * 1.5f + Vector3::Up * 30.0f);
	}	break;
	case EAction::Jump:
	{
		p_AnimTransform.Force = Vector3::Up * m_jumpPower;
	}	break;
	case EAction::Melee:
	case EAction::Throw:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = ObjectManager::Cameras[ECamera::Main]->GetForward();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction);
	}	break;
	case EAction::Left:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetLeft();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::Right:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetRight();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::Forward:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetForward();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::ForwardLeft:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetForward() + m_pParent->GetLeft();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::ForwardRight:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetForward() + m_pParent->GetRight();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::Backward:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetBackward();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::BackwardLeft:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetBackward() + m_pParent->GetLeft();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	case EAction::BackwardRight:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = m_pParent->GetBackward() + m_pParent->GetRight();
		p_AnimTransform.Direction = Normalize(p_AnimTransform.Direction) * m_moveSpeed;
	}	break;
	default:
	{
		p_AnimTransform.Force = m_pParent->GetForce();
		p_AnimTransform.Direction = Vector3::Zero;
	}	break;
	}
	p_AnimTransform.KeyValue = m_pParent->m_keyValue;
	p_AnimTransform.UserSocket = PacketManager::Get().pMyInfo->UserSocket;
	p_AnimTransform.Position = m_pParent->GetPosition();
	p_AnimTransform.Rotation = m_pParent->GetRotation();
	m_prevRotY = p_AnimTransform.Rotation.y;
	p_AnimTransform.EAnimState = m_curAnim = eAction;
	p_AnimTransform.ECharacter = eCharacter;
	PacketManager::Get().SendPacket((char*)&p_AnimTransform, sizeof(Packet_AnimTransform), PACKET_SetAnimTransform);
}

void PlayerController::SendReqRespawn(const ECharacter& eCharacter) noexcept
{
	static Packet_ReqAddPlayer p_ReqAddPlayer;
	p_ReqAddPlayer.ECharacter = eCharacter;
	PacketManager::Get().ReqSendPacket((char*)&p_ReqAddPlayer, (USHORT)sizeof(Packet_ReqAddPlayer), PACKET_ReqAddPlayer);
}

void PlayerController::SendGiantMode(const float& spf) noexcept
{
	Packet_Vector3 p_SetScale;
	p_SetScale.KeyValue = m_pParent->m_keyValue;
	p_SetScale.Vec3 = m_pParent->GetScale() + Vector3::One * spf;
	PacketManager::Get().SendPacket((char*)&p_SetScale, (USHORT)sizeof(Packet_Vector3), PACKET_SetScale);
}

void PlayerController::StartGiantMode() noexcept
{
	PacketManager::Get().SendPlaySound("SE_jajan.mp3", GetWorldPosition(), 1000.0f);
	float frameCount = 0.0f;
	while (frameCount <= 3.0f)
	{
		if (m_pParent == nullptr)
			break;
		SendGiantMode(0.04f);
		frameCount += 0.15f;
		this_thread::sleep_for(chrono::milliseconds(150));
		ResetOption();
	}
	m_moveSpeed = 120.0f;
	m_jumpPower = 100.0f;
}

void PlayerController::CameraInput(const float& spf) noexcept
{
	static const float MinCameraY = -PI * 0.2f;
	static const float MaxCameraY = PI * 0.4f;

	// 마우스 고정
	static POINT prevPoint = { 0, 0 };
	prevPoint = Input::GetCursor();
	SetCursorPos((int)m_setMouseScreen.x, (int)m_setMouseScreen.y);

	Input::OperMoveMousePos({ (float)(-m_setMouseClient.x + prevPoint.x), (float)(-m_setMouseClient.y + prevPoint.y) });

	// 카메라 Arm 조절
	if (!m_isChatting)
	{
		m_pCamera->m_armLength = std::clamp(m_pCamera->m_armLength - Input::GetWheelScroll() * m_mouseSense * spf, 0.0f, 80.0f);
	}
	// 회전
	m_pCamera->SetRotationX(std::clamp(m_pCamera->GetRotation().x + Input::GetMouseMovePos().y * m_mouseSense * 0.004f, MinCameraY, MaxCameraY));
	m_pParent->Rotate(0.0f, Input::GetMouseMovePos().x * m_mouseSense * 0.004f);
	// 회전 동기화
	if (abs(m_prevRotY - m_pParent->GetRotation().y) > 0.08f)
	{
		SendAnimTransform(m_curAnim, ECharacter::EDummy);
	}

	//static Packet_MouseRotate p_MouseRotate;
	//if (Input::GetMouseMovePos().x > 0.0f &&
	//	m_MouseDirection != EDirection::Right)
	//{
	//	m_MouseDirection = EDirection::Right;
	//	m_prevMouseDir = p_MouseRotate.RotateSpeed = m_mouseSense * 0.1f;
	//	p_MouseRotate.KeyValue = m_pParent->m_keyValue;
	//	PacketManager::Get().SendPacket((char*)&p_MouseRotate, sizeof(Packet_MouseRotate), PACKET_MouseRotate);
	//	
	//	ErrorMessage("회전 : " + to_string(Input::GetMouseMovePos().x));
	//}
	//else if (Input::GetMouseMovePos().x < 0.0f &&
	//		 m_MouseDirection != EDirection::Left)
	//{
	//	m_MouseDirection = EDirection::Left;
	//	m_prevMouseDir = p_MouseRotate.RotateSpeed = -m_mouseSense * 0.1f;
	//	p_MouseRotate.KeyValue = m_pParent->m_keyValue;
	//	PacketManager::Get().SendPacket((char*)&p_MouseRotate, sizeof(Packet_MouseRotate), PACKET_MouseRotate);
	//	ErrorMessage("회전 : " + to_string(Input::GetMouseMovePos().x));
	//}
	//else if (Input::GetMouseMovePos().x == 0.0f &&
	//		 m_MouseDirection != EDirection::Middle)
	//{
	//	m_MouseDirection = EDirection::Middle;
	//	m_prevMouseDir = p_MouseRotate.RotateSpeed = 0.0f;
	//	p_MouseRotate.KeyValue = m_pParent->m_keyValue;
	//	PacketManager::Get().SendPacket((char*)&p_MouseRotate, sizeof(Packet_MouseRotate), PACKET_MouseRotate);
	//	ErrorMessage("회전 : " + to_string(Input::GetMouseMovePos().x));
	//}
}

void PlayerController::ResetOption() noexcept
{
	Window::UpdateWindowRect();
	m_setMouseScreen = { (LONG)Window::GetWinCenter().x, (LONG)Window::GetWinCenter().y };
	m_setMouseClient = m_setMouseScreen;
	ScreenToClient(Window::m_hWnd, &m_setMouseClient);
	///
	SetPosition(Vector3::Zero);
	SetRotation(Quaternion::Base);
	m_moveSpeed = 40.0f;
	m_jumpPower = 70.0f;
	m_pCamera = ObjectManager::Cameras[ECamera::Main];
	m_pCamera->SetRotation(Quaternion::Left * PI + Quaternion::Up * PI * 0.2f);
	m_pCamera->m_lerpMoveSpeed = 6.0f;
	m_pCamera->m_lerpRotateSpeed = 6.0f;
	if (m_pParent == nullptr)
		return;
	m_pCamera->SetPosition(Vector3::Up * 100.0f * m_pParent->GetScaleAverage());
	m_pCamera->m_armLength = 12.5f * m_pParent->GetScaleAverage();
	auto pColliders = m_pParent->GetComponentList(EComponent::Collider);
	{
		((Collider*)pColliders->front())->CollisionEvent = nullptr;
		((Collider*)pColliders->front())->m_pivot = Vector3::Up * 40.0f * m_pParent->GetScaleAverage();
	}
}

void PlayerController::Possess(GameObject* pObject) noexcept
{
	if (pObject == nullptr)
		return;
	SetParent(pObject);

	static auto pEvent = [](void* pVoid, void* pVoid2) {
		auto pPlayer = (PlayerController*)pVoid;
		auto pObj = (GameObject*)pVoid2;
		if (pObj->m_myName == L"Guard")
			pPlayer->m_curCharacter = PlayerController::ECharacter::EGuard;
		else if (pObj->m_myName == L"Zombie")
			pPlayer->m_curCharacter = PlayerController::ECharacter::EZombie;
		else
			pPlayer->m_curCharacter = PlayerController::ECharacter::EDummy;

		((JProgressBar*)pPlayer->m_pHpBar)->SetValue(pObj->GetHP(), 1.0f);
		pPlayer->ResetOption();
		SoundManager::Get().m_pListenerPos = &pObj->GetRoot()->GetPosition();
	};

	ObjectManager::PostFrameEvent.emplace(pEvent, this, pObject);

	//if (pCollider->m_pParent->m_myName == L"Guard")
	//	m_curCharacter = PlayerController::ECharacter::EGuard;
	//else if (pCollider->m_pParent->m_myName == L"Zombie")
	//	m_curCharacter = PlayerController::ECharacter::EZombie;
	//else
	//	m_curCharacter = PlayerController::ECharacter::EDummy;
	//((JProgressBar*)m_pHpBar)->SetValue(pCollider->GetHP(), 1.0f);
	//ResetOption();
}


void PlayerController::DeadEvent() noexcept
{
	PacketManager::Get().SendPlaySound("SE_dead.mp3", GetWorldPosition(), 1000.0f);
	m_pParent->SetHP(0.0f);
	SetPosition(m_pParent->GetPosition());
	SetRotation(m_pParent->GetRotation());
	CutParentPost();
	m_curDelayRespawn = 0.0f;
	((JPanel*)m_pRespawn)->m_bRender = true;
	((JProgressBar*)m_pRespawnBar)->SetValue(m_curDelayRespawn, m_DelayRespawn);
	//SoundManager::Get().PlayQueue("SE_dead.mp3", pA->m_pParent->GetWorldPosition(), 1000.0f);
}


void PlayerController::isChatting(const bool& isChat) noexcept
{
	m_isChatting = isChat;
}

bool PlayerController::isChatting() const noexcept
{
	return m_isChatting;
}