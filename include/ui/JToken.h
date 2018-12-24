#pragma once
#include "stdui.h"
namespace UI
{
	class JToken
	{
	public:
		char* m_pData;
		LARGE_INTEGER m_iFileSize;
		vector<string> m_pTokenList;
		string m_CurrentToken;
		//DWORD m_dwMaxTokenLine;
		//DWORD m_dwTokenIndex;
		INT m_dwMaxTokenLine;
		INT m_dwTokenIndex;
	public:
		bool LoadFile(const TCHAR* szFileName);
		const char* GetNextToken();
		INT FindToken(string szToken);
		INT GetLine(string szToken);
	public:
		JToken() {};
		virtual ~JToken() {};
	};
}