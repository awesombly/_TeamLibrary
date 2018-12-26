#pragma once
#include"XMap.h"
#include "XShape.h"
#include "XCamera.h"

#define MAX_DEPTH 4

class XNode
{
public:
	DWORD					m_dwDepth;
	BOOL					m_bLeaf;
	X_Box					m_Box;
	ComPtr<ID3D11Buffer>	m_VertexBuffer;
	vector<DWORD>			m_IndexList;
	ComPtr<ID3D11Buffer>	m_IndexBuffer;
	vector<DWORD>			m_CornerIndex;
	vector<D3DXVECTOR3>		m_CornerList;
	vector<XNode*>			m_ChildList;
public:
	XNode()
	{
		m_bLeaf = FALSE;
		m_dwDepth = 0;
		m_Box.vCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Box.vMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		m_Box.vMin = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		m_Box.vAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		m_Box.vAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_Box.vAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		m_Box.fExtent[0] = 1;
		m_Box.fExtent[1] = 1;
		m_Box.fExtent[2] = 1;
	}
	~XNode()
	{
		if (!m_CornerList.empty()) m_CornerList.clear();
		if (!m_CornerIndex.empty()) m_CornerIndex.clear();
		for (int i = 0; i < m_ChildList.size(); i++)
		{
			SAFE_DEL(m_ChildList[i]);
		}

	}
};

class XQuadTree
{
public:
	XNode*					m_pRootNode;
	queue<XNode*>			m_QuadTreeQueue;
	int						m_iMaxDepth;
	XCamera*				m_pCamera;
	vector<XNode*>			m_DrawNodeList;
public:
	virtual bool		Build(float width, float height);
	virtual XNode*		CreateNode(XNode* pParent, float fL, float fR, float fB, float fT);
	virtual bool		BuildTree(XNode* pNode);
	virtual bool		SubDivide(XNode* pNode);			// 노드를 쪼게는 함수
	bool				AddObject(XShape* pObj);
	XNode*				FindNode(XNode* pNode, XShape* pObj);
	XNode*				GetRootNode() { return m_pRootNode; }
	bool				CheckRect(XNode* pNode, XShape* pObj);
	void				SetCamera(XCamera* pCamera);
	void				DrawFindNode(XNode* pNode);
	bool				PreFrame();
public:
	virtual bool		Init() { return true; }
	virtual bool		Frame();
	virtual bool		Render(ID3D11DeviceContext* pContext) { return true; }
	virtual bool		Release() { return true; }
public:
	XQuadTree();
	virtual ~XQuadTree();
};