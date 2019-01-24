#pragma once
#pragma warning (disable : 4005)
#pragma warning (disable : 4477)
#pragma warning (disable : 4172)
#pragma warning (disable : 4505)
#pragma warning (disable : 4100)
#pragma warning (disable : 4244)
#include <windows.h>
#include <windowsx.h>
#include "stdio.h"
#include "conio.h"
#include <process.h>
#include <time.h>
#include <tchar.h>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <d3d11.h>
#include <d3dx11.h>
//#include <d3dx10.h>
#include <d3dx10math.h>
#include <random>
#include <algorithm>
#include <imm.h>
#include <locale.h>
using namespace std;

namespace UI
{
	typedef basic_string<TCHAR> T_STR;
	typedef basic_string<wchar_t> W_STR;
	typedef basic_string<char>  C_STR;
	typedef vector<T_STR>		T_STR_VECTOR;
	typedef basic_string<TCHAR>::iterator	T_ITOR;

	class JDxHelper
	{
	public:
		/* float Lerp */
		static float fLerp(const float value1, const float value2, const float amount)
		{
			return float(value1 + ((float)(value2 - value1) * amount));
		}
		/* float Random Range */
		static float fRand(const float Min, const float Max)
		{
			return ((Max - Min) * ((float)rand() / RAND_MAX)) + Min;
		}
		/* int Random Range */
		static int iRand(const int Min, const int Max)
		{
			return ((Max - Min) * ((float)rand() / RAND_MAX)) + Min;
		}
		static bool bRand()
		{
			int i = iRand(100, 100000);
			if (i % 2 == 0)
				return false;
			else
				return true;
		}
		/* Vector Rand Range */
		static D3DXVECTOR3 vRand(const D3DXVECTOR3 Min, const D3DXVECTOR3 Max)
		{
			return D3DXVECTOR3(fRand(Min.x, Max.x), fRand(Min.y, Max.y), fRand(Min.z, Max.z));
		}
		static D3DXVECTOR4 vRand(const D3DXVECTOR4 Min, const D3DXVECTOR4 Max)
		{
			return D3DXVECTOR4(fRand(Min.x, Max.x), fRand(Min.y, Max.y), fRand(Min.z, Max.z), fRand(Min.w, Max.w));
		}
		/* Quaternion Rand Range*/
		static D3DXQUATERNION qRand(const D3DXQUATERNION Min, const D3DXQUATERNION Max)
		{
			return D3DXQUATERNION(fRand(Min.x, Max.x), fRand(Min.y, Max.y), fRand(Min.z, Max.z), fRand(Min.w, Max.w));
		}
		static TCHAR* NameDivide(const TCHAR* pszFileName)
		{
			TCHAR szFileName[MAX_PATH] = { 0, };
			TCHAR Dirve[MAX_PATH] = { 0, };
			TCHAR Dir[MAX_PATH] = { 0, };
			TCHAR FileName[MAX_PATH] = { 0, };
			TCHAR FileExt[MAX_PATH] = { 0, };
			if (pszFileName != NULL)
			{
				_tsplitpath_s(pszFileName, Dirve, Dir, FileName, FileExt);
				_stprintf_s(szFileName, _T("%s%s"), FileName, FileExt);
			}
			return szFileName;
		}
		static char* NameDivide(const char* pszFileName)
		{
			char* szFileName[MAX_PATH] = { 0, };
			char Dirve[MAX_PATH] = { 0, };
			char Dir[MAX_PATH] = { 0, };
			char FileName[MAX_PATH] = { 0, };
			char FileExt[MAX_PATH] = { 0, };
			if (pszFileName != NULL)
			{
				_splitpath_s(pszFileName, Dirve, Dir, FileName, FileExt);
				printf_s(*szFileName, _T("%s%s"), FileName, FileExt);
			}
			return *szFileName;
		}
		static bool RectInPt(RECT rt, POINT pt)
		{
			if (rt.left <= pt.x && rt.right >= pt.x)
			{
				if (rt.top >= pt.y && rt.bottom <= pt.y)
				{
					return true;
				}
			}
			return false;
		}
		static char* wtom(wchar_t* str)
		{
			//반환할 char* 변수 선언
			char* pStr;
			//입력받은 wchar_t 변수의 길이를 구함
			int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
			//char* 메모리 할당
			pStr = new char[strSize];
			//형 변환 
			WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
			return pStr;
		}

		static wchar_t* mtow(char* str)
		{
			//wchar_t형 변수 선언
			wchar_t* pStr;
			//멀티 바이트 크기 계산 길이 반환
			int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
			//wchar_t 메모리 할당
			pStr = new WCHAR[strSize];
			//형 변환
			MultiByteToWideChar(CP_ACP, 0, str, (int)strlen(str) + 1, pStr, strSize);
			return pStr;
		}
	};
	struct PNCT_VERTEX
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		D3DXVECTOR4 c;
		D3DXVECTOR2 t;
		PNCT_VERTEX()
		{

		}
		PNCT_VERTEX(D3DXVECTOR3 vp, D3DXVECTOR3 vn, D3DXVECTOR4 vc, D3DXVECTOR2 vt)
		{
			p = vp;
			n = vn;
			c = vc;
			t = vt;
		}
	};
	struct VSCB
	{
		D3DXMATRIX matWorld;
		D3DXMATRIX matView;
		D3DXMATRIX matProj;
		D3DXVECTOR4 vColor;
	};
	template<class T>
	class JSingleton
	{
	public:
		static T& GetInstance()
		{
			static T SingleInstance;
			return SingleInstance;
		}
	};
	/////////////////////////////////////////////////////////////////////////////////////////////
	class JBasis
	{
	public:
		virtual bool Init()
		{
			return true;
		}
		virtual bool Frame()
		{
			return true;
		}
		virtual bool Render()
		{
			return true;
		}
		virtual bool Release()
		{
			return true;
		}
	};
}