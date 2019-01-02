#pragma once
#include "JImageCtrl.h"
namespace UI
{
	class JEditCtrl : public JImageCtrl
	{
	private:
		//
		wstring m_Text;
		D3DXVECTOR4 m_rtText;
		float m_fSize;
	public:
		bool Create(ID3D11Device* pDevice, float TextSize, const TCHAR* szNORMAL, 
			 const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
		bool SetString(wstring str);
	public:
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