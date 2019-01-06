#pragma once
#include "Component.h"


enum class ECollider : char {
	AABB = 0, OBB, Sphere,
};

union q3FeaturePair
{
	struct
	{
		UCHAR inR;
		UCHAR outR;
		UCHAR inI;
		UCHAR outI;
	};
	int key;
};

struct q3Contact
{
	D3DXVECTOR3 position;			// 접촉점 월드 좌표
	float penetration;				// 충돌 침투 깊이
	float normalImpulse;			// 누적 노말 충격
	float tangentImpulse[2];		// 누적 마찰 충격
	float bias;						// Restitution + baumgarte
	float normalMass;				// 노말 제한 질량
	float tangentMass[2];			// 접선 제한 질량
	q3FeaturePair fp;				// 접점에 대한 A와 B의 기능?
	UCHAR warmStarted;				// 디버그 랜더용?
};


class ColliderAABB;
class ColliderOBB;

class Collider : public Component
{
private:
	bool m_usePhysics = true;
	bool m_isMoving   = false;
	//bool m_isGround	  = false;

	D3DXVECTOR3 m_force		= Vector3::Zero;	// 적용중인 힘
	D3DXVECTOR3 m_direction = Vector3::Zero;	// 이동 힘

	float m_radius;								// 반지름
	float m_GravityScale = 1.0f;				// 중력크기
	float m_HP			 = 1.0f;

	forward_list<Collider*> m_CollisionList;	// 체크된 충돌체
	forward_list<Collider*> m_IgnoreList;		// 충돌 제외 대상
public:
	void (*CollisionEvent)(Collider*, Collider*) = nullptr;
	ECollider m_eCollider;

	D3DXVECTOR3 m_pivot = Vector3::Zero;		// 부모 상대 피벗
	float m_mass		= 1.0f;					// 질량
	float m_repulsion	= 0.5f;					// 반발 계수
	float m_drag		= 1.0f;					// 마찰력
	float m_damping		= 0.2f;					// 항력
	//float m_angularDamping = 0.2f;			// 회전 항력
	float m_mapHeight   = -9999.0f;				// 맵 높이

	///
	D3DXVECTOR3 m_normal;						// 충돌 노말
	D3DXVECTOR3 m_tangent[2];					// 접선 벡터
	q3Contact	m_contacts[8];					// 접점 정보?
	int			m_contactCount;
protected:
	bool CollisionAllCheck(const float& spf)		  noexcept;
public:												  
	virtual bool CollisionCheck(Collider* pCollider)				  noexcept;
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
	///
	void AddForce(const D3DXVECTOR3& vForce)		  noexcept;
	void SetForce(const D3DXVECTOR3& vForce)		  noexcept;
	void SetDirectionForce(const D3DXVECTOR3& vForce) noexcept;
	void SetGravityScale(const float& gravityRate)	  noexcept;

	D3DXVECTOR3 GetForce()							  noexcept;
	D3DXVECTOR3 GetTotalForce()						  noexcept;
	D3DXVECTOR3 GetCenter()							  noexcept;
	float GetVelocity()								  noexcept;
	float GetVelocitySq()							  noexcept;
	// 구판정
	float GetWorldRadius()							  noexcept;
	void  SetRadius(const float& radius)			  noexcept;
													  
	void OperHP(const float& value)					  noexcept;
	void SetHP(const float& value)					  noexcept;
	const float& GetHP()							  noexcept;
	///
	void usePhysics(const bool& usePhysics)			  noexcept;
	bool usePhysics()								  noexcept;
	void isMoving(const bool& isMoving)			      noexcept;
	bool isMoving()									  noexcept;

	//void Update()												noexcept override;
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	Component* clone()											noexcept override;
protected:
	Collider() = default;
public:
	Collider(const float& radius);
	virtual ~Collider();
};