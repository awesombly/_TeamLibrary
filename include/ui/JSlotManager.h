#pragma once
#include "JInventory.h"
namespace UI
{
	class JSlotManager
	{
	public:
		bool AddSlot(JSlot* pSlot);
		bool AddSlot(JInventory* pInven);
		bool DelSlot(wstring NodeName);
		list<JPanel*>* GetList();
	public:
		static JSlotManager* Get()
		{
			if (m_pInst == nullptr)
				m_pInst = new JSlotManager();
			return m_pInst;
		}
	private:
		list<JPanel*> m_pSlotList;
		static JSlotManager* m_pInst;
	public:
		JSlotManager() {}
		virtual ~JSlotManager() {}
	};
}