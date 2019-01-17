#pragma once
#include "uiheader.h"
namespace UI
{
	static void IntroEvent(JPanel* pRoot)
	{
		JPanel* pEffect = pRoot->find_child(L"effect_panel");
		if (pEffect == nullptr) return;
		pEffect->PreEvent.first = E_INTRO;
		pEffect->PreEvent.second = pEffect;
	}
	static void LobbyEvent(JPanel* pRoot)
	{
		JPanel* pGuest = (JPanel*)pRoot->find_child(L"Guest");
		if (pGuest == nullptr) return;
		JPanel* pGuestBtn = (JPanel*)pRoot->find_child(L"D_Guest");
		if (pGuestBtn == nullptr) return;
		pGuestBtn->EventClick.first = E_REVERSE_SHOW;
		pGuestBtn->EventClick.second = pGuest;

		JPanel* pGuestExit = (JPanel*)pRoot->find_child(L"G_Exit");
		if (pGuestExit == nullptr) return;
		pGuestExit->EventClick.first = E_NOTSHOW;
		pGuestExit->EventClick.second = pGuestExit->m_pParent;

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

		JPanel* pHelpImg2 = (JPanel*)pRoot->find_child(L"D_AutoMatching");
		if (pHelpImg2 == nullptr) return;
		pHelpImg2->PreEvent.first = E_HELP_IMG;
		pHelpImg2->PreEvent.second = pHelpImg2;

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

		JPanel* pMatchingExit = (JPanel*)pRoot->find_child(L"Matching_Exit");
		if (pMatchingExit == nullptr) return;
		pMatchingExit->EventClick.first = E_NOTSHOW;
		pMatchingExit->EventClick.second = pMatchingExit->m_pParent;

		JImageCtrl* pDrag = (JImageCtrl*)pRoot->find_child(L"Drag_Img");
		if (pDrag == nullptr) return;
		pDrag->PreEvent.first = E_DRAG;
		pDrag->PreEvent.second = pDrag;
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

		JPanel* pJohn = (JPanel*)pRoot->find_child(L"JohnSprite");
		pJohn->PreEvent.first = E_FADEOUT;
		pJohn->PreEvent.second = pJohn;

		JPanel* pCrossHair = (JPanel*)pRoot->find_child(L"CrossHair");
		if (pCrossHair == nullptr) return;
		pCrossHair->PreEvent.first = E_CROSSHAIR;
		pCrossHair->PreEvent.second = pCrossHair;

		JPanel* pFight = (JPanel*)pRoot->find_child(L"fight_panel");
		if (pFight == nullptr) return;
		pFight->PreEvent.first = E_FIGHT;
		pFight->PreEvent.second = pFight;

		JPanel* pTimeOver = (JPanel*)pRoot->find_child(L"TimeOver");
		if (pTimeOver == nullptr) return;
		pTimeOver->PreEvent.first = E_FADEINOUT;
		pTimeOver->PreEvent.second = pTimeOver;
	}
}/*
L"^frame_000_delay-0.03s.jpg
L"^frame_001_delay-0.03s.jpg
L"^frame_002_delay-0.03s.jpg
L"^frame_003_delay-0.03s.jpg
L"^frame_004_delay-0.03s.jpg
L"^frame_005_delay-0.03s.jpg
L"^frame_006_delay-0.03s.jpg
L"^frame_007_delay-0.03s.jpg
L"^frame_008_delay-0.03s.jpg
L"^frame_009_delay-0.03s.jpg
L"^frame_010_delay-0.03s.jpg
L"^frame_011_delay-0.03s.jpg
L"^frame_012_delay-0.03s.jpg
L"^frame_013_delay-0.03s.jpg
L"^frame_014_delay-0.03s.jpg
L"^frame_015_delay-0.03s.jpg
L"^frame_016_delay-0.03s.jpg
L"^frame_017_delay-0.03s.jpg
L"^frame_018_delay-0.03s.jpg
L"^frame_019_delay-0.03s.jpg
L"^frame_020_delay-0.03s.jpg
L"^frame_021_delay-0.03s.jpg
L"^frame_022_delay-0.03s.jpg
L"^frame_023_delay-0.03s.jpg
L"^frame_024_delay-0.03s.jpg
L"^frame_025_delay-0.03s.jpg
L"^frame_026_delay-0.03s.jpg
L"^frame_027_delay-0.03s.jpg
L"^frame_028_delay-0.03s.jpg
L"^frame_029_delay-0.03s.jpg
L"^frame_030_delay-0.03s.jpg
L"^frame_031_delay-0.03s.jpg
L"^frame_032_delay-0.03s.jpg
L"^frame_033_delay-0.03s.jpg
L"^frame_034_delay-0.03s.jpg
L"^frame_035_delay-0.03s.jpg
L"^frame_036_delay-0.03s.jpg
L"^frame_037_delay-0.03s.jpg
L"^frame_038_delay-0.03s.jpg
L"^frame_039_delay-0.03s.jpg
L"^frame_040_delay-0.03s.jpg
L"^frame_041_delay-0.03s.jpg
L"^frame_042_delay-0.03s.jpg
L"^frame_043_delay-0.03s.jpg
L"^frame_044_delay-0.03s.jpg
L"^frame_045_delay-0.03s.jpg
L"^frame_046_delay-0.03s.jpg
L"^frame_047_delay-0.03s.jpg
L"^frame_048_delay-0.03s.jpg
L"^frame_049_delay-0.03s.jpg
L"^frame_000_delay-0.03s.jpg
L"^frame_051_delay-0.03s.jpg
L"^frame_052_delay-0.03s.jpg
L"^frame_053_delay-0.03s.jpg
L"^frame_054_delay-0.03s.jpg
L"^frame_055_delay-0.03s.jpg
L"^frame_056_delay-0.03s.jpg
L"^frame_057_delay-0.03s.jpg
L"^frame_058_delay-0.03s.jpg
L"^frame_059_delay-0.03s.jpg
L"^frame_060_delay-0.03s.jpg
L"^frame_061_delay-0.03s.jpg
L"^frame_062_delay-0.03s.jpg
L"^frame_063_delay-0.03s.jpg
L"^frame_064_delay-0.03s.jpg
L"^frame_065_delay-0.03s.jpg
L"^frame_066_delay-0.03s.jpg
L"^frame_067_delay-0.03s.jpg
L"^frame_068_delay-0.03s.jpg
L"^frame_069_delay-0.03s.jpg
L"^frame_070_delay-0.03s.jpg
L"^frame_071_delay-0.03s.jpg
L"^frame_072_delay-0.03s.jpg
L"^frame_073_delay-0.03s.jpg
L"^frame_074_delay-0.03s.jpg
L"^frame_075_delay-0.03s.jpg
L"^frame_076_delay-0.03s.jpg
L"^frame_077_delay-0.03s.jpg
L"^frame_078_delay-0.03s.jpg
L"^frame_079_delay-0.03s.jpg
L"^frame_080_delay-0.03s.jpg
L"^frame_081_delay-0.03s.jpg
L"^frame_082_delay-0.03s.jpg
L"^frame_083_delay-0.03s.jpg
L"^frame_084_delay-0.03s.jpg
L"^frame_085_delay-0.03s.jpg
L"^frame_086_delay-0.03s.jpg
L"^frame_087_delay-0.03s.jpg
L"^frame_088_delay-0.03s.jpg
L"^frame_089_delay-0.03s.jpg
L"^frame_090_delay-0.03s.jpg
L"^frame_091_delay-0.03s.jpg
L"^frame_092_delay-0.03s.jpg
L"^frame_093_delay-0.03s.jpg
L"^frame_094_delay-0.03s.jpg
L"^frame_095_delay-0.03s.jpg
L"^frame_096_delay-0.03s.jpg
L"^frame_097_delay-0.03s.jpg
L"^frame_098_delay-0.03s.jpg
L"^frame_099_delay-0.03s.jpg
L"^frame_100_delay-0.03s.jpg
L"^frame_101_delay-0.03s.jpg
L"^frame_102_delay-0.03s.jpg
L"^frame_103_delay-0.03s.jpg
L"^frame_104_delay-0.03s.jpg
L"^frame_105_delay-0.03s.jpg
L"^frame_106_delay-0.03s.jpg
L"^frame_107_delay-0.03s.jpg
L"^frame_108_delay-0.03s.jpg
L"^frame_109_delay-0.03s.jpg
L"^frame_110_delay-0.03s.jpg
L"^frame_111_delay-0.03s.jpg
L"^frame_112_delay-0.03s.jpg
L"^frame_113_delay-0.03s.jpg
L"^frame_114_delay-0.03s.jpg
L"^frame_115_delay-0.03s.jpg
L"^frame_116_delay-0.03s.jpg
L"^frame_117_delay-0.03s.jpg
L"^frame_118_delay-0.03s.jpg
L"^frame_119_delay-0.03s.jpg
L"^frame_120_delay-0.03s.jpg
L"^frame_121_delay-0.03s.jpg
L"^frame_122_delay-0.03s.jpg
L"^frame_123_delay-0.03s.jpg
L"^frame_124_delay-0.03s.jpg
L"^frame_125_delay-0.03s.jpg
L"^frame_126_delay-0.03s.jpg
L"^frame_127_delay-0.03s.jpg
L"^frame_128_delay-0.03s.jpg
L"^frame_129_delay-0.03s.jpg
L"^frame_130_delay-0.03s.jpg
L"^frame_131_delay-0.03s.jpg
L"^frame_132_delay-0.03s.jpg
L"^frame_133_delay-0.03s.jpg
L"^frame_134_delay-0.03s.jpg
L"^frame_135_delay-0.03s.jpg
L"^frame_136_delay-0.03s.jpg
L"^frame_137_delay-0.03s.jpg
L"^frame_138_delay-0.03s.jpg
L"^frame_139_delay-0.03s.jpg
L"^frame_140_delay-0.03s.jpg
L"^frame_141_delay-0.03s.jpg
L"^frame_142_delay-0.03s.jpg
L"^frame_143_delay-0.03s.jpg
L"^frame_144_delay-0.03s.jpg
L"^frame_145_delay-0.03s.jpg
L"^frame_146_delay-0.03s.jpg
L"^frame_147_delay-0.03s.jpg
L"^frame_148_delay-0.03s.jpg
L"^frame_149_delay-0.03s.jpg*/