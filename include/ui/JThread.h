#pragma once
#include "JCS.h"
namespace UI
{
	class JThread : public JCS
	{
	public:
		unsigned int m_hThread;
		unsigned int m_dwThreadID;
		bool		 m_bStarted;
	public:
		void CreateThread();
		virtual bool Run();
		static unsigned int WINAPI HandleRunner(LPVOID parameter);
	public:
		JThread();
		virtual ~JThread();
	};
}