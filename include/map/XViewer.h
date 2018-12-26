#pragma once
#include"XDxState.h"
#include"XParser.h"
#include "GameObject.h"

typedef basic_string<TCHAR> tstring;

enum OBJECT_TYPE { GEOMOBJ = 0, HELPEROBJ };

struct XScene
{
	int m_iFirstFrame;
	int m_iLastFrame;
	int m_iFrameSpeed;
	int m_iTickPerFrame;
};

struct XTexture
{
	DWORD	m_dwSubNum;
	TCHAR	m_TextureName[256];
};

struct XMtrl
{
	vector<XTexture>	m_Texture;
	vector<XMtrl>		m_SubMtrl;
};

struct PNCT
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
};

struct PNCTGroup
{
	PNCT		m_Vertex;
	DWORD		m_dwMtrlId;
};

struct TAnim
{
	DWORD		m_dwTick;
	D3DXVECTOR3	m_Pos;
	TAnim() {}
	TAnim(DWORD tick, D3DXVECTOR3 v)
	{
		m_dwTick = tick;
		m_Pos.x = v.x;
		m_Pos.y = v.y;
		m_Pos.z = v.z;
	}
};
struct RAnim
{
	DWORD			m_dwTick;
	D3DXQUATERNION	m_Rot;

	RAnim() {}
	RAnim(DWORD tick, D3DXQUATERNION q)
	{
		m_dwTick = tick;
		m_Rot.x = q.x;
		m_Rot.y = q.y;
		m_Rot.z = q.z;
		m_Rot.w = q.w;
	}
};
struct SAnim
{
	DWORD			m_dwTick;
	D3DXVECTOR3		m_vPos;
	D3DXQUATERNION	m_qRot;

	SAnim() {}
	SAnim(DWORD tick, D3DXVECTOR3 v, D3DXQUATERNION q)
	{
		m_dwTick = tick;
		m_vPos.x = v.x;
		m_vPos.y = v.y;
		m_vPos.z = v.z;
		m_qRot.x = q.x;
		m_qRot.y = q.y;
		m_qRot.z = q.z;
		m_qRot.w = q.w;
	}
	SAnim(DWORD tick, D3DXVECTOR3 v0, D3DXVECTOR3 v1, float f)
	{
		m_dwTick = tick;
		m_vPos.x = v0.x;
		m_vPos.y = v0.y;
		m_vPos.z = v0.z;
		m_qRot.x = v1.x;
		m_qRot.y = v1.y;
		m_qRot.z = v1.z;
		m_qRot.w = f;
	}
};

struct CB////임시
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
};

struct XMesh
{
	BOOL				m_ObjType;
	TCHAR				m_NodeName[256];
	TCHAR				m_Parents[256];
	D3DXMATRIX			m_matTM;
	D3DXMATRIX			m_matCalculation;
	int					m_iNumFace;
	vector<PNCTGroup>	m_Vertex;
	int					m_iRef;
	vector<XMesh>		m_SubMesh;
	vector<TAnim>		m_tAnim;
	vector<RAnim>		m_rAnim;
	vector<SAnim>		m_sAnim;

	vector<DWORD>			m_ParentObj;
	vector<DWORD>			m_ChildObj;

	vector<PNCT>						m_VertexList;
	vector<DWORD>						m_dwIndexData;
	vector<ID3D11ShaderResourceView*>   m_pTextureSRV;
	CB									cb;
	ID3D11Buffer*						m_pVertexBuffer;
	ID3D11Buffer*						m_pIndexBuffer;
	ID3D11Buffer*						m_pConstantBuffer;
};


class XViewer :public GameObject
{
public:
	XParser						m_Parser;
	XScene						m_Scene;
	vector<XMtrl>				m_Mtrl;
	vector<XMesh>				m_Mesh;
	ID3D11Device*				m_pd3dDevice;
	ID3D11DeviceContext*		m_pContext;
	ID3D11Buffer*				m_pConstantBuffer;
	ID3D11InputLayout*			m_pVertexLayout;
	ID3DBlob*					m_pVSBuf;
	ID3DBlob*					m_pPSBuf;
	ID3D11VertexShader*			m_pVS;
	ID3D11PixelShader*			m_pPS;
	CB							cb;
	D3DXMATRIX					m_matWorld;
	D3DXMATRIX					m_matView;
	D3DXMATRIX					m_matProj;
	float						m_fTick;
	float						m_fTickSpeed;
public:
	bool		LoadShk(TCHAR* filename);
	void		ReadMtrl();
	void		ReadMesh();
	bool		ReadObjAnim(XMesh* mesh);
	bool		FindParent();
	bool		SetBuffer(XMesh* mesh);
	HRESULT		CreateVertexBuffer(XMesh* mesh);
	HRESULT		CreateIndexBuffer(XMesh* mesh);
	HRESULT		CreateConstantBuffer(XMesh* mesh);
	HRESULT		CreateShaderResourceView(XMesh* mesh, TCHAR* TextureFile);
	bool		CreateMeshSRV(XMesh* mesh, int idx = 0);
	HRESULT		LoadShader(TCHAR* VSfile, TCHAR* PSfile);
	HRESULT		InputLayout();
	bool		DrawObject(ID3D11DeviceContext* pContext, XMesh* mesh);
	void		SetTickSpeed(float speed);
	D3DXMATRIX	GetSAnim(XMesh* mesh);
	D3DXMATRIX	GetRAnim(XMesh* mesh);
	D3DXMATRIX	GetTAnim(XMesh* mesh);
	void		SetObjAnim(XMesh* mesh);
	void		SetMatrix(D3DXMATRIX *matWorld, D3DXMATRIX *matView, D3DXMATRIX *matProj);
public:
	void		SelectObjTransparency(ID3D11DeviceContext* pContext);	// 알파값을 모두 0.7f로 통일
	void		SelectObjOpaque(ID3D11DeviceContext* pContext);			// 알파값을 모두 1.0f로 통일
public:
	bool		Init(TCHAR* filename, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, TCHAR* VSfile, TCHAR* PSfile);
	bool		PreRender(ID3D11DeviceContext*	pContext);
	bool		PostRender(ID3D11DeviceContext*	pContext);
	virtual bool Init() noexcept { return true; }
	virtual bool Frame(const float& spf, const float& accTime) noexcept;
	virtual bool Render(ID3D11DeviceContext*	pContext)	noexcept;
	virtual bool Release()	noexcept;
public:
	XViewer();
	~XViewer();
};

