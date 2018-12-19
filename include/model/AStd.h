#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <iomanip>
#include <tchar.h>
#include <algorithm>
#include <math.h>




//#include "ADxStd.h"
//#include <dinput.h>
//#include "D3DCompiler.h"
//#include <DXGI.h>
//#include <D3DX11.h>
//#include <D3DX10math.h>
//#pragma comment (lib,"d3dx11.lib")
//#pragma comment (lib,"dinput8.lib")
//#pragma comment (lib,"dxguid.lib")
//#pragma comment (lib,"d3d11.lib")
//#pragma comment (lib,"dxgi.lib")
//#pragma comment (lib,"d2d1.lib" )
//#pragma comment (lib,"dwrite.lib" )
//#pragma comment (lib, "dinput8.lib")
//#pragma comment( lib, "d3dcompiler.lib" )
//#pragma comment(lib, "ACoreLibDX.lib")



using namespace std;

typedef basic_string<TCHAR> T_STR;


template <class T>
class ASingleton
{
public:
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}
};
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 


#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif


#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif

#define RELEASE(A)				{ if(A) { (A)->Release(); } }
//
//
//extern HWND					g_hWnd;
//extern HINSTANCE			g_hInstance;
//extern float				spf;
//extern float				g_fTimer;
//extern float				g_fAccTime;
//
//
//extern RECT					g_rtWindow;
//extern UINT					g_iClientWidth;
//extern UINT					g_iClientHeight;
//
//
//
//
////ID3D11Device*					m_pd3dDevice;
////ID3D11DeviceContext*			m_pImmediateContext;
////IDXGISwapChain*					m_pSwapChain;
////ID3D11RenderTargetView*			m_pRenderTargetView;
////IDXGIFactory*					m_pGIFactory;
////D3D11_VIEWPORT					m_ViewPort;
//
//
//
//
//struct INPUT_MAP
//{
//	bool bUpKey;
//	bool bDownKey;
//	bool bLeftKey;
//	bool bRightKey;
//
//	bool bWKey;
//	bool bSKey;
//	bool bAKey;
//	bool bDKey;
//	bool bQKey;
//	bool bEKey;
//	bool bZKey;
//	bool bCKey;
//
//	bool bLeftClick;
//	bool bRightClick;
//	bool bMiddleClick;
//
//	bool bLeftHold;
//	bool bRightHold;
//	bool bMiddleHold;
//
//	bool bExit;
//	bool bSpace; // 카메라의 이동가속도를 증가시킨다.
//
//	int  iMouseValue[3];
//
//	bool bFullScreen;
//	bool bChangeFillMode;
//	bool bChangePrimitive;
//	bool bChangeCullMode;
//	bool bChangeCameraType;
//	bool bDebugRender;
//};
//extern INPUT_MAP g_InputData;
