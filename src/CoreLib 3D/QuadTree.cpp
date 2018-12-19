#include "QuadTree.h"
#include "ObjectManager.h"

bool QuadTree::Build(const float& width, const float& height) noexcept
{
	m_pRootNode = CreateNode(nullptr, -width / 2, width / 2, -height / 2, height);

	if (!BuildTree(m_pRootNode))
	{
		ErrorMessage(""s + __FUNCTION__ + " -> Build Error!");
		return false;
	}
	return true;
}

bool QuadTree::BuildTree(MapNode* pNode) noexcept
{
	// 분할 가능?
	if (SubDivide(pNode))
	{
		for (int iNode = 0; iNode < pNode->m_ChildList.size(); ++iNode)
		{
			if (m_useIndexList)
			{
				//DWORD index = pNode->m_ChildList[iNode]->m_PositionIndex[1] *
				//			pow(2.0f, (float)pNode->m_ChildList[iNode]->m_NodeDepth) +
				//			pNode->m_ChildList[iNode]->m_PositionIndex[0];
				//DWORD value = pNode->m_ChildList[iNode]->m_NodeDepth;
				//
				//m_Level
			}
			BuildTree(pNode->m_ChildList[iNode]);
		}
	}
	return true;
}

bool QuadTree::SubDivide(MapNode* pNode) noexcept
{
	if (pNode == nullptr)
	{
		return false;
	}

	// 최대 깊이 초과시 강제 리프노드 지정
	if (m_MaxDepthLimit <= pNode->m_NodeDepth)
	{
		pNode->m_isLeaf = true;
		return false;
	}
	
	//  현재 노드의 실제 크기 계산
	float widthSplit = (pNode->m_CornerList[ETopRight].x - pNode->m_CornerList[ETopLeft].x) / 2;
	float heightSplit = (pNode->m_CornerList[ETopLeft].z - pNode->m_CornerList[EBottomLeft].z) / 2;

	// 자식 노드가 지정한 분할 크기보다 작을시 분할 안함
	if (widthSplit < m_MinDivideSize || heightSplit < m_MinDivideSize)
	{
		pNode->m_isLeaf = true;
		return false;
	}

	pNode->m_ChildList.push_back(CreateNode(pNode,
											pNode->m_CornerList[ETopLeft].x,
											pNode->m_CornerList[ETopLeft].x + widthSplit,
											pNode->m_CornerList[ETopLeft].z - heightSplit,
											pNode->m_CornerList[ETopLeft].z));

	pNode->m_ChildList.push_back(CreateNode(pNode,
											pNode->m_CornerList[ETopLeft].x + widthSplit,
											pNode->m_CornerList[ETopRight].x,
											pNode->m_CornerList[ETopLeft].z - heightSplit,
											pNode->m_CornerList[ETopLeft].z));

	pNode->m_ChildList.push_back(CreateNode(pNode,
											pNode->m_CornerList[ETopLeft].x,
											pNode->m_CornerList[ETopLeft].x + widthSplit,
											pNode->m_CornerList[EBottomLeft].z,
											pNode->m_CornerList[ETopLeft].z - heightSplit));

	pNode->m_ChildList.push_back(CreateNode(pNode,
											pNode->m_CornerList[ETopLeft].x + widthSplit,
											pNode->m_CornerList[ETopRight].x,
											pNode->m_CornerList[EBottomRight].z,
											pNode->m_CornerList[ETopLeft].z - heightSplit));

	return true;
}

MapNode* QuadTree::CreateNode(MapNode* pParentNode, const float& topLeft, const float& topRight, const float& bottomLeft, const float& bottomRight) noexcept
{
	MapNode* pNode = new MapNode();
	
	pNode->m_ChildList.reserve(4);
	pNode->m_CornerList.reserve(4);

	pNode->m_Box.Min = D3DXVECTOR3(topLeft, 0.0f, bottomLeft);
	pNode->m_Box.Max = D3DXVECTOR3(topRight, 0.0f, bottomRight);
	pNode->m_Box.Center = (pNode->m_Box.Max + pNode->m_Box.Min) * 0.5f;

	pNode->m_CornerList.push_back({ pNode->m_Box.Min.x, 0.0f, pNode->m_Box.Max.z });
	pNode->m_CornerList.push_back( pNode->m_Box.Max);
	pNode->m_CornerList.push_back( pNode->m_Box.Min);
	pNode->m_CornerList.push_back({ pNode->m_Box.Max.x, 0.0f, pNode->m_Box.Min.z });

	if (pParentNode != nullptr)
	{
		pNode->m_NodeDepth = pParentNode->m_NodeDepth + 1;
		if (m_MaxDepth < pNode->m_NodeDepth)
		{
			m_MaxDepth = pNode->m_NodeDepth;
		}
	}
	return pNode;
}


//int  AddObject(GameObject* pObject) noexcept;
//MapNode* FindNode(MapNode* pNode, GameObject* pObject) noexcept;
//int CheckRect(MapNode* pNode, GameObject* pObject) noexcept;

/// 오브젝트 프러스텀 컬링
void QuadTree::VisibleNode(MapNode* pNode) noexcept
{
	if (pNode->m_NodeDepth < m_RenderDepth)
		return;
	
	//if(ObjectManager::Cameras[ECamera::Main]

}
void QuadTree::VisibleObject(MapNode* pNode) noexcept
{
	///
	pNode;
}

void QuadTree::DrawFindNode(MapNode* pNode) noexcept
{
	///
	pNode;
}

DWORD QuadTree::CheckSize(const DWORD& size)
{
	return 0; size;
}

void QuadTree::SetNeighborNode(MapNode* pNode) noexcept
{
	pNode;
}

void QuadTree::FindNeighborNode(MapNode* pNode) noexcept
{
	//pNode->m_NeighborList.resize(4);
	//
	//DWORD numPatchCount = pow(2.0f, (float)pNode->m_NodeDepth);
	//
	//DWORD neighborCol, neightborRow;
	//if (pNode->m_PositionIndex[1] > 0)
	//{
	//	neighborCol = pNode->m_PositionIndex[0];
	//	neighborRow = (pNode->m_PositionIndex[1] - 1) * numPatchCount;
	//	pNode->m_NeighborList[0] = m_LevelList
	//}
	pNode;
}





















bool QuadTree::Init() noexcept
{
	return true;
}

bool QuadTree::Frame(const float& spf, const float& accTime)	noexcept
{
	//if (!m_isEnable) return false;
	return true;
	spf, accTime;
}

bool QuadTree::Render(ID3D11DeviceContext* pDContext) noexcept
{
	//if (!m_isEnable) return false;
	return true; pDContext;
}

bool QuadTree::Release()	noexcept
{
	return true;
}