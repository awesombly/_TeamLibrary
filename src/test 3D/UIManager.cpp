#include "UIManager.h"



void UIManager::AddSlotItem(const wstring_view& itemName)
{
	if (UIManager::Get().m_pSlot1->Empty())
	{
		UIManager::Get().m_pSlot1->AddItem(itemName.data());
		return;
	}
	if (UIManager::Get().m_pSlot2->Empty())
	{
		UIManager::Get().m_pSlot2->AddItem(itemName.data());
		return;
	}
	if (UIManager::Get().m_pSlot3->Empty())
	{
		UIManager::Get().m_pSlot3->AddItem(itemName.data());
		return;
	}
	UIManager::Get().m_pInvenSlot->AddItem(itemName.data());
}