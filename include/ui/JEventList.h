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
	static void SHOW_PANEL_SCL(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		D3DXVECTOR3 vCurScl = pPanel->m_pCurTransform->m_vScl;
		/* ������ */
		if (pPanel->m_bRender)
		{
			if( pPanel->m_vScl.x >= 0.1f || 
				pPanel->m_vScl.y >= 0.1f)
			{
				if (pPanel->m_vScl.x >= 0.1f)
					pPanel->m_vScl.x -= Timer::SPF * vCurScl.x;
				if( pPanel->m_vScl.y >= 0.1f)
					pPanel->m_vScl.y -= Timer::SPF * vCurScl.y;
			}
			else
			{
				pPanel->m_vScl -= D3DXVECTOR3(0.1f, 0.1f, 0);
				pPanel->m_bRender = false;
			}
		}
		/* ������ */
		else
		{
			if ( pPanel->m_vScl.x <= vCurScl.x ||
				 pPanel->m_vScl.y <= vCurScl.y)
			{
				/* �ʴ� 10�� */
				if (pPanel->m_vScl.x < vCurScl.x)
					pPanel->m_vScl.x += Timer::SPF * vCurScl.x;
				if (pPanel->m_vScl.y < vCurScl.y)
					pPanel->m_vScl.y += Timer::SPF * vCurScl.y;
			}
			else
			{
				pPanel->m_vScl += D3DXVECTOR3(0.1f, 0.1f, 0);
				pPanel->m_bRender = true;
			}
		}
	}
}