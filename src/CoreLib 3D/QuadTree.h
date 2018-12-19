#pragma once
#include "MapNode.h"
#include "IDxBasis.h"
//#include "HeightMap.h"

// ¹Ì¿Ï
class QuadTree : public IDxBasis// : public HeightMap
{
protected:
	enum QuadCorner { ETopLeft = 0, ETopRight = 1, EBottomLeft = 2, EBottomRight = 3 };

	MapNode* m_pRootNode;
	queue< MapNode*> m_NodeQueue;
	vector<GameObject*> m_DrawObjList;
	
	bool m_useIndexList;
	DWORD  m_MaxDepthLimit;
	DWORD  m_MaxDepth;

	DWORD  m_RenderDepth;

	float m_MinDivideSize;
public:
	bool Build(const float& width, const float& height) noexcept;
	bool BuildTree(MapNode* pNode) noexcept;
	bool SubDivide(MapNode* pNode) noexcept;
	MapNode* CreateNode(MapNode* pParentNode, const float& topLeft, const float& topRight, const float& bottomLeft, const float& bottomRight) noexcept;

	//int  AddObject(GameObject* pObject) noexcept;
	//MapNode* FindNode(MapNode* pNode, GameObject* pObject) noexcept;
	//int CheckRect(MapNode* pNode, GameObject* pObject) noexcept;

	void VisibleNode(MapNode* pNode) noexcept;
	void VisibleObject(MapNode* pNode) noexcept;
	void DrawFindNode(MapNode* pNode) noexcept;

	DWORD CheckSize(const DWORD& size);
	void SetNeighborNode(MapNode* pNode) noexcept;
	void FindNeighborNode(MapNode* pNode) noexcept;

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
public:
	QuadTree() = default;
	virtual ~QuadTree() = default;
};