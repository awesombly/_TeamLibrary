#pragma once
#include "JImageCtrl.h"
namespace UI
{
	class JInventory;
	struct Item
	{
	public:
		map<wstring, int> m_pItemList;
	};
	class JSlot : public JPanel
	{
	public:
		bool		m_bEmpty = true;
		bool		m_bDrag = false;
		JImageCtrl* m_pBack;
		JImageCtrl* m_pItem;
		JInventory* m_pInven = nullptr;
	public:
		bool Create(ID3D11Device* pDevice, const TCHAR* szBack, const TCHAR* szFront, const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
		JPanel* FindSlot(list<JPanel*> pSlotList);
		bool AddItem(wstring strItem);
		bool AddItem(const int itxItem);
		bool DelItem();
		bool Empty();
	public:
		void Update();
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
	public:
		JSlot(wstring NodeName) : JPanel(NodeName)
		{
			m_Type = SLOT;
			m_pBack = new JImageCtrl(NodeName + L"_Back");
			m_pItem = new JImageCtrl(NodeName + L"_Item");
		}
		virtual ~JSlot() {}
	};
};