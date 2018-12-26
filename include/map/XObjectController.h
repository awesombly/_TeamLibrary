#pragma once
#include "XMapController.h"
#include "XObjectManager.h"

class XObjectController :public XMapController, public XSingleton<XObjectController>
{
private:
	friend class XSingleton<XObjectController>;
public:
	bool m_bSelect = false;
	TString m_szSelectObject;
	int m_iSelectMatNum = -1;
	bool m_bTransparency = false;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
public:
	TString AddObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TString szFileName);
	bool AddObjectWorldMat(TString szObj, D3DXMATRIX matWorld);
	D3DXMATRIX GetObjectWorldMat(TString szObj, int iSelectMatNum);
	bool SetSelectObject(TString szObj, int iSelectMatNum);
	bool MoveObject(ID3D11DeviceContext * pContext);
	void SelectModeOn();
	void SelectModeOff();
	void SetMatrix(D3DXMATRIX* matView, D3DXMATRIX* matProj);
	void SelectObjTransparency(ID3D11DeviceContext* pContext);
	int GetLastMatIndex(TString szObj);
public:
	virtual void Start() override;

public:
	virtual bool Init() override;
	virtual bool Frame(const float& spf, const float& accTime) override;
	bool Frame(ID3D11DeviceContext* pContext, const float& spf, const float& accTime);
	virtual bool Render(ID3D11DeviceContext* pContext) override;
	virtual bool Release() override;
public:
	XObjectController();
	virtual ~XObjectController();
};

#define I_ObjectCtrl XSingleton<XObjectController>::Get()