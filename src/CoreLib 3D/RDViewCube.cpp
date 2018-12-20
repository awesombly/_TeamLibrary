//#include "RDViewCube.h"
//#include "ObjectManager.h"
//#include <wrl.h>

//
//HRESULT RDViewCube::CreateResourceRTView(ID3D11DeviceContext* pDContext, IDXGISwapChain*& pSwapChain, const DXGI_SWAP_CHAIN_DESC& swapDesc) noexcept
//{
//	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
//	// ·£´õ Å¸°Ù »ý¼º
//	if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer)) ||
//		FAILED(DxManager::GetDevice()->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pRTView)))
//		return E_FAIL;
//	// ÅØ½ºÃÄ ·£´õ Å¸°Ù ¼Ó¼º
//	m_TexDesc = { 0, };
//	m_TexDesc.Width = swapDesc.BufferDesc.Width;// Window::getClientRect().right;
//	m_TexDesc.Height = swapDesc.BufferDesc.Height;// Window::getClientRect().bottom;
//	m_TexDesc.MipLevels = 1;
//	m_TexDesc.ArraySize = 6;
//	m_TexDesc.SampleDesc.Count = 1;
//	m_TexDesc.SampleDesc.Quality = 0;
//	m_TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	m_TexDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
//	m_TexDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
//	m_TexDesc.CPUAccessFlags = 0;
//	if (FAILED(DxManager::GetDevice()->CreateTexture2D(&m_TexDesc, nullptr, &m_pTexTextures[0])))
//		return E_FAIL;
//	// ½¦ÀÌ´õ ¸®¼Ò½º ºä ¼Ó¼º
//	D3D11_SHADER_RESOURCE_VIEW_DESC SRViewDesc = { 0, };
//	SRViewDesc.Format = m_TexDesc.Format;
//	SRViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
//	SRViewDesc.Texture2DArray.MipLevels = 1;
//	if(FAILED(DxManager::GetDevice()->CreateShaderResourceView(m_pTexTextures[0], &SRViewDesc, &m_pTexSRViews[0])))
//		return E_FAIL;
//	if (FAILED(DxManager::GetDevice()->CreateShaderResourceView(m_pTexTextures[1], &SRViewDesc, &m_pTexSRViews[1])))
//		return E_FAIL;
//	// ·£´õÅ¸ÄÏ¿ë ºä ¼Ó¼º
//	D3D11_RENDER_TARGET_VIEW_DESC RTViewDesc = { 0, };
//	RTViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	RTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
//	RTViewDesc.Texture2DArray.ArraySize = 6;
//	if (FAILED(DxManager::GetDevice()->CreateRenderTargetView(m_pTexTextures[0], &RTViewDesc, &m_pTexRTViews[0])) ||
//		FAILED(DxManager::GetDevice()->CreateRenderTargetView(m_pTexTextures[1], &RTViewDesc, &m_pTexRTViews[1])))
//	{
//		return E_FAIL;
//	}
//
//	// ·»Å¸ºä, ±íÀÌ ½ºÅÙ½Ç ºä ¼³Á¤
//	pDContext->OMSetRenderTargets(1, m_pTexRTViews, m_pDepthSView);
//	return S_OK;
//}
//
//HRESULT RDViewCube::CreateDSView(const DXGI_SWAP_CHAIN_DESC& swapDesc) noexcept
//{
//	// ÅØ½ºÃÄ »ý¼º
//	ID3D11Texture2D* pDSTexture = nullptr;
//	D3D11_TEXTURE2D_DESC depthDesc = { 0, };
//	depthDesc.Width = swapDesc.BufferDesc.Width;
//	depthDesc.Height = swapDesc.BufferDesc.Height;
//	depthDesc.MipLevels = 1;
//	depthDesc.ArraySize = 6;
//	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
//	depthDesc.SampleDesc.Count = 1;
//	depthDesc.SampleDesc.Quality = 0;
//	depthDesc.Usage = D3D11_USAGE_DEFAULT;
//	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		// »ý¼º Å¸ÀÔ
//	depthDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
//	depthDesc.CPUAccessFlags = 0;
//	if (FAILED(DxManager::GetDevice()->CreateTexture2D(&depthDesc, NULL, &pDSTexture)))
//		return E_FAIL;
//	// ±íÀÌ ½ºÅÙ½Ç ºä »ý¼º
//	D3D11_DEPTH_STENCIL_VIEW_DESC DSViewDesc = { 0, };
//	DSViewDesc.Format = depthDesc.Format;
//	DSViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
//	DSViewDesc.Texture2DArray.ArraySize = 6;
//	DSViewDesc.Texture2D.MipSlice = 0;
//
//	return DxManager::GetDevice()->CreateDepthStencilView(pDSTexture, &DSViewDesc, &m_pDepthSView);
//	swapDesc;
//}
