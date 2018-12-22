#pragma once
#include "GameObject.h"


class Collider;
class Camera;

class PlayerController : public GameObject
{
public:
	enum EAction : char {
		Idle = 0, 
		Left = 1, Forward = 2, ForwardLeft = 3, 
		Right = 4, ForwardRight = 6, 
		Backward = 8, BackwardLeft = 9, BackwardRight = 12,
		Jump = 100, Dance1, Dance2, Dance3,
	};
private:
	EAction		m_curAction;
	Camera*		m_pCamera		= nullptr;

	D3DXVECTOR3 m_direction;
	float		m_moveSpeed = 100.0f;
	float		m_jumpPower = 180.0f;

	bool		m_isCharacter	= false;
	bool		m_toIdle		= false;
	bool		m_isLoopAnim	= false;
	// 마우스 고정용
	POINT		m_setMouseScreen;
	POINT		m_setMouseClient;
public:
	Collider*	m_pCollider		= nullptr;

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