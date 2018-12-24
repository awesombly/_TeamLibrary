#pragma once
#include "XShape.h"

class XSkyBox :public XBoxShape
{
public:
	D3DXVECTOR3						 m_vCameraPos;
	D3DXVECTOR3						 m_vTargetPos;
	D3DXVECTOR3						 m_vUp;
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV[6];
public:
	HRESULT	LoadTextures(ID3D11Device* pDevice);

	virtual bool Init() noexcept { return true; }
	virtual bool Frame(const float& spf, const float& accTime) noexcept { return true; }
	virtual bool Render(ID3D11DeviceContext* pContext) noexcept;
	virtual bool Release() noexcept { return true; }
public:
	XSkyBox();
	virtual ~XSkyBox();
};

