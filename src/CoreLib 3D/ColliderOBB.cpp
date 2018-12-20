#include "ColliderOBB.h"
#include "ObjectManager.h"
#include "ColliderAABB.h"
#include "ColliderSphere.h"



ColliderOBB::ColliderOBB(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos)
	: Collider()
{
	Init();
	SetMinMax(minPos, maxPos);
	//m_extents = (maxPos - minPos) * 0.5f;
	//m_center = minPos + m_extents;
	//m_rotate[0] = { 1, 0, 0 };
	//m_rotate[1] = { 0, 1, 0 };
	//m_rotate[2] = { 0, 0, 1 };
}

bool ColliderOBB::Init() noexcept
{
	m_myName = L"ColliderOBB";
	m_comptType = EComponent::Collider;
	m_eCollider = ECollider::OBB;
	return true;
}

bool ColliderOBB::Frame(const float& spf, const float& accTime) noexcept
{
	Collider::Frame(spf, accTime);

	m_rotate[0] = m_pParent->GetRight(); 
	m_rotate[1] = m_pParent->GetUp();
	m_rotate[2] = m_pParent->GetForward();
	return true;
}

Component* ColliderOBB::clone() noexcept
{
	auto pCollider = new ColliderOBB(*this);
	ObjectManager::Get().PushCollider(pCollider);
	return (Component*)pCollider;
}

bool ColliderOBB::CollisionCheck(Collider* pCollider) noexcept
{
	switch (pCollider->m_eCollider)
	{
	case ECollider::AABB:
	{
		///
	}	break;
	case ECollider::OBB:
	{
		//translation, in parent frame (note: A = this, B = obb)
		ColliderOBB* pOBB = (ColliderOBB*)pCollider;
		//translation, in A's frame
		D3DXVECTOR3 v = (pOBB->GetCenter() + pOBB->m_center) - (GetCenter() + m_center);
		D3DXVECTOR3 T = { D3DXVec3Dot(&v, &m_rotate[0]), 
						  D3DXVec3Dot(&v, &m_rotate[1]), 
						  D3DXVec3Dot(&v, &m_rotate[2]) };
		//B's basis with respect to A's local frame
		float R[3][3];
		float FR[3][3];
		float ra, rb, t;
		
		//calculate rotation matrix from "obb" to "this" - palso recomputes the fabs matrix
		for (int out = 0; out < 3; ++out)
		{
			for (int in = 0; in < 3; ++in)
			{
				R[out][in] = D3DXVec3Dot(&m_rotate[out], &pOBB->m_rotate[in]);
				// fabs and shift borrowed from RAPID
				FR[out][in] = 1e-6f + abs(R[out][in]);
			}
		}

		// Separating axis theorem: test of all 15 potential separating axes
		// These axes are always parallel to each OBB edges or its normal plane
		const D3DXVECTOR3 &a = m_extents;
		const D3DXVECTOR3 &b = pOBB->m_extents;

		// First stage: each obb's axis!
		//A's basis vectors
		for (int i = 0; i < 3; i++)
		{
			ra = a[i];
			rb = b.x*FR[i][0] + b.y*FR[i][1] + b.z*FR[i][2];
			t = abs(T[i]);

			if (t > ra + rb) return false;
		}

		//B's basis vectors
		for (int i = 0; i < 3; i++)
		{
			ra = a.x*FR[0][i] + a.y*FR[1][i] + a.z*FR[2][i];
			rb = b[i];
			t = abs(T[0] * R[0][i] + T[1] * R[1][i] + T[2] * R[2][i]);

			if (t > ra + rb) return false;
		}

		// Second stage: 9 cross products
		//L = A0 x B0
		ra = a[1] * FR[2][0] + a[2] * FR[1][0];
		rb = b[1] * FR[0][2] + b[2] * FR[0][1];
		t = abs(T[2] * R[1][0] - T[1] * R[2][0]);
		if (t > ra + rb) return false;

		//L = A0 x B1
		ra = a[1] * FR[2][1] + a[2] * FR[1][1];
		rb = b[0] * FR[0][2] + b[2] * FR[0][0];
		t = abs(T[2] * R[1][1] - T[1] * R[2][1]);
		if (t > ra + rb) return false;

		//L = A0 x B2
		ra = a[1] * FR[2][2] + a[2] * FR[1][2];
		rb = b[0] * FR[0][1] + b[1] * FR[0][0];
		t = abs(T[2] * R[1][2] - T[1] * R[2][2]);
		if (t > ra + rb) return false;

		//L = A1 x B0
		ra = a[0] * FR[2][0] + a[2] * FR[0][0];
		rb = b[1] * FR[1][2] + b[2] * FR[1][1];
		t = abs(T[0] * R[2][0] - T[2] * R[0][0]);
		if (t > ra + rb) return false;

		//L = A1 x B1
		ra = a[0] * FR[2][1] + a[2] * FR[0][1];
		rb = b[0] * FR[1][2] + b[2] * FR[1][0];
		t = abs(T[0] * R[2][1] - T[2] * R[0][1]);
		if (t > ra + rb) return false;

		//L = A1 x B2
		ra = a[0] * FR[2][2] + a[2] * FR[0][2];
		rb = b[0] * FR[1][1] + b[1] * FR[1][0];
		t = abs(T[0] * R[2][2] - T[2] * R[0][2]);
		if (t > ra + rb) return false;

		//L = A2 x B0
		ra = a[0] * FR[1][0] + a[1] * FR[0][0];
		rb = b[1] * FR[2][2] + b[2] * FR[2][1];
		t = abs(T[1] * R[0][0] - T[0] * R[1][0]);
		if (t > ra + rb) return false;

		//L = A2 x B1
		ra = a[0] * FR[1][1] + a[1] * FR[0][1];
		rb = b[0] * FR[2][2] + b[2] * FR[2][0];
		t = abs(T[1] * R[0][1] - T[0] * R[1][1]);
		if (t > ra + rb) return false;

		//L = A2 x B2
		ra = a[0] * FR[1][2] + a[1] * FR[0][2];
		rb = b[0] * FR[2][1] + b[1] * FR[2][0];
		t = abs(T[1] * R[0][2] - T[0] * R[1][2]);
		if (t > ra + rb)	return false;

		// Ãæµ¹
		// Phew! No separating axis found, no overlap!
		return true;

	}	break;
	case ECollider::Sphere:
	{
		///
	}	break;
	}

	return false;
}

D3DXVECTOR3 ColliderOBB::GetLength() noexcept
{
	return Product(m_extents, m_pParent->GetScale());
}

void ColliderOBB::SetMinMax(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos) noexcept
{
	m_extents = (maxPos - minPos) * 0.5f;
	m_center = minPos + m_extents;
	m_rotate[0] = { 1, 0, 0 };
	m_rotate[1] = { 0, 1, 0 };
	m_rotate[2] = { 0, 0, 1 };
}