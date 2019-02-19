#pragma once
#include "DxManager.h"
#include "XStd.h"

namespace DX
{
	class XDxHelper
	{
	public:
		ID3D11Buffer* CreateBuffer(ID3D11Device*  pd3dDevice, void *pData, UINT iNumCount, UINT iSize, D3D11_BIND_FLAG bindFlag, bool bDynamic = false);
		ID3D11InputLayout* CreateInputlayout(ID3D11Device*  pd3dDevice, DWORD dwSize, LPCVOID lpData, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements);
		ID3D11VertexShader* CreateVertexShader(ID3D11Device*  pd3dDevice, TCHAR* szFileName, char* szFunctionName, ComPtr<ID3DBlob>* pVertexBuffer);
		ID3D11PixelShader* CreatePixelShader(ID3D11Device*  pd3dDevice, TCHAR* szFileName, char* szFunctionName, ComPtr<ID3DBlob>* pPixelBuffer);
	public:
		XDxHelper();
		~XDxHelper();
	};
}