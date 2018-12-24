#pragma once
#include "stdui.h"
namespace UI
{
	ID3D11Buffer * CreateIndexBuffer(ID3D11Device* device, void* pData, UINT iSize, UINT iNumCount, bool bDynamic = false);
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* device, void* pData, UINT iSize, UINT iNumCount, bool bDynamic = false);
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device* device, void* pData, UINT iSize, UINT iNumCount, bool bDynamic = false);
	ID3D11VertexShader* LoadVertexShader(ID3D11Device* device, const TCHAR* szLoadFileName, ID3DBlob** pBlob, const char* szShaderFunc = 0);
	ID3D11PixelShader* LoadPixelShader(ID3D11Device* device, const TCHAR* szLoadFileName, ID3DBlob** pBlob = 0, const char* szShaderFunc = 0);
	ID3D11InputLayout* CreateLayout(ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC* IED, int ElementCnt, ID3DBlob* pVSBlob);
	HRESULT CompileShader(const TCHAR* szLoadFileName, ID3DBlob** pBlob, const char* szShaderFunc, const char* szShaderModel);
}
