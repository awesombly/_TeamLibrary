#pragma once
#include "XQuadTreeIndex.h"
#include "XSelect.h"

// Sample에서 모두 처리하면 복잡해 지기 때문에 헬퍼 쯤되는 Contoller를 만들어서 texture담당 Height조절 담당클래스로
// 상속을 시켜서 각각 처리할 예정.		ex) XHeightMapController  <-  XMapController  ->  XTextureMapController
// 추후에 LOD를 사용시 PatchNode리스트를 추가

class XMapController
{
protected:
	// Map의 포인터를 가진다. LOD가 아니기 때문에 공통의 VertexList가 필요.
	XMap* m_pMap;
	XNode* m_pRootNode;
	// 시동 변수
	bool m_bStart = false;
	BOOL m_bEnable = FALSE;
	// Map Control이 필요하므로, 리프노드를 전부 저장
	vector<XNode*>	m_LeafNodeList;
	vector<XNode*>	m_CrashNode;
	vector<PNCT_Vertex*> m_CraseVertex;
	// 마우스 포인터와 Node의 충돌지점
	D3DXVECTOR3	m_vIntersection = { 0.0f,0.0f,0.0f };
	// 교점에서의 반지름
	float m_fRadius = 0.0f;
public:
	// 컨트롤러 시동
	void SetEnable(BOOL bEnable) { m_bEnable = bEnable; }
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	virtual void Start() { m_bStart = true; }
	// 루트노드를 쿼드트리에서 받아오면 리프노드로 분해해서 저장하는 함수.
	bool SetLeafNode(XNode* pRootNode);
	// Vertex Control을 위해 Map을 포인터로 가져온다.
	bool SetMap(XMap* pMap);
	bool CheakInRange();
	bool FindIntersection();
public:
	virtual bool Init() { return true; }
	virtual bool Frame(const float& spf, const float& accTime) { return true; }
	virtual bool Render(ID3D11DeviceContext* pContext) { return true; }
	virtual bool Release();
public:
	XMapController();
	virtual ~XMapController();
};

