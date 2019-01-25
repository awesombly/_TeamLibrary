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
	bool		m_isChatting = false;

	Camera*		m_pCamera		= nullptr;
	EAction		m_curAction;			// 현재 눌린 액션
	EAction		m_curAnim;				// 실제 애니메이션

	float		m_prevRotY		= 0.0f;
	float		m_prevMouseDir	= 1.0f;
	// 마우스 고정용
	POINT		m_setMouseScreen;
	POINT		m_setMouseClient;

	GameObject*	m_pEffectFly = nullptr;
	///
	float		m_EXP				= 0.0f;
	UCHAR		m_statPoint			= 0;
	//
	const float	MoveSpeed = 30.0f;
	const float	JumpPower = 70.0f;
public:
	ECharacter  m_curCharacter;			// 현재 캐릭터
	ECharacter  m_selectCharacter;		// 선택 캐릭터
	float		m_moveSpeed;
	float		m_jumpPower;
	///
	float		m_DelayRespawn;
	float		m_DelayEnemyPanel;
	float		m_DelayThrow;
	float		m_DelayDash;
	float		m_DelayMelee;
	float		m_RegenMP;
										 
	float		m_curDelayRespawn	 = 0.0f;
	float		m_curDelayEnemyPanel = 0.0f;
	float		m_curDelayThrow		 = 0.0f;
	float		m_curDelayDash		 = 0.0f;
	float		m_curDelayMelee		 = 0.0f;

	float		m_mouseSense		 = 0.5f;
	float		m_MP				 = 1.0f;
public:
	GameObject* m_pHome			 = nullptr;
	const float HomeRadius		 = 1600.0f;

	void*       m_pOption		 = nullptr;
	void*		m_pHpBar		 = nullptr;		// 체력바
	void*		m_pLeftIcon		 = nullptr;		// JProgressBar
	void*		m_pRightIcon	 = nullptr;		// JProgressBar
	void*		m_pRespawn		 = nullptr;
	void*		m_pRespawnBar	 = nullptr;
	void*		m_pRespawnEffect = nullptr;
	void*		m_pHitEffect	 = nullptr;
	void*		m_pEnemyPanel	 = nullptr;		// JPanel
	void*		m_pEnemyHP		 = nullptr;		// JProgressBar
	void*		m_pEnemyName	 = nullptr;		// JTextCtrl
	void*		m_pEnemyHPText	 = nullptr;		// JTextCtrl
private:
	void SendGiantMode(const float& spf)											noexcept;
public:
	static void SetAnim(AHeroObj* pObject, const UINT& socket, const ECharacter& eCharacter, const EAction& eAction, const D3DXVECTOR3& forward = Vector3::Zero) noexcept;

	void PlayerInput(const float& spf)												noexcept;
	void CameraInput(const float& spf)												noexcept;
	void ResetOption()																noexcept;
	void UpdateStatus()																noexcept;
	void Possess(GameObject* pObject)												noexcept;
	void DeadEvent()																noexcept;
	void HitEvent(Collider* pTarget)												noexcept;
	void OperEXP(const float& value)												noexcept;

	void SendAnimTransform(const EAction& eAction, const ECharacter& eCharacter)	noexcept;
	void SendReqRespawn(const ECharacter& eCharacter)								noexcept;
	void StartGiantMode()															noexcept;

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