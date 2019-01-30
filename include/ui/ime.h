#pragma once
#include "stdui.h"
namespace UI
{
	class ime
	{
	public:
		static ime* Get()
		{
			if (m_pInstance == nullptr)
			{
				m_pInstance = new ime();
			}
			return m_pInstance;
		}
	private:
		static ime* m_pInstance;
		//wstring m_Text; // 텍스트 한줄 저장
		wstring m_CompString; // 조합 중 문자열
		wstring m_CompWord; // 조합중 문자
		HWND m_hWnd = NULL;
		HIMC m_hIMC = NULL;
		int m_iMaxLength = 0;
		bool m_bPlay = false;
	public:
		bool GetPlay();
		void imeStart();
		void imeEnd();	
		wstring GetString();
		void SetLength(const int iLength);
		void SetString(wstring str);
		bool Set(HWND hWnd);
	public:
		int WndProc(MSG msg);
	public:
		ime();
		virtual ~ime();
	};
}