#pragma once
#include "uiheader.h"
namespace UI
{
	static void IntroEvent(JPanel* pRoot)
	{
		JPanel* pMouseParticle = pRoot->find_child(L"mouse_particle");
		if (pMouseParticle == nullptr) return;
		pMouseParticle->PreEvent.first = UI::E_MOUSE_PARTICLE;
		pMouseParticle->PreEvent.second = pMouseParticle;

		JPanel* pMouseCursor = pRoot->find_child(L"mouse_cursor");
		if (pMouseCursor == nullptr) return;
		pMouseCursor->PreEvent.first = UI::E_MOUSE_CURSOR;
		pMouseCursor->PreEvent.second = pMouseCursor;

		JSpriteCtrl* pCursorClick = (JSpriteCtrl*)pRoot->find_child(L"mouse_click_sprite");
		if (pCursorClick == nullptr) return;
		pCursorClick->PreEvent.first = E_MOUSE_CLICK_SPRITE;
		pCursorClick->PreEvent.second = pCursorClick;

		JSpriteCtrl* pCursorErase = (JSpriteCtrl*)pRoot->find_child(L"mouse_click_sprite_panel");
		if (pCursorErase == nullptr) return;
		pCursorErase->PreEvent.first = E_MOUSE_SPRITE_ERASE;
		pCursorErase->PreEvent.second = pCursorErase;

		JPanel* pHelpImg = (JPanel*)pRoot->find_child(L"D_Host");
		if (pHelpImg == nullptr) return;
		pHelpImg->PreEvent.first = E_HELP_IMG;
		pHelpImg->PreEvent.second = pHelpImg;

		// effect
		JPanel* pEff1 = (JPanel*)pRoot->find_child(L"fadeout");
		if (pEff1 == nullptr) return;
		pEff1->PreEvent.first = E_FADEOUT;
		pEff1->PreEvent.second = pEff1;

		JPanel* pEff2 = (JPanel*)pRoot->find_child(L"fadein");
		if (pEff2 == nullptr) return;
		pEff2->PreEvent.first = E_FADEIN;
		pEff2->PreEvent.second = pEff2;

		JPanel* pEff4 = (JPanel*)pRoot->find_child(L"effect_hos");
		if (pEff4 == nullptr) return;
		pEff4->PreEvent.first = E_HOS;
		pEff4->PreEvent.second = pEff4;

		JPanel* pMatching_Panel = (JPanel*)pRoot->find_child(L"Matching_Panel");
		JPanel* AutoMatching = (JPanel*)pRoot->find_child(L"D_AutoMatching");
		AutoMatching->EventClick.first = E_REVERSE_SHOW;
		AutoMatching->EventClick.second = pMatching_Panel;

		JPanel* pLoginBtn = (JPanel*)pRoot->find_child(L"Matching_Login");
		JPanel* pMatching_Loading = (JPanel*)pRoot->find_child(L"Matching_Loading_Panel");
		if (pLoginBtn == nullptr) return;
		pLoginBtn->EventClick.first = E_LOGIN_BUTTON;
		pLoginBtn->EventClick.second = pMatching_Loading;
	}
	static void LobbyEvent(JPanel* pRoot)
	{

	}
	static void InGameEvent(JPanel* pRoot)
	{
		JPanel* pMouseCursor = pRoot->find_child(L"mouse_cursor");
		if (pMouseCursor == nullptr) return;
		pMouseCursor->PreEvent.first = UI::E_MOUSE_CURSOR;
		pMouseCursor->PreEvent.second = pMouseCursor;

		JSpriteCtrl* pCursorClick = (JSpriteCtrl*)pRoot->find_child(L"mouse_click_sprite");
		if (pCursorClick == nullptr) return;
		pCursorClick->PreEvent.first = E_MOUSE_CLICK_SPRITE;
		pCursorClick->PreEvent.second = pCursorClick;

		JSpriteCtrl* pCursorErase = (JSpriteCtrl*)pRoot->find_child(L"mouse_click_sprite_panel");
		if (pCursorErase == nullptr) return;
		pCursorErase->PreEvent.first = E_MOUSE_SPRITE_ERASE;
		pCursorErase->PreEvent.second = pCursorErase;

		JPanel* pStatePanel = (JPanel*)pRoot->find_child(L"State_Panel");
		if (pStatePanel == nullptr) return;
		pStatePanel->PreEvent.first = E_STATE_SHOW;
		pStatePanel->PreEvent.second = pStatePanel;

		JPanel* pSetting = (JPanel*)pRoot->find_child(L"Set_Panel");
		pSetting->PreEvent.first = E_SETTING_SHOW;
		pSetting->PreEvent.second = pSetting;

		JSliderCtrl* pChatSlider = (JSliderCtrl*)pRoot->find_child(L"Chat_Slider");
		JListCtrl* pList = (JListCtrl*)pRoot->find_child(L"Chat_Log");
		pList->m_fValue = &pChatSlider->m_fValue;

		// effect
		JPanel* pEff1 = (JPanel*)pRoot->find_child(L"fadeout");
		if (pEff1 == nullptr) return;
		pEff1->PreEvent.first = E_FADEOUT;
		pEff1->PreEvent.second = pEff1;

		JPanel* pEff3 = (JPanel*)pRoot->find_child(L"fadeout_white");
		if (pEff3 == nullptr) return;
		pEff3->PreEvent.first = E_FADEOUT;
		pEff3->PreEvent.second = pEff3;

		JPanel* pEff2 = (JPanel*)pRoot->find_child(L"fadein");
		if (pEff2 == nullptr) return;
		pEff2->PreEvent.first = E_FADEIN;
		pEff2->PreEvent.second = pEff2;
	}
}