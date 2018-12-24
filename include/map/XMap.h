#pragma once
#include "XShape.h"
#include "XMapImporter.h"

enum TexturePresence { Tex_Have = 0, Tex_None, Tex_Alpha };
struct ConstantBuffer_Light
{
	D3DXMATRIX	matInvWorld;		// 현재는 왜 사용하는지 의문
	D3DXVECTOR4	vDiffuseMaterial;
	D3DXVECTOR4	vAmbientMaterial;	// 어두운 부분을 기본적으로 밝게 하도록 주는 값
	D3DXVECTOR4 vSpecularMaterial;	// 기본 스펙큘러의 값
	D3DXVECTOR4	vDiffuseLightColor;
	D3DXVECTOR4	vAmbientLightColor;	
	D3DXVECTOR4 vSpecularLightColor;// 스펙큘러 라이트의 색
	D3DXVECTOR4 vLightDirection;	// 디렉션 라이트, w는 감쇠(일단은 사용하지 않음)
	D3DXVECTOR4 vSightDirection;	// 시선 벡터, w는 세기

};

struct XVertexLookUp
{
	set<DWORD> vListNormal;	// 공유되는 정점이 겹치면 안되기 때문에 set으로
};

class XMap :public XShape
{
public:
	DX::XDxHelper			m_Object;
	vector<float>			m_fHeightList;
	vector<D3DXVECTOR3>		m_vFaceNormals;
	vector<long>			m_iNormalLookUpTable; //정점 단위의 공유하는 페이스, DWORD에 XVertexLookUp를 넣도록 수정할 예정
	int						m_iRow;
	int						m_iCol;
	int						m_iVertexs;
	int						m_iFace;
	float					m_fDistance;
	float					m_fScaleHeight;
	ComPtr<ID3D11VertexShader>			m_pVS[3];
	ComPtr<ID3D11PixelShader>			m_pPS[3];
	map<int, ComPtr<ID3D11ShaderResourceView>>	m_AlphaSRV;
public:
	D3DXVECTOR3				m_vLook;
	ComPtr<ID3D11Buffer>	m_pLightConstantBuffer;
	ConstantBuffer_Light	m_LightData;
public:
	bool		CreateVertexList();	// Vertex,Index,Normal,Color 등은 툴 상에서 저장을 하고 파일로 불러오는게 맞다.
	bool		CreateIndexList();
	HRESULT		CreateHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TCHAR* HeightMap);
	float		GetHeight(float x, float z);
	float		GetHeightmap(int row, int col);
	float		Lerp(float fStart, float fEnd, float fTangent);
	bool		InitFaceNormals();				//페이스마다 노말벡터 계산해서 저장
	D3DXVECTOR3 ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2);
	bool		GetNormalLookUpTable();			//정점당 공유 FACE갯수 계산
	bool		CalcPerVertexNormalsFastLookUp();
	void		SetScaleHeight(float fScaleHeight = 1.0f);
	void		SetAlphaSRV(ID3D11ShaderResourceView* pSRV,int iColor);
public:
	void		SetLookVector(D3DXVECTOR3 vLook);
	void		InitLight();
	void		UpdateLight();
	bool		ImportData(ID3D11Device* pDevice, XMapImporter* pImporter);
	void		CreateBuffer(ID3D11Device* pDevice );
	void		CreateShader(ID3D11Device* pDevice, TCHAR* szMapShader, TCHAR* szOnlyColorShader, char* szVSFunctionName, char* szPSFunctionName);
public:
	virtual bool Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, float fDistance, TCHAR* szTexture, TCHAR* szHeightTexture, TCHAR* szMapShader, TCHAR* szOnlyColorShader, char* szVSFunctionName, char* szPSFunctionName);
	bool Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, float fCellCount, float fDistance, TCHAR* szTexture, TCHAR* szMapShader, TCHAR* szOnlyColorShader, char* szVSFunctionName, char* szPSFunctionName);
	bool Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, XMapImporter* pImporter, TCHAR* szMapShader, TCHAR* szOnlyColorShader, char* szVSFunctionName, char* szPSFunctionName);
	bool PreRender(ID3D11DeviceContext* pContext);
	bool PostRender(ID3D11DeviceContext* pContext);
	virtual bool Init() noexcept override;
	virtual bool Frame(const float& spf, const float& accTime) noexcept override;
	virtual bool Render(ID3D11DeviceContext* pContext) noexcept override;
	virtual bool Release() noexcept override;
public:
	XMap();
	virtual~XMap();
};
