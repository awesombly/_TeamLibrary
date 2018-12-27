#pragma once
#include "GameObject.h"
#include "ISingleton.h"

class Collider;
class Camera;
class AHeroObj;

class PlayerController : public GameObject, public ISingleton<PlayerController>
{
public:
	enum EAction : char {
		Idle = 0, 
		Left = 1, Forward = 2, ForwardLeft = 3, 
		Right = 4, ForwardRight = 6, 
		Backward = 8, BackwardLeft = 9, BackwardRight = 12,
		Jump = 100, Dance1, Dance2, Dance3, Throw, Run, Fly 
	};
	enum ECharacter : char {
		EDummy = 0, EGuard, EZombie,
	};
private:
	enum class EDirection : char {
		Middle = 0, Left, Right, 
	};

	//AHeroObj*   m_pHero			= nullptr;
	Camera*		m_pCamera		= nullptr;
	EAction		m_curAction;	// 현재 눌린 액션
	EAction		m_curAnim;		// 실제 애니메이션
	EDirection  m_MouseDirection;

	D3DXVECTOR3 m_direction;
	float		m_moveSpeed		= 250.0f;
	float		m_jumpPower		= 180.0f;

	const float	m_DelayThrow	= 0.3f;
	float		m_curDelayThrow = 0.0f;

	//bool		m_toIdle		= false;
	float		m_prevMouseDir = 1.0f;
	// 마우스 고정용
	POINT		m_setMouseScreen;
	POINT		m_setMouseClient;
public:
	ECharacter  m_curCharacter;
	//Collider*	m_pCollider		= nullptr;

	float		m_mouseSense = 0.5f;
	float		m_HP = 1.0f;
	float		m_MP = 1.0f;
public:
	static void SetAnim(AHeroObj* pObject, const ECharacter& eCharacter, const EAction& eAction) noexcept;

	void PlayerInput(const float& spf)					noexcept;
	void CameraInput(const float& spf)					noexcept;
	void ResetOption()									noexcept;

	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
private:
	friend class ISingleton<PlayerController>;
	using GameObject::GameObject;
public:
	virtual ~PlayerController() = default;
};