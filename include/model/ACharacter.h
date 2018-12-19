#pragma once
#include "AModelObj.h"

class ACharacter :public AModelObj
{
public:
	T_STR								m_szName;
	ABoneObj*							m_pBoneObject;
	
	int									m_iStartFrame;
	int									m_iLastFrame;
	int									m_iCurrentFrame;
	float								m_fFrame;
	float								m_fLerpTime;
	D3DXMATRIX							m_pMatrix[255];
	int									m_iMatrixIndex;

	ID3D11Buffer*                       m_pBoneBuffer;
	ID3D11ShaderResourceView*           m_pBoneBufferRV;


public:
	bool		Init() noexcept override;
	bool		Frame(const float& spf, const float& accTime)	noexcept override;
	bool		Render(ID3D11DeviceContext*    pContext)noexcept override;
	bool		Release()noexcept override;
	bool		Add(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*    pImmediateContext,
		const TCHAR* strSkinFileName,
		const TCHAR* strSkinShaderName,
		int		iMatrixIndex = -1,
		int		iModelMatrixIndex = -1);
	// 모델 리스트들에 매트릭스 인덱스 세싱
	void		SetModelMaterixID(int iMatrixIndex);
	void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	void		SetActionFrame(int iStart, int iEnd,
		int iCharacterMatrixIndex = -1,
		int iModelMatrixIndex = -1);
public:
	ACharacter();
	virtual ~ACharacter();
};

