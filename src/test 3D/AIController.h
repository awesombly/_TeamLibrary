#pragma once
#include "Component.h"


class AIController : public Component
{
private:
	enum class EState {
		Idle, Move, Attack,
	};
public:
	EState		m_eState		= EState::Idle;
	D3DXVECTOR3 m_Target		= Vector3::Zero;
	float		m_dirRotY;

	float		m_attackRange;
	float		m_moveSpeed;
	//float		m_damage;
public:
	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone() noexcept;
public:
	AIController();
	virtual ~AIController() = default;
};
