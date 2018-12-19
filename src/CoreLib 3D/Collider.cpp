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
	// 저항
	m_force -= m_force * m_damping * spf;

	// 힘 적용
	m_pParent->GetRoot()->Translate(m_force * spf);
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
			// + 상대 충돌 처리
			m_force -= m_force * (m_drag + iter->m_drag) * 0.5f  * spf;
			m_force *= -(m_repulsion + iter->m_repulsion) * 0.5f;

			iter->m_force -= m_force * (m_drag + iter->m_drag) * 0.5f  * spf;
			iter->m_force *= -(m_repulsion + iter->m_repulsion) * 0.5f;

			ErrorMessage(L"Hit Collider : " + m_pParent->m_myName + L",  " + iter->m_pParent->m_myName);
			iter->m_CollisionList.push_front(this);
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


void Collider::useGravity(const bool& useGravity) noexcept
{
	m_useGravity = useGravity;
}

bool Collider::useGravity() noexcept
{
	return m_useGravity;
}