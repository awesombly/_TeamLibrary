#pragma once
#include "uiheader.h"
namespace UI
{
	class JUIReader
	{
	public:
		FILE* m_pStream;
		wstring m_strFileName; // 출력 파일명
	public:
		bool RunBinaryRead(JPanel* pRoot, wstring ReadFileName);
	public:
		JUIReader() {}
		virtual ~JUIReader() {}
	};
}