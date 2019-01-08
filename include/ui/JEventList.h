#pragma once
#include "JPanel.h"
#include "Timer.h"
/*
	Event 추가시 ==
	:: Lib Panel(enum EVENT_LIST)
	:: Tool InfoEventInit() -> ComboBox String 추가
	:: Tool Load(JParser)   -> EventBind
	:: Tool Sample() 	EVENT_LIST FindPointerFunc(wstring Event); -> Tool 내에서 Event 변환 DWORD <-> wstring
						wstring FindPointerFunc(EVENT_LIST Event); -> Tool 내에서 Event 변환 DWORD <-> wstring
*/
namespace UI
{
	static void E_SHOW(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		pPanel->m_bRender = true;
	}
	static void E_NOTSHOW(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		pPanel->m_bRender = false;
	}
	static void E_REVERSE_SHOW(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		pPanel->m_bRender = !pPanel->m_bRender;
	}
	static void E_ROTATEZ(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		pPanel->m_pShape->m_vRot.z += (float)D3DXToRadian(D3DXToDegree(Timer::SPF * 1.0f)); 
	}
	static void E_SHOW_SCL(void* vp)
	{
		/* SRT가 1 인 부모격 Panel만 넣으세용 */
		JPanel* pPanel = (JPanel*)vp;

		pPanel->m_bRender = true;
		pPanel->PreEvent.first = E_SHOW_SCL;
		pPanel->PreEvent.second = pPanel;
		pPanel->m_vScl.x += Timer::SPF * 4.0f;
		pPanel->m_vScl.y += Timer::SPF * 4.0f;
		if (pPanel->m_vScl.x >= 1.0f || pPanel->m_vScl.y >= 1.0f)
		{
			pPanel->m_vScl.x = 1.0f;
			pPanel->m_vScl.y = 1.0f;
			pPanel->PreEvent.first = nullptr;
			pPanel->PreEvent.second = nullptr;
		}
	}
	static void E_NOTSHOW_SCL(void* vp)
	{
		/* SRT가 1 인 부모격 Panel만 넣으세용 */
		JPanel* pPanel = (JPanel*)vp;

		pPanel->PreEvent.first = E_NOTSHOW_SCL;
		pPanel->PreEvent.second = pPanel;
		pPanel->m_vScl.x -= Timer::SPF * 4.0f;
		pPanel->m_vScl.y -= Timer::SPF * 4.0f;
		if (pPanel->m_vScl.x <= 0.0f || pPanel->m_vScl.y <= 0.0f)
		{
			pPanel->m_bRender = false;
			pPanel->m_vScl.x = 0.0f;
			pPanel->m_vScl.y = 0.0f;
			pPanel->PreEvent.first = nullptr;
			pPanel->PreEvent.second = nullptr;
		}
	}
}