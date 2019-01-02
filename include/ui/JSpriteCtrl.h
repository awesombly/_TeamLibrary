#pragma once
#include "JImageCtrl.h"
namespace UI
{
	enum SpriteType
	{
		TEXSPRITE = 6429,
		UVSPRITE,
		TEMPSPRITE,
	};
	class JSpriteCtrl : public JImageCtrl
	{
	public:
		SpriteType m_SpriteType = TEXSPRITE;
		int	m_iRows;
		int m_iCols;
		float m_fUVRows; //  1.0f / row;
		float m_fUVCols; //  1.0f / col;
		int m_iCurrentRows; // ���� u
		int m_iCurrentCols; // ���� v
		/////////////////////////////////////////
		int m_iCurrentIndex; // ���� �ε���
		int m_iNumTexture; // ��ü �ؽ��� ����
		float m_fTime;  // ���� �ð�
		float m_fChangeTime; // �ؽ��İ� �ٲ�� �ð� (m_fAnimTime / m_iNumTexture)
		float m_fAnimTime; // Animation�� ���ư��� �ð� (1��(animtime)�� 10���� �ؽ��ĸ� ��ü�ض�.)
	public:
		bool Create(ID3D11Device* pDevice, float fAnimTime, vector<T_STR> FileList, const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
		void SetAnimTime(float fAnim);
		void SetUV(int row, int col);
		bool SetSpriteType(SpriteType type, int row = 1, int col = 1);
		void Update();
	public:
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
		//bool PreRender(ID3D11DeviceContext* pContext)noexcept;
		//bool PostRender(ID3D11DeviceContext* pContext)noexcept;
	public:
		JSpriteCtrl(wstring NodeName) : JImageCtrl(NodeName) { m_Type = UI::SPRITE; };
		virtual ~JSpriteCtrl() {};
	};
}