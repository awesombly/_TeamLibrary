#pragma once
#include "XStd.h"
#include "XInput.h"

struct X_Ray
{
	float			fExtent;
	D3DXVECTOR3		vOrigin;
	D3DXVECTOR3		vDirection;
};

class XSelect : public XSingleton<XSelect>
{
public:
	friend class XSingleton<XSelect>;
public:
	X_Ray		m_Ray;
	D3DXVECTOR3 m_vIntersection;//교점

	D3DXMATRIX  m_matWorld;
	D3DXMATRIX  m_matView;
	D3DXMATRIX  m_matProj;
	D3DXVECTOR3 m_vCenter;
	D3DXVECTOR3 m_vDxR;

	D3DXVECTOR3 qvec;	//Q벡터
	D3DXVECTOR3 tvec;	//T벡터
	D3DXVECTOR3 pvec;	//P벡터

	D3DXVECTOR3		m_vSrcVex[3];
	//D3DXMATRIX		m_matWorldPick;
	float			m_fPickDistance;

public:
	void SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj, HWND hWnd, RECT rt);
	void Update(HWND hWnd, RECT rt);

	// 교점 가져오는 함수
	D3DXVECTOR3 GetIntersection();
	//교점계산 함수
	bool GetIntersection(D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR3 vNormal,	D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2);
	bool CheckPick(D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2);
	bool IntersectTriangle(const D3DXVECTOR3& origin, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v);
	bool IntersectBox(X_Box* pBox);
	bool CheakOBBToRay(X_Box* pBox);
	bool PointInRect(POINT pt, RECT rc);
public:
	XSelect();
	virtual ~XSelect();
};

#define I_Select XSingleton<XSelect>::Get()