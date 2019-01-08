#pragma once
#include "JSpriteCtrl.h"
#include "JParticle.h"
#define MAX_PARTICLE 1024
namespace UI
{
	class JParticleSystem : public JPanel
	{
	public:
		JSpriteCtrl* m_pSprite;
		vector<JParticle*> m_pParticle;
		list<JParticle*> m_pRealParticle;
	public:
		int	  m_iMaxParticle;
	public:
		bool m_bTurnX;
		bool m_bTurnY;
		bool m_bTurnZ;
	public:
		bool m_bStartPosRand = false;
	public:
		float m_fMinScale;
		float m_fMaxScale;
		D3DXVECTOR3 m_vMinStartPos;
		D3DXVECTOR3 m_vMaxStartPos;
		float m_fMinLifeTime;
		float m_fMaxLifeTime;
		D3DXVECTOR3 m_vMinDirection;
		D3DXVECTOR3 m_vMaxDirection;
		float m_fMinMoveSpeed;
		float m_fMaxMoveSpeed;
		D3DXQUATERNION m_qMinDirAngle;
		D3DXQUATERNION m_qMaxDirAngle;
		float m_fMinRotateSpeed;
		float m_fMaxRotateSpeed;
		D3DXVECTOR4 m_vMinColor;
		D3DXVECTOR4 m_vMaxColor;
	public:
		void Play();
		bool Create(ID3D11Device* pDevice, int iMaxParticle, float fAnimTime, vector<T_STR> FileList,
			const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
	public:
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
		bool ParticleRelease();
	public:
		void SetScale(const float fMin, const float fMax);
		void SetColor(const D3DXVECTOR4 vMin, const D3DXVECTOR4 vMax);
		void SetTurnDirection(const bool bX, const bool bY, const bool bZ);
		void SetStartPos(const D3DXVECTOR3 vMin, const D3DXVECTOR3 vMax);
		void SetLifeTime(float fMin, float fMax);
		void SetDirection(const D3DXVECTOR3 vMin, const D3DXVECTOR3 vMax);
		void SetDirectionAngle(const D3DXQUATERNION qMin, const D3DXQUATERNION qMax);
		void SetMoveSpeed(const float fMin, const float fMax);
		void SetRotateSpeed(const float fMin, const float fMax);
		JParticleSystem(wstring NodeName) : JPanel(NodeName)
		{
			m_Type = UI::PARTICLE;
			m_pSprite = new JSpriteCtrl(NodeName += L"_Sprite");
		}
		virtual ~JParticleSystem() {}
	};
}