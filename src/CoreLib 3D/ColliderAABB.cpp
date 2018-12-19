#include "ColliderAABB.h"
#include "ObjectManager.h"
#include "ColliderOBB.h"
#include "ColliderSphere.h"



ColliderAABB::ColliderAABB(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos)
	: Collider(), m_minPos(minPos), m_maxPos(maxPos)
{
	Init();
}


bool ColliderAABB::Init() noexcept
{
	m_myName = L"ColliderAABB";
	m_comptType = EComponent::Collider;
	m_eCollider = ECollider::AABB;
	return true;
}

Component* ColliderAABB::clone() noexcept
{
	auto pCollider = new ColliderAABB(*this);
	ObjectManager::Get().PushCollider(pCollider);
	return (Component*)pCollider;
}

bool ColliderAABB::CollisionCheck(Collider* pCollider) noexcept
{
	switch (pCollider->m_eCollider)
	{
	case ECollider::AABB:
	{
		ColliderAABB* pAABB = (ColliderAABB*)pCollider;
		// Ãæµ¹
		auto thisMin  =		   GetCenter() + Product(m_minPos, m_pParent->GetScale());
		auto thisMax  =		   GetCenter() + Product(m_maxPos, m_pParent->GetScale());
		auto otherMin = pAABB->GetCenter() + Product(pAABB->m_minPos, pAABB->m_pParent->GetScale());
		auto otherMax = pAABB->GetCenter() + Product(pAABB->m_maxPos, pAABB->m_pParent->GetScale());
		if (std::max<float>(thisMin.x, otherMin.x) < std::min<float>(thisMax.x, otherMax.x) &&
			std::max<float>(thisMin.y, otherMin.y) < std::min<float>(thisMax.y, otherMax.y) &&
			std::max<float>(thisMin.z, otherMin.z) < std::min<float>(thisMax.z, otherMax.z) )
		{
			return true;
		}
	}	break;
	case ECollider::OBB:
	{
		ColliderOBB* pOBB = (ColliderOBB*)pCollider;

		D3DXVECTOR3 preMin = GetCenter() + Product(m_minPos, m_pParent->GetScale());
		D3DXVECTOR3 preMax = GetCenter() + Product(m_maxPos, m_pParent->GetScale());
		// AABB -> OBB ÁÂÇ¥ º¯È¯(min)
		D3DXVECTOR3 tempMin = preMin - pOBB->GetCenter();
		D3DXVECTOR3 thisMin = {   D3DXVec3Dot(&tempMin, &pOBB->m_pParent->GetRight()),
								  D3DXVec3Dot(&tempMin, &pOBB->m_pParent->GetUp()),
								  D3DXVec3Dot(&tempMin, &pOBB->m_pParent->GetForward()) };
		thisMin += pOBB->GetCenter();
		// AABB -> OBB ÁÂÇ¥ º¯È¯(max)
		D3DXVECTOR3 tempMax = preMax - pOBB->GetCenter();
		D3DXVECTOR3 thisMax = { D3DXVec3Dot(&tempMax, &pOBB->m_pParent->GetRight()),
								  D3DXVec3Dot(&tempMax, &pOBB->m_pParent->GetUp()),
								  D3DXVec3Dot(&tempMax, &pOBB->m_pParent->GetForward()) };
		thisMax += pOBB->GetCenter();
		//
		auto otherMin = pOBB->GetCenter() - pOBB->GetLength();
		auto otherMax = pOBB->GetCenter() + pOBB->GetLength();
		if (std::max<float>(thisMin.x, otherMin.x) < std::min<float>(thisMax.x, otherMax.x) &&
			std::max<float>(thisMin.y, otherMin.y) < std::min<float>(thisMax.y, otherMax.y) &&
			std::max<float>(thisMin.z, otherMin.z) < std::min<float>(thisMax.z, otherMax.z))
		{
			return true;
		}
	}	break;
	case ECollider::Sphere:
	{
		///
	}	break;
	}

	return false;
}

D3DXVECTOR3 ColliderAABB::GetLength() noexcept
{
	return Product((m_maxPos - m_minPos), m_pParent->GetScale());
}

void ColliderAABB::SetMinMax(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos) noexcept
{
	m_minPos = minPos;
	m_maxPos = maxPos;
}