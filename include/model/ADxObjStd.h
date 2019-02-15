#pragma once
#include "ADxStd.h"


#pragma pack(push,1)
struct AInstance
{
	D3DXMATRIX matWorld;
	D3DXVECTOR4  color;
};
#pragma pack(pop)

#pragma region PNCTstruct
struct PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;
	PT_VERTEX() {};
	PT_VERTEX(D3DXVECTOR3 vp, D3DXVECTOR2 vt)
	{
		p = vp;
		t = vt;
	}
};

struct PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
	PC_VERTEX() {}
	PC_VERTEX(D3DXVECTOR3 vp, D3DXVECTOR4 vc)
	{
		p = vp;
		c = vc;
	}

};

struct PCT_VERTEX
{
	D3DXVECTOR3 p;

	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
	PCT_VERTEX() {}
	PCT_VERTEX(D3DXVECTOR3 vp, D3DXVECTOR4 vc, D3DXVECTOR2 vt)
	{
		p = vp;		c = vc;		t = vt;
	}
};


struct PNCT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
	int iSubIndex;	//일단 추가해봄
	bool operator == (const PNCT_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(D3DXVECTOR3 vp, D3DXVECTOR3 vn, D3DXVECTOR4 vc, D3DXVECTOR2 vt)
	{
		p = vp;		n = vn;		c = vc;		t = vt;
	}
};

//struct PNCTT_VERTEX
//{
//	D3DXVECTOR3 p;
//	D3DXVECTOR3 n;
//	D3DXVECTOR4 c;
//	D3DXVECTOR2 t;
//	D3DXVECTOR3 vTangent;
//
//	PNCTT_VERTEX() {}
//	PNCTT_VERTEX(D3DXVECTOR3 vp, D3DXVECTOR3 vn, D3DXVECTOR4 vc, D3DXVECTOR2 vt, D3DXVECTOR3 Tangent)
//	{
//		p = vp;		n = vn;		c = vc;		t = vt;  vTangent = Tangent;
//	}
//};

struct PNCT2_VERTEX
{
	D3DXVECTOR3		p;
	D3DXVECTOR3		n;
	D3DXVECTOR4		c;
	D3DXVECTOR2     t;
	D3DXVECTOR3		vTangent;
	bool operator == (const PNCT2_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c   &&	vTangent == Vertex.vTangent)
		{
			return true;
		}
		return  false;
	}
	PNCT2_VERTEX() {}
	PNCT2_VERTEX(D3DXVECTOR3		vp,
		D3DXVECTOR3		vn,
		D3DXVECTOR4		vc,
		D3DXVECTOR2     vt,
		D3DXVECTOR3		tangent)
	{
		p = vp, n = vn, c = vc, t = vt, vTangent = tangent;
	}
};
#pragma endregion


struct PNCT5_VERTEX
{
	D3DXVECTOR3		p;
	D3DXVECTOR3		n;
	D3DXVECTOR4		c;
	D3DXVECTOR2		t;
	D3DXVECTOR4		w1;// W0, fW1, fW2, fW3;
	D3DXVECTOR4		i1;	// I0, I1, I2, I3
	D3DXVECTOR4		w2;// W4, fW5, fW6, fNumWeight;	
	D3DXVECTOR4		i2;	// I4, I5, I6, I7
	bool operator == (const PNCT5_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT5_VERTEX() {
		w1.x = w1.y = w1.z = w1.w = 0.0f;
		i1.x = i1.y = i1.z = i1.w = 0.0f;
		w2.x = w2.y = w2.z = w2.w = 0.0f;
		i2.x = i2.y = i2.z = i2.w = 0.0f;
	}
	PNCT5_VERTEX(D3DXVECTOR3		vp,
		D3DXVECTOR3		vn,
		D3DXVECTOR4		vc,
		D3DXVECTOR2     vt,
		D3DXVECTOR4		vw0,
		D3DXVECTOR4		vi0,
		D3DXVECTOR4		vw1,
		D3DXVECTOR4		vi1)
	{
		p = vp, n = vn, c = vc, t = vt;
		w1 = vw0, i1 = vi0, w2 = vw1, i2 = vi1;
	}
};

struct PNCT6_VERTEX
{
	D3DXVECTOR3		p;
	D3DXVECTOR3		n;
	D3DXVECTOR4		c;
	D3DXVECTOR2		t;

	D3DXVECTOR4		w1;// W0, fW1, fW2, fW3;
	D3DXVECTOR4		i1;	// I0, I1, I2, I3
	D3DXVECTOR4		w2;// W4, fW5, fW6, fw7;	
	D3DXVECTOR4		i2;	// I4, I5, I6, I7

	D3DXVECTOR4		w3;// W8, fW9, fW10, fNumWeight;	
	D3DXVECTOR4		i3;	// I8, I9, I10, I11


	bool operator == (const PNCT6_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT6_VERTEX() {
		w1.x = w1.y = w1.z = w1.w = 0.0f;
		i1.x = i1.y = i1.z = i1.w = 0.0f;
		w2.x = w2.y = w2.z = w2.w = 0.0f;
		i2.x = i2.y = i2.z = i2.w = 0.0f;
		i3.x = i3.y = i3.z = i3.w = 0.0f;
	}

	PNCT6_VERTEX(D3DXVECTOR3		vp,
		D3DXVECTOR3		vn,
		D3DXVECTOR4		vc,
		D3DXVECTOR2     vt,

		D3DXVECTOR4		vw1,
		D3DXVECTOR4		vi1,
		D3DXVECTOR4		vw2,
		D3DXVECTOR4		vi2,
		D3DXVECTOR4		vw3,
		D3DXVECTOR4		vi3)
	{
		p = vp, n = vn, c = vc, t = vt;
		w1 = vw1, i1 = vi1, w2 = vw2, i2 = vi2, w3 = vw3, i3 = vi3;
	}
};




struct ATri_PNCT5
{
	PNCT5_VERTEX						vVertex[3];
	D3DXVECTOR3							vNormal;
	int									iSubIndex;
	ATri_PNCT5(int Index) : iSubIndex(Index) {}
	ATri_PNCT5() : iSubIndex(-1) {}
};


struct PNC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR4 c;
	bool operator == (const PNC_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c)
		{
			return true;
		}
		return  false;
	}
	PNC_VERTEX() {}
	PNC_VERTEX(D3DXVECTOR3		vp,
		D3DXVECTOR3		vn,
		D3DXVECTOR4		vc)
	{
		p = vp, n = vn, c = vc;
	}
};





struct A_Box
{
	D3DXVECTOR3		vCenter;
	D3DXVECTOR3		vPos[8];

	D3DXVECTOR3		vMin;			//min[0] [1] [2]
	D3DXVECTOR3		vMax;			//max[0] [1] [2]

	D3DXVECTOR3		vAxis[3];		//3개 직교축
	float			fExtent[3];		//3개의 직교축 거리
};

struct A_Sphere
{
	D3DXVECTOR3		vCenter;
	float			fRadius;
};

//
struct A_Plane
{
	float	fA, fB, fC, fD;
	//bool	CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2);
	//bool	CreatePlane(D3DXVECTOR3 vNormal, D3DXVECTOR3 v0);
	void	Normalize()
	{
		float fValue = sqrt(fA*fA + fB * fB + fC * fC);
		fA = fA / fValue;
		fB = fB / fValue;
		fC = fC / fValue;
		fD = fD / fValue;
	}
};


//struct AScene
//{
//	int iVersion;		// 버전
//	int	iFirstFrame;    // 시작 프레임
//	int	iLastFrame;     // 마지막 프레임
//	int	iFrameSpeed;    // 1초당 프레임 개수(30)
//	int	iTickPerFrame;  // 1프레임의 틱 값(160)
//	int iNumMesh;		// 메쉬오브젝트 개수
//	int iMaxWeight;		// 정점 당 가중치
//	int iBindPose;		// 바인딩 포즈 에니메이션 여부
//};