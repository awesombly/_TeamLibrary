#pragma once
#include "JSlot.h"
namespace UI
{
	class JInventory : public JPanel
	{
	public:
		bool m_bUpdate = true;
		int	m_iRows;
		int m_iCols;
		int m_iMaxItem;
		JImageCtrl* m_pBack;
		std::vector<JSlot*> m_pItemList;
	public:
		bool Create(ID3D11Device* pDevice, int iRow, int iCol, const TCHAR* szBack, const TCHAR* szSlotBack, const char* PSName = "PS", const TCHAR* szShaderName = L"../../data/ui/shader/DefaultUI.hlsl");
		bool CreateItemSlot(ID3D11Device* pDevice, int iRow, int iCol, const TCHAR* szSlotBack);
	public:
		void UpdateInventoryPos();
		void Update();
		bool Frame(const float& spf, const float& accTime) noexcept override;
		bool Render(ID3D11DeviceContext* pContext)noexcept override;
		bool Release()noexcept override;
	public:
		JInventory(wstring NodeName) : JPanel(NodeName)
		{
			m_Type = INVENTORY;
			m_pBack = new JImageCtrl(NodeName + L"_Back");
		}
		virtual ~JInventory() {}
	};
}