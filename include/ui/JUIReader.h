#pragma once
#include "uiheader.h"
namespace UI
{
	class JUIReader
	{
	public:
		FILE* m_pStream;
		wstring m_strFileName; // ��� ���ϸ�
	public:
		bool RunBinaryRead(JPanel* pRoot, wstring ReadFileName);
	public:
		JUIReader() {}
		virtual ~JUIReader() {}
	};
}