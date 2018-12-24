#pragma once
#include "XShape.h"

class XDxRT
{
public:
	UINT								m_iViewPorts;
	D3D11_VIEWPORT						m_pOldVp;
	ID3D11RenderTargetView*				m_pOldRTV;
	ID3D11DepthStencilView*				m_pOldDSV;
	ComPtr<ID3D11RenderTargetView>		m_pRTV;
	ComPtr<ID3D11DepthStencilView>		m_pDSV;
	ComPtr<ID3D11ShaderResourceView>	m_pSRV;
	ComPtr<ID3D11ShaderResourceView>	m_pDsvSRV;
	ComPtr<ID3D11Texture2D>				m_pTexture;
	D3D11_VIEWPORT						m_vp;
	shared_ptr<XPlaneShape>				m_pPlaneShape;
	ID3D11Device*						m_pd3dDevice;
	ID3D11DeviceContext*				m_pContext;

public:
	HRESULT			SetViewPort(float fWidth, float fHeight);
	HRESULT			Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, float fWidth, float fHeight);
	bool			begin();//D3D11_VIEWPORT* vp, ID3D11RenderTargetView* rt, ID3D11DepthStencilView* ds);
	bool			end();
	bool			Render(ID3D11DeviceContext* pContext);
public:
	XDxRT();
	~XDxRT();
};

