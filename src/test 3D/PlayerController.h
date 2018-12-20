#pragma once
#include "GameObject.h"

enum class EAction {
	Idle = 0, Jump,
	Left, Right, Forward, Backward, 
	Dance1, Dance2, Dance3,
};

class Collider;

class PlayerController : public GameObject
{
private:
	EAction		m_curAction;
	Collider*	m_pCollider		= nullptr;

	bool		m_isCharacter	= false;
	bool		m_toIdle	 = false;
	bool		m_isLoopAnim = false;
public:

public:
	void PlayerInput() noexcept;
	void SetAnimation(const EAction& eAction) noexcept;

	bool isCharacter()						  noexcept;
	void isCharacter(const bool& isCharacter) noexcept;

	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
public:
	//PlayerController() = default;
	using GameObject::GameObject;
	virtual ~PlayerController() = default;
};