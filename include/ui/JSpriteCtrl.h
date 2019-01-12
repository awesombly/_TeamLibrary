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
		bool m_bPlay = true;
		bool m_bLoop = true;
		int	m_iRows;
		int m_iCols;
		float m_fUVRows; //  1.0f / row;
		float m_fUVCols; //  1.0f / col;
		int m_iCurrentRows; // 현재 u
		int m_iCurrentCols; // 현재 v
		/////////////////////////////////////////
		int m_iCurrentIndex; // 현재 인덱스
		int m_iNumTexture; // 전체 텍스쳐 개수
		float m_fTime;  // 현재 시간
		float m_fChangeTime; // 텍스쳐가 바뀌는 시간 (m_fAnimTime / m_iNumTexture)
		float m_fAnimTime; // Animation이 돌아가는 시간 (1초(animtime)에 10장의 텍스쳐를 교체해라.)
	public:
		void* uiclone();
		void Play();
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