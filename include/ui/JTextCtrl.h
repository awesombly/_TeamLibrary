#pragma once
#include "JButtonCtrl.h"
#include "WriteManager.h"
namespace UI
{
	class JTextCtrl : public JButtonCtrl
	{
	public:
		wstring m_Text;
		D3DXVECTOR4 m_rtText;
		float m_fSize;
		EAlign m_Align = EAlign::Center;
	public:
		bool Create(ID3D11Device* pDevice, wstring Text, float TextSize,
			const TCHAR* szNORMAL, const TCHAR* szHOVER, const TCHAR* szPRESS,
			const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
		void* uiclone();
		wstring GetString();
		void SetString(wstring strData);
	public:
		void Update();
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
	public:
		JTextCtrl(wstring NodeName) : JButtonCtrl(NodeName) { m_Type = UI::TEXT; };
		virtual ~JTextCtrl() {};
	};
}