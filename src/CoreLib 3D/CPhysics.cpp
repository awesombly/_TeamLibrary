#include "CPhysics.h"
#include "ObjectManager.h"
#include "HeightMap.h"
#include "RPlane.h"

CPhysics::CPhysics()
{
	m_comptType = EComponent::Phsics;
	m_pFollower = new RPlane(L"Settings");
	m_pMap = (HeightMap*)ObjectManager::GetInstance().GetObjectList(EObjType::Map)->front();
}


void CPhysics::Update() noexcept
{
	//
}

bool CPhysics::Init() noexcept
{
	m_pParent->AddComponent(m_pFollower);
	return true;
}

bool CPhysics::Frame(const float& spf, const float& accTime)	noexcept
{
	// ¸Ê ³ôÀÌ
	float mapHeight = m_pMap->GetMapHeight(m_pParent->GetPosition(), m_pFollower) + m_pParent->GetScale().y * 0.5f;
	// Áß·Â
	if (useGravity)
	{
		static D3DXQUATERNION quaternion;
		static D3DXMATRIX matTarget;
		D3DXQuaternionRotationYawPitchRoll(&quaternion, m_pParent->GetRotation().y, m_pParent->GetRotation().x, m_pParent->GetRotation().z);
		D3DXMatrixAffineTransformation(&matTarget, 1.0f, NULL, &quaternion, &m_pParent->GetPosition());

		D3DXVECTOR4 vector4;
		D3DXMatrixTranspose(&matTarget, &m_pParent->GetWorldMatrix());

		for (auto& iter : *m_pParent->GetChildList())
		{
			D3DXVECTOR3 rotPosition;
			D3DXVec3Transform(&vector4, &iter->GetPosition(), &matTarget);
			rotPosition = { vector4.x, vector4.y, vector4.z };
			
			mapHeight = m_pMap->GetMapHeight(rotPosition, nullptr) + iter->GetWorldScale().y * 0.5f;
			float subHeight = mapHeight - rotPosition.y;
			if (rotPosition.y < mapHeight)
			{
				m_curGravity = 0;
				m_pParent->Translate(0.0f, subHeight, 0.0f);
				m_pParent->Rotate(-iter->GetPosition().z * subHeight * spf, 0.0f, iter->GetPosition().x * subHeight * spf * 0.2f);
			}
			else
			{
				m_pParent->Rotate(-iter->GetPosition().z * subHeight * 0.3f * spf, 0.0f, iter->GetPosition().x * subHeight * spf * 0.2f);
			}
		}

		m_curGravity -= m_weight * GravityPower * spf;
		m_pParent->SetPositionY(std::max<float>(m_pParent->GetPosition().y + m_curGravity * spf, mapHeight));
		if (m_pParent->GetPosition().y <= mapHeight)
			m_curGravity = 0;
	}

	if (Input::isDebug)
		m_pFollower->Frame(spf, accTime);
	return true;
	accTime;
}

bool CPhysics::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (Input::isDebug)
		m_pFollower->Render(pDContext);
	return true;
	pDContext;
}

bool CPhysics::Release()	noexcept
{
	return true;
}


Component* CPhysics::clone() noexcept
{
	return (Component*)new CPhysics(*this);
}