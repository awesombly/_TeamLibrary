#include "Character.h"
#include "ObjectManager.h"
#include "HeightMap.h"
#include "RPlane.h"

Character::Character(const wstring_view& myName, const EComponent& eComType, const wstring_view& srcName,
					 const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	SetInfo(myName, eComType, srcName, vertexShaderName, pixelShaderName);
	Create();
}

bool Character::Init() noexcept
{
	Renderer::Init();
	ObjectManager::Cameras[ECamera::Main]->SetParent(m_pParent);
	return true;
}

HRESULT Character::Create() noexcept
{
	RCube::Create();
	return S_OK;
}

bool Character::Frame(const float& spf, const float& accTime)	noexcept
{
	//if (!m_isEnable) return false;
	Renderer::Frame(spf, accTime);

	static bool isMoving = false;
	static float operSpeed = 0.0f;
	isMoving = false;
	operSpeed = 0.0f;
	if (Input::GetInstance().GetKeyState('W') == EKeyState::HOLD)
	{
		isMoving = true;
		operSpeed = m_accMoveSpeed;
	}
	if (Input::GetInstance().GetKeyState('S') == EKeyState::HOLD)
	{
		isMoving = true;
		operSpeed = -m_accMoveSpeed;
	}
	if (Input::GetInstance().GetKeyState('A') == EKeyState::HOLD)
	{
		m_pParent->Rotate(Quaternion::Left *  m_rotateSpeed * spf);
	}
	if (Input::GetInstance().GetKeyState('D') == EKeyState::HOLD)
	{
		m_pParent->Rotate(Quaternion::Right * m_rotateSpeed * spf);
	}
	m_curMoveSpeed += operSpeed * spf;
	// 조종중 아니거나, 진행방향과 반대로 눌렀을시
	if (!isMoving || (operSpeed * m_curMoveSpeed < 0.0f))
	{
		m_curMoveSpeed -= m_curMoveSpeed * 0.4f * spf;
	}
	m_curMoveSpeed = std::clamp(m_curMoveSpeed, -m_maxMoveSpeed, m_maxMoveSpeed);
	
	//// 경사
	//if (m_position.y <= mapHeight)
	//{
	//	m_curGravity = 0;
	//	//m_curMoveSpeed -= m_curMoveSpeed * 0.6f * spf;
	//	m_curMoveSpeed += (prePosY - mapHeight) * 250 * spf;
	//	ErrorMessage(std::to_string((prePosY - mapHeight) * spf));
	//}
	//else
	//{
	//	m_curMoveSpeed -= m_curMoveSpeed * 0.4f * spf;
	//}

	// 최종 이동, 회전
	m_pParent->Translate(m_pParent->GetForward() * m_curMoveSpeed * spf);
	for (auto& iter : *m_pParent->GetChildList())
		iter->Rotate(Quaternion::Up * m_curMoveSpeed * spf);
	return true;
}

bool Character::Render(ID3D11DeviceContext* pDContext) noexcept
{
	//if (!m_isEnable) return false;
	Renderer::Render(pDContext);
	return true;
}

bool Character::Release()	noexcept
{
	Renderer::Release();
	return true;
}

Component* Character::clone() noexcept
{
	//return (Component*)(new Character(*this));
	return (new Character(*this))->cloneAddition();
}