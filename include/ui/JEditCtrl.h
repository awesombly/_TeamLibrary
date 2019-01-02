#pragma once
#include "JTextCtrl.h"
namespace UI
{
	class JEditCtrl : public JTextCtrl
	{
	public:
		bool m_bStart;
		DWORD m_KeyBind;
	public:
		bool Create(ID3D11Device* pDevice, wstring Text, float TextSize,
			const TCHAR* szNORMAL, const TCHAR* szHOVER, const TCHAR* szPRESS,
			const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
	public:
		void Update();
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
	public:
		JEditCtrl(wstring NodeName) : JTextCtrl(NodeName) 
		{
			m_Type = UIType::EDIT;
		};
		virtual ~JEditCtrl() {};
	};
}