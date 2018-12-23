#include "ColliderSphere.h"
#include "ObjectManager.h"
#include "ColliderAABB.h"
#include "ColliderOBB.h"

ColliderSphere::ColliderSphere(const float& radius) 
	: Collider(), m_radius(radius)
{
	Init();
}

bool ColliderSphere::Init() noexcept
{
	m_myName = L"ColliderSphere";
	m_comptType = EComponent::Collider;
	m_eCollider = ECollider::Sphere;
	return true;
}

Component* ColliderSphere::clone() noexcept
{
	auto pCollider = new ColliderSphere(*this);
	ObjectManager::Get().PushCollider(pCollider);
	return (Component*)pCollider;
}

bool ColliderSphere::CollisionCheck(Collider* pCollider) noexcept
{
	switch (pCollider->m_eCollider)
	{
	case ECollider::AABB:
	{
		ColliderAABB* pAABB = (ColliderAABB*)pCollider;
		
		D3DXVECTOR3 nearPoint = Vector3::Zero;

		// x 비교
		if (GetCenter().x < (nearPoint.x = pAABB->GetCenter().x - pAABB->GetLength().x * 0.5f))
		{		}
		else if (GetCenter().x > (nearPoint.x = pAABB->GetCenter().x + pAABB->GetLength().x * 0.5f))
		{		}
		else
		{
			nearPoint.x = GetCenter().x;
		}
		// y 비교
		if (GetCenter().y < (nearPoint.y = pAABB->GetCenter().y - pAABB->GetLength().y * 0.5f))
		{		}
		else if (GetCenter().y > (nearPoint.y = pAABB->GetCenter().y + pAABB->GetLength().y * 0.5f))
		{		}
		else
		{
			nearPoint.y = GetCenter().y;
		}
		// z 비교
		if (GetCenter().z < (nearPoint.z = pAABB->GetCenter().z - pAABB->GetLength().z * 0.5f))
		{		}
		else if (GetCenter().z > (nearPoint.z = pAABB->GetCenter().z + pAABB->GetLength().z * 0.5f))
		{		}
		else
		{
			nearPoint.z = GetCenter().z;
		}

		auto vDistance = GetCenter() - nearPoint;
		if ((D3DXVec3Length(&vDistance) - GetWorldRadius()) <= 0)
		{
			return true;
		}
	}	break;
	case ECollider::OBB:
	{
		ColliderOBB* pOBB = (ColliderOBB*)pCollider;
		// 최단점
		D3DXVECTOR3 nearPoint = Vector3::Zero;
		// 구 -> OBB 박스 좌표계 변환
		D3DXVECTOR3 tempCenter = GetCenter() - pOBB->GetCenter();
		D3DXVECTOR3 newCenter = { D3DXVec3Dot(&tempCenter, &pOBB->m_rotate[0]),
								  D3DXVec3Dot(&tempCenter, &pOBB->m_rotate[1]),
								  D3DXVec3Dot(&tempCenter, &pOBB->m_rotate[2]) };
		newCenter += pOBB->GetCenter();

		// x 비교
		if (newCenter.x < (nearPoint.x = pOBB->GetCenter().x - pOBB->GetExtents().x))
		{		}
		else if (newCenter.x > (nearPoint.x = pOBB->GetCenter().x + pOBB->GetExtents().x))
		{		}
		else
		{
			nearPoint.x = newCenter.x;
		}
		// y 비교
		if (newCenter.y < (nearPoint.y = pOBB->GetCenter().y - pOBB->GetExtents().y))
		{		}
		else if (newCenter.y > (nearPoint.y = pOBB->GetCenter().y + pOBB->GetExtents().y))
		{		}
		else
		{
			nearPoint.y = newCenter.y;
		}
		// z 비교
		if (newCenter.z < (nearPoint.z = pOBB->GetCenter().z - pOBB->GetExtents().z))
		{		}
		else if (newCenter.z > (nearPoint.z = pOBB->GetCenter().z + pOBB->GetExtents().z))
		{		}
		else
		{
			nearPoint.z = newCenter.z;
		}

		auto vDistance = newCenter - nearPoint;
		if ((D3DXVec3Length(&vDistance) - GetWorldRadius()) <= 0)
		{
			return true;
		}
	}	break;
	case ECollider::Sphere:
	{
		ColliderSphere* pSphere = (ColliderSphere*)pCollider;

		auto vRange = GetCenter() - pSphere->GetCenter();
		float distance = D3DXVec3Length(&vRange);
		// 충돌
		if (distance < (GetWorldRadius() + pSphere->GetWorldRadius()))
		{
			return true;
		}
	}	break;
	}

	return false;
}

float ColliderSphere::GetWorldRadius() noexcept
{
	return m_radius * m_pParent->GetScaleAverage();
}

void ColliderSphere::SetRadius(const float& radius) noexcept
{
	m_radius = radius;
}