#pragma once
#include "JPanel.h"
#include "Input.h"
namespace UI
{
	/* �̺�Ʈ ��ü ���� �� �Լ������� ���ε��ϴ� �� */
	class JEvent : public JSingleton<JEvent>
	{
	private:
		friend class JSingleton<JEvent>;
	public:
		bool	 bEventPlay = false;
		JPanel* pSrc;
		JPanel* pDesk;
		void* vp;
	public:
		bool Frame();
		
	public:
		/* desk bRender ���� */
		JEvent* ShowPanel(UI::JPanel& src, UI::JPanel& desk);
		/* desk scl �ָ鼭 bRender ���� */
		JEvent* ShowPanelScl(UI::JPanel& src, UI::JPanel& desk);
	public:
		JEvent();
		virtual ~JEvent();
	};
}
#define I_Event JEvent::GetInstance()
#define EVENT_SHOWPANEL(s) I_Event.UI::JEvent::ShowPanel(s)
#define EVENT_SHOWPANEL_SCL(s) I_Event.UI::JEvent::ShowPanelScl(s)