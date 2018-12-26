#pragma once
#include "stdui.h"
namespace UI
{
	class JEventdf
	{
	public:
		wstring m_pfHover;
		wstring m_pfPress;
		wstring m_pfClick;
	public:
		wstring m_vpHover;
		wstring m_vpPress;
		wstring m_vpClick;
	public:
		JEvent();
		virtual ~JEvent();
	};
}