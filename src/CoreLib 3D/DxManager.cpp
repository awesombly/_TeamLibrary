#include "DxManager.h"
#include "WriteManager.h"
#include "Window.h"
#include "Texture.h"
#include <wrl.h>						// ComPtr
#include "../../data/shader/ShaderData.hlsl"

bool DxManager::Init() noexcept
{
	// DX �ʱ�ȭ
	if (FAILED(CreateDevice()) ||
		FAILED(CreateGIFactory()) ||
		FAILED(CreateSwapChain()) )
	{
		ErrorMessage(__FUNCTION__ + " -> DX �ʱ�ȭ ����!"s);
		return false;
	}
	// ���̴� �ε�
	InitLoadShader();

	// ���µ� ����
	if (FAILED(CreateRasterizerState()) ||
		FAILED(CreateBlendState()) ||
		FAILED(CreateDepthStencilState()))
	{
		ErrorMessage(__FUNCTION__ + " -> ���� ��ü ���� ����!"s);
		return false;
	}
	// ���µ� �⺻�� ����
	SetRasterizerState(ERasterS::Basic);
	SetBlendState(EBlendS::Basic);
	SetDepthStencilState(EDepthS::Basic);
	SetSamplerState(0, ESamTextureS::Basic, ESamFilterS::Basic);

	// Writer �ʱ�ȭ
	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&pBackBuffer);
	if (!WriteManager::GetInstance().Init(pBackBuffer))
	{
		ErrorMessage(__FUNCTION__ + " -> Write Init Failed!"s);
		return false;
	}

	// ȭ�� ���� ����
	if (FAILED(m_pGIFactory->MakeWindowAssociation(Window::m_hWnd, 
				DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER)))
	{
		return false;
	}
	
	return true;
}

void DxManager::InitLoadShader() noexcept
{
	LoadVertexShader(L"../../data/shader/Final.hlsl", "VS_Final");
	LoadPixelShader(L"../../data/shader/Final.hlsl", "PS_Final");
	// ���̸� ������
	LoadVertexShader(L"../../data/shader/ShadowMap.hlsl", "VS_Shadow");
	LoadVertexShader(L"../../data/shader/ShadowMap.hlsl", "VS_DepthMap");
	LoadVertexShader(L"../../data/shader/ShadowMap.hlsl", "VS_DepthMapPNCT");
	LoadPixelShader(L"../../data/shader/ShadowMap.hlsl", "PS_Shadow");
	LoadPixelShader(L"../../data/shader/ShadowMap.hlsl", "PS_DepthMap");
	LoadPixelShader(L"../../data/shader/ShadowMap.hlsl", "PS_NO_CMP");

	LoadPixelShader(L"../../data/shader/ShadowMap.hlsl", "PS_PlaneShadow");
	// Multi Render Target 
	LoadVertexShader(L"../../data/shader/RenderTarget.hlsl", "VS_MRT");
	LoadPixelShader(L"../../data/shader/RenderTarget.hlsl", "PS_MRT");
	LoadPixelShader(L"../../data/shader/RenderTarget.hlsl", "PS_MRT_None");
	LoadPixelShader(L"../../data/shader/RenderTarget.hlsl", "PS_MRT_Normal");
	// ���� ���̴� �ε�
	LoadVertexShader(L"../../data/shader/Shader.hlsl", "VS_Basic");
	LoadVertexShader(L"../../data/shader/Shader.hlsl", "VS_Light");
	LoadVertexShader(L"../../data/shader/Shader.hlsl", "VS_PC");
	// �ȼ� ���̴� �ε�
	LoadPixelShader(L"../../data/shader/Shader.hlsl", "PS_Basic");
	LoadPixelShader(L"../../data/shader/Shader.hlsl", "PS_PC");
	LoadPixelShader(L"../../data/shader/Shader.hlsl", "PS_Frustum");
	// ��ü ť���
	LoadVertexShader(L"../../data/shader/CubeMap.hlsl", "VS_Sky");
	LoadPixelShader(L"../../data/shader/CubeMap.hlsl", "PS_Sky");
	// �ν��Ͻ�
	LoadVertexShader(L"../../data/shader/Instance.hlsl", "VS_Particle");
	LoadVertexShader(L"../../data/shader/Instance.hlsl", "VS_Instance");
	LoadPixelShader(L"../../data/shader/Instance.hlsl", "PS_Instance");
	LoadPixelShader(L"../../data/shader/Instance.hlsl", "PS_SingleInstance");
	// �븻��
	LoadVertexShader(L"../../data/shader/NormalMap.hlsl", "VS_Normal");
	LoadPixelShader(L"../../data/shader/NormalMap.hlsl", "PS_Normal");
	// ȯ�����
	LoadVertexShader(L"../../data/shader/EnviMap.hlsl", "VS_Envi");
	LoadPixelShader(L"../../data/shader/EnviMap.hlsl", "PS_Envi");
	LoadPixelShader(L"../../data/shader/EnviMap.hlsl", "PS_Fresnel");
	LoadPixelShader(L"../../data/shader/EnviMap.hlsl", "PS_Refraction");
	LoadVertexShader(L"../../data/shader/EnviMap.hlsl", "VS_EnviDy");
	LoadPixelShader(L"../../data/shader/EnviMap.hlsl", "PS_EnviDy");
	LoadGeometryShader(L"../../data/shader/EnviMap.hlsl", "GS_EnviDy");
	// �ܰ��� ���
	LoadVertexShader(L"../../data/shader/OutLine.hlsl", "VS_OutLine");
	LoadPixelShader(L"../../data/shader/OutLine.hlsl", "PS_OutLine");
	// ���� ���̴� �ε�
	LoadGeometryShader(L"../../data/shader/CubeMap.hlsl", "GS_SO");
	// ��Ʈ�� �ƿ��� ���̴�
	LoadVertexShader(L"../../data/shader/CubeMap.hlsl", "VS_SO");
}

bool DxManager::InitRTView() noexcept
{
	// ���� Ÿ��, ����Ʈ ����
	m_RTDSView.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc);
	m_RTDSViewShadow.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc, true);
	//m_RTDSViewCube.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc);
	CreateViewPort(EViewPort::Main, 0, 0, (float)m_SwapChainDesc.BufferDesc.Width, (float)m_SwapChainDesc.BufferDesc.Height);
	CreateViewPort(EViewPort::MiniMap,
				   Window::getClientRect().right * 0.03f, Window::getClientRect().bottom * 0.03f,
				   Window::getClientRect().right * 0.3f, Window::getClientRect().bottom * 0.3f);
	m_RTDSView.ClearView(m_pD3dContext);
	SetViewPort(EViewPort::Main);
	return true;
}

bool DxManager::Frame() noexcept
{
	// ȭ�� ���� ���� ����
	if (Input::GetKeyState(VK_F1) == EKeyState::DOWN)
	{
		m_RasterList[ERasterS::Current] = m_RasterList[ERasterS::Wireframe];
	}
	else if (Input::GetKeyState(VK_F2) == EKeyState::DOWN)
	{
		m_RasterList[ERasterS::Current] = m_RasterList[ERasterS::Basic];
		m_BlenderList[EBlendS::Current] = m_BlenderList[EBlendS::NoAlpha];
		m_RTDSView.m_pScreen->SetPixelShader("PS_MRT_Normal");
	}
	else if (Input::GetKeyState(VK_F3) == EKeyState::DOWN)
	{
		m_RasterList[ERasterS::Current] = m_RasterList[ERasterS::Basic];
#ifdef Deferred
		m_BlenderList[EBlendS::Current] = m_BlenderList[EBlendS::NoAlpha];
#else
		m_BlenderList[EBlendS::Current] = m_BlenderList[EBlendS::Basic];
#endif
		m_DepthList[EDepthS::Current] = m_DepthList[EDepthS::Basic];
		m_RTDSView.m_pScreen->SetPixelShader("PS_MRT_None");
	}
	else if (Input::GetKeyState(VK_F4) == EKeyState::DOWN)
	{
		m_RasterList[ERasterS::Current] = m_RasterList[ERasterS::Basic];
		m_BlenderList[EBlendS::Current] = m_BlenderList[EBlendS::Basic];
		m_DepthList[EDepthS::Current] = m_DepthList[EDepthS::Basic];
		m_RTDSView.m_pScreen->SetPixelShader("PS_MRT");
	}

#pragma region StateKeyCheck
	// �����Ͷ����� ���� ����
	if (Input::GetInstance().GetKeyState(VK_F5) == EKeyState::DOWN)
	{
		auto iter = std::find_if(m_RasterList.rbegin(), m_RasterList.rend(),
						 [&](auto iter) {
					return iter.second == m_RasterList[ERasterS::Current];
		});
		// ������ ���Ҷ�� �⺻������, �ƴ� ��������
		if (iter == m_RasterList.rbegin())
			m_RasterList[ERasterS::Current] = m_RasterList[ERasterS::Basic];
		else
			m_RasterList[ERasterS::Current] = (--iter)->second;
		SetRasterizerState(ERasterS::Current);
	}
	// ���� ���� ����
	if (Input::GetInstance().GetKeyState(VK_F6) == EKeyState::DOWN)
	{
		auto iter = std::find_if(m_BlenderList.rbegin(), m_BlenderList.rend(),
						 [&](auto iter) {
					return iter.second == m_BlenderList[EBlendS::Current];
		});
		// ������ ���Ҷ�� �⺻������, �ƴ� ��������
		if (iter == m_BlenderList.rbegin())
			m_BlenderList[EBlendS::Current] = m_BlenderList[EBlendS::Basic];
		else
			m_BlenderList[EBlendS::Current] = (--iter)->second;
		SetBlendState(EBlendS::Current);
	}
	// ����-���ٽ� ���� ����
	if (Input::GetInstance().GetKeyState(VK_F7) == EKeyState::DOWN)
	{
		auto iter = std::find_if(m_DepthList.rbegin(), m_DepthList.rend(),
								 [&](auto iter) {
			return iter.second == m_DepthList[EDepthS::Current];
		});
		// ������ ���Ҷ�� �⺻������, �ƴ� ��������
		if (iter == m_DepthList.rbegin())
			m_DepthList[EDepthS::Current] = m_DepthList[EDepthS::Basic];
		else
			m_DepthList[EDepthS::Current] = (--iter)->second;
		SetDepthStencilState(EDepthS::Current);
	}
	// ���÷� ���� ���� (F8, F9)
	m_SamplerState.Frame(m_pD3dContext);
#pragma endregion
	return true;
}

bool DxManager::PrevRender() noexcept
{
	// RenderTarget ����
	//SetViewPort();
	WriteManager::Get().Begin();
	return true;
}

bool DxManager::PostRender() noexcept
{
	m_RTDSView.DrawView(m_pD3dContext);
	WriteManager::Get().End();
	//  ����� ���� Ŭ����
	m_pSwapChain->Present(0, 0);					
	// ����, ���� �� Ŭ����
	m_RTDSView.ClearView(m_pD3dContext);
	return true;
}

bool DxManager::Render() noexcept
{
	return true;
}

bool DxManager::Release() noexcept
{
	// DX ����
	m_pSwapChain->Release();
	m_pGIFactory->Release();
	m_pD3dContext->Release();
	m_pD3dDevice->Release();

	WriteManager::Get().Release();
	return true;
}

void DxManager::CreateViewPort(const EViewPort& viewEnum, const float& x, const float& y, const float& width, const float& height)
{
	D3D11_VIEWPORT viewPort;
	// ����Ʈ �Ӽ� ����   
	viewPort.TopLeftX = x;				// ���� ���� ����
	viewPort.TopLeftY = y;				// ���� ���� ����
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	m_ViewPortList[viewEnum] = std::move(viewPort);
}

void DxManager::SetViewPort(const EViewPort& viewEnum)
{
	m_pD3dContext->RSSetViewports(1, &m_ViewPortList[viewEnum]);
}

void DxManager::ClearDepthStencilView()
{
	m_pD3dContext->ClearDepthStencilView(m_RTDSView.m_pDepthSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

Texture* DxManager::GetTexture(const wstring_view& textureName, const bool& isFullPath)
{
	// ������ ���ҽ� �ε�
	auto iter = m_TextureList.find(textureName.data());
	if (iter == m_TextureList.end())
		return LoadShaderResourceView(textureName, isFullPath);
	if (iter->second == nullptr)
	{
		ErrorMessage(__FUNCTION__ + " -> : Texture is Null"s);
		return nullptr;
	}
	return iter->second;
}

Texture* DxManager::LoadShaderResourceView(const filesystem::path& textureName, const bool& isFullPath)
{
	Texture* pTexture = nullptr;
	if (isFullPath)
	{
		pTexture = (new Texture())->CreateShaderResourceView(textureName.c_str());
	}
	else
	{
		pTexture = (new Texture())->CreateShaderResourceView(L"../../data/texture/"s + textureName.c_str());
	}
	if (pTexture == nullptr)
		return nullptr;
	else
		return m_TextureList[textureName.filename()] = pTexture;
}

HRESULT DxManager::LoadVertexShader(const wstring_view& loadUrl, const string_view& funcName)
{
	// ���� ���̴� ����
	ID3D11VertexShader* pVShader = nullptr;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* pOutputBuf = nullptr;
	ID3DBlob* pErrorBuf  = nullptr;
	hr = D3DX11CompileFromFile(loadUrl.data(), nullptr, nullptr, funcName.data(), "vs_5_0",
							   shaderFlags, 0, nullptr, &pOutputBuf, &pErrorBuf, nullptr);
	if (FAILED(hr))
	{
		ErrorMessage(__FUNCTION__ + " -> : Shader Load Error : "s + funcName.data() , true);
		return E_FAIL;
	}
	m_pD3dDevice->CreateVertexShader(pOutputBuf->GetBufferPointer(), pOutputBuf->GetBufferSize(), nullptr, &pVShader);
	m_VShaderList[funcName.data()] = pVShader;

	if (FAILED(LoadInputLayout(pOutputBuf, funcName)))
	{
		ErrorMessage(__FUNCTION__ + " -> ���̾ƿ� ���� ���� : "s + funcName.data() , true);
		return E_FAIL;
	}

	if (pOutputBuf != nullptr)
		pOutputBuf->Release();
	if (pErrorBuf != nullptr)
		pErrorBuf->Release();
	return hr;
}

HRESULT DxManager::LoadPixelShader(const wstring_view& loadUrl, const string_view& funcName)
{
	// �ȼ� ���̴� ����
	ID3D11PixelShader* pPShader;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* pOutputBuf = nullptr;
	ID3DBlob* pErrorBuf = nullptr;
	hr = D3DX11CompileFromFile(loadUrl.data(), nullptr, nullptr, funcName.data(), "ps_5_0",
							   shaderFlags, 0, nullptr, &pOutputBuf, &pErrorBuf, nullptr);
	if (FAILED(hr))
	{
		ErrorMessage(__FUNCTION__ + " -> Shader Load Error!"s, true);
		return E_FAIL;
	}
	m_pD3dDevice->CreatePixelShader(pOutputBuf->GetBufferPointer(), pOutputBuf->GetBufferSize(), nullptr, &pPShader);
	m_PShaderList[funcName.data()] = pPShader;

	if (pOutputBuf != nullptr)
		pOutputBuf->Release();
	if (pErrorBuf != nullptr)
		pErrorBuf->Release();
	return hr;
}

HRESULT DxManager::LoadGeometryShader(const wstring_view& loadUrl, const string_view& funcName)
{
	// ���� ���̴� ����
	ID3D11GeometryShader* pGShader = nullptr;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* pOutputBuf = nullptr;
	ID3DBlob* pErrorBuf = nullptr;
	hr = D3DX11CompileFromFile(loadUrl.data(), nullptr, nullptr, funcName.data(), "gs_5_0",
							   shaderFlags, 0, nullptr, &pOutputBuf, &pErrorBuf, nullptr);
	if (FAILED(hr))
	{
		ErrorMessage(__FUNCTION__ + " -> Shader Load Error : "s + funcName.data(), true);
		return E_FAIL;
	}
	m_pD3dDevice->CreateGeometryShader(pOutputBuf->GetBufferPointer(), pOutputBuf->GetBufferSize(), nullptr, &pGShader);
	m_GShaderList[funcName.data()] = pGShader;

	if (pOutputBuf != nullptr)
		pOutputBuf->Release();
	if (pErrorBuf != nullptr)
		pErrorBuf->Release();
	return hr;
}

HRESULT DxManager::LoadInputLayout(ID3DBlob* pVertexBuf, const string_view& funcName)
{
	ID3D11InputLayout* pInputLayout = nullptr;
	if (funcName._Equal("VS_PC"))
	{
		const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			//{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			// �ṉ̀���,     , Ÿ��,                       ���۹���Ʈ, �Է�Ÿ��
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		if (FAILED(m_pD3dDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(layout[0]), pVertexBuf->GetBufferPointer(), pVertexBuf->GetBufferSize(), &pInputLayout)))
			return E_FAIL;
	}
	else if (funcName._Equal("VS_Instance") || funcName._Equal("VS_Particle"))
	{
		const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			//{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			// �ṉ̀���,     , Ÿ��,                       ���۹���Ʈ, �Է�Ÿ��
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA,   0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 24, D3D11_INPUT_PER_VERTEX_DATA,   0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D11_INPUT_PER_VERTEX_DATA,   0 },
			{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 48, D3D11_INPUT_PER_VERTEX_DATA,   0 },

			{ "mTransform",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "mTransform",	1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "mTransform",	2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "mTransform",	3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXTURE",	0, DXGI_FORMAT_R32_FLOAT,			1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "InsCOLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 68, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		};
		if (FAILED(m_pD3dDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(layout[0]), pVertexBuf->GetBufferPointer(), pVertexBuf->GetBufferSize(), &pInputLayout)))
			return E_FAIL;
	}
	else
	{
		const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			//{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			// �ṉ̀���,     , Ÿ��,                       ���۹���Ʈ, �Է�Ÿ��
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		if (FAILED(m_pD3dDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(layout[0]), pVertexBuf->GetBufferPointer(), pVertexBuf->GetBufferSize(), &pInputLayout)))
			return E_FAIL;
	}
	m_LayoutList[funcName.data()] = pInputLayout;
	return S_OK;
}


HRESULT DxManager:: CreateRasterizerState()
{
	for (int i = 1; i < (int)ERasterS::size; ++i)
		m_RasterList.try_emplace((ERasterS)i, new RasterizerState());

	// ������ �� ������Ƽ��, �����Ͷ����� Ÿ�� ����(��, ��, �ﰢ��, �Ѹ� �Ӽ�)
	m_RasterList[ERasterS::Basic]->SetRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID);
	m_RasterList[ERasterS::Wireframe]->SetRasterizerState(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME);
	m_RasterList[ERasterS::CullFront]->SetRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID);
	m_RasterList[ERasterS::CullBack]->SetRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID);
	m_RasterList[ERasterS::Line]->SetRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	D3D11_RASTERIZER_DESC RSDesc = { 0, };
	RSDesc.DepthClipEnable = true;
	RSDesc.FillMode = D3D11_FILL_SOLID;
	RSDesc.CullMode = D3D11_CULL_BACK;
	RSDesc.DepthBias = 6;
	RSDesc.DepthBiasClamp = 0.0f;
	RSDesc.SlopeScaledDepthBias = 1.0f;
	m_RasterList[ERasterS::DepthBias]->SetRasterizerState(RSDesc);
	m_RasterList[ERasterS::Current] = m_RasterList[ERasterS::Basic];
	return S_OK;
}

HRESULT DxManager::CreateBlendState()
{
	D3D11_BLEND_DESC blendDesc = { 0, };
	// ���� ����
	blendDesc.AlphaToCoverageEnable = false;
	// 8�� �迭�� ����� 0���� ���
	blendDesc.RenderTarget[0].BlendEnable = true;
	// Src : ���, Desc : �ѷ��� ��ġ, Op : ������
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	// ���İ�
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	// ������ �÷� RGB
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pD3dDevice->CreateBlendState(&blendDesc, &m_BlenderList[EBlendS::Basic]);
	// =============================================================================
	//blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED;
	//m_pD3dDevice->CreateBlendState(&blendDesc, &m_BlenderList[EBlendS::Red]);
	//blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_GREEN;
	//m_pD3dDevice->CreateBlendState(&blendDesc, &m_BlenderList[EBlendS::Green]);
	//blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_BLUE;
	//m_pD3dDevice->CreateBlendState(&blendDesc, &m_BlenderList[EBlendS::Blue]);

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	m_pD3dDevice->CreateBlendState(&blendDesc, &m_BlenderList[EBlendS::NoAlpha]);
	
	m_BlenderList[EBlendS::Current] = m_BlenderList[EBlendS::Basic];
	return S_OK;
}

HRESULT DxManager::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC dsDescDepth = { 0, };
	// ���̹��� ���°� ����
	dsDescDepth.DepthEnable = TRUE;
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;			// ���� �� ����
	// ���ٽ� ���� ����
	dsDescDepth.StencilEnable = FALSE;
	dsDescDepth.StencilReadMask = 1;// 0xff;
	dsDescDepth.StencilWriteMask = 1;// 0xff;
	// ����-�ĸ� ���̽� �Ӽ�, ���̽� �븻���Ͱ� ī�޶� ���� ������ ��
	dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	dsDescDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	// ���̴� ���� �Ӽ��� ���� ����
	dsDescDepth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDescDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescDepth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::Basic]);
	dsDescDepth.StencilEnable = true;
	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::D_Less_S_Always]);
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDescDepth.StencilEnable = false;
	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::D_Less_NoWrite]);
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDescDepth.DepthFunc = D3D11_COMPARISON_ALWAYS;
	dsDescDepth.StencilEnable = false;
	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::D_Always_S_Off]);
	dsDescDepth.DepthEnable = false;
	dsDescDepth.StencilEnable = true;
	dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::D_Off_S_Less]);
	dsDescDepth.StencilEnable = false;
	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::D_Off_S_Off]);
	dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pD3dDevice->CreateDepthStencilState(&dsDescDepth, &m_DepthList[EDepthS::D_Off_NoWrite]);

	m_DepthList[EDepthS::Current] = m_DepthList[EDepthS::Basic];
	return S_OK;
}


void DxManager::SetRasterizerState(const ERasterS& rasterEnum)
{
	m_RasterList[rasterEnum]->SetState(m_pD3dContext);
}
void DxManager::SetBlendState(const EBlendS& blendEnum)
{
	m_pD3dContext->OMSetBlendState(m_BlenderList[blendEnum], nullptr, (UINT)-1);
}
void DxManager::SetDepthStencilState(const EDepthS& depthEnum, const UINT& stencilRef)
{
	m_pD3dContext->OMSetDepthStencilState(m_DepthList[depthEnum], stencilRef);
}
void DxManager::SetSamplerState(const UINT& slotIndex, const ESamTextureS& textureEnum, const ESamFilterS& filterEnum, const UINT& maxAnisotropy, const D3D11_COMPARISON_FUNC& compFunc)
{
	m_SamplerState.SetSate(m_pD3dContext, slotIndex, textureEnum, filterEnum, maxAnisotropy, compFunc);
}

HRESULT	DxManager::ResizeDevice(const int& width, const int& height)
{
	if (m_pD3dDevice == nullptr)	return E_FAIL;

	WriteManager::Get().ReleaseDeviceResource();
	// ����Ÿ�ٺ� �ʱ�ȭ, ����
	m_pD3dContext->OMSetRenderTargets(0, nullptr, nullptr);
	m_pD3dContext->OMSetDepthStencilState(nullptr, 0);
	if (m_RTDSView.m_pRTView != nullptr)	
		m_RTDSView.m_pRTView->Release();
	if (m_RTDSView.m_pDepthSView != nullptr)
		m_RTDSView.m_pDepthSView->Release();
	///if (m_RTDSViewCube.m_pRTView != nullptr)
	///	m_RTDSViewCube.m_pRTView->Release();
	///if (m_RTDSViewCube.m_pDepthSView != nullptr)
	///	m_RTDSViewCube.m_pDepthSView->Release();
	// ����� ũ�� ����
	m_SwapChainDesc.BufferDesc.Width = width;
	m_SwapChainDesc.BufferDesc.Height = height;
	m_SwapChainDesc.Windowed = false;
	m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, width, height,
								m_SwapChainDesc.BufferDesc.Format, m_SwapChainDesc.Flags);
	// ����Ÿ�ٺ� ����, ����Ʈ ����
	m_RTDSView.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc);
	///m_RTDSViewCube.Create(m_pD3dContext, m_pSwapChain, m_SwapChainDesc);
	SetViewPort(EViewPort::Main);

	//GetClientRect(Window::m_hWnd, &Window::getClientRect());
	Window::ResizeWindow({0, 0, width, height});

	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackBuffer);
	WriteManager::Get().CreateDeviceResource(pBackBuffer);
	pBackBuffer->Release();

	return S_OK;
}

// Dx �ʱ� ����
HRESULT DxManager::CreateDevice()
{
	D3D_FEATURE_LEVEL pInFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	// Alt + Enter �������� ù��° ���� ä���į
	//	����̽� ����(�����:�׷��� ����̹�, ����̽� ����̹� Ÿ��, ����Ʈ���� ���, ����̽� ���� �÷���
	return D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
							 // ��� �� ����̹� ����, ���� ����, ���� SDK ����,
							 &pInFeatureLevel, 1, D3D11_SDK_VERSION,
							 // ��ȯ ���� Device �������̽�, ��ȯ ���� ����̹� ����
							 &m_pD3dDevice, nullptr,
							 // ��ȯ ���� DeviceContext �������̽�
							 &m_pD3dContext);
}

HRESULT DxManager::CreateGIFactory()
{
	// GIFactory ����(DXGI) : GPU �� ���� ���� ������ ������ �����ϴ� ���� �������̽�?
	// �ش� ��ü�� ���� GUID ��ȣ�� ��ȯ���ְ�, �������̽��� �ش� ��ȣ�� ���� ��ü�� ��ȯ����

	// ����̽� �������̽� ���, �������̽����� ������ ���, �����Ϳ��� GI���丮 ����?
	Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice;
	if (FAILED(m_pD3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice) ))
		return E_FAIL;

	Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter;
	if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter)) ||
		FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_pGIFactory)) ||
		FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pGIFactory)))
	{
		ErrorMessage(__FUNCTION__ + " -> InitError !"s);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT DxManager::CreateSwapChain()
{
	if (m_pGIFactory == nullptr)
		return E_FAIL;

	// ���� ü�� ����
	m_SwapChainDesc = { 0, };
	m_SwapChainDesc.BufferCount = 1;									// ����� ����?
	m_SwapChainDesc.OutputWindow = Window::m_hWnd;						// ������ ������ �ڵ�
	m_SwapChainDesc.Windowed = true;									// �Ϲ�=true, Ǯ ��ũ��=false
	// �� ���� ����
	m_SwapChainDesc.BufferDesc.Width = Window::getClientRect().right;	// �ػ� �ʺ�
	m_SwapChainDesc.BufferDesc.Height = Window::getClientRect().bottom;	// �ػ� ����
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;				// �����(�ֻ���)
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;				// ���� / �и�
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// ����� ����ȼ� ����(Unsigned Normalize)
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// ǥ�� �� ���ҽ� ���� �ɼ�, ���� ��� �뵵
	// ��Ƽ ���ø�
	m_SwapChainDesc.SampleDesc.Count = 1;								// �ȼ� ���� ��Ƽ ���ø� ��(1=X),  ex)��Ƽ �ٸ����
	//SwapDesc.SampleDesc.Quality = 0;									// �̹��� ǰ�� ����

	//SwapDesc.BufferDesc.ScanlineOrdering;								// �ֻ缱 ������ ��� ����
	//SwapDesc.BufferDesc.Scaling;										// ũ�� ���� ���
	m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;			// ������ �ø���(Present)�� ����� ���� ����
	m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// ���� ü���� ���� ����(��ü ȭ��� �ػ� ����x?)

	return m_pGIFactory->CreateSwapChain(m_pD3dDevice, &m_SwapChainDesc, &m_pSwapChain);
}

//HRESULT DxManager::SetViewPort()
//{
//	// ����Ʈ �Ӽ� ����   
//	m_ViewPort.TopLeftX = 0;				// ���� ���� ����
//	m_ViewPort.TopLeftY = 0;				// ���� ���� ����
//	m_ViewPort.Width = (float)m_SwapChainDesc.BufferDesc.Width;
//	m_ViewPort.Height = (float)m_SwapChainDesc.BufferDesc.Height;
//	m_ViewPort.MinDepth = 0.0f;
//	m_ViewPort.MaxDepth = 1.0f;
//
//	m_pD3dContext->RSSetViewports(1, &m_ViewPort);
//	return S_OK;
//}


ID3D11Device*& DxManager::GetDevice()
{
	return GetInstance().m_pD3dDevice;
}

ID3D11DeviceContext*& DxManager::GetDContext()
{
	return GetInstance().m_pD3dContext;
}