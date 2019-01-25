#pragma once
#include "Component.h"


class AIZombie : public Component
{
protected:
	enum class EState {
		Idle, Move, Attack,
	};
public:
	EState		m_eState		= EState::Idle;
	EState		m_eDirState		= EState::Idle;
	D3DXVECTOR3 m_Target;
	//float		m_dirRotY;

	float		m_attackRange;
	float		m_moveSpeed;
	float		m_delay = 0.0f;
public:
	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
	virtual Component* clone()									noexcept override;
public:
	AIZombie();
	virtual ~AIZombie() = default;
};
