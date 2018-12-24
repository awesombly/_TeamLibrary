#pragma once
#include "stdui.h"
namespace UI
{
	class JState
	{
	public:
		static ID3D11RasterizerState*			g_pRSWireFrame;
		static ID3D11RasterizerState*			g_pRSBackCullSolid;
		static ID3D11RasterizerState*			g_pRSDepthShadow;
		static ID3D11RasterizerState*			g_pRSNoneCullSolid;
		static ID3D11RasterizerState*			g_pRSFrontCullSolid;
		static ID3D11DepthStencilState*			g_pEnableDSS;
		static ID3D11DepthStencilState*			g_pDisableDSS;
		static ID3D11SamplerState*				g_pSSShadowMap;
		static ID3D11SamplerState*				g_pTexSS;
		static ID3D11SamplerState*				g_pClampLinear;
		static ID3D11BlendState*				g_pAlphaBlend;
		static ID3D11BlendState*				g_pNoAlphaBlend;
		static ID3D11BlendState*				g_pColorBlend;
	public:
		static HRESULT SetState(ID3D11Device*	pd3dDevice);
		static bool  Release();
	};
}