#pragma once
#pragma warning(disable : 4005)			// 매크로 재정의 경고 무시
#pragma warning(disable : 4201)			// 
#pragma warning(disable : 4099)			//

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include <Windows.h>
#include <d3d11.h>						// DX11
#include <d3dx11.h>						// 확장 버전
#include <d3dcompiler.h>				// 쉐이더 컴파일 함수
#include <D3DX10math.h>

#include <algorithm>
#include <ctime>
#include <cmath>
#include <tchar.h>						// _swprintf_s(TCHAR).. 정의
#include <future>
#include <random>
#include <iostream>

#include <sstream>						// ostringstream
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <list>
#include <forward_list>
#include <map>
#include <stack>
#include <queue>
//#include <numeric>					// 수치 관련 알고리즘
//#include <functional>					// 범용비교함수, 함수포인터..

// 라이브러리 링크를 전처리 해준다
#ifdef _DEBUG
	#pragma comment( lib, "_CoreLib D3D_d.lib")
#else
	#pragma comment( lib, "_CoreLib D3D.lib")
#endif
//#pragma comment( lib, "ServerLib.lib")
#pragma comment( lib, "d2d1.lib")
#pragma comment( lib, "dwrite.lib")
#pragma comment( lib, "d3d11.lib")		// DX11
#pragma comment( lib, "d3dx10d.lib")	// 벡터, 행렬..
#pragma comment( lib, "d3dx11.lib")		// DX11 Expension
#pragma comment( lib, "dxgi.lib")
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "winmm.lib")		// +timeGetTime()
#pragma comment( lib, "msimg32.lib")	// +TransparentBlt()	

using namespace std;

constexpr static float PI = 3.14159265358979323846f;
constexpr static float EPSILON = std::numeric_limits<float>::epsilon();
constexpr static float GravityPower = 9.8f * 25.0f;

namespace Vector3 {
	const static D3DXVECTOR3 One	  =	{ 1.0f,  1.0f,  1.0f };			// 1, 1, 1
	const static D3DXVECTOR3 Zero	  =	{ 0.0f,  0.0f,  0.0f };			// 0, 0 ,0
	const static D3DXVECTOR3 Left	  =	{ -1.0f, 0.0f,  0.0f };			// -1, 0 ,0
	const static D3DXVECTOR3 Right	  =	{ 1.0f,  0.0f,  0.0f };			// 1, 0 ,0
	const static D3DXVECTOR3 Up		  =	{ 0.0f,  1.0f,  0.0f };			// 0, 1 ,0
	const static D3DXVECTOR3 Down	  =	{ 0.0f, -1.0f,  0.0f };			// 0, -1 ,0
	const static D3DXVECTOR3 Forward  =	{ 0.0f,  0.0f,  1.0f };			// 0, 0 ,1
	const static D3DXVECTOR3 Backward = { 0.0f,  0.0f, -1.0f };			// 0, 0 ,-1
};

namespace Vector4 {
	const static D3DXVECTOR4 One	  = { 1.0f,  1.0f,  1.0f, 1.0f };	// 1, 1, 1, 1
	const static D3DXVECTOR4 Zero	  = { 0.0f,  0.0f,  0.0f, 0.0f };	// 0, 0 ,0, 0
	const static D3DXVECTOR4 Left	  = { -1.0f, 0.0f,  0.0f, 0.0f };	// -1, 0 ,0, 0
	const static D3DXVECTOR4 Right	  = { 1.0f,  0.0f,  0.0f, 0.0f };	// 1, 0 ,0, 0
	const static D3DXVECTOR4 Up		  = { 0.0f,  1.0f,  0.0f, 0.0f };	// 0, 1 ,0, 0
	const static D3DXVECTOR4 Down	  = { 0.0f, -1.0f,  0.0f, 0.0f };	// 0, -1 ,0, 0
	const static D3DXVECTOR4 Forward  = { 0.0f,  0.0f,  1.0f, 0.0f };	// 0, 0 ,1, 0
	const static D3DXVECTOR4 Backward = { 0.0f,  0.0f, -1.0f, 0.0f };	// 0, 0 ,-1, 0
};

namespace Quaternion {
	const static D3DXQUATERNION Base	 = { 0.0f,  0.0f,  0.0f, 1.0f };	// 0, 0, 0, 1
	const static D3DXQUATERNION One		 = { 1.0f,  1.0f,  1.0f, 1.0f };	// 1, 1, 1, 1
	const static D3DXQUATERNION Zero	 = { 0.0f,  0.0f,  0.0f, 0.0f };	// 0, 0 ,0,0
	const static D3DXQUATERNION Left	 = { 0.0f, -1.0f,  0.0f, 0.0f };	// 0, -1 ,0, 0
	const static D3DXQUATERNION Right	 = { 0.0f,  1.0f,  0.0f, 0.0f };	// 0, 1 ,0, 0
	const static D3DXQUATERNION Up		 = { 1.0f,  0.0f,  0.0f, 0.0f };	// 1, 0 ,0,0
	const static D3DXQUATERNION Down	 = { -1.0f,  0.0f, 0.0f, 0.0f };	// -1, 0 ,0, 0
	const static D3DXQUATERNION Forward  = { 0.0f,  0.0f,  1.0f, 0.0f };	// 0, 0 ,1, 0
	const static D3DXQUATERNION Backward = { 0.0f,  0.0f, -1.0f, 0.0f };	// 0, 0 ,-1 0
};

namespace Color {
	const static D3DXVECTOR4 White  = { 1.0f,  1.0f,  1.0f, 1.0f };		// 1, 1, 1, 1
	const static D3DXVECTOR4 Black  = { 0.0f,  0.0f,  0.0f, 1.0f };		// 0, 0, 0, 1
	const static D3DXVECTOR4 Red    = { 1.0f,  0.0f,  0.0f, 1.0f };		// 1, 0, 0, 1
	const static D3DXVECTOR4 Green  = { 0.0f,  1.0f,  0.0f, 1.0f };		// 0, 1, 0, 1
	const static D3DXVECTOR4 Blue   = { 0.0f,  0.0f,  1.0f, 1.0f };		// 0, 0, 1, 1
	const static D3DXVECTOR4 Yellow = { 1.0f,  1.0f,  0.0f, 1.0f };		// 1, 1, 0, 1
	const static D3DXVECTOR4 Purple = { 1.0f,  0.0f,  1.0f, 1.0f };		// 1, 0, 1, 1
	const static D3DXVECTOR4 Cyan   = { 0.0f,  1.0f,  1.0f, 1.0f };		// 0, 1, 1, 1
};

namespace Matrix {
	const static D3DXMATRIX Identity = { 1, 0, 0, 0,
										 0, 1, 0, 0,
										 0, 0, 1, 0,
										 0, 0, 0, 1 };
	const static D3DXMATRIX Zero = { 0, 0, 0, 0,
									 0, 0, 0, 0,
									 0, 0, 0, 0,
									 0, 0, 0, 0 };
};


char * WCharToChar(const wchar_t* str)	noexcept;
wchar_t* CharToWChar(const char* str)		noexcept;
void ErrorMessage(const string_view& msg, const bool& useLoop = false)	noexcept;
void ErrorMessage(const wstring_view& msg, const bool& useLoop = false) noexcept;
static constexpr float DegreeToRadian(const float& degree) noexcept
{
	return degree * (PI / 180);
}
static constexpr float RadianToDegree(const float& radian) noexcept
{
	return radian * (180 / PI);
}
D3DXVECTOR3 Lerp(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const float& time) noexcept;
D3DXQUATERNION Lerp(const D3DXQUATERNION& start, const D3DXQUATERNION& end, const float& time) noexcept;
D3DXVECTOR3 Product(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB) noexcept;
D3DXQUATERNION Product(const D3DXQUATERNION& quatA, const D3DXQUATERNION& quatB) noexcept;
D3DXVECTOR3 Divide(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB) noexcept;
D3DXQUATERNION Divide(const D3DXQUATERNION& quatA, const D3DXQUATERNION& quatB) noexcept;
D3DXVECTOR2 Normalize(const D3DXVECTOR2& vector2) noexcept;
D3DXVECTOR3 Normalize(const D3DXVECTOR3& vector3) noexcept;
float VectorLength(const D3DXVECTOR2& vector2) noexcept;
float VectorLength(const D3DXVECTOR3& vector3) noexcept;
float VectorLengthSq(const D3DXVECTOR2& vector2) noexcept;
float VectorLengthSq(const D3DXVECTOR3& vector3) noexcept;



// WELLRNG512 난수 생성
unsigned long Random(void) noexcept;
float RandomNormal(void) noexcept;
// 난수 시드 설정
void InitWELLState() noexcept;





namespace myStd {
	// 32비트용 sqrt
	//double inline __declspec (naked) __fastcall sqrt14(double n)
	//{
	//	_asm fld qword ptr[esp + 4]
	//	_asm fsqrt
	//	_asm ret 8
	//}

	// 약간 빠른 sqrt
	//float sqrtQuick(const float& value) noexcept
	//{
	//	union
	//	{
	//		int i;
	//		float x;
	//	} u;
	//	u.x = value;
	//	u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
	//	// Two Babylonian Steps (simplified from:)
	//	// u.x = 0.5f * (u.x + x/u.x);
	//	// u.x = 0.5f * (u.x + x/u.x);
	//	u.x = u.x + value / u.x;
	//	u.x = 0.25f * u.x + value / u.x;
	//	return u.x;
	//}

	//  템플릿 abs
	template <typename Type>
	constexpr Type AbsT(const Type& value) noexcept
	{
		return value >= 0 ? value : -value;
	}
	// 템플릿 pow
	template <typename Type, typename Type2>
	constexpr Type PowT(const Type& target, const Type& result, const Type2& value) noexcept
	{
		return value <= 1 ? result : PowT(target, result * target, value - 1);
	}
	template <typename Type, typename Type2>
	constexpr Type PowT(const Type& target, const Type2& value) noexcept
	{
		return value <= 1 ? target : PowT(target, target * target, value - 1);
	}
	// 기본 2제곱
	template <typename Type>
	constexpr Type PowT(const Type& target) noexcept
	{
		return target * target;
	}
	/*template <typename Type>
	struct PowT
	{
	enum { value = target * target }
	};*/

	// 컴파일타임 sin
	constexpr float SinSeries(const float& radian, const int& cnt, const int& maxTerms) noexcept
	{
		return cnt > maxTerms ? 1.0f :
			1.0f - (radian * radian / (2.0f * cnt + 2.0f) / (2.0f * cnt + 3.0f) * SinSeries(radian, cnt + 1, maxTerms));
	}
	constexpr float Sin(const float& radian) noexcept
	{
		constexpr int maxTerms = 10;
		return radian * SinSeries(radian, 0, maxTerms);
	}

	// con, tan, 
}



















// 메모리 누수
// 객체 스크립트-> 디버그 모드일시 마우스 클릭 드래그로 상태값 변경 : 적용시 객체 위치 찾아서 갱신



// 횡스크롤 or 탑뷰
// 드래그 & 드랍으로 적 집고, 날리고(힘 적용) 충돌 시켜 딜링.. 맵 타일 주어서 던지거나...
// 오른쪽 드래그&드랍 : 해당 방향으로 힘 가함(날라간 객체끼리 또 충돌된다거나.. , 날라가는 중인지:날리는 힘 계산해 연쇄충돌)
// 왼클릭 드래그로 베거나, 스킬 드래그해서 라인, 오브젝트(벽 등) 생성 및 투하
// 객체 클릭후 방향 드래그로 해당 방향 날리기

// 와이어 선 각각을 객체로, 전후 선들과 리스트처럼 연결,
// 와이어 액션(끌당, 이동, 엮기..), 와이어 줄에도 충돌 및 데미지 판정-> 날리거나 교살 등..
// 왼쪽, 오른쪽 각각 할당 : 상하키 입력으로 이동 및 끌당(조이기, 늘리기), 가운데 버튼-엮기(끊기)
// 완전 탄성 충돌(튕기고 튕기고..)