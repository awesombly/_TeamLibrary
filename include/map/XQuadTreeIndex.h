#pragma once
#include "XQuadTree.h"

class XStaticData	// PatchLevel에서 정적으로 가지고 있을 모든 패치, 총 16개의 패치가 저장된다. 
{
public:
	DWORD					dwLevel;
	vector<vector<DWORD>>	IndexList;
	vector<ComPtr<ID3D11Buffer>>	IndexBufferList;
public:
	XStaticData() {}
	~XStaticData()
	{
		for (int iList = 0; iList < IndexBufferList.size(); iList++)
		{
			IndexList[iList].clear();
		}
		IndexBufferList.clear();
		IndexList.clear();
	}
};

class XQuadTreeIndex :public XQuadTree
{
public:
	enum TREECORNER { TL = 0, TR = 1, BL = 2, BR = 3, };
	DWORD				m_dwWidth;
	DWORD				m_dwHeight;
	int					m_iNumFace;
	XMap*				m_Map;
	vector<XStaticData*>	m_pdwLodIndexArray;
public:
	bool				Build(XMap* pMap);
	bool				FindLeafNode(XNode* pNode);			// 재귀적으로 리프노드를 찾아서 CreateLeafNodeIndex함수를 활용하여 리프노드에 인덱스를 부여한다.
	HRESULT				CreateLeafNodeIndex(XNode* pNode);
	XNode*				CreateNode(XNode* pParent, DWORD TL, DWORD TR, DWORD BL, DWORD BR);
	bool				SubDivide(XNode* pNode);
	bool				LeafRender(ID3D11DeviceContext* pContext, XNode* pNode);
public:
	virtual	bool		Init() { return true; }
	virtual bool		Frame();
	virtual bool		Render(ID3D11DeviceContext* pContext);
	virtual bool		Release();
public:
	XQuadTreeIndex();
	virtual ~XQuadTreeIndex();
};