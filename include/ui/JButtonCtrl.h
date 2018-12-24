#pragma once
#include "JImageCtrl.h"
namespace UI
{
	class JButtonCtrl : public JImageCtrl
	{
	public:
		bool Create(ID3D11Device* pDevice, 
			const TCHAR* szNORMAL, const TCHAR* szHOVER, const TCHAR* szPRESS,
			const char* PSName = "PS", const TCHAR* szShaderName = L"../../Data/Shader/DefaultUI.hlsl");
	public:
		void Update();
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
	public:
		JButtonCtrl(wstring NodeName) : JImageCtrl(NodeName) { m_Type = UI::BUTTON; };
		virtual ~JButtonCtrl() {};
	};
}