#pragma once
#include "JImageCtrl.h"
namespace UI
{
	class JSpriteCtrl : public JImageCtrl
	{
	public:
		int m_iCurrentIndex; // 현재 인덱스
		int m_iNumTexture; // 전체 텍스쳐 개수
		float m_fTime;  // 현재 시간
		float m_fChangeTime; // 텍스쳐가 바뀌는 시간 (m_fAnimTime / m_iNumTexture)
		float m_fAnimTime; // Animation이 돌아가는 시간 (1초(animtime)에 10장의 텍스쳐를 교체해라.)
	public:
		bool Create(ID3D11Device* pDevice, float fAnimTime, vector<T_STR> FileList, const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");

		//void SetTexture(ID3D11Device* pDevice, float fAnimTime, vector<T_STR> FileList);
		void Update();
	public:
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		//bool PreRender(ID3D11DeviceContext* pContext)noexcept;
		//bool PostRender(ID3D11DeviceContext* pContext)noexcept;
	public:
		JSpriteCtrl(wstring NodeName) : JImageCtrl(NodeName) { m_Type = UI::SPRITE; };
		virtual ~JSpriteCtrl() {};
	};
}