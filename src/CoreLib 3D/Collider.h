#pragma once
#include "Component.h"
#include "Physics.h"

enum class ECollider : char {
	AABB = 0, OBB, Sphere,
};


class ColliderAABB;
class ColliderOBB;

class Collider : public Component, public Physics
{
protected:
	float m_radius;								// 반지름
	
	forward_list<Collider*> m_CollisionList;	// 체크된 충돌체
	forward_list<Collider*> m_IgnoreList;		// 충돌 제외 대상
public:
	PhysicsInfo* m_pPhysics	= nullptr;
	void (*CollisionEvent)(Collider*, Collider*) = nullptr;
	ECollider	m_eCollider;

	D3DXVECTOR3 m_pivot = Vector3::Zero;		// 부모 상대 피벗
	
	float m_mapHeight = -9999.0f;				// 맵 높이
private:
	bool CollisionAllCheck(const float& spf)						  noexcept;
protected:												  
	virtual bool CollisionCheck(Collider* pCollider)				  noexcept;
public:
	bool SphereToSphere(Collider* pSphereA, Collider* pSphereB)	const noexcept;
	bool SphereToAABB(Collider* pSphere, ColliderAABB* pAABB)	const noexcept;
	bool SphereToOBB(Collider* pSphere, ColliderOBB* pOBB)		const noexcept;
	bool AABBToAABB(ColliderAABB* ApAABB, ColliderAABB* BpAABB)	const noexcept;
	bool AABBToOBB(ColliderAABB* pAABB, ColliderOBB* pOBB)		const noexcept;
	bool OBBToOBB(ColliderOBB* ApOBB, ColliderOBB* BpOBB)		const noexcept;
	///
	void AddIgnoreList(Collider* pCollider)			  noexcept;
	void ClearIgnoreList()							  noexcept;
	void ClearCollisionList()						  noexcept;
	// 구판정
	float GetWorldRadius()							  noexcept;
	void  SetRadius(const float& radius)			  noexcept;

	D3DXVECTOR3 GetCenter()							  noexcept;

	//void Update()												noexcept override;
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	Component* clone()											noexcept override;
protected:
	Collider();
public:
	Collider(const float& radius);
	virtual ~Collider();
};