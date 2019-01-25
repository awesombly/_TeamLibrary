#pragma once
#include "Component.h"


class AIZombieEx : public Component
{
private:
	enum class EState {
		Idle, Move, Attack,
	};
public:
	EState		m_eState = EState::Idle;
	EState		m_eDirState = EState::Idle;
	D3DXVECTOR3 m_Target;
	float		m_dirRotY;

	float		m_attackRange;
	float		m_moveSpeed;
	float		m_delay = 0.0f;
public:
	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone() noexcept;
public:
	AIZombieEx();
	virtual ~AIZombieEx() = default;
};
