#pragma once
#include "JModel.h"
#include "JTextureMgr.h"
namespace UI
{
	class JPlane : public JModel
	{
	public:
		vector<PNCT_VERTEX> m_pVertices;
		vector<DWORD>		m_pIndices;
		UI::VSCB			m_cbData;
		bool				m_bHovered = false;
		bool				m_bPressed = false;
		bool				m_bClicked = false;
	private:
		bool				m_bClickedTemp = false;
	public:
		//bool CheckPicking(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
		bool Clicked(RECT& rt, POINT& pt);
		bool Hovered(RECT& rt, POINT& pt);
		bool Pressed(RECT& rt, POINT& pt);
		bool Create(ID3D11Device* pDevice, const char* PSName = "PS", const TCHAR* szShaderName = L"../../Data/Shader/DefaultUI.hlsl");
	public:
		bool Frame();
		bool PreRender(ID3D11DeviceContext* pContext);
		bool Render(ID3D11DeviceContext* pContext, JTexture* pTexture);
		bool PostRender(ID3D11DeviceContext* pContext);
	public:
		JPlane();
		virtual ~JPlane();
	};
}