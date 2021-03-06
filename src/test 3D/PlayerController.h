#pragma once
#include "GameObject.h"
#include "ISingleton.h"
#include "PlayerState.h"

class Collider;
class Camera;
class AHeroObj;
class UIManager;
class ParticleSystem;


class PlayerController : public GameObject, public ISingleton<PlayerController>
{
public:
	enum EAction : UCHAR {
		Idle		= 0,	Left = 1,	Forward = 2,	ForwardLeft = 3, 
		Right		= 4,	ForwardRight = 6, 
		Backward	= 8,	BackwardLeft = 9, BackwardRight = 12,
		NIdle		= 20,	NLeft = 21, NForward = 22, NRight = 24, NBackward = 28,
		RLeft		= 41,	RForward = 42, RRight = 44,  RBackward = 48,
		NForwardLeft = 63,  NForwardRight = 66,
		NBackwardLeft = 69, NBackwardRight = 72,
		Jump		= 100,	Dance1, Dance2, Dance3, LSkill, RSkill, Fly, FlyEnd, 
		Dash, DashLeft, DashRight, Run, RunLeft, RunRight, 
		Special,		Special2, Special3, Attack, Attack2, Wait,
		ChargeAttack,	ChargeAttack2, LCharging, LCharge1, LCharge2,
		Motion1,		Motion2,	Motion3,
		ShockWave,		ThrowBomb,	ITimeBomb, IShockBomb, IMine, IMissile, INuclear, IPotion, IBarricade
	};
	enum ECharacter : UCHAR {
		EDummy = 0, EGuard, EArcher, EMage
	};
	enum class ECarpet : UCHAR {
		Smithy = 0, Church, Shop, Tower
	};
	enum class EViewPoint : UCHAR {
		Back, Quarter, 
	};
public:
	PlayerState*					 m_curState = nullptr;	// 현재 상태
private:
	map<EPlayerState, PlayerState*>  m_stateList;			// 상태별 행동
	PlayerState*					 m_preState;			// 이전 상태
	EViewPoint  m_eView			= EViewPoint::Back;
	///
	Collider*	m_pCollider		= nullptr;
	Camera*		m_pCamera		= nullptr;
	EAction		m_preAction;			// 현재 눌린 액션
	EAction		m_curAnim;				// 실제 애니메이션

	float		m_prevRotY		= 0.0f;
	float		m_prevMouseDir	= 1.0f;
	// 마우스 고정용
	POINT		m_setMouseScreen;
	POINT		m_setMouseClient;

	GameObject* m_pTargetEnemy	= nullptr;
	GameObject*	m_pEffectFly	= nullptr;
	GameObject*	m_pEffectBerserk= nullptr;
	bool		m_inputCombo	= false;
	char		m_comboCount	= 0;
	///
	float		m_EXP			= 0.0f;
	float		m_disEXP		= 0.0f;
	//
	const float	MoveSpeed		= 105.0f;
	const float	JumpPower		= 210.0f;
public:
	map<int, void(*)(PlayerController*, void*)> m_ItemList;
	UIManager*  pUIManager			= nullptr;
	EAction		m_eAction;									// 눌린 애니메이션
	ECharacter  m_curCharacter		= ECharacter::EDummy;	// 현재 캐릭터
	ECharacter  m_selectCharacter	= ECharacter::EDummy;

	float		m_NeedEXP			= 1.0f;
	UCHAR		m_statPoint			= 0;
	float		m_moveSpeed;
	float		m_jumpPower;
	///
	float		m_DelayFrame = 0.0f;
	float		m_DelayRespawn;
	float		m_DelayEnemyPanel;

	float		m_DelayLSkill;
	float		m_DelayRSkill;
	float		m_DelayDash;
	float		m_DelaySpecial;
	float		m_RegenMP;
	float		m_RegenHP;
										 
	float		m_curDelayRespawn	 = 0.0f;
	float		m_curDelayEnemyPanel = 0.0f;

	float		m_curDelayLSkill	 = 0.0f;
	float		m_curDelayRSkill	 = 0.0f;
	float		m_curDelayDash		 = 0.0f;
	float		m_curDelaySpecial	 = 0.0f;

	char		m_defencePoint		 = 0;
	float		m_chargeCount		 = 0.0f;
	float		m_berserkFrame		 = 0.0f;
	float		m_motionBuff		 = 0.0f;
	char		m_cooltimeBuff		 = 0;

	float		m_maxMP				 = 1.0f;
	float		m_curMP				 = 0.0f;
	float		m_disMP				 = 0.0f;
	float		m_mouseSense		 = 0.5f;
	float		m_GameFrameCount	 = 0.0f;
	// 들을 거리
	const float SoundRange			 = 302500.0f;
	float		m_LightAmount		 = 0.3f;
public:
	GameObject* m_pHome			 = nullptr;
	D3DXVECTOR3 m_HomePos;
	const float HomeRadius		 = 15000.0f;

	D3DXVECTOR3 m_CarpetPos[4];
	bool		m_canChurh = false;

	UCHAR		m_upgradeWeapon = 0;
	UCHAR		m_upgradeArmor = 0;
	UCHAR		m_upgradeAcce1 = 0;
	UCHAR		m_upgradeAcce2 = 0;

	int			m_money = 0;

	forward_list<ParticleSystem*> m_followEffects;
	forward_list<pair<AHeroObj*, float> > m_dyingEnemys;
private:
	void SendGiantMode(const float& spf)											noexcept;
public:
	static void SetAnim(AHeroObj* pObject, const UINT& socket, const ECharacter& eCharacter, const EAction& eAction, const D3DXVECTOR3& forward = Vector3::Zero) noexcept;

	void SetState(const EPlayerState& eState)										noexcept;
	void PlayerInput(const float& spf)												noexcept;
	void CameraInput(const float& spf)												noexcept;
	void ResetOption()																noexcept;
	void UpdateStatus(const bool& infoUpdate = true)								noexcept;
	void Possess(GameObject* pObject)												noexcept;
	void DeadEvent()																noexcept;
	void HitEvent(Collider* pTarget)												noexcept;
	void OperEXP(const float& value)												noexcept;
	void CheckTownCollision()														noexcept;
	void UpdateShopInfo()															noexcept;

	void SendAnimTransform(const EAction& eAction, const ECharacter& eCharacter)	noexcept;
	void SendReqRespawn(const ECharacter& eCharacter)								noexcept;
	void StartGiantMode()															noexcept;
	void StartVibration(float seconds, const float& shakePower)						noexcept;
	void SendPhysicsInfo()															noexcept;
	void SendGameStart()															noexcept;
	//static void StartTaunt(GameObject* pTarget, float seconds)						noexcept;

	bool Init()																		noexcept override;
	bool Frame(const float& spf, const float& accTime)								noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)										noexcept override;
	bool Release()																	noexcept override;
private:
	using GameObject::GameObject;
public:
	virtual ~PlayerController() = default;

	friend class ISingleton<PlayerController>;
	friend class PlayerState;
	friend class PlayerStateBasic;
	friend class PlayerStateLSkill;
	friend class PlayerStateRSkill;
	friend class PlayerStateRun;
	friend class PlayerStateGuard;

	friend class ArcherStateBasic;
	friend class ArcherStateLSkill;
	friend class ArcherStateRSkill;
	friend class ArcherStateDash;
	friend class ArcherStateTumbling;

	friend class MageStateBasic;
	friend class MageStateLSkill;
	friend class MageStateRSkill;
	friend class MageStateDash;
	friend class MageStateSpecial;
};