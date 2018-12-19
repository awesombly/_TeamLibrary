#pragma once
//#include "AStd.h"
#include "ADxStd.h"


namespace DX
{
	//ComPtr<ID3D11ShaderResourceView> CreateSRV(ID3D11Device* pdevice, const LPCWSTR* pName);
	void		ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);

	ID3D11Buffer* CreateVertexBuffer(ID3D11Device*  m_pd3dDevice, void *vertices, UINT iNumVertex, UINT iVertexSize, bool bDynamic = false);
	ID3D11Buffer* CreateIndexBuffer(ID3D11Device*  m_pd3dDevice, void *indices, UINT iNumIndex, UINT iSize, bool bDynamic = false);

	ID3D11Buffer* CreateConstantBuffer(ID3D11Device*  m_pd3dDevice, void *data, UINT iNumIndex, UINT iSize, bool bDynamic = false);
	
	ID3D11GeometryShader*		LoadGeometryShaderFile(ID3D11Device*  m_pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pdevice, const TCHAR* pName);
	//ID3D11DepthStencilView*		CreateDepthStencilView(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight);
	ID3D11HullShader* LoadHullShaderFile(ID3D11Device*  m_pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, char *pFuntionName = 0, bool bBinary = false);
	ID3D11DomainShader*  LoadDomainShaderFile(ID3D11Device*  m_pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, char *pFuntionName = 0, bool bBinary = false);
	ID3D11ComputeShader*  LoadComputeShaderFile(ID3D11Device*  m_pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, char *pFuntionName = 0, bool bBinary = false);

	HRESULT				CompileShaderFromFile(TCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	ID3D11VertexShader* LoadVertexShaderFile(ComPtr<ID3D11Device>   m_pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char *pFuntionName = 0, bool bBinary = false);
	ID3D11PixelShader*  LoadPixelShaderFile(ComPtr<ID3D11Device>  m_pd3dDevice, const void* pLoadShaderFile, const char *pFuntionName = 0, bool bBinary = false, ID3DBlob** pRetBlob = nullptr);

	ID3D11InputLayout*	CreateInputlayout(ComPtr<ID3D11Device>  m_pd3dDevice, DWORD dwSize, LPCVOID lpData, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements);



	class ADxObject
	{
	public:
		//ID3D11ShaderResourceView*	g_pTextureSRV[];

		ComPtr<ID3D11Buffer>			g_pVertexBuffer;
		ComPtr<ID3D11Buffer>			g_pIndexBuffer;
		ComPtr<ID3D11Buffer>			g_pConstantBuffer;
		ComPtr<ID3D11VertexShader>		g_pVertexShader;
		ComPtr<ID3D11PixelShader>		g_pPixelShader;
		ComPtr<ID3D11GeometryShader>	g_pGeometryShader;
		ComPtr<ID3D11HullShader>		g_pHullShader;
		ComPtr<ID3D11DomainShader>		g_pDomainShader;
		ComPtr<ID3D11ComputeShader>		g_pComputeShader;


		ComPtr<ID3DBlob>				g_pVSBlob;
		ComPtr<ID3DBlob>				g_pGSBlob;
		ComPtr<ID3DBlob>				g_pPSBlob;
		ComPtr<ID3DBlob>				g_pHSBlob;
		ComPtr<ID3DBlob>				g_pDSBlob;
		ComPtr<ID3DBlob>				g_pCSBlob;

		ComPtr<ID3D11InputLayout>				g_pInputlayout;
		ComPtr<ID3D11ShaderResourceView>		g_pTextureSRV;

		ComPtr<ID3D11Buffer>					m_pVBInstance;
	public:
		UINT					m_iPrimitiveType;
		UINT					m_iCullMode;
		UINT					m_iSamplerMode;
		UINT					m_iNumVertex;
		UINT					m_iNumIndex;
		UINT					m_iVertexSize;
		UINT					m_iIndexSize;
		D3D11_BOX				m_BoxVB;
		D3D11_BOX				m_BoxIB;
		UINT					m_iBeginVB;
		UINT					m_iBeginIB;


	public:
		void PreRender(ComPtr<ID3D11DeviceContext>    pContext, UINT iVertexSize = 0);
		void PostRender(ComPtr<ID3D11DeviceContext>    pContext, UINT iCount = 0);
		bool Render(ComPtr<ID3D11DeviceContext>    pContext, UINT iVertexSize = 0, UINT iCount = 0);

	public:
		ADxObject();
		~ADxObject();

	};
}