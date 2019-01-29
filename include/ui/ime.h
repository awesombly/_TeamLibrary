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
		//wstring m_Text; // �ؽ�Ʈ ���� ����
		wstring m_CompString; // ���� �� ���ڿ�
		wstring m_CompWord; // ������ ����
		HWND m_hWnd = NULL;
		HIMC m_hIMC = NULL;
		int m_iMaxLength = 0;
		bool m_bPlay = false;
	public:
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