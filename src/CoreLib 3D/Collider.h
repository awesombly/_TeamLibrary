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
	D3DXVECTOR3 position;			// ������ ���� ��ǥ
	float penetration;				// �浹 ħ�� ����
	float normalImpulse;			// ���� �븻 ���
	float tangentImpulse[2];		// ���� ���� ���
	float bias;						// Restitution + baumgarte
	float normalMass;				// �븻 ���� ����
	float tangentMass[2];			// ���� ���� ����
	q3FeaturePair fp;				// ������ ���� A�� B�� ���?
	UCHAR warmStarted;				// ����� ������?
};


class ColliderAABB;
class ColliderOBB;

class Collider : public Component
{
private:
	bool m_usePhysics = true;
	bool m_isMoving   = false;
	//bool m_isGround	  = false;

	D3DXVECTOR3 m_force		= Vector3::Zero;	// �������� ��
	D3DXVECTOR3 m_direction = Vector3::Zero;	// �̵� ��

	float m_radius;								// ������
	float m_GravityScale = 1.0f;				// �߷�ũ��
	float m_HP			 = 1.0f;

	forward_list<Collider*> m_CollisionList;	// üũ�� �浹ü
	forward_list<Collider*> m_IgnoreList;		// �浹 ���� ���
public:
	void (*CollisionEvent)(Collider*, Collider*) = nullptr;
	ECollider m_eCollider;

	D3DXVECTOR3 m_pivot = Vector3::Zero;		// �θ� ��� �ǹ�
	float m_mass		= 1.0f;					// ����
	float m_repulsion	= 0.5f;					// �ݹ� ���
	float m_drag		= 1.0f;					// ������
	float m_damping		= 0.2f;					// �׷�
	//float m_angularDamping = 0.2f;			// ȸ�� �׷�
	float m_mapHeight   = -9999.0f;				// �� ����

	///
	D3DXVECTOR3 m_normal;						// �浹 �븻
	D3DXVECTOR3 m_tangent[2];					// ���� ����
	q3Contact	m_contacts[8];					// ���� ����?
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
	// ������
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