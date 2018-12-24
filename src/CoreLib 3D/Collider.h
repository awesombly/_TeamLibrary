#pragma once
#include "Component.h"


enum class ECollider : char {
	AABB = 0, OBB, Sphere,
};

class Collider : public Component
{
private:
	bool m_useGravity = true;
	bool m_usePhysics = true;
	//bool m_isGround	  = false;
	bool m_isMoving   = false;

	D3DXVECTOR3 m_force		= Vector3::Zero;	// 적용중인 힘
	D3DXVECTOR3 m_direction = Vector3::Zero;	// 이동 힘
public:
	ECollider m_eCollider;
	forward_list<Collider*> m_CollisionList;

	D3DXVECTOR3 m_pivot = Vector3::Zero;	// 부모 상대 피벗
	float m_mass		= 1.0f;				// 질량
	float m_repulsion	= 0.7f;				// 반발 계수
	float m_drag		= 1.0f;				// 마찰력
	float m_damping		= 0.2f;				// 항력
	//float m_angularDamping = 0.2f;			// 회전 항력
	float* m_pMapHeight = nullptr;

	D3DXVECTOR3 m_normal = Vector3::Zero;	// 계산용 노말
protected:
	bool CollisionAllCheck(const float& spf)		  noexcept;
public:												  
	void AddForce(const D3DXVECTOR3& vForce)		  noexcept;
	void SetForce(const D3DXVECTOR3& vForce)		  noexcept;
	void SetDirectionForce(const D3DXVECTOR3& vForce) noexcept;
											 
	D3DXVECTOR3 GetTotalForce()						  noexcept;
	D3DXVECTOR3 GetCenter()							  noexcept;
	float GetVelocity()								  noexcept;
	float GetVelocitySq()							  noexcept;
													  
	void useGravity(const bool& useGravity)			  noexcept;
	bool useGravity()								  noexcept;
	void usePhysics(const bool& usePhysics)			  noexcept;
	bool usePhysics()								  noexcept;
	void isMoving(const bool& isMoving)			      noexcept;
	bool isMoving()									  noexcept;

	virtual bool CollisionCheck(Collider* pCollider)			noexcept = 0;
	//void OnCollision(Collider* pCollider) noexcept;

	//void Update()												noexcept override;
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
protected:
	Collider();
public:
	virtual ~Collider();
};