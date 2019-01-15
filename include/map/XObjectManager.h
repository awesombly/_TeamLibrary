#pragma once
#include "XViewer.h"
#include "XShape.h"

struct XCollider
{
	vector<X_Box>	ColliderOBB;
	vector<D3DXQUATERNION> qRotation; // OBB를 그릴때 회전량을 알기 위해 저장
	vector<X_Box>	ColliderAABB;	  // 회전을 시키지 않을 상태
	vector<X_Sphere> ColliderSphere;
};

class XObjectManager :public XSingleton<XObjectManager>
{
private:
	friend class XSingleton<XObjectManager>;
public:
	map<TString, shared_ptr<XViewer>>	m_ObjectList;		// 같은 오브젝트 하나만 저장
	map<TString, vector<D3DXMATRIX>>	m_ObjectMatrix;		// 오브젝트 별 matWorld 배열
	map<TString, XCollider>				m_ObjectCollider;	// 같은 오브젝트 당 하나의 충돌체만 저장
	map<TString, vector<XBoxShape>>		m_OBBViewerList;	// 오브젝트 종류 당 하나만 추가
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	bool m_bViewCollider = false;
public:
	TString AddObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TString szFileName);
	bool DelObjWorldMat(TString szObjName, int iSelecObj);
	bool AddObjectWorldMat(TString szObj, D3DXMATRIX matWorld);
	void SetMatrix(D3DXMATRIX* matView, D3DXMATRIX* matProj);
	void AddCollider(TString szObject, X_Box OBB);
	void ViewColliderSwitch();
	void DrawCollider(TString szObject, ID3D11DeviceContext* pContext, D3DXMATRIX matObjWorld);
public:
	virtual bool Init();
	virtual bool Frame(const float& spf, const float& accTime);
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool Release();
public:
	XObjectManager();
	virtual ~XObjectManager();
};

#define I_Object XSingleton<XObjectManager>::Get()