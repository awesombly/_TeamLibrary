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
	static void E_SHOW_SCL(void* vp)
	{
		/* SRT�� 1 �� �θ�� Panel�� �������� */
		JPanel* pPanel = (JPanel*)vp;

		pPanel->m_bRender = true;
		pPanel->EventDefault.first = E_SHOW_SCL;
		pPanel->EventDefault.second = pPanel;
		pPanel->m_vScl.x += Timer::SPF * 4.0f;
		pPanel->m_vScl.y += Timer::SPF * 4.0f;
		if (pPanel->m_vScl.x >= 1.0f || pPanel->m_vScl.y >= 1.0f)
		{
			pPanel->m_vScl.x = 1.0f;
			pPanel->m_vScl.y = 1.0f;
			pPanel->EventDefault.first = nullptr;
			pPanel->EventDefault.second = nullptr;
		}
	}
	static void E_NOTSHOW_SCL(void* vp)
	{
		/* SRT�� 1 �� �θ�� Panel�� �������� */
		JPanel* pPanel = (JPanel*)vp;

		pPanel->EventDefault.first = E_NOTSHOW_SCL;
		pPanel->EventDefault.second = pPanel;
		pPanel->m_vScl.x -= Timer::SPF * 4.0f;
		pPanel->m_vScl.y -= Timer::SPF * 4.0f;
		if (pPanel->m_vScl.x <= 0.0f || pPanel->m_vScl.y <= 0.0f)
		{
			pPanel->m_bRender = false;
			pPanel->m_vScl.x = 0.0f;
			pPanel->m_vScl.y = 0.0f;
			pPanel->EventDefault.first = nullptr;
			pPanel->EventDefault.second = nullptr;
		}
	}
}