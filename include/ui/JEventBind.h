#pragma once
#include "uiheader.h"
#include "LoadingParameter.h"
#include <Thread>

namespace UI
{
	static bool Loading(const JPanel* pRoot)
	{
		if (pRoot == nullptr) return false;

		LoadClass::LoadingCount += 1;
		LoadClass::LoadingString = pRoot->m_NodeName + L"(Name)";
		this_thread::sleep_for(std::chrono::milliseconds(5));

		LoadClass::LoadingCount += 1;
		LoadClass::LoadingString = pRoot->m_NodeName + L"(Parent)";
		this_thread::sleep_for(std::chrono::milliseconds(5));

		LoadClass::LoadingCount += 1;
		LoadClass::LoadingString = pRoot->m_NodeName + L"(Transform)";
		this_thread::sleep_for(std::chrono::milliseconds(5));

		LoadClass::LoadingCount += 1;
		LoadClass::LoadingString = pRoot->m_NodeName + L"(Type)";
		this_thread::sleep_for(std::chrono::milliseconds(5));

		LoadClass::LoadingCount += 1;
		LoadClass::LoadingString = pRoot->m_NodeName + L"(Texture)";
		this_thread::sleep_for(std::chrono::milliseconds(5));

		LoadClass::LoadingCount += 1;
		LoadClass::LoadingString = pRoot->m_NodeName + L"(Event)";
		this_thread::sleep_for(std::chrono::milliseconds(5));


		for (auto iC = pRoot->m_pChildList.begin(); iC != pRoot->m_pChildList.end(); iC++)
		{
			Loading(*iC);
		}

		return true;
	}
	static void IntroEvent(JPanel* pRoot)
	{			   
		JPanel* pMouseCursor = pRoot->find_child(L"mouse_cursor");
		pMouseCursor->PreEvent.first = UI::E_MOUSE_CURSOR;
		pMouseCursor->PreEvent.second = pMouseCursor;

		JPanel* pGameStart = pRoot->find_child(L"gamestart");
		JPanel* pLogin = pRoot->find_child(L"Login_Panel");
		pGameStart->EventClick.first = E_REVERSE_SHOW;
		pGameStart->EventClick.second = pLogin;

		JPanel* pSignUp = (JPanel*)pRoot->find_child(L"Login_SignUp");
		JPanel* pSignUpPanel = (JPanel*)pRoot->find_child(L"SignUp_Panel");
		pSignUp->EventClick.first = E_REVERSE_SHOW;
		pSignUp->EventClick.second = pSignUpPanel;

		JPanel* pHelpText = (JPanel*)pRoot->find_child(L"Help_txt");
		static auto E_HelpText = [](void* vp)
		{
			JPanel* pPanel = (JPanel*)vp;

			if (pPanel->m_bRender)
				pPanel->m_fUITimer += Timer::SPF;

			if (pPanel->m_fUITimer >= 3.0f)
			{
				pPanel->m_bRender = false;
				pPanel->m_fUITimer = 0.0f;
			}
		};
		pHelpText->PreEvent.first = E_HelpText;
		pHelpText->PreEvent.second = pHelpText;

		JPanel* pSkeleton = (JPanel*)pRoot->find_child(L"SkeletonFront");
		static auto E_REVERSE_ALPHA = [](void* vp)
		{
			JPanel* pPanel = (JPanel*)vp;
			static bool bAlpha = false;
			if (!bAlpha)
			{
				pPanel->m_pShape->m_cbData.vColor.w -= Timer::SPF;
				if (pPanel->m_pShape->m_cbData.vColor.w <= 0.0f)
					bAlpha = true;
			}
			else
			{
				pPanel->m_pShape->m_cbData.vColor.w += Timer::SPF;
				if (pPanel->m_pShape->m_cbData.vColor.w >= 1.0f)
					bAlpha = false;
			}
		};
		pSkeleton->PreEvent.first = E_REVERSE_ALPHA;
		pSkeleton->PreEvent.second = pSkeleton;
	}
	static void LobbyEvent(JPanel* pRoot)
	{
		JPanel* pGuest = (JPanel*)pRoot->find_child(L"Guest");
		JPanel* pGuestBtn = (JPanel*)pRoot->find_child(L"D_Guest");
		pGuestBtn->EventClick.first = E_REVERSE_SHOW;
		pGuestBtn->EventClick.second = pGuest;

		JPanel* pMouseCursor = pRoot->find_child(L"mouse_cursor");
		pMouseCursor->PreEvent.first = UI::E_MOUSE_CURSOR;
		pMouseCursor->PreEvent.second = pMouseCursor;

		JPanel* pHelpImg = (JPanel*)pRoot->find_child(L"HelpRect");
		pHelpImg->PreEvent.first = E_HELP_IMG;
		pHelpImg->PreEvent.second = pHelpImg;

		JPanel* pEff4 = (JPanel*)pRoot->find_child(L"effect_hos");
		pEff4->PreEvent.first = E_HOS;
		pEff4->PreEvent.second = pEff4;

		JPanel* pMatching_Panel = (JPanel*)pRoot->find_child(L"Matching_Panel");
		JPanel* AutoMatching = (JPanel*)pRoot->find_child(L"D_AutoMatching");
		AutoMatching->EventClick.first = E_REVERSE_SHOW;
		AutoMatching->EventClick.second = pMatching_Panel;

		JImageCtrl* pDrag = (JImageCtrl*)pRoot->find_child(L"Drag_Img");
		pDrag->PreEvent.first = E_DRAG;
		pDrag->PreEvent.second = pDrag;

		JPanel* pisMatching = (JPanel*)pRoot->find_child(L"isMatching_Sprite_Panel");
		pisMatching->PreEvent.first = E_ISMATCHING;
		pisMatching->PreEvent.second = pisMatching;

		JPanel* pisMatchingTitle = (JPanel*)pRoot->find_child(L"isMatching_Title");
		pisMatchingTitle->PreEvent.first = E_FONTCOLOR;
		pisMatchingTitle->PreEvent.second = pisMatchingTitle;
	}
	static void InGameEvent(JPanel* pRoot)
	{
		JPanel* pMouseCursor = pRoot->find_child(L"mouse_cursor");
		pMouseCursor->PreEvent.first = UI::E_MOUSE_CURSOR;
		pMouseCursor->PreEvent.second = pMouseCursor;

		JPanel* pStatePanel = (JPanel*)pRoot->find_child(L"State_Panel");
		pStatePanel->PreEvent.first = E_STATE_SHOW;
		pStatePanel->PreEvent.second = pStatePanel;

		JPanel* pSetting = (JPanel*)pRoot->find_child(L"Set_Panel");
		pSetting->PreEvent.first = E_KEY_REVERSESHOW;
		pSetting->PreEvent.second = pSetting;

		JSliderCtrl* pChatSlider = (JSliderCtrl*)pRoot->find_child(L"Chat_Slider");
		JListCtrl* pList = (JListCtrl*)pRoot->find_child(L"Chat_Log");
		pList->m_fValue = pChatSlider->m_fValue;

		// effect
		JPanel* pEff1 = (JPanel*)pRoot->find_child(L"fadeout");
		pEff1->PreEvent.first = E_FADEOUT;
		pEff1->PreEvent.second = pEff1;

		JPanel* pEff3 = (JPanel*)pRoot->find_child(L"fadeout_white");
		pEff3->PreEvent.first = E_FADEOUT;
		pEff3->PreEvent.second = pEff3;

		JPanel* pEff4 = (JPanel*)pRoot->find_child(L"fadeout_green");
		pEff4->PreEvent.first = E_FADEOUT;
		pEff4->PreEvent.second = pEff4;

		JPanel* pEff2 = (JPanel*)pRoot->find_child(L"fadein");
		pEff2->PreEvent.first = E_FADEIN;
		pEff2->PreEvent.second = pEff2;

		JPanel* pCrossHair = (JPanel*)pRoot->find_child(L"CrossHair");
		pCrossHair->PreEvent.first = E_CROSSHAIR;
		pCrossHair->PreEvent.second = pCrossHair;

		JPanel* pFight = (JPanel*)pRoot->find_child(L"fight_panel");
		pFight->PreEvent.first = E_FIGHT;
		pFight->PreEvent.second = pFight;

		//JPanel* pTimeOver = (JPanel*)pRoot->find_child(L"TimeOver");
		//pTimeOver->PreEvent.first = E_FADEINOUT;
		//pTimeOver->PreEvent.second = pTimeOver;

		JListCtrl* pKillToDeath = (JListCtrl*)pRoot->find_child(L"KilltoDeath");
		pKillToDeath->PreEvent.first = E_KILLTODEATH;
		pKillToDeath->PreEvent.second = pKillToDeath;

		static JPanel* pInventoryPanel = (JPanel*)pRoot->find_child(L"Inventory_Panel");
		pInventoryPanel->PreEvent.first = E_KEY_REVERSESHOW;
		pInventoryPanel->PreEvent.second = pInventoryPanel;

		JPanel* pInventorySort = (JPanel*)pInventoryPanel->find_child(L"Inventory_Sort");
		JPanel* pInventorySlot = (JPanel*)pInventoryPanel->find_child(L"Inventory_Slot");
		pInventorySort->EventClick.first = E_INVENTORY_SORT;
		pInventorySort->EventClick.second = pInventorySlot;

		JPanel* pInventoryAdd = (JPanel*)pInventoryPanel->find_child(L"Inventory_ItemAdd");
		pInventoryAdd->EventClick.first = E_INVENTORY_ADD;
		pInventoryAdd->EventClick.second = pInventorySlot;

		//JPanel* pInventoryDel = (JPanel*)pInventoryPanel->find_child(L"Inventory_Del");
		//pInventoryDel->EventClick.first = E_INVENTORY_DEL;
		//pInventoryDel->EventClick.second = pInventorySlot;

		JPanel* pInfoPanel = (JPanel*)pRoot->find_child(L"Info_Panel");
		pInfoPanel->PreEvent.first = E_KEY_REVERSESHOW;
		pInfoPanel->PreEvent.second = pInfoPanel;

		static JPanel* pItemHelp = (JPanel*)pRoot->find_child(L"Item_Info_Panel");
		static JPanel* pItemHelpBack = (JPanel*)pRoot->find_child(L"Item_Info_Back");
		static JPanel* Weapon = (JPanel*)pRoot->find_child(L"Inventory_Weapon_Back");
		static JPanel* Armor = (JPanel*)pRoot->find_child(L"Inventory_Armor_Back");
		static JPanel* Accessoris = (JPanel*)pRoot->find_child(L"Inventory_Accessories_Back");
		static JPanel* Accessoris2 = (JPanel*)pRoot->find_child(L"Inventory_Accessories2_Back");
		static JTextCtrl* ItemInfoName = (JTextCtrl*)pRoot->find_child(L"Item_Info_Name");
		static JTextCtrl* ItemInfoKind = (JTextCtrl*)pRoot->find_child(L"Item_Info_Kind");

		static auto E_SHOW_ITEM_INFO = [](void* vp)
		{
			pItemHelp->m_bRender = false;
			if (!pInventoryPanel->m_bRender) return;

			if (Weapon->CheckHovered())
			{
				ItemInfoName->SetString(L"초보자의 검");
				ItemInfoKind->SetString(L"무기");
				pItemHelp->m_vPos.x = pItemHelp->m_ptMouse.Getpt().x - pItemHelpBack->m_vScl.x;
				pItemHelp->m_vPos.y = pItemHelp->m_ptMouse.Getpt().y - pItemHelpBack->m_vScl.y;
				pItemHelp->m_bRender = true;
			}
			if (Armor->CheckHovered())
			{
				ItemInfoName->SetString(L"초보자의 갑옷");
				ItemInfoKind->SetString(L"방어구");
				pItemHelp->m_vPos.x = pItemHelp->m_ptMouse.Getpt().x - pItemHelpBack->m_vScl.x;
				pItemHelp->m_vPos.y = pItemHelp->m_ptMouse.Getpt().y - pItemHelpBack->m_vScl.y;
				pItemHelp->m_bRender = true;
			}
			if (Accessoris->CheckHovered())
			{
				ItemInfoName->SetString(L"초보자의 목걸이");
				ItemInfoKind->SetString(L"악세사리");
				pItemHelp->m_vPos.x = pItemHelp->m_ptMouse.Getpt().x - pItemHelpBack->m_vScl.x;
				pItemHelp->m_vPos.y = pItemHelp->m_ptMouse.Getpt().y - pItemHelpBack->m_vScl.y;
				pItemHelp->m_bRender = true;
			}
			if (Accessoris2->CheckHovered())
			{
				ItemInfoName->SetString(L"초보자의 팔찌");
				ItemInfoKind->SetString(L"악세사리");
				pItemHelp->m_vPos.x = pItemHelp->m_ptMouse.Getpt().x - pItemHelpBack->m_vScl.x;
				pItemHelp->m_vPos.y = pItemHelp->m_ptMouse.Getpt().y - pItemHelpBack->m_vScl.y;
				pItemHelp->m_bRender = true;
			}
		};
		pItemHelp->PreEvent.first = E_SHOW_ITEM_INFO;
		pItemHelp->PreEvent.second = pItemHelp;

		auto pHelpText = (JTextCtrl*)pRoot->find_child(L"HelpText");
		pHelpText->SetString(L"금액이 부족합니다.");
		JPanel* pHelpTextPanel = pHelpText->m_pParent;
		static auto E_HelpText = [](void* vp)
		{
			JPanel* pPanel = (JPanel*)vp;

			if (pPanel->m_bRender)
				pPanel->m_fUITimer += Timer::SPF;

			if (pPanel->m_fUITimer >= 3.0f)
			{
				pPanel->m_bRender = false;
				pPanel->m_fUITimer = 0.0f;
			}
		};
		pHelpTextPanel->PreEvent.first = E_HelpText;
		pHelpTextPanel->PreEvent.second = pHelpTextPanel;
	}
	static void LoadingEvent(JPanel* pRoot)
	{
		static JPanel* pLoadingSprite = pRoot->find_child(L"Loading_Sprite");
		JPanel* pLoadingProgress = pRoot->find_child(L"Loading_Progress");
		if (pLoadingProgress == nullptr) return;
		auto E_Loading_Sprite = [](void* vp)
		{
			JProgressBar* pProg = (JProgressBar*)vp;
			pLoadingSprite->m_vPos.x = ((pProg->m_fCurValue * (pProg->m_vScl.x * 2.0f)) - pProg->m_vScl.x) + pProg->m_vPos.x;
		};
		pLoadingProgress->PreEvent.first = E_Loading_Sprite;
		pLoadingProgress->PreEvent.second = pLoadingProgress;
	}
}