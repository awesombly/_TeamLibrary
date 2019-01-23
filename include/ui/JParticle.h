#pragma once
#include "JTransform.h"
#include "JSpriteCtrl.h"
namespace UI
{
	class JParticle : public JTransform
	{
	public:
		D3DXMATRIX m_matWorld;
		float fMoveSpeed;
		float fRotateSpeed;
		/* 디렉션 벡터 */
		D3DXVECTOR3 vDir;
		D3DXQUATERNION qDirAngle;
		/* turn */
		bool bTurnX;
		bool bTurnY;
		bool bTurnZ;
		float fTimer;
		float fRandom;
		/* 컬러 */
		D3DXVECTOR4 vColor;
		/* 누적시간 */
		float		fElapseTime;
		/* 생명주기 */
		float		fLifeTime;
		bool		bRender;
		/* Render true time */
		float		fStartTime;
	public:
		void Update();
		bool Frame(const float& spf);
	public:
		void SetScale(const float fMin, const float fMax);
		void SetColor(const D3DXVECTOR4 vMin, const D3DXVECTOR4 vMax);
		void SetTurnDirection(const bool bX, const bool bY, const bool bZ);
		void SetStartPos(const D3DXVECTOR3 vMin, D3DXVECTOR3 vMax);
		void SetLifeTime(const float MinTime, const float MaxTime);
		void SetDirection(const D3DXVECTOR3 vMin, const D3DXVECTOR3 vMax);
		void SetDirectionAngle(const D3DXQUATERNION qMin, const D3DXQUATERNION qMax);
		void SetMoveSpeed(const float fMinSpeed, const float fMaxSpeed);
		void SetRotateSpeed(const float fMinSpeed, const float fMaxSpeed);
	public:
		JParticle();
		virtual ~JParticle() {}
	};
}