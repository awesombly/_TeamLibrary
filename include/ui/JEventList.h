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
		//Input::isDebug = false;
	}
	static void E_NOTSHOW(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		pPanel->m_bRender = false;
		//Input::isDebug = true;
	}
	static void E_REVERSE_SHOW(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		pPanel->m_bRender = !pPanel->m_bRender;
		//if (pPanel->m_bRender)
		//	Input::isDebug = false;
		//else
		//	Input::isDebug = true;
	}
	static void E_ROTATEZ(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		pPanel->m_pShape->m_vRot.z += (float)D3DXToRadian(D3DXToDegree(Timer::SPF * 1.0f)); 
	}
	static void E_SHOW_SCL(void* vp)
	{
		/* SRT�� 1 �� �θ�� Panel�� �������� */
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
		/* SRT�� 1 �� �θ�� Panel�� �������� */
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
	static void E_MOUSE_PARTICLE(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;

		pPanel->m_vPos.x = pPanel->m_ptMouse.Getpt().x + 10.0f;
		pPanel->m_vPos.y = pPanel->m_ptMouse.Getpt().y - 10.0f;
	}
	static void E_MOUSE_CURSOR(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;

		pPanel->m_vPos.x = pPanel->m_ptMouse.Getpt().x + 10.0f;
		pPanel->m_vPos.y = pPanel->m_ptMouse.Getpt().y - 10.0f;
	}
	static void E_LOGIN_BUTTON(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		pPanel->m_bRender = true;

		JPanel* pSrc = pPanel->find_root()->find_child(L"Matching_Panel");
		pSrc->m_bRender = false;
	}
	// mouse click sprite clone create ==============================================================
	static void E_MOUSE_SPRITE_CLONE(void* vp)
	{
		JSpriteCtrl* pSprite = (JSpriteCtrl*)vp;
		pSprite->m_vScl.x += Timer::SPF * 30;
		pSprite->m_vScl.y += Timer::SPF * 30;
	}
	static void E_MOUSE_CLICK_SPRITE(void* vp)
	{
		if (Input::GetKeyState(EMouseButton::Left) == EKeyState::DOWN)
		{
			JSpriteCtrl* pSprite = (JSpriteCtrl*)vp;
			JSpriteCtrl* pTemp = (JSpriteCtrl*)pSprite->uiclone();
			pTemp->m_NodeName += L"_clone";
			pTemp->m_bPlay = true;
			//pTemp->m_bLoop = false;
			pTemp->PreEvent.first = E_MOUSE_SPRITE_CLONE;
			pTemp->PreEvent.second = pTemp;
			//pTemp->PostEvent.first = nullptr;
			//pTemp->EventHover.first = nullptr;
			//pTemp->EventClick.first = nullptr;
			//pTemp->EventPress.first = nullptr;

			pTemp->m_vPos.x = pTemp->m_ptMouse.Getpt().x;
			pTemp->m_vPos.y = pTemp->m_ptMouse.Getpt().y;

			pSprite->m_pParent->push_child(pTemp);
		}
	}
	static void E_MOUSE_SPRITE_ERASE(void* vp)
	{
		JPanel* pParent = (JPanel*)vp;
		for (auto iC = pParent->m_pChildList.begin(); iC != pParent->m_pChildList.end(); iC++)
		{
			JSpriteCtrl* pSprite = (JSpriteCtrl*)*iC;
			if (!pSprite->m_bPlay &&
				pParent->m_pChildList.begin() != iC &&
				++pParent->m_pChildList.begin() != iC)
			{
				pParent->m_pChildList.erase(iC);
				SAFE_DELETE(pSprite);
				break;
			}
		}
	}
	static void E_HELP_IMG(void* vp)
	{
		JPanel* pHost = (JPanel*)vp;

		JPanel* pRoot = pHost->find_root();
		JButtonCtrl* pHelp = (JButtonCtrl*)pRoot->find_child(L"Helptxt");

		if (pHost->m_pShape->Hovered(pHost->m_rt, pHost->m_ptMouse.Getpt()))
		{
			pHelp->m_vPos.x = pHost->m_ptMouse.Getpt().x + pHelp->m_vScl.x;
			pHelp->m_vPos.y = pHost->m_ptMouse.Getpt().y;
			pHelp->m_bRender = true;
		}
		else
		{
			pHelp->m_bRender = false;
		}
	}
	// InGame State_Panel
	static void E_STATE_SHOW(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		if (Input::GetKeyState(pPanel->m_pKeyHold) == EKeyState::HOLD)
		{
			pPanel->m_bRender = true;
		}
		else
		{
			pPanel->m_bRender = false;
		}
	}
	static void E_SETTING_SHOW(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		JPanel* pRoot = pPanel->find_root();
		JPanel* pMouse = pRoot->find_child(L"mouse_click_sprite_panel");
		if (Input::GetKeyState(pPanel->m_pKeyDown) == EKeyState::DOWN)
		{
			pPanel->m_bRender = !pPanel->m_bRender;
		}
		if (pPanel->m_bRender)
			pMouse->m_bRender = true;
		else
			pMouse->m_bRender = false;
	}
	// ui effect
	static void E_FADEOUT(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;

		if (pPanel->m_bRender)
		{
			if (pPanel->m_pShape->m_cbData.vColor.w >= 0.0f)
				pPanel->m_pShape->m_cbData.vColor.w -= Timer::SPF / 1.0f; // 2�ʿ� ������
			//else
			//	pPanel->m_bRender = false;
		}
		if (pPanel->m_bEffect)
		{
			pPanel->m_bRender = true;
			pPanel->m_pShape->m_cbData.vColor.w = 1.0f;

			pPanel->m_bEffect = false;
		}
	}
	static void E_FADEIN(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;

		if (pPanel->m_bRender)
		{
			if (pPanel->m_pShape->m_cbData.vColor.w <= 1.0f)
				pPanel->m_pShape->m_cbData.vColor.w += Timer::SPF / 1.0f; // 2�ʿ� ������
			else
				pPanel->m_bRender = false;
		}
		if (pPanel->m_bEffect)
		{
			pPanel->m_bRender = true;
			pPanel->m_pShape->m_cbData.vColor.w = 0.0f;
			pPanel->m_bEffect = false;
		}
	}
	static void E_FADEINOUT(void* vp)
	{
		JPanel* pChild = (JPanel*)vp;
		if (!pChild->m_bRender) return;
		pChild->m_fUITimer += Timer::SPF;
		if (pChild->m_fUITimer >= 0.0f && pChild->m_fUITimer <= 1.0f)
		{
			if (pChild->m_pShape->m_cbData.vColor.w <= 1.0f)
				pChild->m_pShape->m_cbData.vColor.w += Timer::SPF * 2.0f;
		}
		else if (pChild->m_fUITimer >= 2.0f && pChild->m_fUITimer <= 3.0f)
		{
			if (pChild->m_pShape->m_cbData.vColor.w >= 0.0f)
				pChild->m_pShape->m_cbData.vColor.w -= Timer::SPF * 2.0f;
		}
	}
	static void E_EFFECT3(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		if (pPanel->m_bEffect)
		{
			pPanel->m_bRender = !pPanel->m_bRender;
			pPanel->m_bEffect = false;
		}
	}
	static void E_HOS(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		if (pPanel->m_bRender)
		{
			if (pPanel->m_vScl.x >= 100.0f || pPanel->m_vScl.y >= 100.0f)
			{
				if (pPanel->m_vScl.x >= 100.0f)
					pPanel->m_vScl.x -= Timer::SPF * 1000.0f; // 2�ʿ� ������
				if (pPanel->m_vScl.y >= 100.0f)
					pPanel->m_vScl.y -= Timer::SPF * 1000.0f; // 2�ʿ� ������
			}
			pPanel->m_vRot.z -= Timer::SPF * 50.0f;
		}
		if (pPanel->m_bEffect)
		{
			pPanel->m_bRender = !pPanel->m_bRender;
			if (pPanel->m_bRender)
			{
				pPanel->m_vScl.x = 500.0f;
				pPanel->m_vScl.y = 500.0f;
			}
			pPanel->m_bEffect = false;
		}
	}
	static void E_CROSSHAIR(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		if (Input::GetKeyState(EMouseButton::Left) == EKeyState::DOWN)
		{
			pPanel->EffectPlay();
		}
		if (!pPanel->m_bEffect)
		{
			if (pPanel->m_vScl.x >= 25.0f || pPanel->m_vScl.y >= 25.0f)
			{
				if (pPanel->m_vScl.x >= 25.0f)
					pPanel->m_vScl.x -= Timer::SPF * 150.0f; // 2�ʿ� ������
				if (pPanel->m_vScl.y >= 25.0f)
					pPanel->m_vScl.y -= Timer::SPF * 150.0f; // 2�ʿ� ������
			}
		}
		if (pPanel->m_bEffect)
		{
			pPanel->m_vScl.x = 50.0f;
			pPanel->m_vScl.y = 50.0f;
			pPanel->m_bEffect = false;
		}
	}
	static POINT pttemp;
	static void E_DRAG(void* vp)
	{
		JImageCtrl* pImg = (JImageCtrl*)vp;
	//	JPanel* pParent = pImg->m_pParent;

		if (Input::GetKeyState(EMouseButton::Left) == EKeyState::DOWN)
		{
			//pParent->m_bRender = true;
			pttemp.x = pImg->m_ptMouse.Getpt().x;
			pttemp.y = pImg->m_ptMouse.Getpt().y;
		}
		if (Input::GetKeyState(EMouseButton::Left) == EKeyState::HOLD)
		{
			pImg->m_vScl.x = (pImg->m_ptMouse.Getpt().x - pttemp.x) / 2.0f;
			pImg->m_vScl.y = (pImg->m_ptMouse.Getpt().y - pttemp.y) / 2.0f;

			pImg->m_vPos.x = pttemp.x + pImg->m_vScl.x;
			pImg->m_vPos.y = pttemp.y + pImg->m_vScl.y;
		}
		if (Input::GetKeyState(EMouseButton::Left) == EKeyState::UP)
		{
			//pParent->m_bRender = false;
			pImg->m_vScl.x = 0;
			pImg->m_vScl.y = 0;
		}
	}
	static void E_FIGHT(void* vp)
	{
		JPanel* pParent = (JPanel*)vp;
		if (!pParent->m_bRender) return;
		pParent->m_fUITimer += Timer::SPF;
		if (pParent->m_fUITimer >= 0.0f && pParent->m_fUITimer <= 1.0f)
		{
			JPanel* pChild = pParent->find_child(L"fight_3");
			pChild->m_bRender = true;
			if (pChild->m_vScl.x >= 30.0f || pChild->m_vScl.y >= 40.0f)
			{
				if (pChild->m_vScl.x >= 30.0f)
					pChild->m_vScl.x -= Timer::SPF * 1500.0f; // 2�ʿ� ������
				if (pChild->m_vScl.y >= 40.0f)
					pChild->m_vScl.y -= Timer::SPF * 1500.0f; // 2�ʿ� ������
			}
		}
		else if (pParent->m_fUITimer >= 1.0f && pParent->m_fUITimer <= 2.0f)
		{
			JPanel* pRender = pParent->find_child(L"fight_3");
			pRender->m_bRender = false;
			JPanel* pChild = pParent->find_child(L"fight_2");
			pChild->m_bRender = true;
			if (pChild->m_vScl.x >= 30.0f || pChild->m_vScl.y >= 40.0f)
			{
				if (pChild->m_vScl.x >= 30.0f)
					pChild->m_vScl.x -= Timer::SPF * 1500.0f; // 2�ʿ� ������
				if (pChild->m_vScl.y >= 40.0f)
					pChild->m_vScl.y -= Timer::SPF * 1500.0f; // 2�ʿ� ������
			}
		}
		else if (pParent->m_fUITimer >= 2.0f && pParent->m_fUITimer <= 3.0f)
		{
			JPanel* pRender = pParent->find_child(L"fight_2");
			pRender->m_bRender = false;
			JPanel* pChild = pParent->find_child(L"fight_1");
			pChild->m_bRender = true;
			if (pChild->m_vScl.x >= 30.0f || pChild->m_vScl.y >= 40.0f)
			{
				if (pChild->m_vScl.x >= 30.0f)
					pChild->m_vScl.x -= Timer::SPF * 1500.0f; // 2�ʿ� ������
				if (pChild->m_vScl.y >= 40.0f)
					pChild->m_vScl.y -= Timer::SPF * 1500.0f; // 2�ʿ� ������
			}
		}
		else if (pParent->m_fUITimer >= 3.0f && pParent->m_fUITimer <= 5.0f)
		{
			JPanel* pRender = pParent->find_child(L"fight_1");
			pRender->m_bRender = false;
			JPanel* pChild = pParent->find_child(L"Fight");
			pChild->m_bRender = true;
			if (pChild->m_pShape->m_cbData.vColor.w <= 1.0f)
				pChild->m_pShape->m_cbData.vColor.w += Timer::SPF * 2.0f;
		}
		else if (pParent->m_fUITimer >= 5.0f && pParent->m_fUITimer <= 7.0f)
		{
			JPanel* pChild = pParent->find_child(L"Fight");
			if (pChild->m_pShape->m_cbData.vColor.w >= 0.0f)
				pChild->m_pShape->m_cbData.vColor.w -= Timer::SPF * 2.0f;
		}
		else if (pParent->m_fUITimer >= 7.0f)
		{
			JPanel* pChild = pParent->find_child(L"Fight");
			pChild->m_bRender = false;
		}
	}
	static void E_INTRO(void* vp)
	{
		JPanel* pParent = (JPanel*)vp;
		if (Input::GetKeyState(EMouseButton::Left) == EKeyState::DOWN)
		{
			pParent->m_bRender = true;
			pParent->find_root()->find_child(L"click")->m_bRender = false;
		}

		if (!pParent->m_bRender) return;
		pParent->m_fUITimer += Timer::SPF;
		//if (pParent->m_fUITimer >= 0.0f && pParent->m_fUITimer <= 0.8f)
		//{
		//	JPanel* pChild = pParent->find_child(L"left");
		//	if (pChild->m_vPos.x >= 0.0f)
		//	{
		//		pChild->m_vPos.x = 0.0f;
		//		return;
		//	}
		//	pChild->m_vPos.x += Timer::SPF * 800.0f * 3.0f;
		//}
		//else if (pParent->m_fUITimer >= 0.8f && pParent->m_fUITimer <= 1.6f)
		//{
		//	JPanel* pChild = pParent->find_child(L"top");
		//	if (pChild->m_vPos.y <= 0.0f)
		//	{
		//		pChild->m_vPos.y = 0.0f;
		//		return;
		//	}
		//	pChild->m_vPos.y -= Timer::SPF * 600.0f * 3.0f;
		//}
		//else if (pParent->m_fUITimer >= 1.6f && pParent->m_fUITimer <= 2.4f)
		//{
		//	JPanel* pChild = pParent->find_child(L"right");
		//	if (pChild->m_vPos.x <= 0.0f)
		//	{
		//		pChild->m_vPos.x = 0.0f;
		//		return;
		//	}
		//	pChild->m_vPos.x -= Timer::SPF * 800.0f * 3.0f;
		//}
		//else if (pParent->m_fUITimer >= 2.4f && pParent->m_fUITimer <= 3.2f)
		//{
		//	JPanel* pChild = pParent->find_child(L"bottom");
		//	if (pChild->m_vPos.y >= 0.0f)
		//	{
		//		pChild->m_vPos.y = 0.0f;
		//		return;
		//	}
		//	pChild->m_vPos.y += Timer::SPF * 600.0f * 3.0f;
		//}
		//else if (pParent->m_fUITimer >= 3.2f && pParent->m_fUITimer <= 4.0f)
		//{
		//	JPanel* pChild = pParent->find_child(L"left2");
		//	if (pChild->m_vPos.x >= 0.0f)
		//	{
		//		pChild->m_vPos.x = 0.0f;
		//		return;
		//	}
		//	pChild->m_vPos.x += Timer::SPF * 800.0f * 3.0f;
		//}
		if (pParent->m_fUITimer >= 0.0f && pParent->m_fUITimer <= 1.0f)
		{
			JPanel* pChild = pParent->find_child(L"fadein");
			if (pChild->m_pShape->m_cbData.vColor.w <= 1.0f)
				pChild->m_pShape->m_cbData.vColor.w += Timer::SPF * 2.0f;
		}
		else if (pParent->m_fUITimer >= 1.0f && pParent->m_fUITimer <= 2.5f)
		{
			JPanel* pParticle = pParent->find_child(L"snow_particle");
			pParticle->m_bRender = true;
			JPanel* pChild = pParent->find_child(L"title");
			pChild->m_bRender = true;
			if (pChild->m_vScl.x >= 150.0f || pChild->m_vScl.y >= 50.0f)
			{
				if (pChild->m_vScl.x >= 150.0f)
					pChild->m_vScl.x -= Timer::SPF * 2000.0f; // 2�ʿ� ������
				if (pChild->m_vScl.y >= 50.0f)
					pChild->m_vScl.y -= Timer::SPF * 2000.0f; // 2�ʿ� ������
			}
			if (pChild->m_vScl.x <= 150.0f || pChild->m_vScl.y <= 50.0f)
			{
				if (pChild->m_vScl.x <= 150.0f)
					pChild->m_vScl.x = 150.0f;
				if (pChild->m_vScl.y <= 50.0f)
					pChild->m_vScl.y = 50.0f;
			}
		}
		else if (pParent->m_fUITimer >= 2.5f && pParent->m_fUITimer <= 3.5f)
		{
			JPanel* pChild = pParent->find_child(L"teamname");
			if (pChild->m_pShape->m_cbData.vColor.w <= 1.0f)
				pChild->m_pShape->m_cbData.vColor.w += Timer::SPF * 2.0f;
		}
		else if (pParent->m_fUITimer >= 3.5f && pParent->m_fUITimer <= 4.5f)
		{
			JPanel* pChild = pParent->find_child(L"team");
			if (pChild->m_pShape->m_cbData.vColor.w <= 1.0f)
				pChild->m_pShape->m_cbData.vColor.w += Timer::SPF * 2.0f;
		}
		else if (pParent->m_fUITimer >= 4.5f && pParent->m_fUITimer <= 5.5f)
		{
			JPanel* pChild = pParent->find_child(L"gamestart");
			pChild->m_bRender = true;
			if (pChild->m_pShape->m_cbData.vColor.w <= 1.0f)
				pChild->m_pShape->m_cbData.vColor.w += Timer::SPF * 2.0f;

			JSpriteCtrl* pCursor = (JSpriteCtrl*)pParent->find_child(L"cursor");
			if (pCursor == nullptr) return;
			pCursor->m_bRender = true;
			pCursor->PreEvent.first = E_MOUSE_CURSOR;
			pCursor->PreEvent.second = pCursor;
		}
	}
}