#pragma once
#include "XDxHelper.h"
#include "XSRVManager.h"
#include "GameObject.h"
struct PC_Vertex
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;	
};

struct PNCT_Vertex			//position,normal,color,texture
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
};

struct ConstantBuffer_Matrix
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	//D3DXVECTOR4 vLight;
};

class XShape :public GameObject
{
public:
	DX::XDxHelper						m_Object;
	vector<PNCT_Vertex>					m_VertexList;
	vector<DWORD>						m_dwIndexList;
	ComPtr<ID3D11Buffer>				m_pVertexBuffer;
	ComPtr<ID3D11Buffer>				m_pIndexBuffer;
	ComPtr<ID3D11Buffer>				m_pConstantBuffer;
	ComPtr<ID3D11InputLayout>			m_pVertexLayout;
	ComPtr<ID3D11VertexShader>			m_pVS;
	ComPtr<ID3D11PixelShader>			m_pPS;
	ComPtr<ID3DBlob>					m_pVSBuf;
	ComPtr<ID3DBlob>					m_pPSBuf;
	ComPtr<ID3D11ShaderResourceView>	m_pTextureSRV;
	D3DXMATRIX							m_matWorld;
	D3DXMATRIX							m_matView;
	D3DXMATRIX							m_matProj;
	ConstantBuffer_Matrix				m_ConstantDataMatrix;
	int									m_iTopology;
	X_Box								m_Box;
public:
	void SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);
	void SetMatrix(D3DXMATRIX *matWorld, D3DXMATRIX *matView, D3DXMATRIX *matProj);
	virtual void SetVertex() {}
	virtual void SetIndex() {}
	D3DXMATRIX GetWorldMatrix() { return m_matWorld; }
	D3DXMATRIX GetViewMatrix() { return m_matView; }
	D3DXMATRIX GetProjMatrix() { return m_matProj; }
public:
	virtual bool Init() noexcept { return true; }
	virtual bool Frame(const float& spf, const float& accTime) noexcept { return true; }
	virtual bool Render(ID3D11DeviceContext* pContext) noexcept { return true; }
	virtual bool Release() noexcept { return true; }
public:
	XShape();
	virtual ~XShape();
};

class XLineShape :public XShape
{
public:
	vector<PC_Vertex>					m_VertexList_Line;
public:
	virtual void SetVertex();
public:
	bool Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor);
public:
	bool Create(TCHAR* szVertexShaderFile, TCHAR* szPixelShaderFile, char* szVSFunctionName, char* szPSFunctionName);
	virtual bool Init() noexcept;
	virtual bool Frame(const float& spf, const float& accTime) noexcept;
	virtual bool Render(ID3D11DeviceContext* pContext) noexcept;
	virtual bool Release() noexcept;
public:
	XLineShape();
	virtual ~XLineShape();
};

class XBoxShape :public XShape
{
public:
	virtual void SetVertex();
	virtual void SetIndex();
public:
	bool Create(TCHAR* szVertexShaderFile, TCHAR* szPixelShaderFile, char* szVSFunctionName, char* szPSFunctionName);
	virtual bool Init() noexcept;
	virtual bool Frame(const float& spf, const float& accTime) noexcept;
	virtual bool Render(ID3D11DeviceContext* pContext) noexcept;
	virtual bool Release() noexcept;
public:
	XBoxShape();
	virtual ~XBoxShape();
};

class XPlaneShape :public XShape
{
public:
	virtual void SetVertex();
	virtual void SetIndex();
public:
	bool Create(TCHAR* szTexture, TCHAR* szVertexShaderFile, TCHAR* szPixelShaderFile, char* szVSFunctionName, char* szPSFunctionName);
	virtual bool Init() noexcept;
	virtual bool Frame(const float& spf, const float& accTime) noexcept;
	virtual bool Render(ID3D11DeviceContext* pContext) noexcept;
	virtual bool Release() noexcept;
public:
	XPlaneShape();
	virtual ~XPlaneShape();
};
