#pragma once
#include "ASkinObj.h"
#include "ABoneObj.h"



class AObjMgr : public ASingleton<AObjMgr>
{
private:
	friend class ASingleton<AObjMgr>;

	AModel*		m_pModelObj;

	map<int, AModel*>	m_MgrList;


public:

	int			LoadObj(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		const TCHAR* strFileName,
		const TCHAR* strShaderName, bool bThread = false);



	FILE_TYPE	GetFileTypeID(const TCHAR* pszFileName);
	void		SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	int											m_iCurIndex;

public:
	bool		Init();// noexcept override;
	AModel*	const	GetPtr(int index);
	AModel* const	GetPtr(const TCHAR* szName);
	int			GetID(AModel*);
	int			GetID(const TCHAR* szName);
	int			Count();
	bool		Release();// noexcept override;
	bool		Delete(int iIndex);
	bool		Delete(AModel* pPoint);
	bool		Frame(const float& spf, const float& accTime);//	noexcept override;
	bool		Render(ID3D11DeviceContext*    pContext);// noexcept override;


private:
	AObjMgr(void);
	virtual ~AObjMgr(void);
};

#define I_OBJMGR AObjMgr::GetInstance()