#pragma once
#include "JPanel.h"

namespace UI
{
	class JImageCtrl : public JPanel
	{
	public:
		virtual bool Create(ID3D11Device* pDevice, const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
		bool SetTexture(ID3D11Device* pDevice, const TCHAR* txName = NULL);
	public:
		void Update();
		bool Frame(const float& spf, const float& accTime)noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
	public:
		JImageCtrl(wstring NodeName) : JPanel(NodeName)
		{
			m_pShape = new JPlane();
			m_Type = IMAGE;
		};
		~JImageCtrl() {};
	};
}