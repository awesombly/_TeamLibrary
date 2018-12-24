#pragma once
#include"XStd.h"

class XSRVManager :public XSingleton <XSRVManager>
{
public:
	friend class XSingleton<XSRVManager>;
public:
	map<TString, ComPtr<ID3D11ShaderResourceView>> m_SRVList;
public:
	TString Add(ID3D11Device* pd3dDevice, TString szTexture);
	ComPtr<ID3D11ShaderResourceView> Find(TString szTexture);
public:
	void	Clear();
private:
	XSRVManager();
public:
	virtual ~XSRVManager();
};

#define I_SRV XSRVManager::Get()