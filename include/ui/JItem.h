#pragma once
#include "JTextureMgr.h"
namespace UI
{
	class JItem
	{
	public:
		std::map<wstring, int> m_pItemList;
	private:
		static JItem* m_pInst;
	public:
		bool Load(ID3D11Device* pDevice, const TCHAR* FileName = L"../../data/ui/ItemList.txt");
		int GetKey(wstring ItemName);
	public:
		static JItem* Get()
		{
			if (m_pInst == nullptr)
				m_pInst = new JItem();
			return m_pInst;
		}
	public:
		JItem() {}
		virtual ~JItem() {}
	};
}