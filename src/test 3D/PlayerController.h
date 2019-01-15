#pragma once
#include "GameObject.h"
#include "ISingleton.h"
//#include "uiheader.h"

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
		Jump = 100, Dance1, Dance2, Dance3, Throw, Run, Fly, FlyEnd, Dash, Melee
	};
	enum ECharacter : char {
		EDummy = 0, EGuard, EZombie,
	};
private:
	enum class EDirection : char {
		Middle = 0, Left, Right, 
	};
	bool		m_isChatting = false;

	Camera*		m_pCamera		= nullptr;
	ECharacter  m_curCharacter;			// 현재 캐릭터
	EAction		m_curAction;			// 현재 눌린 액션
	EAction		m_curAnim;				// 실제 애니메이션
	EDirection  m_MouseDirection;
	///
	D3DXVECTOR3 m_direction;
	float		m_moveSpeed		= 100.0f;
	float		m_jumpPower		= 150.0f;

	//bool		m_toIdle		= false;
	float		m_prevRotY		= 0.0f;
	float		m_prevMouseDir	= 1.0f;
	// 마우스 고정용
	POINT		m_setMouseScreen;
	POINT		m_setMouseClient;

	Component* m_pEffectFly = nullptr;
public:
	Collider*  m_pMeleeCollider = nullptr;

	const float	m_DelayRespawn  = 998.0f;
	const float	m_DelayThrow	= 0.3f;
	const float	m_DelayDash		= 1.0f;
	const float	m_DelayMelee	= 1.0f;

	float		m_curDelayRespawn = 0.0f;
	float		m_curDelayThrow = 0.0f;
	float		m_curDelayDash	= 0.0f;
	float		m_curDelayMelee = 0.0f;

	float		m_mouseSense = 0.5f;
	//float		m_HP = 1.0f;
	float		m_MP = 1.0f;
	
	void*		m_pHpBar		 = nullptr;		// 체력바
	void*		m_pRespawn		 = nullptr;
	void*		m_pRespawnBar	 = nullptr;
	void*		m_pRespawnEffect = nullptr;
	void*		m_pHitEffect	 = nullptr;
public:
	static void SetAnim(AHeroObj* pObject, const ECharacter& eCharacter, const EAction& eAction, const D3DXVECTOR3& forward = Vector3::Zero) noexcept;

	void PlayerInput(const float& spf)												noexcept;
	void CameraInput(const float& spf)												noexcept;
	void ResetOption()																noexcept;
	void Possess(GameObject* pObject)												noexcept;
	void DeadEvent()																noexcept;

	void SendAnimTransform(const EAction& eAction, const ECharacter& eCharacter)	noexcept;

	void isChatting(const bool& isChat)												noexcept;
	bool isChatting()														  const noexcept;

	bool Init()																		noexcept override;
	bool Frame(const float& spf, const float& accTime)								noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)										noexcept override;
	bool Release()																	noexcept override;
private:
	friend class ISingleton<PlayerController>;
	using GameObject::GameObject;
public:
	virtual ~PlayerController() = default;
};