#pragma once
#include "JImageCtrl.h"
namespace UI
{
	class JListCtrl : public JImageCtrl
	{
	public:
		float						m_fSize; // font size
		int							m_iCols; // ��ŭ�ɰ����ΰ�
		std::list<wstring>			m_pText; // text array
		std::vector<D3DXVECTOR4>	m_pTextrt; // text rect  image rect / cols
		float*						m_fValue; // 0�̸� �Ǿ��� string 1�̸� �ǵ��� string
		float						m_fTemp;
		//
		float fDivisionValue;       // CurValue Increase
	public:
		void SetColumn(const int iCols);
		void AddValue(const float value);
		void push_string(wstring str);
		bool Create(ID3D11Device* pDevice, const TCHAR* szNORMAL, float fSize = 15, int iCols = 6,
			 const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
		void* uiclone();
	public:
		void Update();
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
	public:
		JListCtrl(wstring NodeName) : JImageCtrl(NodeName)
		{
			m_Type = UI::LIST;
		}
		virtual ~JListCtrl() {}
	};
}