#include "Collider.h"
#include "ColliderAABB.h"
#include "ColliderOBB.h"
#include "ObjectManager.h"



Collider::Collider(const float& radius)
	: m_radius(radius)
{
	ObjectManager::Get().PushCollider(this);
	Init();
}

Collider::~Collider()
{
	ObjectManager::Get().PopCollider(this);
}

bool Collider::Init() noexcept
{
	m_myName = L"ColliderSphere";
	m_comptType = EComponent::Collider;
	m_eCollider = ECollider::Sphere;
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
		if (GetVelocitySq() > 60.0f)
		{
			m_pParent->isMoved(true);
			m_pParent->GetRoot()->Translate((GetTotalForce() + Vector3::Up * 5.0f) * spf);
			if (m_pParent->GetWorldPosition().y < m_mapHeight)
			{
				m_pParent->SetPositionY(m_mapHeight + m_pivot);
				m_force *= -m_drag * m_repulsion * spf;
			}
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



bool Collider::CollisionCheck(Collider* pCollider) noexcept
{
	if (!SphereToSphere(this, pCollider))
		return false;
	switch (pCollider->m_eCollider)
	{
	case ECollider::AABB:
	{
		if (SphereToAABB(this, (ColliderAABB*)pCollider))
			return true;
	}	break;
	case ECollider::OBB:
	{
		if (SphereToOBB(this, (ColliderOBB*)pCollider))
			return true;
	}	break;
	case ECollider::Sphere:
	{
		return true;
	}	break;
	}

	return false;
}




bool Collider::SphereToSphere(Collider* pSphereA, Collider* pSphereB) const	noexcept
{
	auto vRange = pSphereA->GetCenter() - pSphereB->GetCenter();
	float distance = D3DXVec3Length(&vRange);
	// 충돌
	if (distance < (pSphereA->GetWorldRadius() + pSphereB->GetWorldRadius()))
	{
		//m_normal = pSphereB->GetCenter() - pSphereA->GetCenter();
		//D3DXVec3Normalize(&m_normal, &m_normal);
		//pSphereB->m_normal = -m_normal;
		return true;
	}
	return false;
}

bool Collider::SphereToAABB(Collider* pSphere, ColliderAABB* pAABB)	const noexcept
{
	D3DXVECTOR3 nearPoint = Vector3::Zero;

	// x 비교
	if (pSphere->GetCenter().x < (nearPoint.x = pAABB->GetCenter().x - pAABB->GetLength().x * 0.5f))
	{	}
	else if (pSphere->GetCenter().x > (nearPoint.x = pAABB->GetCenter().x + pAABB->GetLength().x * 0.5f))
	{	}
	else
	{
		nearPoint.x = pSphere->GetCenter().x;
	}
	// y 비교
	if (pSphere->GetCenter().y < (nearPoint.y = pAABB->GetCenter().y - pAABB->GetLength().y * 0.5f))
	{	}
	else if (pSphere->GetCenter().y > (nearPoint.y = pAABB->GetCenter().y + pAABB->GetLength().y * 0.5f))
	{	}
	else
	{
		nearPoint.y = pSphere->GetCenter().y;
	}
	// z 비교
	if (pSphere->GetCenter().z < (nearPoint.z = pAABB->GetCenter().z - pAABB->GetLength().z * 0.5f))
	{	}
	else if (pSphere->GetCenter().z > (nearPoint.z = pAABB->GetCenter().z + pAABB->GetLength().z * 0.5f))
	{	}
	else
	{
		nearPoint.z = pSphere->GetCenter().z;
	}

	auto vDistance = pSphere->GetCenter() - nearPoint;
	if ((D3DXVec3Length(&vDistance) - pSphere->GetWorldRadius()) <= 0)
	{
		//pSphere->m_normal = pAABB->GetCenter() - nearPoint;
		//D3DXVec3Normalize(&pSphere->m_normal, &pSphere->m_normal);
		//pAABB->m_normal = -pSphere->m_normal;
		return true;
	}
	return false;
}

bool Collider::SphereToOBB(Collider* pSphere, ColliderOBB* pOBB) const noexcept
{
	// 최단점
	D3DXVECTOR3 nearPoint = Vector3::Zero;
	// 구 -> OBB 박스 좌표계 변환
	D3DXVECTOR3 tempCenter = pSphere->GetCenter() - pOBB->GetCenter();
	D3DXVECTOR3 newCenter = { D3DXVec3Dot(&tempCenter, &pOBB->m_rotate[0]),
							  D3DXVec3Dot(&tempCenter, &pOBB->m_rotate[1]),
							  D3DXVec3Dot(&tempCenter, &pOBB->m_rotate[2]) };
	newCenter += pOBB->GetCenter();

	// x 비교
	if (newCenter.x < (nearPoint.x = pOBB->GetCenter().x - pOBB->GetExtents().x))
	{	}
	else if (newCenter.x > (nearPoint.x = pOBB->GetCenter().x + pOBB->GetExtents().x))
	{	}
	else
	{
		nearPoint.x = newCenter.x;
	}
	// y 비교
	if (newCenter.y < (nearPoint.y = pOBB->GetCenter().y - pOBB->GetExtents().y))
	{	}
	else if (newCenter.y > (nearPoint.y = pOBB->GetCenter().y + pOBB->GetExtents().y))
	{	}
	else
	{
		nearPoint.y = newCenter.y;
	}
	// z 비교
	if (newCenter.z < (nearPoint.z = pOBB->GetCenter().z - pOBB->GetExtents().z))
	{	}
	else if (newCenter.z > (nearPoint.z = pOBB->GetCenter().z + pOBB->GetExtents().z))
	{	}
	else
	{
		nearPoint.z = newCenter.z;
	}

	auto vDistance = newCenter - nearPoint;
	if ((D3DXVec3Length(&vDistance) - pSphere->GetWorldRadius()) <= 0)
	{
		pSphere->m_normal = pOBB->GetCenter() - nearPoint;
		D3DXVec3Normalize(&pSphere->m_normal, &pSphere->m_normal);
		pOBB->m_normal = -pSphere->m_normal;
		return true;
	}
	return false;
}

bool Collider::AABBToAABB(ColliderAABB* ApAABB, ColliderAABB* BpAABB) const noexcept
{
	// 충돌
	auto&& thisMin = ApAABB->GetCenter() + ApAABB->GetMin();
	auto&& thisMax = ApAABB->GetCenter() + ApAABB->GetMax();
	auto&& otherMin = BpAABB->GetCenter() + BpAABB->GetMin();
	auto&& otherMax = BpAABB->GetCenter() + BpAABB->GetMax();
	if (std::max<float>(thisMin.x, otherMin.x) < std::min<float>(thisMax.x, otherMax.x) &&
		std::max<float>(thisMin.y, otherMin.y) < std::min<float>(thisMax.y, otherMax.y) &&
		std::max<float>(thisMin.z, otherMin.z) < std::min<float>(thisMax.z, otherMax.z))
	{
		return true;
	}
	return false;
}

bool Collider::AABBToOBB(ColliderAABB* pAABB, ColliderOBB* pOBB) const noexcept
{
	D3DXVECTOR3 preMin = pAABB->GetCenter() + pAABB->GetMin();
	D3DXVECTOR3 preMax = pAABB->GetCenter() + pAABB->GetMax();
	// AABB -> OBB 좌표 변환(min)
	D3DXVECTOR3 tempMin = preMin - pOBB->GetCenter();
	D3DXVECTOR3 thisMin = {	  D3DXVec3Dot(&tempMin, &pOBB->m_pParent->GetRight()),
							  D3DXVec3Dot(&tempMin, &pOBB->m_pParent->GetUp()),
							  D3DXVec3Dot(&tempMin, &pOBB->m_pParent->GetForward()) };
	thisMin += pOBB->GetCenter();
	// AABB -> OBB 좌표 변환(max)
	D3DXVECTOR3 tempMax = preMax - pOBB->GetCenter();
	D3DXVECTOR3 thisMax = {   D3DXVec3Dot(&tempMax, &pOBB->m_pParent->GetRight()),
							  D3DXVec3Dot(&tempMax, &pOBB->m_pParent->GetUp()),
							  D3DXVec3Dot(&tempMax, &pOBB->m_pParent->GetForward()) };
	thisMax += pOBB->GetCenter();
	//
	auto&& otherMin = pOBB->GetCenter() - pOBB->GetExtents();
	auto&& otherMax = pOBB->GetCenter() + pOBB->GetExtents();
	if (std::max<float>(thisMin.x, otherMin.x) < std::min<float>(thisMax.x, otherMax.x) &&
		std::max<float>(thisMin.y, otherMin.y) < std::min<float>(thisMax.y, otherMax.y) &&
		std::max<float>(thisMin.z, otherMin.z) < std::min<float>(thisMax.z, otherMax.z))
	{
		return true;
	}
	return false;
}

bool Collider::OBBToOBB(ColliderOBB* ApOBB, ColliderOBB* BpOBB) const noexcept
{
	//translation, in parent frame (note: A = this, B = obb)
	//translation, in A's frame

	D3DXVECTOR3 v = BpOBB->GetCenter() - ApOBB->GetCenter();
	D3DXVECTOR3 T = { D3DXVec3Dot(&v, &ApOBB->m_rotate[0]),
					  D3DXVec3Dot(&v, &ApOBB->m_rotate[1]),
					  D3DXVec3Dot(&v, &ApOBB->m_rotate[2]) };
	//B's basis with respect to A's local frame
	float R[3][3];
	float FR[3][3];
	float ra, rb, t;

	//calculate rotation matrix from "obb" to "this" - palso recomputes the fabs matrix
	for (int out = 0; out < 3; ++out)
	{
		for (int in = 0; in < 3; ++in)
		{
			R[out][in] = D3DXVec3Dot(&ApOBB->m_rotate[out], &BpOBB->m_rotate[in]);
			// fabs and shift borrowed from RAPID
			FR[out][in] = 1e-6f + abs(R[out][in]);
		}
	}

	// Separating axis theorem: test of all 15 potential separating axes
	// These axes are always parallel to each OBB edges or its normal plane
	D3DXVECTOR3 a = ApOBB->GetExtents();
	D3DXVECTOR3 b = BpOBB->GetExtents();

	// First stage: each obb's axis!
	//A's basis vectors
	for (int i = 0; i < 3; i++)
	{
		ra = a[i];
		rb = b.x * FR[i][0] + b.y * FR[i][1] + b.z * FR[i][2];
		t = abs(T[i]);

		if (t > ra + rb) return false;
	}

	//B's basis vectors
	for (int i = 0; i < 3; i++)
	{
		ra = a.x * FR[0][i] + a.y * FR[1][i] + a.z * FR[2][i];
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

	// 충돌
	// Phew! No separating axis found, no overlap!
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

float Collider::GetWorldRadius() noexcept
{
	return m_radius * m_pParent->GetScaleAverage();
}

void Collider::SetRadius(const float& radius) noexcept
{
	m_radius = radius;
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

Component* Collider::clone() noexcept
{
	auto pCollider = new Collider(*this);
	ObjectManager::Get().PushCollider(pCollider);
	return (Component*)pCollider;
}