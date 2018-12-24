#pragma once
#include "JPanel.h"
#include "Input.h"
namespace UI
{
	/* 이벤트 객체 생성 및 함수포인터 바인딩하는 곳 */
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
		/* desk bRender 반전 */
		JEvent* ShowPanel(UI::JPanel& src, UI::JPanel& desk);
		/* desk scl 주면서 bRender 반전 */
		JEvent* ShowPanelScl(UI::JPanel& src, UI::JPanel& desk);
	public:
		JEvent();
		virtual ~JEvent();
	};
}
#define I_Event JEvent::GetInstance()
#define EVENT_SHOWPANEL(s) I_Event.UI::JEvent::ShowPanel(s)
#define EVENT_SHOWPANEL_SCL(s) I_Event.UI::JEvent::ShowPanelScl(s)