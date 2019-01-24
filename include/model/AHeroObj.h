#pragma once
#include "ACharMgr.h"
//#include "ACamera.h"
#include "GameObject.h"

#define MAX_BONE_MATRICES 255

enum PLAYER_SELECT
{
	GUARD,
	ZOMBIE,
	ZOMBIE_KING,
	DEFAULT
};


struct CBConstBoneWorld
{
	D3DXMATRIX    g_mConstBoneWorld[MAX_BONE_MATRICES];
};



class AHeroObj : public AModel
{
	PLAYER_SELECT			m_PlayerSelect;

public:

	int						m_iObjectIndex;
	ACharacter*				m_pChar;
	T_STR					m_szName;
	ABoneObj*				m_pBoneObject;
	bool					m_bConstantBufferType;
	int						m_iStartFrame;
	int						m_iLastFrame;
	int						m_iCurrentFrame;
	float					m_fFrame;
	float					m_fLerpTime;
	float					m_fSpeed;
	D3DXMATRIX				m_pMatrix[255];
	CBConstBoneWorld		m_cbBoneData;
	ComPtr<ID3D11Buffer>    m_pCBConstBoneWorld;
	int						m_iMatrixIndex;
	bool					m_bBoneRender;



	
	AHeroObj* m_Parent;

	GameObject* clone() noexcept override;


	D3DXVECTOR3 GetRightHandPos();
	D3DXVECTOR3 GetLeftHandPos();
public:

	
	void		SetBoneMatrices(vector<D3DXMATRIX>* pList);
	bool		Frame(const float& spf, const float& accTime) noexcept override;
	bool		Render(ID3D11DeviceContext*    pContext) noexcept override;
	bool		Release() noexcept override;

	void		SetActionFrame(int iStart, int iEnd, int iCharacterMatrixIndex = -1, int iModelMatrixIndex = -1);
	bool		CreateConstantBuffer(ID3D11Device* pDevice);


	//bool		SetPlayerCharacter(const TCHAR* fileName);// , D3DXVECTOR3 minBox, D3DXVECTOR3 maxBox);

	bool		SetPlayerCharacter(const TCHAR* fileName, float x=0,float y=0, float z=0);
	
	bool		SetANIM_Loop(const TCHAR* szName);
	bool		SetANIM_OneTime(const TCHAR* szName);
	//D3DXVECTOR3 GetPlayerPos();
	//INT GetPlayerPosX();
	//INT GetPlayerPosY();
	//INT GetPlayerPosZ();
	//bool CollRect_Rect(A_Box& Dest, A_Box& Src);

public:
	AHeroObj();
	virtual ~AHeroObj();
};




