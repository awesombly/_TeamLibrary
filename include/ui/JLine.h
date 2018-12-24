#pragma once
#include "JModel.h"
namespace UI
{
	class JLine : public JModel
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
		bool Create(ID3D11Device* pDevice, const char* PSName = "PS_COLOR", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
		bool Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vPos, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor);
	private:
		bool PreRender(ID3D11DeviceContext* pContext);
		bool Render(ID3D11DeviceContext* pContext);
		bool PostRender(ID3D11DeviceContext* pContext);
	public:
		JLine() {};
		virtual ~JLine() {};
	};

}