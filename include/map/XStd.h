#pragma once
#define _CRT_SECURE_NO_WARNINGS

// engine header
#include "header.h"

#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <windowsx.h>
#include <time.h>
#include <map>
#include <list>
#include <vector>
#include <set>
#include <queue>
#include <memory>
#include <process.h> // _beginthreadex()의 헤더
#include <crtdbg.h> // 메모리 누수를 잡기위한 헤더
#include <assert.h>
// Dx
#include <d3d11.h>
#include <dxgi.h>
#include <d2d1.h>
#include <dinput.h>	
#include <dwrite.h>
/////////다이렉트 컴파일러헤더///////////
#include "D3DX11.h"
#include <D3DX10math.h>
#include <d3dcompiler.h>
#include <wrl.h>


#ifdef NDEBUG
#ifdef _DEBUG
#undef _DEBUG
#endif
#endif

#ifdef _DEBUG
#pragma comment(lib, "XDxMapLibrary_d.lib")
#pragma comment(lib, "_CoreLib D3D_d.lib")
#else
#pragma comment(lib, "XDxMapLibrary.lib")
#pragma comment(lib, "_CoreLib D3D.lib")  // engine lib

#endif // DEBUG

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib,"dxguid.lib")

//#define CRTDBG_MAP_ALLOC
//#define new new(_CLIENT_BLOCK, __FILE__,__LINE__)

using namespace Microsoft::WRL;

class XBase
{
public:
	virtual bool Init() = 0;
	virtual bool Frame() = 0;
	virtual bool Render() = 0;
	virtual bool Release() = 0;
public:
	XBase() {}
	virtual ~XBase() {}
};

template<class X>
class XSingleton
{
public:
	static X& Get()
	{
		static X instance;
		return instance;
	}

};

using namespace std;
typedef basic_string<TCHAR> TString;

#ifndef V
#define V(x)	{hr = (x);}
#endif

#ifndef V_RETURN
#define V_RETURN(x)	{hr = (x); if(FAILED(hr)) return hr; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(x, y)				{ if (!x) x = new y; }
#endif

#ifndef SAFE_DEL
#define SAFE_DEL(x)					{ if (x) delete x; (x)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)				{ if(x) { (x)->Release(); (x)=NULL; } }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 
struct float2
{
	union
	{
		struct
		{
			float x;
			float y;
		};
		float v[2];
	};
};


class XPosition
{
public:
	float x, y;
public:
	XPosition& operator =(const XPosition& pos)
	{
		x = pos.x;
		y = pos.y;
		return *this;
	}
public:
	XPosition() { x = y = 0.0f; }
	XPosition(float fX, float fY = 0.0f)
	{
		x = fX;
		y = fY;
	}
	~XPosition() {}
};

class XRectangle
{
public:
	float left, top, right, bottom;
public:
	XRectangle& operator =(const XRectangle& rt)
	{
		left = rt.left;
		top = rt.top;
		right = rt.right;
		bottom = rt.bottom;
		return *this;
	}
public:
	XRectangle() { left = top = right = bottom = 0.0f; }
	XRectangle(float fLeft, float fTop = 0.0f, float fRight = 0.0f, float fBottom = 0.0f)
	{
		left = fLeft;
		top = fTop;
		right = fRight;
		bottom = fBottom;
	}
	~XRectangle() {}
};

struct X_Plane
{
	float		 a, b, c, d;
	D3DXVECTOR3  vCenter;
	float		 fRadius;
	void		 CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2)
	{
		D3DXVECTOR3 e0 = v1 - v0;
		D3DXVECTOR3 e1 = v2 - v0;
		D3DXVECTOR3 n;
		D3DXVec3Cross(&n, &e0, &e1);
		D3DXVec3Normalize(&n, &n);

		a = n.x;	b = n.y;	c = n.z;
		d = -(n.x * v0.x + n.y * v0.y + n.z * v0.z);
	}
	void		 CreatePlane(D3DXVECTOR3 n, D3DXVECTOR3 v0)
	{
		a = n.x;	b = n.y;	c = n.z;
		d = -(n.x * v0.x + n.y * v0.y + n.z * v0.z);
	}
};

struct X_Sphere
{
	D3DXVECTOR3  vCenter;
	float		 fRadius;
};

struct X_Box
{
	D3DXVECTOR3  vMin;
	D3DXVECTOR3  vMax;

	D3DXVECTOR3  vCenter;
	D3DXVECTOR3  vAxis[3];		// 축
	float		 fExtent[3];	// 센터부터 x,y,z축의 거리

};