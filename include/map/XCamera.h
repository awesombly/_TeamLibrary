#pragma once
#include "XInput.h"
#include "XFrustum.h"

class XCamera :public XFrustum
{
public:
	//D3DXMATRIX			m_matWorld;
	//D3DXMATRIX			m_matView;
	//D3DXMATRIX			m_matProj;

	D3DXVECTOR3			m_vCameraPos;			// 카메라 위치
	D3DXVECTOR3			m_vTargetPos;			// 카메라 타겟		
	D3DXVECTOR3			m_vAt;					// 카메라 머리

	D3DXVECTOR3			m_vLook;				// z축 벡터
	D3DXVECTOR3			m_vSide;				// y축 벡터
	D3DXVECTOR3			m_vUp;					// x축 벡터

	float				m_fFOV;					// 카메라 시야각
	float				m_fAspect;				// 카메라 비율
	float				m_fNearPlane;			// 투영 최소 거리
	float				m_fFarPlane;			// 투영 최대 거리

	// 오일러 각
	float				m_fYaw;
	float				m_fPitch;
	float				m_fRoll;

	float				m_fSpeed;
public:
	D3DXMATRIX			GetWorldMatrix() { return m_matWorld; }
	D3DXMATRIX			GetViewMatrix() { return m_matView; }
	D3DXMATRIX			GetProjMatrix() { return m_matProj; }

	bool				SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp);			//뷰행렬 생성
	bool				SetProjMatrix(float fFOV, float fAspect, float fNearPlane, float fFarPlane);	//투영행렬 생성
	bool				SetYawPitchRoll();	//뷰행렬로 부터 오일러각을 받아옴
	bool				Update();
	bool				UpdateVector();
	bool				UpdateQuaternion();
	//////////////////////////////////
	bool				 Init(D3DXVECTOR3 vCameraPos, D3DXVECTOR3 vTargetPos, D3DXVECTOR3 vUp, float fFOV, float fAspect, float fNearPlane, float fFarPlane);
	bool				 Init();
	virtual bool		 Frame(const float& spf, const float& accTime);
	bool				 Render();
	bool				 Release();
public:
	XCamera();
	virtual ~XCamera();
};

