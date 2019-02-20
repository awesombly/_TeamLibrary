#pragma once
#pragma comment( lib, "_CoreLib D3D_d.lib")
#include "GameObject.h"
#include "Timer.h"

#include "ADxHelperEx.h"
#include "ADxObjStd.h"
#include "AAseObjStd.h"
#include "AStd.h"

#include "Transform.h"

#include "_ActionList.h"

class AMesh;

struct AInstatnce
{
	D3DXMATRIX matWorld;
};

struct VS_CONSTANT_BUFFER
{
	D3DXMATRIX matWorld;// c0						
	D3DXMATRIX matView;	// c4						
	D3DXMATRIX matProj;	// c8						
	D3DXVECTOR4 vColor; // 12
};


class AModel : public GameObject 
{
protected:
	ID3D11VertexShader*		m_HeroVS;
	ID3D11PixelShader*		m_HeroPS;
	vector<PT_VERTEX>			 m_PTList;
	vector<PCT_VERTEX>			 m_PCTList;
	vector<PNCT_VERTEX>			 m_PNCTList;
	vector<DWORD>				 m_IndexList;

	///  Buffer Comine 
	UINT				m_iMaxVertex;
	UINT				m_iMaxIndex;
	UINT				m_iBeginPosVB;
	UINT				m_iBeginPosIB;
	bool				m_bOptimization;
	int					m_iMatrixIndex;

public:
	float						m_fTickSpeed;
	AScene						 m_Scene;

	//D3DXMATRIX			m_matLocal;	// c0						
	D3DXMATRIX*				m_matView;	// c4						
	D3DXMATRIX*				m_matProj;	// c8	

public:
	VS_CONSTANT_BUFFER			 cbData;
	DX::ADxObject				 m_dxObj;
	vector<AMesh*>				 m_pMesh;

	T_STR				m_szDirName;
	T_STR				m_szName;


	float				m_fElapseTime;
	int					m_iStartFrame;
	int					m_iLastFrame;


	FILE_TYPE			m_ObjType;



	vector<AInstatnce>			m_pInstance;
	ComPtr<ID3D11Buffer>		m_pVBInstance;

protected:

	virtual bool		SetInputLayout(ID3D11Device* pdevice);
	virtual bool		CreateVertexData();

	virtual bool		CreateIndexData();

	virtual bool		CreateVertexBuffer(ID3D11Device* pdevice);
	virtual bool		CreateIndexBuffer(ID3D11Device* pdevice);
	virtual bool		CreateConstantBuffer(ID3D11Device* pdevice);
	virtual bool		UpdateBuffer(ID3D11DeviceContext* pContext = 0);


	virtual bool		CompileShader(ID3D11Device* device, const TCHAR* fileName);
	virtual bool		LoadTexture(ID3D11Device* device, const TCHAR* fileName);

	virtual bool		CreatePrimitiveType();
	virtual void		UpdateConstantBuffer(ID3D11DeviceContext* pContext, AModel* pParent = NULL);
	virtual HRESULT		TextureInfo(const TCHAR* texture);

public:		
	virtual  bool		Set(ID3D11Device* device, ID3D11DeviceContext* pContext, const TCHAR* shaderName, const TCHAR* fileName = 0, bool pass = false); //  float l = 0, float t = 0, float r = 1, float b = 1);
	virtual void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);



	virtual bool		Init() noexcept override;
	virtual bool		Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool		Release() noexcept override;

	virtual bool		Render(ID3D11DeviceContext* context) noexcept override;
	virtual bool		PreRender(ID3D11DeviceContext* context);
	virtual bool		PostRender(ID3D11DeviceContext* context);
	

	virtual void		SetMatrixIndex(int iMatrixIndex) { m_iMatrixIndex = iMatrixIndex; }
	virtual  bool		Load(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread);

	virtual void		SetCollisionData(D3DXVECTOR3 min, D3DXVECTOR3 max);
	virtual void		UpdateCollisionData(D3DXVECTOR3 position);
	virtual void		UpdateMatrix() noexcept override;

	//virtual GameObject* clone() noexcept override;
	bool					SetVSShader(ID3D11VertexShader* pVs);
	bool					SetPSShader(ID3D11PixelShader* pPs);

	ID3D11VertexShader*		GetVSShader();
	ID3D11PixelShader*		GetPSShader();

	bool					ReturnVSShader();
	bool					ReturnPSShader();
public:
	AModel() ;
	virtual ~AModel() ;
};



struct ObjWM
{
	AModel*				m_pModel;
	float				m_fElapseTime;
	AScene				m_Scene;
	vector<D3DXMATRIX>	m_matCalculation;
	vector<D3DXMATRIX>	m_matAnim;
	vector<int>			m_iParent;


	ObjWM(int iNumMesh)
	{
		m_pModel = NULL;
		m_fElapseTime = 0.0f;
		m_matCalculation.resize(iNumMesh);
		m_matAnim.resize(iNumMesh);
		m_iParent.resize(iNumMesh);
		for (int iMesh = 0; iMesh < iNumMesh; iMesh++)
		{
			D3DXMatrixIdentity(&m_matCalculation[iMesh]);
			D3DXMatrixIdentity(&m_matAnim[iMesh]);
			m_iParent[iMesh] = -1;
		}
	}
	ObjWM();
	~ObjWM();
};