#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "ADxStd.h"
#include "AStd.h"
#include "AModel.h"
#include <map>



class ATexture
{
public:
	ComPtr<ID3D11ShaderResourceView> m_TextureSRV;
	T_STR		m_szName;
	T_STR		m_szPath;

	HRESULT Load(ID3D11Device* device, const TCHAR* fileName);
	bool	Apply(ID3D11DeviceContext*    pImmediateContext);
	void	SetPath(const TCHAR* pPath);
public:
	ATexture();
	virtual ~ATexture();
};




class ATextureMgr : public ASingleton<ATextureMgr>
{
private:
	friend class ASingleton<ATextureMgr>;
public:
	map<INT, ATexture*>							m_TextureList;
	INT											m_iIndex;

public:
	INT				Add(ID3D11Device*	 pDevice, const TCHAR *pFileName);
	INT				Add(ID3D11Device*	 pDevice, const TCHAR *pFileName, const TCHAR* szPath);
	ATexture*		GetPtr(INT index);


public:
	ATextureMgr();
	virtual ~ATextureMgr();
};
#define I_TEXTURE ATextureMgr::GetInstance()