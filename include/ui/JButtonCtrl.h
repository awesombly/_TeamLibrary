#pragma once
#include "JImageCtrl.h"
namespace UI
{
	class JButtonCtrl : public JImageCtrl
	{
	public:
		bool Create(ID3D11Device* pDevice, 
			const TCHAR* szNORMAL, const TCHAR* szHOVER, const TCHAR* szPRESS,
			const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
	public:
		void* uiclone();
		void Update();
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
	public:
		JButtonCtrl(wstring NodeName) : JImageCtrl(NodeName) { m_Type = UI::BUTTON; };

		virtual ~JButtonCtrl() {};
	};
}