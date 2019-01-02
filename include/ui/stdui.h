#pragma once
#include "utillui.h"
#pragma comment( lib, "d3dx10.lib" )
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib") //확장 버전
#pragma comment(lib,"winmm.lib")   // timeGetTime
#pragma comment( lib, "IMM32.lib" ) // ime

//#pragma comment(lib,"D2D1.lib")
//#pragma comment(lib, "DWrite.lib")
//#pragma comment(lib, "dinput8.lib") // dxInput
//#pragma comment(lib, "dxguid.lib") // guid
//#pragma comment(lib, "UIToolLib.lib")
#define SAMPLE_BUFFER_SIZE 16
#define KEYSTATECOUNT 256

#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#define SAFE_DELETE(A)				{ if(A) { delete[] (A); (A)=NULL; } }
