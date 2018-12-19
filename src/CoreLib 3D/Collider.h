#pragma once
#include "Component.h"


enum class ECollider : char {
	AABB = 0, OBB, Sphere,
};

class Collider : public Component
{
private:
	bool m_useGravity = true;
public:
	ECollider m_eCollider;

	D3DXVECTOR3 m_pivot = Vector3::Zero;	// �θ� ��� �ǹ�
	D3DXVECTOR3 m_force = Vector3::Zero;	// �������� ��
	float m_mass		= 1.0f;				// ����
	float m_repulsion	= 0.7f;				// �ݹ� ���
	float m_drag		= 0.4f;				// ������
	float m_damping		= 0.1f;				// �׷�
	//float m_angularDamping = 0.2f;			// ȸ�� �׷�
	
	forward_list<Collider*> m_CollisionList;

protected:
	bool CollisionAllCheck(const float& spf)					noexcept;
public:
	D3DXVECTOR3 GetCenter()										noexcept;
	float GetVelocity()											noexcept;

	void useGravity(const bool& useGravity) noexcept;
	bool useGravity()						noexcept;

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