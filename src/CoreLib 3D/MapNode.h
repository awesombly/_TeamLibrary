#pragma once
#include "GameObject.h"

struct AABB
{
	D3DXVECTOR3 Center;
	D3DXVECTOR3 Pos[8];
	
	D3DXVECTOR3 Max;
	D3DXVECTOR3 Min;
};


class MapNode
{
public:
	bool m_isLeaf;

	DWORD m_NodeDepth;

	// 바운딩 박스
	AABB				m_Box;
	// 노드 코너 정점 인덱스 및 위치
	vector<DWORD>		m_CornerIndex;
	vector<D3DXVECTOR3> m_CornerList;
	vector<MapNode*>	m_ChildList;
	vector<MapNode*>	m_NeighborList;
	vector<GameObject*> m_ObjectList;
	// 추가,, x,y
	DWORD				m_PositionIndex[2]; 

public:

	MapNode();
	virtual ~MapNode();
};

