#pragma once
#include "GameObject.h"

enum class EAction {
	Idle = 0, Jump,
	Left, Right, Forward, Backward, 
	Dance1, Dance2, Dance3,
};

class Collider;
class Camera;

class PlayerController : public GameObject
{
private:
	EAction		m_curAction;
	Collider*	m_pCollider		= nullptr;
	Camera*		m_pCamera		= nullptr;

	bool		m_isCharacter	= false;
	bool		m_toIdle		= false;
	bool		m_isLoopAnim	= false;
	// 마우스 고정용
	POINT		m_setMouseScreen;
	POINT		m_setMouseClient;
public:

public:
	void SetAnimation(const EAction& eAction) noexcept;
	void PlayerInput(const float& spf)		  noexcept;
	void CameraInput(const float& spf)		  noexcept;
	void ResetOption()						  noexcept;

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