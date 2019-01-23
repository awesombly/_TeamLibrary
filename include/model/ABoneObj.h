#pragma once
#include "AMesh.h"


struct BoneMesh : public PNC_VERTEX
{
public:
	int										m_iNumFace;
	vector<PNC_VERTEX>						m_VertexArray;
	vector<DWORD>							m_IndexArray;
	vector<ATri>							m_TriList;
	vector<BoneMesh*>						m_pSubMesh;
	vector<PNC_VERTEX*>						m_pChildMesh;
	PNC_VERTEX*								m_pDrawVertex;
	D3DXMATRIX								m_matInverse;
public:
//	int IsEqulVertexList(vector<PNC_VERTEX>&  VertexArray, PNC_VERTEX& Vertex);
//	int SetUniqueBuffer(vector <ATri_PNCT5>& TriList, int iMtrl, int iStartTri);
	BoneMesh(void)
	{
		m_pDrawVertex = NULL;
		m_iNumFace = 0;
	}
};

class ABoneObj : public AModel
{
public:
	vector<BoneMesh*>			m_pBonehList;
	FLOAT						m_fSpeed;
	FLOAT						m_fLerpTime;
	int							m_iCurrentFrame;
	D3DXMATRIX*					m_pMatrix;			//본애니메이션 보간을 위한 포인터 매트릭스?
	D3DXMATRIX**				m_ppAniMatrix;
	D3DXQUATERNION**			m_ppAniQuater;
	D3DXVECTOR3**				m_ppScaleVector;
	D3DXVECTOR3**				m_ppTransVector;
	ID3D11Buffer*               m_pBoneBuffer;
	ID3D11ShaderResourceView*	m_pBoneBufferRV;
	bool m_bOneTime;
	bool m_bAnimPlay;
	bool m_NoneANIM;


public:
	// 상수 버퍼 및 에니메이션 보간
	void		SetBoneMatrices(ID3D11DeviceContext*    pContext);						//멤버 변수인 m_pMatrix를 사용하여 상수버퍼를 갱신
	void		SetBoneMatrices(ID3D11DeviceContext*    pContext, D3DXMATRIX* pMatrix);	//pMatrix  를 사용하여 상수버퍼를 갱신한다.
	void		SetBoneMatrices(ID3D11DeviceContext*    pContext, D3DXMATRIX* pMatrix, vector<D3DXMATRIX>* pList);	//pMatrix 를 사용하여 상수버퍼를 갱신한다.-> 바이패드에 해당하는 행렬에 곱하여 갱신한다*(바이드 포즈에 사용)

	bool		AniFrame(FLOAT fCurFrame, FLOAT fElapsedTime,
		int iFirstFrame, int iLastFrame,
		D3DXMATRIX* pMatrix, D3DXMATRIX* pParent=0);


public:
	bool		Init()noexcept override;
	bool		Frame(const float& spf, const float& accTime)	noexcept override;

	bool		Render(ID3D11DeviceContext*    pContext) noexcept override;
	bool		Release()noexcept override;
	//bool		Load(ID3D11Device* pDevice,
	//	const TCHAR* szLoadName,
	//	const TCHAR* pLoadShaderFile,
	//	bool bThread = false);

	bool		Load(ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		const TCHAR* szLoadName,
		const TCHAR* pLoadShaderFile,
		bool bThread = false);



	bool		Convert(const TCHAR* strPathName = 0);
public:
	//void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	bool		SetInputLayout(ID3D11Device* pdevice);
	bool		SetBuffer(ID3D11Device* pd3dDevice);
	void		SetBoundBox(D3DXVECTOR3* Quad, BoneMesh* pMesh);
	bool		UpdateBuffer(ID3D11DeviceContext* pContext);
//	bool		CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB);
	bool		Draw(ID3D11DeviceContext*    pContext, AModel* pParent);
//	bool	CreateVertexBuffer();
	//bool	CreateIndexBuffer();

public:
	ABoneObj();
	virtual ~ABoneObj();
};

