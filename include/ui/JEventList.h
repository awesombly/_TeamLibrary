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
		Input::isDebug = false;
	}
	static void E_NOTSHOW(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		pPanel->m_bRender = false;
		Input::isDebug = true;
	}
	static void E_REVERSE_SHOW(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		pPanel->m_bRender = !pPanel->m_bRender;
		if (pPanel->m_bRender)
			Input::isDebug = false;
		else
			Input::isDebug = true;
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
				pPanel->m_pShape->m_cbData.vColor.w -= Timer::SPF / 1.0f; // 2초에 없어짐
			//else
			//	pPanel->m_bRender = false;
		}
		if (Input::GetKeyState('1') == EKeyState::DOWN ||
			pPanel->m_bEffect)
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
				pPanel->m_pShape->m_cbData.vColor.w += Timer::SPF / 1.0f; // 2초에 없어짐
			else
				pPanel->m_bRender = false;
		}
		if (Input::GetKeyState('2') == EKeyState::DOWN ||
			pPanel->m_bEffect)
		{
			pPanel->m_bRender = true;
			pPanel->m_pShape->m_cbData.vColor.w = 0.0f;
			pPanel->m_bEffect = false;
		}
	}
	static void E_EFFECT3(void* vp)
	{
		JPanel* pPanel = (JPanel*)vp;
		if (Input::GetKeyState('3') == EKeyState::DOWN ||
			pPanel->m_bEffect)
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
					pPanel->m_vScl.x -= Timer::SPF * 1000.0f; // 2초에 없어짐
				if (pPanel->m_vScl.y >= 100.0f)
					pPanel->m_vScl.y -= Timer::SPF * 1000.0f; // 2초에 없어짐
			}
			pPanel->m_vRot.z -= Timer::SPF * 50.0f;
		}
		if (Input::GetKeyState('4') == EKeyState::DOWN ||
			pPanel->m_bEffect)
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
					pPanel->m_vScl.x -= Timer::SPF * 150.0f; // 2초에 없어짐
				if (pPanel->m_vScl.y >= 25.0f)
					pPanel->m_vScl.y -= Timer::SPF * 150.0f; // 2초에 없어짐
			}
		}
		if (pPanel->m_bEffect)
		{
			pPanel->m_vScl.x = 50.0f;
			pPanel->m_vScl.y = 50.0f;
			pPanel->m_bEffect = false;
		}
	}
}