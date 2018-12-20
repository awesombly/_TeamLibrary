#include "header.h"
#include <fstream>

#ifdef _DEBUG
	static std::wofstream debugStream(L"../../debugOut.txt", std::ios::app);
#endif

void ErrorMessage(const string_view& msg, const bool& useLoop)
{
#ifdef _DEBUG
	// 디버그 출력
	static stringstream ostr;
	ostr.str("");
	ostr << endl << msg << "\t";
	//try	{
	OutputDebugStringA(ostr.str().c_str());
	//debugStream.open(L"../../debugOut.txt", std::ios::app);
	if(debugStream.is_open())
		debugStream << ostr.str().c_str();
	//debugStream.close();
	//}
	//catch (...) { }
	while (useLoop)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		OutputDebugStringA(ostr.str().c_str());
	}
#endif
}

void ErrorMessage(const wstring_view& msg, const bool& useLoop)
{
#ifdef _DEBUG
	// 디버그 출력
	static wstringstream ostr;
	ostr.str(L"");
	ostr << endl << msg << L"\t";
	OutputDebugStringW(ostr.str().c_str());
	if (debugStream.is_open())
		debugStream << ostr.str().c_str();
	while (useLoop)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		OutputDebugStringW(ostr.str().c_str());
	}
#endif
}

D3DXVECTOR3 Lerp(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const float& time)
{
	return (1 - time) * start + time * end;
}

D3DXQUATERNION Lerp(const D3DXQUATERNION& start, const D3DXQUATERNION& end, const float& time)
{
	return (1 - time) * start + time * end;
}

D3DXVECTOR3 Product(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB)
{
	return { vectorA.x * vectorB.x, vectorA.y * vectorB.y, vectorA.z * vectorB.z };
}
D3DXQUATERNION Product(const D3DXQUATERNION& quatA, const D3DXQUATERNION& quatB)
{
	return { quatA.x * quatB.x, quatA.y * quatB.y, quatA.z * quatB.z, quatA.w * quatB.w };
}

D3DXVECTOR3 Divide(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB)
{
	return { vectorA.x / vectorB.x, vectorA.y / vectorB.y, vectorA.z / vectorB.z };
}
D3DXQUATERNION Divide(const D3DXQUATERNION& quatA, const D3DXQUATERNION& quatB)
{
	return { quatA.x / quatB.x, quatA.y / quatB.y, quatA.z / quatB.z, quatA.w / quatB.w };
}

char * WCharToChar(wchar_t* str)
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

wchar_t* CharToWChar(char* str)
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


// + Random 비트 초기화
static unsigned long wellState[16];
static unsigned int wellIndex = 0;
const static unsigned long MaxValue = (unsigned long)-1;

// WELLRNG512 난수 생성기
unsigned long Random(void) noexcept
{
	static unsigned long a, b, c, d;
	a = wellState[wellIndex];
	c = wellState[(wellIndex + 13) & 15];
	b = a ^ c ^ (a << 16) ^ (c << 15);
	c = wellState[(wellIndex + 9) & 15];
	c ^= (c >> 11);
	a = wellState[wellIndex] = b ^ c;
	d = a ^ ((a << 5) & 0xDA442D20UL);
	wellIndex = (wellIndex + 15) & 15;
	a = wellState[wellIndex];
	wellState[wellIndex] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);
	return wellState[wellIndex];
}

float RandomNormal(void) noexcept
{
	return (float)Random() / (float)MaxValue;
}

void InitWELLState() noexcept
{
	for (auto& iter : wellState)
	{
		iter = rand();
	}
}
