#pragma once
#include "JCS.h"
namespace UI
{
	class JSync
	{
	public:
		JCS* m_pCS;
	public:
		JSync(JCS* cs);
		virtual ~JSync();
	};

}