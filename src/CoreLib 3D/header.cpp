#include "header.h"
#include <fstream>

#ifdef _DEBUG
static std::wofstream debugStream(L"../../debugOut.txt", std::ios::app);
#endif


char * WCharToChar(wchar_t* str) noexcept
{
	//��ȯ�� char* ���� ����
	char* pStr;
	//�Է¹��� wchar_t ������ ���̸� ����
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	//char* �޸� �Ҵ�
	pStr = new char[strSize];
	//�� ��ȯ 
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}

wchar_t* CharToWChar(char* str) noexcept
{
	//wchar_t�� ���� ����
	wchar_t* pStr;
	//��Ƽ ����Ʈ ũ�� ��� ���� ��ȯ
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	//wchar_t �޸� �Ҵ�
	pStr = new WCHAR[strSize];
	//�� ��ȯ
	MultiByteToWideChar(CP_ACP, 0, str, (int)strlen(str) + 1, pStr, strSize);
	return pStr;
}

void ErrorMessage(const string_view& msg, const bool& useLoop) noexcept
{
#ifdef _DEBUG
	// ����� ���
	static stringstream ostr;
	ostr.str("");
	ostr << endl << msg << "\t";
	//try	{
	OutputDebugStringA(ostr.str().c_str());
	//debugStream.open(L"../../debugOut.txt", std::ios::app);
	if (debugStream.is_open())
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

void ErrorMessage(const wstring_view& msg, const bool& useLoop) noexcept
{
#ifdef _DEBUG
	// ����� ���
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

D3DXVECTOR3 Lerp(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const float& time) noexcept
{
	return (1 - time) * start + time * end;
}

D3DXQUATERNION Lerp(const D3DXQUATERNION& start, const D3DXQUATERNION& end, const float& time) noexcept
{
	return (1 - time) * start + time * end;
}

D3DXVECTOR3 Product(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB) noexcept
{
	return { vectorA.x * vectorB.x, vectorA.y * vectorB.y, vectorA.z * vectorB.z };
}
D3DXQUATERNION Product(const D3DXQUATERNION& quatA, const D3DXQUATERNION& quatB) noexcept
{
	return { quatA.x * quatB.x, quatA.y * quatB.y, quatA.z * quatB.z, quatA.w * quatB.w };
}

D3DXVECTOR3 Divide(const D3DXVECTOR3& vectorA, const D3DXVECTOR3& vectorB) noexcept
{
	return { vectorA.x / vectorB.x, vectorA.y / vectorB.y, vectorA.z / vectorB.z };
}
D3DXQUATERNION Divide(const D3DXQUATERNION& quatA, const D3DXQUATERNION& quatB) noexcept
{
	return { quatA.x / quatB.x, quatA.y / quatB.y, quatA.z / quatB.z, quatA.w / quatB.w };
}

D3DXVECTOR2 Normalize(const D3DXVECTOR2& vector2) noexcept
{
	float length = sqrt(vector2.x * vector2.x + vector2.y * vector2.y);
	return { vector2.x / length, vector2.y / length };
}
D3DXVECTOR3 Normalize(const D3DXVECTOR3& vector3) noexcept
{
	float length = sqrt(vector3.x * vector3.x + vector3.y * vector3.y + vector3.z * vector3.z);
	return { vector3.x / length, vector3.y / length, vector3.z / length };
}

float VectorLength(const D3DXVECTOR2& vector2) noexcept
{
	return std::sqrtf(std::powf(vector2.x, 2) + std::powf(vector2.y, 2));
}
float VectorLength(const D3DXVECTOR3& vector3) noexcept
{
	return std::sqrtf(std::powf(vector3.x, 2) + std::powf(vector3.y, 2) + std::powf(vector3.z, 2));
}

float VectorLengthSq(const D3DXVECTOR2& vector2) noexcept
{
	return std::powf(vector2.x, 2) + std::powf(vector2.y, 2);
}
float VectorLengthSq(const D3DXVECTOR3& vector3) noexcept
{
	return std::powf(vector3.x, 2) + std::powf(vector3.y, 2) + std::powf(vector3.z, 2);
}

// + Random ��Ʈ �ʱ�ȭ
static unsigned long wellState[16];
static unsigned int wellIndex = 0;
const static unsigned long MaxValue = (unsigned long)-1;

// WELLRNG512 ���� ������
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
