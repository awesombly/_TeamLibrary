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

	if (m_useGravity)
	{
		m_force += Vector3::Down * GravityPower * spf;
	}
	// 항력
	m_force -= m_force * m_damping * spf;

	// 힘 적용
	if(GetVelocitySq() > 15.0f)
		m_pParent->GetRoot()->Translate(m_force * spf);

	// 충돌 체크
	CollisionAllCheck(spf);
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

			// 반작용
			D3DXVECTOR3 vDirection		= Vector3::Zero;
			D3DXVECTOR3 vForceDis		= Vector3::Zero;
			D3DXVECTOR3 vForceDisOther	= Vector3::Zero;
			//if (m_usePhysics)
			//{
			//	vDirection = GetCenter() - iter->GetCenter();
			//	D3DXVec3Normalize(&vDirection, &vDirection);
			//	vForceDis = iter->m_force - m_force;
			//	vForceDis = vDirection * (iter->m_mass / m_mass) * D3DXVec3Length(&vForceDis);
			//}
			//if (iter->m_usePhysics)
			//{
			//	vDirection = iter->GetCenter() - GetCenter();
			//	D3DXVec3Normalize(&vDirection, &vDirection);
			//	vForceDisOther = m_force - iter->m_force;
			//	vForceDisOther = vDirection * (m_mass / iter->m_mass) * D3DXVec3Length(&vForceDisOther);
			//}


			if (m_usePhysics)
			{
				m_force += vForceDis;
				// 반발력
				m_force *= -(m_repulsion + iter->m_repulsion) * 0.5f;
			}
			if (iter->m_usePhysics)
			{
				iter->m_force += vForceDisOther;
				iter->m_force *= -(m_repulsion + iter->m_repulsion) * 0.5f;	
			}
			// 마찰력
			m_force		  -= m_force	   * (m_drag + iter->m_drag) * 0.5f  * spf;
			iter->m_force -= iter->m_force * (m_drag + iter->m_drag) * 0.5f  * spf;
		}
	}
	return true;
}


D3DXVECTOR3 Collider::GetCenter() noexcept
{
	return m_pParent->GetWorldPosition() + m_pivot;
}


float Collider::GetVelocity() noexcept
{
	return D3DXVec3Length(&m_force);
}

float Collider::GetVelocitySq() noexcept
{
	return D3DXVec3LengthSq(&m_force);
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