#pragma once
#include "stdui.h"
namespace UI
{
	class JCS
	{
	public:
		CRITICAL_SECTION m_cs;
	public:
		JCS();
		virtual ~JCS();
	};

}