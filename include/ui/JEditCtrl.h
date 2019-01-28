#pragma once
#include "JImageCtrl.h"
#include "WriteManager.h"
namespace UI
{
	class JEditCtrl : public JImageCtrl
	{
	public:
		//
		wstring m_Text;
		wstring m_PreText;
		wstring m_strPrivate;
		D3DXVECTOR4 m_rtText;
		int m_iMaxLength = 0;
		float m_fSize;
		bool m_bFocus = false;
		bool m_bPrivate = false;
		EAlign						m_Align = EAlign::Near;
	public:
		bool Create(ID3D11Device* pDevice, float TextSize, const TCHAR* szNORMAL, 
			 const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
		wstring GetString();
		void SetString(wstring strText);
	public:
		void Play();
		void End();
		void Clear();
		void Update();
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
	public:
		JEditCtrl(wstring NodeName) : JImageCtrl(NodeName)
		{
			m_Type = UIType::EDIT;
		};
		virtual ~JEditCtrl() {};
	};
}