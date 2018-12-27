#pragma once
#include "JImageCtrl.h"
namespace UI
{
	class JProgressBar : public JPanel
	{
	public:
		JImageCtrl* m_pBackGround;
		JImageCtrl* m_pFrontGround;
		const float*		m_fCurValue;
		float			    m_fMaxValue = 1.0f;
		float				m_fValue;
	public:
		void SetValue(const float& fValue, float fMaxValue);
		bool Create(ID3D11Device* pDevice, const TCHAR* szBack, const TCHAR* szFront, 
			const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
	public:
		void Update();
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
	public:
		JProgressBar(wstring NodeName) : JPanel(NodeName) 
		{
			m_Type = UI::PROGRESS;
			m_pBackGround = new JImageCtrl(NodeName + L"_Back");
			m_pFrontGround = new JImageCtrl(NodeName + L"_Front");
		};
		virtual ~JProgressBar() {};
	};
}