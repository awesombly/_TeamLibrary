#pragma once
#include "AMapObj.h"
#include "ACharacterLoader.h"


class AMapMgr : public ASingleton<AMapMgr>
{
private:
	friend class ASingleton<AMapMgr>;
	map<int, AMapObj*> m_MapObjList;

	AModel*		m_pModelObj;

public:
	ACharacterLoader	m_Parser;
	int					m_iCurIndex;

	bool		Load(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*    pImmediateContext,
		const TCHAR* strFileName);

	int		Create(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*    pImmediateContext,
		const TCHAR* strFileName,	const TCHAR* strShaderName);

	AMapObj*	const	GetPtr(int index);
	AMapObj*	const	GetPtr(const TCHAR* szName);

	int			GetID(AModel* pChild);
	int			GetID(const TCHAR* szName);

	int			Count();


	bool		Delete(int iIndex);
	bool		Delete(AModel* pPoint);

	bool		Init();
	bool		Frame(const float& spf, const float& accTime);
	bool		Render(ID3D11DeviceContext*    pContext);
	bool		Release();
public:
	AMapMgr();
	~AMapMgr();
};

#define I_MAPMGR AMapMgr::GetInstance()