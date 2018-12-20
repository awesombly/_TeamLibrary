#pragma once
#include "GameObject.h"

enum class EAction {
	Idle = 0, Jump,
	Left, Right, Forward, Backward, 
	Dance1, Dance2, Dance3,
};


class PlayerController : public GameObject
{
public:
	EAction m_curAction;
	bool m_isCharacter = false;
public:
	void PlayerInput() noexcept;
	void SetAnimation(const EAction& eAction) noexcept;

	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
public:
	PlayerController() = default;
	virtual ~PlayerController() = default;
};