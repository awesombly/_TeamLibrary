#pragma once
#include "JModel.h"
namespace UI
{
	/* 원인척하는 평면 */
	class JSphere : public JModel
	{
	public:
		vector<PNCT_VERTEX> m_pVertices;
		vector<DWORD>		m_pIndices;
		UI::VSCB			m_cbData;
	public:
		D3DXMATRIX				 m_matWorld;
		D3DXMATRIX				 m_matView;
		D3DXMATRIX				 m_matProj;
	public:
		void SetMatrix(D3DXMATRIX* matWorld, D3DXMATRIX* matView, D3DXMATRIX* matProj);
		bool Create(ID3D11Device* pDevice, const char* PSName = "PS_Sphere", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
	public:
		bool PreRender(ID3D11DeviceContext* pContext);
		bool Render(ID3D11DeviceContext* pContext);
		bool PostRender(ID3D11DeviceContext* pContext);
	public:
		JSphere() {};
		virtual ~JSphere() {};
	};
}