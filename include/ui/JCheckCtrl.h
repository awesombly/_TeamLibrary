#pragma once
#include "JImageCtrl.h"
namespace UI
{
	enum CheckState
	{
		txENABLE = 0,
		txDISABLE,
	};
	class JCheckCtrl : public JImageCtrl
	{
	public:
		bool m_bCheck = false;
	public:
		bool Create(ID3D11Device* pDevice, const TCHAR* szENABLE, const TCHAR* szDISABLE,
			const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
		void* uiclone();
		void SetCheck(bool Check);
		bool GetCheck();
	public:
		void Update();
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
	public:
		JCheckCtrl(wstring NodeName) : JImageCtrl(NodeName) { m_Type = UI::CHECK; };
		virtual ~JCheckCtrl() {};
	};
}