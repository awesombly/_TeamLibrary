#pragma once
#include "XStd.h"


enum X_POSITION
{
	P_BACK = 0,    // Point is positioned behind plane
	P_FRONT,   // Point is positioned in front of plane	
	P_ONPLANE, // Point is positioned on plane
	P_SPANNING // Point is spanning plane
};

class XFrustum
{
public:
	D3DXVECTOR3			m_vFrustum[8];
	vector<DWORD>		m_dwIndexData;

	D3DXMATRIX			m_matWorld;
	D3DXMATRIX			m_matView;
	D3DXMATRIX			m_matProj;

	X_Plane				m_Plane[6];
public:
	bool				CreateFrustum();
	void				SetMatrix(D3DXMATRIX *matWorld, D3DXMATRIX *matView, D3DXMATRIX *matProj);
	bool				CheckPoint(D3DXVECTOR3* pPos);
	bool				CheckSphere(X_Sphere* pSphere);
	bool				CheckOBBInPlane(X_Box* pOBB);
	X_POSITION			CheckPoitionOBBInPlane(X_Box*  pOBB);
public:
	XFrustum();
	virtual ~XFrustum();
};

