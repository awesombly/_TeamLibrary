#include "Collider.h"
//#include "GameObject.h"
#include "ObjectManager.h"


Collider::Collider()
{
	ObjectManager::Get().PushCollider(this);
}

Collider::~Collider()
{
	ObjectManager::Get().PopCollider(this);
}

bool Collider::Init() noexcept
{
	return true;
}

//void Collider::Update() noexcept
//{
//
//}

bool Collider::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable)	return false;

	// 충돌 체크
	CollisionAllCheck(spf);

	if (m_useGravity)
	{
		m_force += Vector3::Down * GravityPower * spf;
	}
	// 항력
	m_force -= m_force * m_damping * spf;

	// 힘 적용
	//if (isHost)
	//{
		if (GetVelocitySq() > 80.0f)
		{
			m_pParent->isMoved(true);
			m_pParent->GetRoot()->Translate((GetTotalForce() + Vector3::Up * 5.0f) * spf);
			//if (m_pParent->GetWorldPosition().y < *m_pMapHeight)
			//{
			//	m_pParent->SetPositionY(*m_pMapHeight);
			//}
		}
	//}
	return true;
	accTime;
}

bool Collider::Render(ID3D11DeviceContext* pDContext) noexcept
{
	return true;
	pDContext;
}

bool Collider::Release()	noexcept
{
	return true;
}


bool Collider::CollisionAllCheck(const float& spf) noexcept
{
	//m_CollisionList.clear();
	m_CollisionList.push_front(this);
	for (auto& iter : ObjectManager::Get().GetColliderList())
	{
		// 자신 또는 처리된 객체 제외
		auto finder = std::find(m_CollisionList.begin(), m_CollisionList.end(), iter);
		if(finder != m_CollisionList.end())
		{
			continue;
		}
		if (CollisionCheck(iter))
		{
			// 상대 충돌 목록 추가
			iter->m_CollisionList.push_front(this);

			//D3DXVECTOR3 vDirection		= Vector3::Zero;
			D3DXVECTOR3 vForceDis		= Vector3::Zero;
			D3DXVECTOR3 vForceDisOther	= Vector3::Zero;
			
			//// 반작용
			//vForceDis = iter->GetTotalForce();
			//
			//vForceDisOther = GetTotalForce();
			////vDirection = iter->GetCenter() - GetCenter();
			////D3DXVec3Normalize(&vDirection, &vDirection);
			////vForceDisOther = -iter->GetTotalForce();
			////vForceDisOther = D3DXVec3Dot(&vForceDis, &vDirection) * 2.0f * vDirection - vForceDis;
			//
			//
			////vDirection = iter->GetCenter() - GetCenter();
			////D3DXVec3Normalize(&vDirection, &vDirection);
			////vForceDisOther = m_force - iter->m_force;
			////vForceDisOther = vDirection * (m_mass / iter->m_mass) * D3DXVec3Length(&vForceDisOther);
			//
			//

			vForceDis = iter->GetTotalForce();
			vForceDisOther = GetTotalForce();
			if (m_usePhysics)
			{
				//m_force += vForceDis;
				//D3DXVec3Normalize(&vForceDis, &m_force);
				// 반발력
				m_force = vForceDis + -m_force * (m_repulsion + iter->m_repulsion) * 0.5f;
				//m_pParent->Translate(-m_force * 0.001f);
				//m_pParent->Translate(Vector3::Up * spf);
			}
			if (iter->m_usePhysics)
			{
				//iter->m_force += vForceDisOther;
				//D3DXVec3Normalize(&vForceDisOther, &iter->m_force);
				iter->m_force = vForceDisOther + -iter->m_force * (m_repulsion + iter->m_repulsion) * 0.5f;
				//iter->m_pParent->Translate(-iter->m_force * 0.001f);
				//iter->m_pParent->Translate(Vector3::Up * spf);
			}
			// 마찰력
			m_force		  -= m_force	   * (m_drag + iter->m_drag) * 0.5f  * spf;
			iter->m_force -= iter->m_force * (m_drag + iter->m_drag) * 0.5f  * spf;

			//m_force = Vector3::Zero;
			//iter->m_force = Vector3::Zero;
		}
	}
	return true;
}


void Collider::AddForce(const D3DXVECTOR3& vForce) noexcept
{
	m_force += vForce;
}

void Collider::SetForce(const D3DXVECTOR3& vForce) noexcept
{
	m_force = vForce;
}

void Collider::SetDirectionForce(const D3DXVECTOR3& vForce) noexcept
{
	m_isMoving = true;
	m_direction = vForce;
}

D3DXVECTOR3 Collider::GetTotalForce() noexcept
{
	return m_force + m_direction;
}

D3DXVECTOR3 Collider::GetCenter() noexcept
{
	return m_pParent->GetWorldPosition() + m_pivot;
}


float Collider::GetVelocity() noexcept
{
	return VectorLength(m_force + m_direction);
}

float Collider::GetVelocitySq() noexcept
{
	return VectorLengthSq(m_force + m_direction);
}


void Collider::useGravity(const bool& useGravity) noexcept
{
	m_useGravity = useGravity;
}
bool Collider::useGravity() noexcept
{
	return m_useGravity;
}

void Collider::usePhysics(const bool& usePhysics) noexcept
{
	m_usePhysics = usePhysics;
}
bool Collider::usePhysics()	noexcept
{
	return m_usePhysics;
}

void Collider::isMoving(const bool& isMoving) noexcept
{
	m_isMoving = isMoving;
	if (!m_isMoving)
		m_direction = Vector3::Zero;
}
bool Collider::isMoving() noexcept
{
	return m_isMoving;
}