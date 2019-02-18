#pragma once
#include "JImageCtrl.h"
namespace UI
{
	class JProgressBar : public JPanel
	{
	public:
		JImageCtrl*			m_pBackGround;
		JImageCtrl*			m_pMiddleGround;
		JImageCtrl*			m_pFrontGround;
		VHType				m_VHType = Horizontal;
		const float*		m_fCur;
		float				m_fTemp = 1.0f; // �ʱ⿡ �־���� ��..
		float			    m_fMaxValue = 1.0f;
		float				m_fCurValue;
		float				m_fDisValue = 0.0f;
		bool				m_bDecrease = true;
	public:
		void CheckDis(const float& spf);
		void SetColor(D3DXVECTOR4 vColor);
		void SetMiddleColor(D3DXVECTOR4 vColor);
		void SetValue(const float& fValue, float fMaxValue, float& temp);
		bool Create(ID3D11Device* pDevice, const TCHAR* szBack, const TCHAR* szFront, 
			 const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
	public:
		void* uiclone();
		void UpdateTexture(JImageCtrl* pImage, float fValue);
		void Update(const float& spf);
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
	public:
		JProgressBar(wstring NodeName) : JPanel(NodeName) 
		{
			m_Type = UI::PROGRESS;
			m_pBackGround = new JImageCtrl(NodeName + L"_Back");
			m_pFrontGround = new JImageCtrl(NodeName + L"_Front");
			m_pMiddleGround = new JImageCtrl(NodeName + L"_Dis");
		};
		virtual ~JProgressBar() {};
	};
}