#pragma once
#include "JButtonCtrl.h"
namespace UI
{
	class JTextCtrl : public JButtonCtrl
	{
	public:
		wstring m_Text;
		D3DXVECTOR4 m_rtText;
		float m_fSize;
	public:
		bool Create(ID3D11Device* pDevice, wstring Text, float TextSize,
			const TCHAR* szNORMAL, const TCHAR* szHOVER, const TCHAR* szPRESS,
			const char* PSName = "PS", const TCHAR* szShaderName = L"../../Data/Shader/DefaultUI.hlsl");
	public:
		void Update();
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
	public:
		JTextCtrl(wstring NodeName) : JButtonCtrl(NodeName) { m_Type = UI::TEXT; };
		virtual ~JTextCtrl() {};
	};
}