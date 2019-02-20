#pragma once
#include "AObjMgr.h"
//#include "AAseObj.h"

class AModelObj : public AModel
{
public:
	vector<ObjWM*>				m_pModelList;
	vector<AModelObj*>			m_pSubObjList;
	bool						m_bDefferedMultiThread;
	AAnimation					m_Animation;

public:
	void		Add(ID3D11Device* pDevice, AModelObj* pObj);
	bool		Init()noexcept override;
	bool		Load(ID3D11Device* pd3dDevice, ID3D11DeviceContext* context, const TCHAR* strFileName, const TCHAR* strShaderName, bool bThread = false);
	bool		Frame(const float& spf, const float& accTime) noexcept override;
	bool		Render(ID3D11DeviceContext*    pContext)noexcept override;
	bool		Draw(ID3D11DeviceContext* pContext, ObjWM* pUint, bool bCommand);
	virtual bool		PreDraw(ID3D11DeviceContext*    pContext, ObjWM* pUnit, bool bCommand);
	virtual bool		PostDraw(ID3D11DeviceContext*    pContext, ObjWM* pUnit, bool bCommand);
	bool		Release()noexcept override;

	void		SetActionFrame(TCHAR* pStrModel, DWORD dwStrat, DWORD dwEnd);
	void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	// 1개의 메쉬를 다수의 오브젝트가 사용할 경우에 사용된다.
	bool		ObjectFrame(const float& spf, const float& accTime);
	bool		ObjectRender(ID3D11DeviceContext*    pContext, bool bCommand = false);
	bool		Set(ID3D11Device* pd3dDevice, int iIndex);
	void		SetConstantBuffers(ID3D11DeviceContext* pContext, ObjWM* pUint, int iMesh);

public:
	AModelObj(void);
	AModelObj(int iIndex);

	explicit AModelObj(AModelObj* myStuff)
	{
		AModelObj* pObj(myStuff);
		m_pSubObjList.push_back(move(pObj));
	}

	AModelObj(AModelObj &obj);
	virtual ~AModelObj(void);
};

