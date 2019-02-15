#pragma once
#include "JImageCtrl.h"
namespace UI
{
	class JSliderCtrl : public JPanel
	{
	public:
		/* 0 ~ 1°ª °íÁ¤ */
		float m_fValue;
		JImageCtrl* m_pSliderBar;
		JImageCtrl* m_pSliderHandle;
		VHType      m_VHType = Horizontal;
		bool        m_bClick = false;
	public:
		void SetColor(const D3DXVECTOR4 vColor);
		bool Create(ID3D11Device* pDevice, const TCHAR* szBar, const TCHAR* szHandle, const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
		bool SetTexture(ID3D11Device* pDevice, const TCHAR* szBar, const TCHAR* szHandle);
		const float* GetValue();
		void SetValue(float fValue);
	public:
		void* uiclone();
		void Update();
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
	public:
		JSliderCtrl(wstring NodeName) : JPanel(NodeName) 
		{
			m_Type = UI::SLIDER;
			m_pSliderBar = new JImageCtrl(NodeName + L"_Bar");
			m_pSliderHandle = new JImageCtrl(NodeName + L"_Handle");
		};
		virtual ~JSliderCtrl() {};
	};
}