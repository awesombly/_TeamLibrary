#pragma once
#include "JImageCtrl.h"
namespace UI
{
	class JSpriteCtrl : public JImageCtrl
	{
	public:
		int m_iCurrentIndex; // ���� �ε���
		int m_iNumTexture; // ��ü �ؽ��� ����
		float m_fTime;  // ���� �ð�
		float m_fChangeTime; // �ؽ��İ� �ٲ�� �ð� (m_fAnimTime / m_iNumTexture)
		float m_fAnimTime; // Animation�� ���ư��� �ð� (1��(animtime)�� 10���� �ؽ��ĸ� ��ü�ض�.)
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