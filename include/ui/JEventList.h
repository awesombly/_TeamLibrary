#pragma once
#include "JPanel.h"
#include "Timer.h"
/*
	Event �߰��� ==
	:: Lib Panel(enum EVENT_LIST)
	:: Tool InfoEventInit() -> ComboBox String �߰�
	:: Tool Load(JParser)   -> EventBind
	:: Tool Sample() 	EVENT_LIST FindPointerFunc(wstring Event); -> Tool ������ Event ��ȯ DWORD <-> wstring
						wstring FindPointerFunc(EVENT_LIST Event); -> Tool ������ Event ��ȯ DWORD <-> wstring
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
		pPanel->m_pShape->m_vRot.z += (float)D3DXToRadian(D3DXToDegree(Timer::SPF * 1.0f)); // �ʴ� 30��
	}
}