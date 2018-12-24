#pragma once
#include "JThread.h"
#include "JEvent.h"
#include "queue"
namespace UI
{
	class JEventPool : public JThread , public JSingleton<JEventPool>
	{
	private:
		friend class JSingleton<JEventPool>;
	public:
		queue<JEvent*> m_pEventList;
	public:
		bool Run();
		void push_event(JEvent* pEvent);
	public:
		JEventPool();
		virtual ~JEventPool();
	};
}
#define I_EventPool JEventPool::GetInstance()