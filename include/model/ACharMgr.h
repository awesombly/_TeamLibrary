#pragma once
#include "ACharacterLoader.h"
#include "ACharacter.h"

class ACharMgr : public ASingleton< ACharMgr >
{
private:
	friend class ASingleton<ACharMgr>;
public:
	ACharacterLoader				m_Parser;

	int								m_iCurIndex;

	map<int, ACharacter*>		m_MgrList;

public:
	bool		Init();// noexcept override;;
	ACharacter*	const	GetPtr(int index);
	ACharacter* const	GetPtr(const TCHAR* szName);
	int			GetID(AModel*);
	int			GetID(const TCHAR* szName);
	int			Count();
	bool		Release();
	bool		Delete(int iIndex);
	bool		Delete(AModel* pPoint);
	bool		Frame(const float& spf, const float& accTime);//	noexcept override;
	bool		Render(ID3D11DeviceContext*    pContext);// noexcept override;

public:
	int			Create(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*    pImmediateContext,
		const TCHAR* strFileName,
		const TCHAR* strShaderName,
		int iMatrixIndex = -1);
	bool		Load(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*    pImmediateContext,
		const TCHAR* strFileName);
private:
	ACharMgr();
	virtual~ACharMgr();
};
#define I_CHARMGR ACharMgr::GetInstance()