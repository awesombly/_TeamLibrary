#pragma once
#include"XStd.h"

class XDxState
{
public:
	static ComPtr<ID3D11RasterizerState>     g_pRSFSolid;
	static ComPtr<ID3D11RasterizerState>     g_pRSFSolidNone;
	static ComPtr<ID3D11RasterizerState>     g_pRSFWire;

	static ComPtr<ID3D11DepthStencilState>	 g_DSSEnable;
	static ComPtr<ID3D11DepthStencilState>	 g_DSSDisable;
	static ComPtr<ID3D11DepthStencilState>	 g_DSSEnableNoWrite;

	static ComPtr<ID3D11SamplerState>		 g_SSLinear;
	static ComPtr<ID3D11SamplerState> 	     g_SSPoint;

	static ComPtr<ID3D11BlendState>			 g_pAlphaBlend;
	static ComPtr<ID3D11BlendState>			 g_pAlphaBlendDisable;
	static ComPtr<ID3D11BlendState>			 g_pAlphaBlendOneZero;
public:
	static bool	SetState(ID3D11Device* d3dDevice);
};