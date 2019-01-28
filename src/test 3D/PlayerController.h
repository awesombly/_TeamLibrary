#pragma once
#include "GameObject.h"
#include "ISingleton.h"
//#include "uiheader.h"

class Collider;
class Camera;
class AHeroObj;
class UIManager;



class PlayerController : public GameObject, public ISingleton<PlayerController>
{
public:
	enum EAction : UCHAR {
		Idle = 0, 
		Left = 1, Forward = 2, ForwardLeft = 3, 
		Right = 4, ForwardRight = 6, 
		Backward = 8, BackwardLeft = 9, BackwardRight = 12,
		Jump = 100, Dance1, Dance2, Dance3, LSkill, RSkill, Run, Fly, FlyEnd, Dash,
		ShockWave, ThrowBomb,
	};
	enum ECharacter : UCHAR {
		EDummy = 0, EGuard, EZombie, EArcher, EMage,
	};
	enum EItem : UCHAR {
		Shock = 0, Bomb, 
	};
private:
	UIManager*  pUIManager		= nullptr;
	Camera*		m_pCamera		= nullptr;
	EAction		m_curAction;			// ���� ���� �׼�
	EAction		m_curAnim;				// ���� �ִϸ��̼�

	float		m_prevRotY		= 0.0f;
	float		m_prevMouseDir	= 1.0f;
	// ���콺 ������
	POINT		m_setMouseScreen;
	POINT		m_setMouseClient;

	GameObject*	m_pEffectFly	= nullptr;
	GameObject* m_pTargetEnemy	= nullptr;
	///
	float		m_EXP			= 0.0f;
	float		m_disEXP		= 0.0f;
	//
	const float	MoveSpeed = 30.0f;
	const float	JumpPower = 70.0f;
	
	map<int, void(*)(PlayerController*, void*)> m_ItemList;
public:
	EAction		m_eAction;				// ���� �ִϸ��̼�
	ECharacter  m_curCharacter;			// ���� ĳ����

	float		m_NeedEXP			= 1.0f;
	UCHAR		m_statPoint			= 0;
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

	float		m_maxMP				 = 1.0f;
	float		m_curMP				 = 0.0f;
	float		m_mouseSense		 = 0.5f;
	// ���� �Ÿ�
	const float SoundRange			 = 30000.0f;
public:
	GameObject* m_pHome			 = nullptr;
	const float HomeRadius		 = 1600.0f;
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
	void StartVibration(float seconds, const float& shakePower)						noexcept;

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